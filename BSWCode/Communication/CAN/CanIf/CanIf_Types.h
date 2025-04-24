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
**  FILENAME    : CanIf_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of CANIF                                   **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/
#ifndef CANIF_TYPES_H
#define CANIF_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "CanIf_Cfg.h"
#include "CanIf_PBcfg.h"
#if (STD_ON == CANIF_WAKE_UP_SUPPORT)
#include "EcuM.h"
#endif

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(CANIF_PUBLISHED_INFORMATION)
#define CANIF_PUBLISHED_INFORMATION
#define CANIF_MODULE_ID                   60u
#define CANIF_VENDOR_ID                   62u
#define CANIF_AR_RELEASE_MAJOR_VERSION    4u
#define CANIF_AR_RELEASE_MINOR_VERSION    5u
#define CANIF_AR_RELEASE_REVISION_VERSION 0u
#define CANIF_SW_MAJOR_VERSION            2u
#define CANIF_SW_MINOR_VERSION            2u
#define CANIF_SW_PATCH_VERSION            3u
#elif ((CANIF_SW_MAJOR_VERSION != 2u) || (CANIF_SW_MINOR_VERSION != 2u) || (CANIF_SW_PATCH_VERSION != 3u))
#error "CanIf: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Local Data Types                                      **
*******************************************************************************/
/* max extended can id */
#define CANIF_EXTENDED_CANID_MAX (0x1FFFFFFFu)
/* max standard can id */
#define CANIF_STANDARD_CANID_MAX (0x7FFu)
/* Can Fd flag frame mask */
#define CANIF_CANID16_CANFD_MASK (0x4000u)
#define CANIF_CANID32_CANFD_MASK (0x40000000u)
/* Can extended frame mask */
#define CANIF_CANID_EXTENDED_MASK (0x80000000u)

#define CANIF_UNUSED_UINT32       (0xFFFFFFFFu)
#define CANIF_UNUSED_UINT16       (0xFFFFu)
#define CANIF_UNUSED_UINT8        (0xFFu)

typedef uint32 CanIf_IdType;

/* USER define data structure,TxPDU structure */
typedef struct
{
    boolean used;
    uint8 length;
    PduIdType TxPduIndex;
    Can_IdType canId;
    uint8* data;
} CanIf_TxPduBufferType;

/* USER define data structure,RxPDU structure */
typedef struct
{
    /* length */
    uint8 length;
    /* data */
    uint8* data;
    CanIf_IdType metaData;
} CanIf_RxPduBufferType;

typedef struct
{
    P2VAR(CanIf_TxPduBufferType, AUTOMATIC, CANIF_CONST) CanIf_TxPduBufferRef;
} CanIf_TxBufferType;
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* define CANIF module initiation status */
typedef enum
{
    CANIF_INITED = 0u,
    CANIF_UNINIT
} CanIf_InitStatusType;

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
#define CAN_OK     E_OK
#define CAN_NOT_OK E_NOT_OK
#else /*The default version of can driver is 4.2.2*/
/* The type Can_ControllerStateType is used in CanIf with R19 version */
#define Can_ControllerStateType CanIf_ControllerModeType
#define CAN_CS_UNINIT           CANIF_CS_UNINIT
#define CAN_CS_STARTED          CANIF_CS_STARTED
#define CAN_CS_STOPPED          CANIF_CS_STOPPED
#define CAN_CS_SLEEP            CANIF_CS_SLEEP
typedef enum
{
    CANIF_CS_UNINIT = 0u,
    CANIF_CS_STARTED,
    CANIF_CS_STOPPED,
    CANIF_CS_SLEEP
} CanIf_ControllerModeType;
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */

/* Return value of CAN L-PDU notification status */
typedef enum
{
    CANIF_NO_NOTIFICATION = 0u, /**<No new event since the last read attempt*/
    CANIF_TX_RX_NOTIFICATION    /**<New Rx/Tx event since the last read*/
} CanIf_NotifStatusType;

