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
 **  FILENAME    : ComStack_Types.h                                            **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :                                                             **
 **  Vendor      :                                                             **
 **  DESCRIPTION :This file is partly ECU dependent.                           **
 **               Implemented SWS: Communication Stack Types (ComStackTypes)   **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

#include "Std_Types.h"
#include "ComStack_Cfg.h"

/* Variables of this type shall be used to store the basic information
 * about a PDU of any type, namely a pointer variable pointing to its SDU (payload),
 * and the corresponding length of the SDU in bytes.*/
typedef struct
{
    uint8* SduDataPtr; /*Pointer to the SDU (i.e. payload data) of the PDU. The type of this pointer depends on the
                          memory model being used at compile time.*/
    PduLengthType SduLength; /*Length of the SDU in bytes.*/
    uint8* MetaDataPtr; /*Pointer to the meta data (e.g. CAN ID, socket ID, diagnostic addresses) of the PDU, consisting
                           of a sequence of meta data items. The length and type of the meta data items is statically
                           configured for each PDU. Meta data items with more than 8 bits use platform byte order.*/
} PduInfoType;

/*Used to store the identifier of a partial network cluster.*/
typedef uint8 PNCHandleType;

/*Specify the parameter to which the value has to be changed (BS or STmin).*/
typedef enum
{
    TP_STMIN = 0x00, /*Separation Time*/
    TP_BS = 0x01,    /*Block Size*/
    TP_BC = 0x02     /*The Band width control parameter used in FlexRay transport
                   protocol module.*/
} TPParameterType;

/*Variables of this type shall be used to store the result of a buffer request.*/
typedef enum
{
    BUFREQ_OK = 0x00,       /*Buffer request accomplished successful.*/
    BUFREQ_E_NOT_OK = 0x01, /*Buffer request not successful. Buffer cannot be accessed*/
    BUFREQ_E_BUSY = 0x02,   /*Temporarily no buffer available. It's up the requester
                          to retry request for a certain time.*/
    BUFREQ_E_OVFL = 0x03    /*No Buffer of the required length can be provided*/
} BufReq_ReturnType;

/*Variables of this type shall be used to store the state of TP buffer.*/
typedef enum
{
    TP_DATACONF = 0x00,   /*Indicates that all data, that have been copied so far,
                        are confirmed and can be removed from the TP buffer.*/
    TP_DATARETRY = 0x01,  /*indicates that this API call shall copy already copied
                        data in order to recover from an error.*/
    TP_CONFPENDING = 0x02 /*indicates that the previously copied data must remain
                        in the TP.*/
} TpDataStateType;

/*Variables of this type shall be used to store the information about Tp
 * buffer handling.*/
typedef struct
{
    TpDataStateType TpDataState; /*The enumeration type to be used to store the
                                  state of Tp buffer.*/
    PduLengthType TxTpDataCnt;   /*Offset from the current position which identifies
                                  the number of bytes to be retransmitted.*/
} RetryInfoType;

/*Variables of the type NetworkHandleType shall be used to store the identifier
 * of a communication channel.*/
typedef uint8 NetworkHandleType;

/*IcomConfigIdType defines the configuration ID. An ID of 0 is the default
 * configuration. An ID greater than 0 shall identify a configuration for
 * Pretended Networking. There is more than 1 configuration possible.*/
typedef uint8 IcomConfigIdType;

typedef enum
{
    ICOM_SWITCH_E_OK = 0x00,
    ICOM_SWITCH_E_FAILED = 0x01
} IcomSwitch_ErrorType;

#endif /* COMSTACK_TYPES_H */
