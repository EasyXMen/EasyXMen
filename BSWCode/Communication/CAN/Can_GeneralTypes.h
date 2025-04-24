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
*******************************************************************************
**                                                                           **
**  FILENAME   : Can_GeneralTypes.h                                          **
**                                                                           **
**  AUTHOR      :                                                            **
**                                                                           **
**  VENDOR      :                                                            **
**                                                                           **
**  DESCRIPTION :General type definitions for CAN bus                        **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
******************************************************************************/

#ifndef CAN_GENERALTYPES
#define CAN_GENERALTYPES

/* CAN id Type Mixed is supported */
typedef uint32 Can_IdType;

/*This type unites PduId (swPduHandle), SduLength (length), SduData (sdu), and
 * CanId (id) for any CAN L-SDU.*/
typedef struct
{
    PduIdType swPduHandle;
    uint8 length;
    Can_IdType id;
    P2VAR(uint8, TYPEDEF, TYPEDEF) sdu;
} Can_PduType;

/*This type defines a data structure which clearly provides an Hardware Object
 * Handle including its corresponding CAN Controller and therefore CanDrv as
 * well as the specific CanId*/
typedef struct
{
    Can_IdType CanId;     /*Standard/Extended CAN ID of CAN L-PDU*/
    Can_HwHandleType Hoh; /*ID of the corresponding Hardware Object Range*/
    uint8 ControllerId;   /*ControllerId provided by CanIf clearly identify
                           the corresponding controller*/
} Can_HwType;

/*State transitions that are used by the function CAN_SetControllerMode*/
typedef enum
{
    CAN_T_START, /*CAN controller transition value to request state STARTED*/
    CAN_T_STOP,  /*CAN controller transition value to request state STOPPED*/
    CAN_T_SLEEP, /*CAN controller transition value to request state SLEEP*/
    CAN_T_WAKEUP /*transition value to request state STOPPED from state SLEEP*/
} Can_StateTransitionType;

/*Return values of CAN driver API*/
typedef enum
{
    CAN_OK,     /*success*/
    CAN_NOT_OK, /*error occurred or wakeup event occurred during sleep transition*/
    CAN_BUSY    /*transmit request could not be processed because no transmit
                 object was available*/
} Can_ReturnType;

/*Data type used for describing whether PN wakeup functionality in CanTrcv is
 * enabled or disabled.*/
typedef enum
{
    PN_ENABLED, /*PN wakeup functionality in CanTrcv is enabled*/
    PN_DISABLED /*PN wakeup functionality in CanTrcv is disabled*/
} CanTrcv_PNActivationType;

/*Provides the state of a flag in the transceiver hardware*/
typedef enum
{
    CANTRCV_FLAG_SET,    /*The flag is set in the transceiver hardware*/
    CANTRCV_FLAG_CLEARED /*The flag is cleared in the transceiver hardware*/
} CanTrcv_TrcvFlagStateType;

/*Operating modes of the CAN Transceiver Driver*/
typedef enum
{
    CANTRCV_TRCVMODE_NORMAL = 0, /*Transceiver mode NORMAL*/
    CANTRCV_TRCVMODE_SLEEP,      /*Transceiver mode SLEEP*/
    CANTRCV_TRCVMODE_STANDBY     /*Transceiver mode STANDBY*/
} CanTrcv_TrcvModeType;

/*This type shall be used to control the CAN transceiver concerning wake up
 * events and wake up notifications*/
typedef enum
{
    CANTRCV_WUMODE_ENABLE = 0, /*notification for wakeup events enabled on the
                                addressed transceiver*/
    CANTRCV_WUMODE_DISABLE,    /*notification for wakeup events disabled on the
                                addressed transceiver*/
    CANTRCV_WUMODE_CLEAR       /*A stored wakeup event cleared on the addressed
                                transceiver*/
} CanTrcv_TrcvWakeupModeType;

/*This type denotes the wake up reason detected by the CAN transceiver in detail*/
typedef enum
{
    CANTRCV_WU_ERROR = 0,     /*Due to an error wake up reason was not detected.
                               This value may only be reported when error was
                               reported to DEM before*/
    CANTRCV_WU_NOT_SUPPORTED, /*The transceiver not support any information for
                               the wake up reason*/
    CANTRCV_WU_BY_BUS,        /*The transceiver has detected, that the network
                               has caused the wake up of the ECU*/
    CANTRCV_WU_INTERNALLY,    /*The transceiver has detected that the network
                              has woken up by the ECU via a request to NORMAL mode*/
    CANTRCV_WU_RESET,         /*The transceiver has detected, that the "wake up"
                               is due to an ECU reset*/
    CANTRCV_WU_POWER_ON,      /*The transceiver has detected, that the "wake up"
                               is due to an ECU reset after power on*/
    CANTRCV_WU_BY_PIN,        /*The transceiver has detected a wake-up event
                               at one of the transceiver's pins(not at the CAN bus)*/
    CANTRCV_WU_BY_SYSERR      /*The transceiver has detected, that the wake up
                              of the ECU was caused by a HW related device failure*/
} CanTrcv_TrcvWakeupReasonType;

#endif /*CAN_GENERALTYPES*/
