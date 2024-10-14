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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : DoIP_Types.h                                               **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : YB                                                          **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

#ifndef DOIP_TYPES_H
#define DOIP_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#ifndef _DEFINED_TYPEDEF_FOR_DoIP_PowerStateType_
typedef uint8 DoIP_PowerStateType;
#endif /* _DEFINED_TYPEDEF_FOR_DoIP_PowerStateType_ */

#ifndef _DEFINED_TYPEDEF_FOR_DoIP_FurtherActionByteType_
typedef uint8 DoIP_FurtherActionByteType;
#endif /* _DEFINED_TYPEDEF_FOR_DoIP_FurtherActionByteType_ */

/*Event to user*/
typedef enum
{
    DOIP_CLIENT_EVENT_CONNECTION,
    DOIP_CLIENT_EVENT_DISCOVERY,
    DOIP_CLIENT_EVENT_VEHICLE_STATUS,
    DOIP_CLIENT_EVENT_POWER_MODE,
    DOIP_CLIENT_EVENT_ALIVE_CHECK = 0x04u
} DoIPClientEventType;

typedef uint8 DoIP_RoleType;
#define DOIP_ROLE_SERVER      1u
#define DOIP_ROLE_CLIENT      2u
#define DOIP_ROLE_GATEWAY_IN  4u
#define DOIP_ROLE_GATEWAY_OUT 8u
#define DOIP_ROLE_GATEWAY_ETH 16u

typedef enum
{
    DOIP_ACTIVATION_LINE_ACTIVE,
    DOIP_ACTIVATION_LINE_INACTIVE
} DoIP_ActivationLineType;

/*This container cantains the max time and count that remote node can wait for diagnostic response*/
typedef struct
{
    uint32 wait1stRspTime;
    uint32 waitRspTime;
    uint8 fakeRspMaxCnt;
} DoIP_GatewayWaitResponseType;

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#endif /*DOIP_TYPES_H*/
/*=======[E N D   O F   F I L E]==============================================*/