typedef enum
{
/* extended 29 */
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
    CANIF_TX_EXTENDED_CAN = 0u,
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT */
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
    CANIF_TX_EXTENDED_FD_CAN,
#endif /* STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
    /* standard 11 */
#if (STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT)
    CANIF_TX_STANDARD_CAN,
#endif /* STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT */
#if (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)
    CANIF_TX_STANDARD_FD_CAN
#endif /* STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT */
} CanIf_TxPduCanIdMode;
typedef enum
{
/* extended 29 */
#if (STD_ON == CANIF_RX_EXTENDED_CAN_SUPPORT)
    CANIF_RX_EXTENDED_CAN = 0u,
#endif /* STD_ON == CANIF_RX_EXTENDED_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_EXTENDED_FD_CAN_SUPPORT)
    CANIF_RX_EXTENDED_FD_CAN,
#endif /* STD_ON == CANIF_RX_EXTENDED_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_EXTENDED_NO_FD_CAN_SUPPORT)
    CANIF_RX_EXTENDED_NO_FD_CAN,
#endif /* STD_ON == CANIF_RX_EXTENDED_NO_FD_CAN_SUPPORT */
    /* standard 11 */
#if (STD_ON == CANIF_RX_STANDARD_CAN_SUPPORT)
    CANIF_RX_STANDARD_CAN,
#endif /* STD_ON == CANIF_RX_STANDARD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_STANDARD_FD_CAN_SUPPORT)
    CANIF_RX_STANDARD_FD_CAN,
#endif /* STD_ON == CANIF_RX_STANDARD_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_STANDARD_NO_FD_CAN_SUPPORT)
    CANIF_RX_STANDARD_NO_FD_CAN
#endif /* STD_ON == CANIF_RX_STANDARD_NO_FD_CAN_SUPPORT */
} CanIf_RxPduCanIdMode;

/*Defines the HRH type*/
typedef enum
{
    CANIF_BASIC_CAN = 0u,
    CANIF_FULL_CAN
} CanIf_HohType;

/**
 * The PduMode of a channel defines its transmit or receive activity.
 * Communication direction (transmission and/or reception) of the channel can
 * be controlled separately or together by upper layers.
 */
typedef enum
{
    CANIF_OFFLINE = 0u,
    CANIF_TX_OFFLINE,
    CANIF_TX_OFFLINE_ACTIVE,
    CANIF_ONLINE
} CanIf_PduModeType;

typedef enum
{
    CANID_DYNAMIC = 0u,
    CANID_STATIC
} CanIfTxPduMode;

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
typedef enum
{
    CANIF_LINEAR = 0,
    CANIF_BINARY
} CanIf_SoftwareFilterType;
#endif /* CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY */

typedef struct
{
    /*CanIf uniform definition all CanTrcv Id*/
    uint8 CanIfTrcvId;
    /*the id of TrcvDriver*/
    uint8 CanTrcvDriverId;
    /*Support wake up or not*/
    boolean CanIfTrcvWakeupSupport;
    /*Symbolic name reference to CanTrcvChannel,the lower Trcv id*/
    uint8 CanTrcvId;
#if (STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT)
    EcuM_WakeupSourceType CanIfWakeUpSource;
#endif
} CanIf_TrcvCfgType;

typedef struct
{
    /*CanIf uniform definition all controller Id*/
    uint8 CanIfCtrlId;
    /*the id of CanDriver*/
    uint8 CanDriverId;
    /* This parameter defines if a respective controller of the referenced CAN Driver modules is queriable for wake up
     * events*/
    boolean CanIfCtrlWakeupSupport;
/*if more than zero Tx L-PDUs per CAN Controller are configured as CanIfTxPduPnFilterPdu is TRUE*/
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
    boolean CanIfPnFilter;
#endif
    /*Symbolic name reference to CanCtrl,the lower CanCtrl id*/
    uint8 CanCtrlId;
#if (STD_ON == CANIF_WAKE_UP_SUPPORT)
    /*Controller WakeUpSource | Controller Ref Trcv WakeUpSource*/
    EcuM_WakeupSourceType CanIfWakeUpSource;
#endif
} CanIf_ControllerCfgType;

typedef struct
{
    /*Lower CAN Identifier of a receive CAN L-PDU for identifier range definition*/
    CanIf_IdType CanIfHrhRangeRxPduLowerCanId;
    /*Upper CAN Identifier of a receive CAN L-PDU for identifier range definition*/
    CanIf_IdType CanIfHrhRangeRxPduUpperCanId;
} CanIfHrhRangeType;

