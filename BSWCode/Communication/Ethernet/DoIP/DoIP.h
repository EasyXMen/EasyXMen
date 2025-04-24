/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : DoIP.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YB                                                          **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef DOIP_H
#define DOIP_H
/*=======[R E V I S I O N   H I S T O R Y]====================================*
 *  <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2019/8/30    YangBo          remove the retry operation for DoIP_SoAdCopyTxData.
 *  V1.0.1       2019/9/17    YangBo          fix the copy data function from/to Pdur module.
 *  V1.0.2       2019/10/12   YangBo          fix the message reception process when some event happen and the doip
                                              should discard the according message before a new message has been
                                              recepted.
 *  V1.0.3       2019/11/29   YangBo          Add Compile abstraction
 *  V1.0.4       2019/12/9    YangBo          QAC TEST FIX
 *  V1.0.5       2020/03/30   YangBo          Funtional  address support
 *  V2.0.0       2020/08/05   YangBo          Update to R19-11
 *  V2.0.1       2020/09/18   YangBo          QAC FIX
 *  V2.0.2       2020/10/15   YangBo          metric test,add vehicle annoncement connection configration
 *  V2.0.3       2020/11/30   YangBo          fix the bug because of metadata
 *  V2.0.4       2021/03/29   YangBo          because of Pduid,alive check cannot send(fix)
 *  V2.0.5       2021/03/31   YangBo          det method change.
 *  V2.0.6       2021/04/28   YangBo          change metadata byteOrder transmit from/to DCM.
 *  V2.0.7       2021/06/16   zhiqiang.huang  add tx queue buffer.
 *  V2.0.8       2022/01/07   Zhiqiang.huang  add doip client.
 *  V2.0.9       2022/04/28   Zhiqiang.huang  1. Fix the role assigned policy. 2. Remove unused internal function.
 *  V2.0.10      2022/05/10   Zhiqiang.huang  Reset udp remote addr when 1. iftxconfirmation and 2. default udp data
                                              type received in ifrxindication
 *  V2.0.11      2022/07/29   zhiqiang.huang  Add If message handle in API DoIP_SoAdTpCopyTxData.
 *  V2.0.12      2022/08/04   zhiqiang.huang  Canonical interface prototype for DoIP_SoAdTpCopyTxData.
 *  V2.0.13      2022/08/12   zhiqiang.huang  1. DoIP_SoAdTpTxConfirmation, add condition check when reset buffer.
                                              2. Set buffer and buffer control state before call SoAd_TpTransmit.
 *  V2.0.14      2023/04/04   zhiqiang.huang  Remove useless assignment for routActiveState
 *  V2.0.15      2023/04/25   Xinrun.Wang     Fix the following bug: 1. IfTransmit check SduLength when pending
                                              2. GetRoleByCfgUdpConnIdx correct typo tcp->udp
                                              3. Send 8003 when receving 8001 before routingActivation
                                              4. Not check for routingActivation for client in SoAdTpCopyTxData
                                              5. Send RoutingActivation with a registerd SA, shall send aliveCheck and
                                              send respCode 03 after received aliveCheck Resp
 *  V2.0.16      2023/06/13   tao.yu          in DoIP_SoAdTpCopyTxData do not determine whether it is registered
                                              change version type
 *  V2.0.17      2023-8-11    peng.wu         Change DoIP_ResetTcpInnerTxBuf to DoIP_ResetTcpInnerBuf in
 * DoIP_SoAdTpTxConfirmation
 *  V2.0.18      2023-8-30    tao.yu          fix CPT-6752 call upper API when close
 *               2023-11-15   xue.han         fix CPT-7619 change DoIP_ActivationLineType
 *  V2.0.19      2023-11-21   Xinrun.Wang     Optimize code performance, change some functions to inline
 *               2023-12-27   xue.han         QAC
 *  V2.0.20      2024-02-02   xue.han         QAC
 *               2024-03-09   xue.han         delete redundant type definitions
 *               2024-04-09   xue.han         fix CPT-8630 null pointer in DoIP_SoAdTpTxConfirmation and compile error
 *               2024-03-19   xue.han         change DoIPActivationLine function name
 *               2024-03-22   xue.han         clear 0210HuaYangproject compilation warnings
 *  V2.0.21      2024-03-26   tao.yu          fix schm
 *               2024-04-26   xue.han         delete Det report in DoIP_MainFunction
 *               2024-04-30   xue.han         QAC
                 2024-05-06   hao.wen         fix CPT-8965 greenhills compilation error caused by DoIPVehicle not being
                                              configured on the server
 *  V2.0.22      2024-06-17   hao.wen         Fix an issue where vehicle announcement messages could not be sent in
                                              time, CPT-9228
                 2024-07-05   hao.wen         Fixed an issue where an incorrect PduID was passed to the PduR due to
                                              disconnection without message delivery, CPT-9517
                 2024-07-17   hao.wen         Resolve compilation warnings, CPT-9724 CPT-9724
                 2024-07-23   hao.wen         Let routing active 0x01 negative response priority higher than 0x03
                                              negative response, CPT-9640
                 2024-08-08   hao.wen         QAC
 *  V2.0.23      2024-08-26   hao.wen         Modify interface's service ID
                 2024-10-23   hao.wen         Fix CPT-9656,alive check response message was received but the route
                                              activation request that triggered the sending of the alive check was
                                              not processed.
 *  V2.0.24      2025-02-26   hao.wen         1. Realize routing activation authentication and confirmation.
                                              2. When metadata is not configured, obtain sa and ta from the channel.
============================================================================*/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "DoIP_Cfg.h"
#include "DoIP_PCCfg.h"
#include "DoIP_PBCfg.h"
#include "SoAd.h"
#include "Rte_DoIP.h"
#include "Dcm.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define DOIP_VENDOR_ID                (62u)
#define DOIP_MODULE_ID                (173u)
#define DOIP_INSTANCE_ID              (0u)
#define DOIP_AR_RELEASE_MAJOR_VERSION (4u)
#define DOIP_AR_RELEASE_MINOR_VERSION (5u)
#define DOIP_AR_RELEASE_PATCH_VERSION (0u)
#define DOIP_SW_MAJOR_VERSION         (2u)
#define DOIP_SW_MINOR_VERSION         (0u)
#define DOIP_SW_PATCH_VERSION         (24u)

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
#define DOIP_E_UNINIT                   (0x01)
#define DOIP_E_PARAM_POINTER            (0x02)
#define DOIP_E_INVALID_PDU_SDU_ID       (0x03)
#define DOIP_E_INVALID_PARAMETER        (0x04)
#define DOIP_E_INIT_FAILED              (0x05)

