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
**  FILENAME    : CanTrcv_Types.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public Types declared by CanTrcv module                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef CANTRCV_Types_H_
#define CANTRCV_Types_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "Spi.h"
#include "Dio.h"
#include "CanTrcv_Cfg.h"
#include "EcuM.h"
#include "Icu.h"
#include "Dem.h"

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* CanTrcv Initialization state */
typedef enum
{
    CANTRCV_NOT_ACTIVE = 0,
    CANTRCV_ACTIVE
} CanTrcv_StateType;

/* CanTrcv used Dio Interface Type */
typedef enum
{
    CANTRCV_DIO_PORT = 0,
    CANTRCV_DIO_CHANNEL,
    CANTRCV_DIO_CHANNELGROUP
} CanTrcv_HardwareInterfaceNameType;

typedef struct CanTrcv_PnFrameDataMaskSpec_T
{
    /* defines the n byte (Byte0 = LSB) of the data payload mask to be used on the received payload */
    uint8 CanTrcvPnFrameDataMask;
    /* holds the position n in frame of the mask-part */
    uint8 CanTrcvPnFrameDataMaskIndex;
} CanTrcv_PnFrameDataMaskSpecType;

typedef struct CanTrcv_PartialNetwork_T
{
    /* indicates the data transfer rate in kbps */
    uint16 CanTrcvBaudRate;
    /* indicates if the Bus Error (BUSERR) flag is managed by the BSW */
    boolean CanTrcvBusErrFlag;
    /* indicates whether extended or standard ID is used */
    boolean CanTrcvPnCanIdIsExtended;
    /* indicates whether the selective wake-up function is enabled or disabled in HW */
    boolean CanTrcvPnEnabled;
    /* CAN ID of the Wake-up Frame (WUF) */
    uint32 CanTrcvPnFrameCanId;
    /* ID Mask for the selective activation of the transceiver */
    uint32 CanTrcvPnFrameCanIdMask;
    /* data Length of the Wake-up Frame (WUF) */
    uint8 CanTrcvPnFrameDlc;
    /* Indicates if the Power On Reset (POR) flag is available and is managed by the transceiver */
    boolean CanTrcvPowerOnFlag;
    /* defines data payload mask to be used on the received payload to check if must be woken up by WUF */
    P2CONST(CanTrcv_PnFrameDataMaskSpecType, TYPEDEF, CANTRCV_CONST) CanTrcvPnFrameDataMaskSpec;
} CanTrcv_PartialNetworkType;

typedef struct CanTrcv_DioChannelAccess_T
{
    CanTrcv_HardwareInterfaceNameType CanTrcvHardwareInterfaceName;
    Dio_ChannelType CanTrcvSymNameRef;
} CanTrcv_DioChannelAccessType;

typedef struct CanTrcv_DioAccess_T
{
    P2CONST(CanTrcv_DioChannelAccessType, TYPEDEF, CANTRCV_CONST) CanTrcvDioChannelAccess;
} CanTrcv_DioAccessType;

typedef struct CanTrcv_SpiSequence_T
{
    /* This parameter is used to define whether the access to the Spi sequence is synchronous or asynchronous */
    P2CONST(boolean, TYPEDEF, CANTRCV_CONST) CanTrcvSpiAccessSynchronous;
    P2CONST(Spi_SequenceType, TYPEDEF, CANTRCV_CONST) CanTrcvSpiSequenceName;
} CanTrcv_SpiSequenceType;

typedef struct CanTrcv_SpiAccess_T
{
    P2CONST(CanTrcv_SpiSequenceType, TYPEDEF, CANTRCV_CONST) CanTrcvSpiSequence;
} CanTrcv_SpiAccessType;

typedef struct CanTrcv_Access_T
{
    P2CONST(CanTrcv_DioAccessType, TYPEDEF, CANTRCV_CONST) CanTrcvDioAccess;
    P2CONST(CanTrcv_SpiAccessType, TYPEDEF, CANTRCV_CONST) CanTrcvSpiAccess;
} CanTrcv_AccessType;

typedef struct CanTrcv_DemEventParameterRefs_T
{
    uint8 CanTrcvBusErrParam;
} CanTrcv_DemEventParameterRefsType;

typedef struct CanTrcv_Channel_T
{
    /* channel unique id */
    uint8 CanTrcvChannelId;
    /* whether to use this channel */
    boolean CanTrcvChannelUsed;
    /* whether ECU power supply controlled by this transceiver */
    boolean CanTrcvControlsPowerSupply;
    /* whether the HW supports the selective wake-up function */
    boolean CanTrcvHwPnSupport;
    /* state of CAN transceiver after call to CanTrcv_Init */
    CanTrcv_TrcvModeType CanTrcvInitState;
    /* the data transfer rate in kbps */
    uint16 CanTrcvMaxBaudrate;
#if (CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT)
    /* whether wake up by bus is supported and used */
    boolean CanTrcvWakeupByBusUsed;
#endif /* CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT */
    /* reference to the IcuChannel to enable/disable the interrupts for wakeups */
    P2CONST(Icu_ChannelType, TYPEDEF, CANTRCV_CONST) CanTrcvIcuChannelRef;
    /* reference to specify the wakeup sources that should be used in the calls to EcuM_SetWakeupEvent */
    P2CONST(EcuM_WakeupSourceType, TYPEDEF, CANTRCV_CONST) CanTrcvPorWakeupSourceRef;
    /* reference to specify the wakeup sources that should be used in the calls to EcuM_SetWakeupEvent */
    P2CONST(EcuM_WakeupSourceType, TYPEDEF, CANTRCV_CONST) CanTrcvSyserrWakeupSourceRef;
#if (CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT)
    /* reference to a wakeup source in the EcuM configuration */
    P2CONST(EcuM_WakeupSourceType, TYPEDEF, CANTRCV_CONST) CanTrcvWakeupSourceRef;
#endif /* CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT */
    /* gives information about access to a single CAN transceiver */
    CanTrcv_AccessType CanTrcvAccess;
    /* gives information about the configuration of Partial Networking functionality */
    P2CONST(CanTrcv_PartialNetworkType, TYPEDEF, CANTRCV_CONST) CanTrcvPartialNetwork;
    /* Refenrece to Dem Event Parameters */
    P2CONST(CanTrcv_DemEventParameterRefsType, TYPEDEF, CANTRCV_CONST) CanTrcvDemEventParameterRefs;
    /* Reference to CanIfTrcvId */
    P2CONST(uint8, TYPEDEF, CANTRCV_CONST) CanIfCanTrcvIdRef;
} CanTrcv_ChannelType;

/* config Type */
typedef struct CanTrcv_Config_T
{
    /* max number of communication retries for a failed SPI communication */
    uint8 CanTrcvSPICommRetries;
    /* max time allowed to the CanTrcv for replying to a SPI command */
    uint8 CanTrcvSPICommTimeout;
    /* information about a single CAN transceiver channel */
    P2CONST(CanTrcv_ChannelType, TYPEDEF, CANTRCV_CONST) CanTrcvChannel;
} CanTrcv_ConfigType;

#endif /* CANTRCV_TYPES_H_ */