typedef struct
{
    /* Reference to controller Id to which the HRH belongs to*/
    uint8 CanIfHrhCanCtrlId;
#if (STD_ON == CANIF_HRH_RANGE_SUPPORT)
    uint8 CanIfHrhCanIdRangeNum;
#endif
    /*The parameter refers to a particular HRH object in the CanDrv configuration*/
    Can_HwHandleType CanObjectId;
    PduIdType CanIfRxPduRefNum;
    CanIf_HohType CanIfHrhType;
#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
    /* The parameter indicates which way the HRH choose to find RxPdu */
    CanIf_SoftwareFilterType CanIfSoftFilterType;
#endif /* CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY */
    P2CONST(PduIdType, CANIF_CONST, CANIF_CONST) CanIfRxPduIdRef;
#if (STD_ON == CANIF_HRH_RANGE_SUPPORT)
    P2CONST(CanIfHrhRangeType, CANIF_CONST, CANIF_CONST) CanIfHrhCanIdRange;
#endif
} CanIfHrhCfgType;

typedef struct
{
    /* Reference to controller Id to which the HTH belongs to*/
    uint8 CanIfHthCanCtrlId;
    /*The parameter refers to a particular HTH object in the CanDrv configuration*/
    Can_HwHandleType CanObjectId;
    CanIf_HohType CanIfHthType;
} CanIfHthCfgType;

typedef void (*CanIfTxPduUserTxConfirmationNameApiType)(PduIdType TxPduId);
typedef Std_ReturnType (*CanIfTxPduUserTriggerTransmitNameApiType)(PduIdType TxPduId, PduInfoType* PduInfoPtr);
typedef void (*CanIfRxPduUserRxIndicationNameApiType)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#if (CANIF_RXINDICATION_FUNC_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_RxApiNumType;
#else
typedef PduIdType CanIf_RxApiNumType;
#endif /* CANIF_RXINDICATION_FUNC_NUMBER < CANIF_UNUSED_UINT8 */

#if (CANIF_TXCONFIRMATION_FUNC_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_TxConfApiNumType;
#else
typedef PduIdType CanIf_TxConfApiNumType;
#endif /* CANIF_TXCONFIRMATION_FUNC_NUMBER < CANIF_UNUSED_UINT8 */

#if ((STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT) && (CANIF_TRIGGER_TRANSMIT_FUNC_NUMBER < CANIF_UNUSED_UINT8))
typedef uint8 CanIf_TxTrigApiNumType;
#else
typedef PduIdType CanIf_TxTrigApiNumType;
#endif /* STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT && CANIF_TRIGGER_TRANSMIT_FUNC_NUMBER < CANIF_UNUSED_UINT8 */

/*  contains the configur ation (parameters) of each transmit CAN L-PDU */
typedef struct
{
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
    /* If CanIfPublicPnFilterSupport is enabled, by this parameter PDUs could be configured which will pass the
     * CanIfPnFilter */
    boolean CanIfTxPduPnFilterPdu;
#endif
    boolean TxMetaDataEnable;
    PduLengthType CanIfTxPduDlc; /*take from Ecu config*/
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
    /* Enables and disables transmit confirmation for each transmit CAN L-SDU for reading its notification status. */
    boolean CanIfTxPduReadNotifyStatus;
#endif
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
    /*Determines if or if not CanIf shall use the trigger transmit API for this PDU. Will be removed, with
     * CanIfTxPduUserTriggerTransmitName repeat */
    boolean CanIfTxPduTriggerTransmit;
    /*This parameter defines the index for the name of the <User_TriggerTransmit> in array
     * CanIf_UpTriggerTransmitArray*/
    CanIf_TxTrigApiNumType CanIfUpTxTrigApiIndex;
#endif /* STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT */
    /*This parameter defines the index for the name of the <User_TxConfirmation> in array CanIf_UpTxConfirmationArray*/
    CanIf_TxConfApiNumType CanIfUpTxConfApiIndex;
    /* from the configuration parameter CanIfTxPduBufferRef */
    Can_HwHandleType CanIfTxPduHthId;
    /*the up layer pdu id.*/
    PduIdType CanIfUpPduId;
    /*Tx buffer index for CanIf_TxPduBuffer,not support the defaut data is CANIF_PDUID_TYPE_INVALID*/
    PduIdType TxBufferIdndex;
    /*If CanIfTxPduType is CANID_DYNAMIC,Dynamic CanId index for CanIf_DynamicTxPduCanId buffer*/
    PduIdType CanIfDynamicCanIdIndex;
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
    /*if CanIfTxPduReadNotifyStatus is TRUE ,then this pdu has a Notify buffer,mark the buffer index*/
    PduIdType CanIfTxNotifyIndex;
#endif
    /* CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-PDU transmission */
    CanIf_IdType CanIfTxPduCanId; /*if no config,the default data is 0xffffffff*/
    /* Identifier mask which denotes relevant bits in the CAN Identifier.*/
    CanIf_IdType CanIfTxPduCanIdMask; /*when mask is 0x00000000,the tx canid is all metadata,0-0x1fffffff*/
    /* Defines the type of each transmit CAN L-PDU */
    CanIf_TxPduCanIdMode CanIfTxPduCanIdType;
    /*Defines the type of each transmit CAN L-PDU*/
    CanIfTxPduMode CanIfTxPduType;
} CanIf_TxPduConfigType;