#define DOIP_SID_TPTRANSMIT             (0x53)
#define DOIP_SID_TPCANCELTRANSMIT       (0x54)
#define DOIP_SID_TPCANCELRECEIVE        (0x4c)
#define DOIP_SID_IFTRANSMIT             (0x49)
#define DOIP_SID_IFCANCELTRANSMIT       (0x4a)
#define DOIP_SID_INIT                   (0x01)
#define DOIP_SID_GETVERSIONINFO         (0x00)
#define DOIP_SID_ACTIVATIONLINESWITCH   (0x0e)
#define DOIP_SID_SOADTPCOPYTXDATA       (0x43)
#define DOIP_SID_SOADTPTXCONFIRMATION   (0x48)
#define DOIP_SID_SOADTPCOPYRXDATA       (0x44)
#define DOIP_SID_SOADTPSTARTOFRECEPTION (0x46)
#define DOIP_SID_SOADTPRXINDICATION     (0x45)
#define DOIP_SID_SOADIFRXINDICATION     (0x42)
#define DOIP_SID_SOADIFTXCONFIRMATION   (0x40)
#define DOIP_SID_SOCONMODECHG           (0x0b)
#define DOIP_SID_LOCALIPADDRASSIGNMENT  (0x0c)
#define DOIP_SID_MAINFUNCTION           (0x02)
#define DoIP_SID_MAINFUNCTIONHIGHFEQ    (0x02)

/*Client use, developer specification*/
#define DOIP_SID_CONNECTTOVEHICLE         (0xf0)
#define DOIP_SID_DISCONNECTTOVEHICLE      (0xf1)
#define DOIP_SID_SENDENTITYSTATUSREQ      (0xf2)
#define DOIP_SID_SENDPOWERMODEINFOREQ     (0xf3)
#define DOIP_SID_SENDVEHICLEIDREQ         (0xf4)
#define DOIP_SID_SENDVEHICLEIDREQWITHEID  (0xf5)
#define DOIP_SID_SENDVEHICLEIDREQWITTHVIN (0xf6)

#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
#define DOIP_VEHICLEID_DONT_CARE 0xFF
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
extern const DoIP_ConfigType DoIP_PBConfigPtr;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/*
 * Brief               This service initializes all global variables of the DoIP module. After return of
 *                     this service the DoIP module is operational.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DoIPConfigPtr: Pointer to the configuration data of the DoIP module
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_Init(P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) DoIPConfigPtr);
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     versioninfo:Pointer to where to store the version information of this module.
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, TCPIP_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DoIP_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_VAR) versioninfo);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
/******************************************************************************/
/*
 * Brief               Schedules the Diagnostic over IP module. (Entry point
 *                     for scheduling)
 * ServiceId           0x02
 *
 */
