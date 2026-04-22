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
 **  @file               : DoIP_ConfigType.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Data types associated with configuration in DoIP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_CONFIG_TYPE_H_
#define DOIP_CONFIG_TYPE_H_

/* =================================================== inclusions =================================================== */

#include "Std_Types.h"
#include "Rte_DoIP_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ type definitions ================================================ */

/*+++++++++++++++++++++++++++++++++++++++++++
+					                        +
+             DoIPGeneral config            +
+                                           +
+++++++++++++++++++++++++++++++++++++++++++*/

/* PRQA S 5004 ++ */                                        /* VL_QAC_FuncPtr */
typedef Std_ReturnType (*DoIP_GetGidCbkType)(uint8* gid);   /**< Callback type of DoIPGetGidDirect */
typedef Std_ReturnType (*DoIP_TriggerGidSyncCbkType)(void); /**< Callback type of DoIPTriggerGidSyncDirect */
typedef Std_ReturnType (*DoIP_GetPowerModeCbkType)(
    DoIP_PowerStateType* state); /**< Callback type of DoIPGetPowerModeDirect */
typedef Std_ReturnType (*DoIP_FurtherActionByteCbkType)(
    DoIP_FurtherActionByteType* furtherActionByte); /**< Callback type of DoIPFurtherActionByteDirect  */
/* PRQA S 5004 -- */

/**
 * @brief General config of DoIP
 */
typedef struct
{
    const DoIP_GetPowerModeCbkType      PowerModeDirect;         /**< DoIPPowerModeDirect @range NA */
    const DoIP_GetGidCbkType            GetGidDirect;            /**< DoIPGetGidDirect @range NA */
    const DoIP_TriggerGidSyncCbkType    TriggerGidSyncDirect;    /**< DoIPTriggerGidSyncDirect @range NA */
    const DoIP_FurtherActionByteCbkType FurtherActionByteDirect; /**< DoIPFurtherActionByteDirect @range NA */
} DoIP_GeneralCfgType;

/*+++++++++++++++++++++++++++++++++++++++++++
+											+
+           DoIPConfigSet config            +
+                                           +
+++++++++++++++++++++++++++++++++++++++++++*/

/**
 * @brief PC configuration of connection
 */
typedef struct
{
    uint8          TcpConnNum;                       /**< Num of TcpRequestAddressAssignmentArray @range 0..255 */
    const boolean* TcpRequestAddressAssignmentArray; /** DoIPRequestAddressAssignment @range NA*/

    uint8          UdpConnNum;                       /**< Num of UdpRequestAddressAssignmentArray @range 0..255 */
    const boolean* UdpRequestAddressAssignmentArray; /**< DoIPRequestAddressAssignment @range NA*/

    uint8          AncConnNum;                       /**< Num of AncRequestAddressAssignmentArray @range 0..255 */
    const boolean* AncRequestAddressAssignmentArray; /**< DoIPRequestAddressAssignment @range NA*/
} DoIP_PCConnectionsCfgType;                         /**< DoIPConnections */

/**
 * @brief Configuration of RA authentication callback
 */
typedef struct
{
    Std_ReturnType (*RaAuthenticationFunc)(
        boolean*     authentified,
        const uint8* reqData,
        uint8*       resData);        /**< DoIPRoutingActivationAuthenticationFunc @range NA*/
    uint8 ReqLen;               /**< DoIPRoutingActivationAuthenticationReqLength @range 0..255 */
    uint8 ResLen;               /**< DoIPRoutingActivationAuthenticationResLength @range 0..255 */
} DoIP_RaAuthenticationCbkType; /**< DoIPRoutingActivationAuthenticationCallback */

/**
 * @brief Configuration of RA confirmation callback
 */
typedef struct
{
    Std_ReturnType (*RaConfirmationFunc)(
        boolean*     authentified,
        const uint8* reqData,
        uint8*       resData);      /**< DoIPRoutingActivationConfirmationFunc @range NA */
    uint8 ReqLen;             /**< DoIPRoutingActivationConfirmationReqLength @range 0..255 */
    uint8 ResLen;             /**< DoIPRoutingActivationConfirmationResLength @range 0..255 */
} DoIP_RaConfirmationCbkType; /**< DoIPRoutingActivationConfirmationCallback */

/**
 * @brief PC configuration of routing activation
 */
typedef struct
{
    const DoIP_RaAuthenticationCbkType* AuthenticationCbk; /**< DoIPRoutingActivationAuthenticationCallback @range NA */
    const DoIP_RaConfirmationCbkType*   ConfirmationCbk;   /**< DoIPRoutingActivationConfirmationCallback @range NA */
} DoIP_PCRaCfgType;

/**
 * @brief PC configuration
 */
typedef struct
{
    uint8                            RaNum;      /**< Num of routing activation @range 0..255 */
    const DoIP_PCRaCfgType*          RaCfgArray; /**< DoIPRoutingActivation @range NA */
    uint8                            TesterNum;  /**< Num of tester @range 0..255 */
    uint16                           ChannelNum; /**< Num of channel  @range 0..65535 */
    const DoIP_PCConnectionsCfgType* ConnsCfg;
} DoIP_PCConfigType;

#ifdef __cplusplus
}
#endif
#endif