#if (STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT)
typedef struct
{
    CanIf_IdType CanIfRxPduLowerCanId;
    CanIf_IdType CanIfRxPduUpperCanId;
} CanIfRxCanIdRangeRefType;
#endif

#if (CANIF_RXBUFFER_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_RxBufferNumType;
#else
typedef PduIdType CanIf_RxBufferNumType;
#endif /* CANIF_RXBUFFER_NUMBER < CANIF_UNUSED_UINT8 */

/* contains the configur ation (parameters) of each receive CAN L-PDU */
typedef struct
{
    boolean CanIfRxPduForNM;
    boolean RxMetaDataEnable;
#if CANIF_PRIVATE_DLC_CHECK == STD_ON
    /*used for DLC check*/
    boolean CanIfRxPduDataLengthCheck;
#endif /* CANIF_PRIVATE_DLC_CHECK == STD_ON */
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
    /* Enables and disables receive indication for each receive CAN L-SDU for reading its notification status. */
    boolean CanIfRxPduReadNotifyStatus;
#endif
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
    /*CanIfRxPduReadData is TRUE,the CanIf_RxBufferIndex is RxBuffer index*/
    CanIf_RxBufferNumType CanIf_RxBufferIndex;
#endif
    /*This parameter defines the index for the name of the <User_RxIndication> in array CanIf_UpRxIndicationArray*/
    CanIf_RxApiNumType CanIfUpRxIndicationApiIndex;
    /* The HRH to which Rx L-PDU belongs to, is referred through this parameter. */
    Can_HwHandleType
        CanIfRxPduHrhId; /*dependency: This information has to be derived from the CAN Driver configuration.*/
    /*the up layer pdu id.*/
    PduIdType CanIfUpPduId;
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
    /*if CanIfRxPduReadNotifyStatus is TRUE ,then this pdu has a Notify buffer,mark the buffer index*/
    PduIdType CanIfRxNotifyIndex;
#endif
    /* Data length of the received CAN L-PDUs used by the CAN Interface */
    PduLengthType CanIfRxPduDlc; /*the receive length need >= the Dlc is OK*/
    /* CAN ID Masked */
    CanIf_IdType CanIfRxPduCanIdMasked;
    /* Identifier mask which denotes relevant bits in the CAN Identifier*/
    CanIf_IdType CanIfRxPduCanIdMask; /*0-0x1fffffff*/
    /* CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN L-PDU reception.*/
    CanIf_RxPduCanIdMode CanIfRxPduCanIdType;
#if (STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT)
    CanIfRxCanIdRangeRefType* CanIfRxCanIdRangeRef;
#endif
} CanIf_RxPduConfigType;

/* Callout functions with respect to the upper layers */
typedef struct
{
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
    /* CanSM_CheckTransceiverWakeFlagIndication */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserCheckTrcvWakeFlagIndicationName)
    (uint8 Transceiver);
    /* CanSM_ClearTrcvWufFlagIndication */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserClearTrcvWufFlagIndicationName)(uint8 Transceiver);
    /* CanSM_ConfirmPnAvailability */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserConfirmPnAvailabilityName)(uint8 TransceiverId);
