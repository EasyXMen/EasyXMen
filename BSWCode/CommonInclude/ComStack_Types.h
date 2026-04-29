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
 **  @file               : ComStack_Types.h
 **  @author             : darren.zhang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Specification of Communication Stack Types
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2025-03-05  darren.zhang        1. Update code according AUTOSAR R23-11 Specification
 *
 ==================================================================================================================== */
#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

/* =================================================== inclusions =================================================== */

#include "Std_Types.h"
#include "ComStack_Cfg.h"

/* ================================================ type definitions ================================================ */

/** @req <SWS_COMTYPE_00011> */
/**
 * @brief Variables of this type shall be used to store the basic information
 * about a PDU of any type, namely a pointer variable pointing to its SDU (payload),
 * and the corresponding length of the SDU in bytes.
 */
typedef struct
{
    uint8*        SduDataPtr;
    uint8*        MetaDataPtr;
    PduLengthType SduLength;
} PduInfoType;

/** @req <SWS_COMTYPE_00036> */
/**
 * @brief Used to store the identifier of a partial network cluster.
 */
typedef uint8 PNCHandleType;

/** @req <SWS_COMTYPE_00031> */
/**
 * @brief Specify the parameter to which the value has to be changed (BS or STmin).
 */
typedef enum
{
    TP_STMIN = 0x00, /*Separation Time*/
    TP_BS    = 0x01, /*Block Size*/
    TP_BC    = 0x02  /*The Band width control parameter used in FlexRay transport protocol module.*/
} TPParameterType;

/** @req <SWS_COMTYPE_00012> */
/**
 * @brief Variables of this type shall be used to store the result of a buffer request.
 */
typedef enum
{
    BUFREQ_OK       = 0x00, /*Buffer request accomplished successful.*/
    BUFREQ_E_NOT_OK = 0x01, /*Buffer request not successful. Buffer cannot be accessed*/
    BUFREQ_E_BUSY =
        0x02,            /*Temporarily no buffer available. It's up the requester to retry request for a certain time.*/
    BUFREQ_E_OVFL = 0x03 /*No Buffer of the required length can be provided*/
} BufReq_ReturnType;

/** @req <SWS_COMTYPE_00027> */
/**
 * @brief Variables of this type shall be used to store the state of TP buffer.
 */
typedef enum
{
    TP_DATACONF = 0x00, /*Indicates that all data, that have been copied so far, are confirmed and can be removed from
                           the TP buffer.*/
    TP_DATARETRY =
        0x01, /*indicates that this API call shall copy already copied data in order to recover from an error.*/
    TP_CONFPENDING = 0x02 /*indicates that the previously copied data must remain in the TP.*/
} TpDataStateType;

/** @req <SWS_COMTYPE_00037> */
/**
 * @brief Variables of this type shall be used to store the information about Tp buffer handling.
 */
typedef struct
{
    TpDataStateType TpDataState; /*The enumeration type to be used to store the state of Tp buffer.*/
    PduLengthType
        TxTpDataCnt; /*Offset from the current position which identifies the number of bytes to be retransmitted.*/
} RetryInfoType;

/** @req <SWS_COMTYPE_00038> */
/**
 * @brief Variables of the type NetworkHandleType shall be used to store the identifier of a communication channel.*/
typedef uint8 NetworkHandleType;

/** @req <SWS_COMTYPE_00039> */
/**
 * @brief IcomConfigIdType defines the configuration ID. An ID of 0 is the default configuration. An ID greater than 0
 * shall identify a configuration for Pretended Networking. There is more than 1 configuration possible.
 * r23 shall be delete
 */
typedef uint8 IcomConfigIdType;

/** @req <SWS_COMTYPE_00040> */
/**
 * @brief IcomSwitch_ErrorType defines the errors which can occur when activating or deactivating Pretended Networking
 * r23 shall be delete
 */
typedef enum
{
    ICOM_SWITCH_E_OK     = 0x00,
    ICOM_SWITCH_E_FAILED = 0x01
} IcomSwitch_ErrorType;

/** @req <SWS_COMTYPE_91001> */
/**
 * @brief Definition of datatype CbkHandleIdType
 */
typedef uint16 CbkHandleIdType;

/** @req <SWS_COMTYPE_91004> */
/**
 * @brief Depending on the HW, quality information regarding the evaluated timestamp might be supported.
 */
typedef enum
{
    TSQ_VALID     = 0x00, /*Timestamp is valid.*/
    TSQ_INVALID   = 0x01, /*Timestamp is invalid.*/
    TSQ_UNCERTAIN = 0x02  /*Status of timestamp is uncertain.*/
} TimeStampQualType;

/** @req <SWS_COMTYPE_91003> */
/**
 * @brief Variables of this type are used for expressing time stamps including relative time and absolute calendar time
 */
typedef struct
{
    uint32 nanoseconds; /* Nanoseconds part of the time */
    uint32 seconds;     /* 32 bit LSB of the 48 bits Seconds part of the time */
    uint16 secondsHi;   /* 16 bit MSB of the 48 bits Seconds part of the time */
} TimeStampType;

/** @req <SWS_COMTYPE_91002> */
/**
 * @brief The Time Tuple represents the clock values of two related HW clocks.
 */

typedef struct
{
    TimeStampType     timestampClockValue;   /* Value of the clock, which is used of ingress/egress timestamping */
    TimeStampType     disciplinedClockValue; /* Value of the adjustable HW clock */
    TimeStampQualType timeQuality;           /* Status of time tuple */
} TimeTupleType;

/** @req <SWS_COMTYPE_91005> */
/**
 * @brief This type defines one element of a single linked list.
 */
typedef struct ListElemStructTypeTag
{
    uint16                        DataLength;      /* Represents length of data */
    uint8*                        DataPtr;         /*Represents pointer to data.*/
    struct ListElemStructTypeTag* NextListElemPtr; /*Pointer to next list element.*/
} ListElemStructType;

/** @req <SWS_COMTYPE_91007> */
/**
 * @brief Type that indicates the current status of the rate calculation.
 */
typedef enum
{
    RATE_OK            = (uint8)0,    /* A valid rate deviaton value is available/calculated */
    RATE_NOT_AVAILABLE = (uint8)0xFE, /* No valid rate deviation value available/calculated */
    RATE_EXCEEDED      = (uint8)0xFF, /* The calculated rate deviation value exceeds limits */
} RateDeviationStatusType;

/** @req <SWS_COMTYPE_91006> */
/**
 * @brief Rate deviation value and status
 */
typedef struct
{
    sint32                  rateDeviationValue;  /* Rate deviation value */
    RateDeviationStatusType rateDeviationStatus; /* Current state of the rate deviation calculation */
} RateDeviationWithStatusType;
#endif