FUNC(void, DOIP_CODE) DoIP_MainFunction(void);
/******************************************************************************/
/*
 * Brief               This function is used to notify the DoIP on a switch of
 *                     the DoIPActivationLine
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
/* PRQA S 0624,3451,3449++ */ /* MISRA Rule 8.3,8.5,8.5 */
FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitch(boolean* Active);
/* PRQA S 0624,3451,3449-- */ /* MISRA Rule 8.3,8.5,8.5 */
/******************************************************************************/
/*
 * Brief               By calling this API with the corresponding DoIPPduRRxId the currently ongoing data reception
 *                     is terminated immediately. When the function returns, no reception is in progress anymore with
 *                     the given DoIPPduRRxId identifier.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      DoIPPduRTxId: DoIP unique identifier of the PDU for which reception shall be canceled by the PduR
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     Reception was canceled successfully
 *                     E_NOT_OK: E_NOT_OK: Reception was not canceled
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelReceive(VAR(PduIdType, AUTOMATIC) DoIPPduRRxId);
/******************************************************************************/
/*
 * Brief               This service is called to request the transfer data from the PduRouter to the SoAd.
 *                     It is used to indicate the transmission which will be performed in the DoIP_Mainfunction.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      DoIPPduRTxId: DoIP unique identifier of the PDU to be transmitted by the PduR
 *                     DoIPPduRTxInfoPtr: Tx Pdu information structure which contains the length of the DoIPTxMessage.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE)
DoIP_TpTransmit(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) DoIPPduRTxInfoPtr);
/******************************************************************************/
/*
 * Brief               This service primitive is used to cancel the transfer of pending DoIPPduRTxIds. The connection is
 *                     identified by DoIPPduRTxId. When the function returns, no transmission is in progress anymore
 * with the given DoIPPduRTxId identifier. ServiceId           0x04 Sync/Async          Synchronous Reentrancy Reentrant
 * for different PduIds. Non reentrant for the same PduId. Param-Name[in]      DoIPPduRTxId: DoIP unique identifier of
 * the PDU to be transmitted by the PduR Param-Name[in/out]  None Param-Name[out]     None Return Std_ReturnType E_OK:
 * Transmit cancellation request of the specified DoIPPduRTxId is accepted. E_NOT_OK: The transmit cancellation request
 * of the DoIPPduRTxId has been rejected.
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelTransmit(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId);
/******************************************************************************/
/*
 * Brief               Requests transmission of an I-PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info:Length and pointer to the buffer of the I-PDU
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     request is accepted by the destination module.
 *                     E_NOT_OK: request is not accepted by the destination module.
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE)
DoIP_IfTransmit(VAR(PduIdType, AUTOMATIC) id, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info);
/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of an I-PDU in a lower layer
 *                     communication interface module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU to be cancelled.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     Cancellation was executed successfully by the destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination module.
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfCancelTransmit(VAR(PduIdType, AUTOMATIC) id);
/******************************************************************************/
/*
 * Brief               Schedules the Diagnostic over IP module. (Entry point
 *                     for scheduling) for hight frequency tasks.
 * ServiceId           0xxx
 *
 */
#if (DOIP_HIGH_FREQ_TASK_SUPPORT == TRUE)
extern FUNC(void, DOIP_CODE) DoIP_MainFunction_HighFrequency(void);
#endif /*DOIP_HIGH_FREQ_TASK_SUPPORT == TRUE*/

#if ((DOIP_ROLE_SERVER_SUPPORT == TRUE) && (DOIP_ROLE_GATEWAY_SUPPORT == TRUE))
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to connect
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              E_OK,E_NOT_OK
 *                     indicates the process resulet of this function
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_ConnectToVehicle(uint8 VehicleID);
#endif /* DOIP_ROLE_SERVER_SUPPORT == TRUE && DOIP_ROLE_GATEWAY_SUPPORT == TRUE */
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to disconnect
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              E_OK,E_NOT_OK
 *                     indicates the process resulet of this function
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_DisconnectToVehicle(uint8 VehicleID);
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendEntityStatusReq(uint8 vehicleId);
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendPowerModeInfoReq(uint8 vehicleId);
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdReq(uint8 vehicleId);
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdReqWithEID(uint8 vehicleId);
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdReqWithVIN(uint8 vehicleId);

#endif /*ifndef DOIP_H*/
/*=======[E N D   O F   F I L E]==============================================*/