#endif
    /* CanSM_ControllerBusOff */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserCtrlBusOffName)(uint8 ControllerId);
    /* CanSM_ControllerModeIndication */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserCtrlModeIndicationName)
    (uint8 ControllerId, Can_ControllerStateType ControllerMode);
    /* CanSM_TransceiverModeIndication */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserTrcvModeIndicationName)
    (uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
    /* EcuM_ValidateWakeupEvent */
    P2FUNC(void, CANIF_APPL_CODE, CanIfDispatchUserValidateWakeupEventName)
    (EcuM_WakeupSourceType sources);
#endif
} CanIf_DispatchConfigType;

/**
 * This type defines a data structure for the post build parameters of the CAN
 * interface for all underlying CAN drivers. At initialization the CanIf gets a
 * pointer to a structure of this type to get access to its configuration data, which
 * is necessary for initialization.
 */
typedef struct
{
    PduIdType CanIfTxBufferSize;
    PduIdType CanIfRxPduNumber;
    PduIdType CanIfTxPduNumber;
    Can_HwHandleType CanIfHrhNumber;
    Can_HwHandleType CanIfHthNumber;
    P2CONST(CanIfHrhCfgType, AUTOMATIC, CANIF_APPL_CONST) CanIfHrhCfgRef;
    P2CONST(CanIfHthCfgType, AUTOMATIC, CANIF_APPL_CONST) CanIfHthCfgRef;
    P2CONST(CanIf_RxPduConfigType, AUTOMATIC, CANIF_CONST) CanIfRxPduConfigRef;
    P2CONST(CanIf_TxPduConfigType, AUTOMATIC, CANIF_CONST) CanIfTxPduConfigRef;
    P2CONST(uint8, AUTOMATIC, CANIF_CONST) CanIfBufferSize; /*CanIfBufferSize need > 0*/
} CanIf_ConfigType;

typedef struct
{
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanSetControllerModeApi) /* PRQA S 3432 */
    (uint8 Controller, Can_ControllerStateType Transition);
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanWriteApi) /* PRQA S 3432 */
    (Can_HwHandleType Hth, P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) PduInfo);
#else  /*The default version of can driver is 4.2.2*/
    P2FUNC(Can_ReturnType, CANIF_APPL_CODE, CanSetControllerModeApi)
    (uint8 Controller, Can_StateTransitionType Transition);
    P2FUNC(Can_ReturnType, CANIF_APPL_CODE, CanWriteApi)
    (Can_HwHandleType Hth, P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) PduInfo);
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
#if (STD_ON == CANIF_SET_BAUDRATE_API)
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanSetBaudrateApi)(uint8 Controller, uint16 BaudRateConfigID);
#endif /*STD_ON == CANIF_SET_BAUDRATE_API*/
#if (STD_ON == CANIF_PUBLIC_ICOM_SUPPORT)
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanSetIcomConfigurationApi)
    (uint8 Controller, IcomConfigIdType ConfigurationId);
#endif /*STD_ON == CANIF_PUBLIC_ICOM_SUPPORT*/
#if (STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT)
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanCheckWakeupApi)(uint8 Controller);
#else  /*The default version of can driver is 4.2.2*/
    P2FUNC(Can_ReturnType, CANIF_APPL_CODE, CanCheckWakeupApi)(uint8 Controller);
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
#endif /*STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT*/
} Can_DriverApiType;

typedef struct
{
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvSetOpModeApi)(uint8 Transceiver, CanTrcv_TrcvModeType OpMode);
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvGetOpModeApi)
    (uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANIF_CONST) OpMode);
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvGetBusWuReasonApi)
    (uint8 Transceiver, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_CONST) reason);
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvSetWakeupModeApi)
    (uint8 Transceiver, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvClearTrcvWufFlagApi)(uint8 Transceiver);
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvCheckWakeFlagApi)(uint8 Transceiver);
#endif
#if (STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT)
    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, CanTrcvCheckWakeupApi)(uint8 Transceiver);
#endif /*STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT*/
} Can_TrcvApiType;

typedef struct
{
    P2CONST(uint16, AUTOMATIC, CANIF_CONST) CanIfCanHoh2HrhRef;
    uint32 CanIfCanHohLen;
} CanIf_CanHOH2HRHType;

#endif /* end of CANIF_TYPES_H */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
