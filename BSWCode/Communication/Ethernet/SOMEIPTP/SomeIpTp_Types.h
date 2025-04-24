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
**  FILENAME    : SomeIpTp_Types.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of SomeIpTp                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
************************************************************************** ****/
/*  <VERSION>    <DATE>      <AUTHOR>    <REVISION LOG>
 *  V2.0.0       2020-07-06  HuRongbo    R19_11 SomeIpTp initial version.
 */

#ifndef SOMEIPTP_TYPES_H
#define SOMEIPTP_TYPES_H

#include "ComStack_Types.h"

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* NSDU which has to be passed as one assembled RxPdu to the upper layer.*/
typedef struct
{
    /*
     * This parameter defines the handle ID that is used by the PduR when
     * calling SomeIpTp_RxIndication.
     */
    PduIdType SomeIpTpRxNPduHandleId;
    /*
     * Reference to a Pdu in the COM-Stack that represents the assembled
     * RxPdu which is passed via the PduR to the upper layer
     */
    PduIdType SomeIpTpRxSduRef;
} SomeIpTp_RxNSduType;

/* NSDU that the SomeIpTp module transmits via the SomeIpTpChannel. */
typedef struct
{
    /*
     * This handle ID is used by PduR when calling SomeIpTp_Transmit.
     * SomeIpTp as Lower Layer.
     */
    PduIdType SomeIpTpTxNSduHandleId;
    /* SomeIpTp as lower layer calling PduR's API with this Id */
    PduIdType SomeIpTpTxNSduUpperLayerId;

    /*
     * This handle ID is used by PduR when calling SomeIpTp_TriggerTransmit.
     * SomeIpTp as Upper Layre.
     */
    PduIdType SomeIpTpTxNPduHandleId;
    /* SomeIpTp as upper layer calling PduR's API with this Id */
    PduIdType SomeIpTpTxNPduLowerLayerId;

    /*
     * The maximum possible size of the segmented SOME/IP PDUs derive from
     * parameter SomeIpTpTxNPduRef.
     */
    PduLengthType SomeIpTpTxMaxPduLen;
} SomeIpTp_TxNSduType;

/* Configuration parameters of the SomeIpTp channel. */
typedef struct
{
    /*
     * Sets the duration of the minimum time in seconds the SomeIpTp module
     * shall wait between the transmissions of N-PDUs.
     */
    uint32 SomeIpTpNPduSeparationTime;

    /* Timer to monitor the successful reception. */
    uint32 SomeIpTpRxTimeoutTime;

    /* Timer to monitor the successful transmission. */
    uint32 SomeIpTpTxTimeoutTime;

    /* NSDU which has to be passed as one assembled RxPdu to the upper layer.*/
    P2CONST(SomeIpTp_RxNSduType, TYPEDEF, SD_CONST) SomeIpTpRxNSdu;
    PduIdType SomeIpTpRxNSduNum;

    /* NSDU that the SomeIpTp module transmits via the SomeIpTpChannel. */
    P2CONST(SomeIpTp_TxNSduType, TYPEDEF, SD_CONST) SomeIpTpTxNSdu;
    PduIdType SomeIpTpTxNSduNum;
} SomeIpTp_ChannelType;

/* Configuration of the SomeIpTp module. */
typedef struct
{
    P2CONST(SomeIpTp_ChannelType, TYPEDEF, SD_CONST) SomeIpTpChannel;
} SomeIpTp_ConfigType;

#endif /* SOMEIPTP_TYPES_H */
