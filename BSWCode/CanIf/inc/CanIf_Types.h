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
/*
 ************************************************************************************************************************
 **
 **  @file               : CanIf_Types.h
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf header file for external type definitions
 **
 ***********************************************************************************************************************/

#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "CanIf_Cfg.h"
#include "CanIf_PBcfg.h"
#if (STD_ON == CANIF_WAKEUP_SUPPORT)
#include "EcuM.h"
#endif
#include "Os.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/**
 * @brief Set according to the situation when integrating project
 */
#define CANIF_RXINDICATION_AUTOSAR_PARAMETER STD_OFF

/**
 * @brief max extended CanId
 */
#define CANIF_EXTENDED_CANID_MAX (0x1FFFFFFFu)
/**
 * @brief max standard CanId
 */
#define CANIF_STANDARD_CANID_MAX (0x7FFu)
/**
 * @brief CAN FD flag frame mask for 16-bit CanId
 */
#define CANIF_CANID16_CANFD_MASK (0x4000u)
/**
 * @brief CAN FD flag frame mask for 32-bit CanId
 */
#define CANIF_CANID32_CANFD_MASK (0x40000000u)
/**
 * @brief CAN extended frame mask
 */
#define CANIF_CANID_EXTENDED_MASK (0x80000000u)

/**
 * @brief max value of uint8
 */
#define CANIF_UNUSED_UINT8 (0xFFu)
/**
 * @brief max value of uint16
 */
#define CANIF_UNUSED_UINT16 (0xFFFFu)
/**
 * @brief max value of uint32
 */
#define CANIF_UNUSED_UINT32 (0xFFFFFFFFu)

/* ================================================ type definitions ================================================ */
/**
 * @brief type of CanId
 */
typedef uint32 CanIf_IdType;

/**
 * @brief type of CanIf TxByteBuffer number
 */
#if (CANIF_TXBYTEBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_TxByteBufferNumberType;
#elif (CANIF_TXBYTEBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT16)
typedef uint16 CanIf_TxByteBufferNumberType;
#else
typedef uint32 CanIf_TxByteBufferNumberType;
#endif

/**
 * @brief type of CanIf TxPduBuffer number
 */
#if (CANIF_TXPDUBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_TxPduBufferNumberType;
#else
typedef uint16 CanIf_TxPduBufferNumberType;
#endif

/**
 * @brief type of CanIf TxBuffer number
 */
#if (CANIF_TXBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_TxBufferNumberType;
#define CANIF_TXBUFFER_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef uint16 CanIf_TxBufferNumberType;
#define CANIF_TXBUFFER_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif

/**
 * @brief type of CanIf RxByteBuffer number
 */
#if (CANIF_RXBYTEBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_RxByteBufferNumberType;
#elif (CANIF_RXBYTEBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT16)
typedef uint16 CanIf_RxByteBufferNumberType;
#else
typedef uint32 CanIf_RxByteBufferNumberType;
#endif

/**
 * @brief type of CanIf RxBuffer number
 */
#if (CANIF_RXBUFFER_MAX_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_RxBufferNumberType;
#define CANIF_RXBUFFER_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef uint16 CanIf_RxBufferNumberType;
#define CANIF_RXBUFFER_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif

#if (STD_ON == CANIF_HRH_CANID_RANGE_SUPPORT)
/**
 * @brief type of CanIf HRH CanId range number
 */
#if (CANIF_HRHCANIDRANGE_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_HrhRangeNumberType;
#define CANIF_HRHCANIDRANGE_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef uint16 CanIf_HrhRangeNumberType;
#define CANIF_HRHCANIDRANGE_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif
#endif

#if (STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT)
/**
 * @brief type of CanIf RxPdu CanId range number
 */
#if (CANIF_RXPDUCANIDRANGE_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_RxPduRangeNumberType;
#define CANIF_RXPDUCANIDRANGE_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef uint16 CanIf_RxPduRangeNumberType;
#define CANIF_RXPDUCANIDRANGE_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif
#endif

/**
 * @brief type of CanIf User Tx Confirmation API number
 */
#if (CANIF_UPTXCONFIRMATION_API_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_UpTxCfmApiNumberType;
#define CANIF_UPTXCONFIRMATION_API_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef unit16 CanIf_UpTxCfmApiNumberType;
#define CANIF_UPTXCONFIRMATION_API_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif

#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
/**
 * @brief type of CanIf User Trigger Transmit API number
 */
#if (CANIF_UPTRIGGERTRANSMIT_API_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_UpTrgTxApiNumberType;
#define CANIF_UPTRIGGERTRANSMIT_API_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef uint16 CanIf_UpTrgTxApiNumberType;
#define CANIF_UPTRIGGERTRANSMIT_API_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif
#endif

/**
 * @brief type of CanIf User Rx Indication number API number
 */
#if (CANIF_UPRXINDICATION_API_NUMBER < CANIF_UNUSED_UINT8)
typedef uint8 CanIf_UpRxIndApiNumberType;
#define CANIF_UPRXINDICATION_API_NUMBER_INVALID CANIF_UNUSED_UINT8
#else
typedef uint16 CanIf_UpRxIndApiNumberType;
#define CANIF_UPRXINDICATION_API_NUMBER_INVALID CANIF_UNUSED_UINT16
#endif

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
/**
 * @brief type of Can controller mode
 */
typedef enum CanIf_ControllerModeTag
{
    CANIF_CS_UNINIT,
    CANIF_CS_STARTED,
    CANIF_CS_STOPPED,
    CANIF_CS_SLEEP
} CanIf_ControllerModeType;
#endif

/**
 * @brief Return value of CAN L-PDU notification status
 */
typedef enum CanIf_NotifStatusTag
{
    CANIF_NO_NOTIFICATION,   /**<No new event since the last read attempt*/
    CANIF_TX_RX_NOTIFICATION /**<New Rx/Tx event since the last read*/
} CanIf_NotifStatusType;

/**
 * @brief type of CanIf TxPdu CanId mode
 */
typedef enum CanIf_TxPduCanIdModeTag
{
    CANIF_TXPDU_CANID_MODE_NONE,
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
    CANIF_TX_EXTENDED_CAN,
#endif
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
    CANIF_TX_EXTENDED_FD_CAN,
#endif
#if (STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT)
    CANIF_TX_STANDARD_CAN,
#endif
#if (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)
    CANIF_TX_STANDARD_FD_CAN
#endif
} CanIf_TxPduCanIdModeType;

/**
 * @brief type of CanIf RxPdu CanId mode
 */
typedef enum CanIf_RxPduCanIdModeTag
{
    CANIF_RXPDU_CANID_MODE_NONE,
#if (STD_ON == CANIF_RX_EXTENDED_CAN_SUPPORT)
    CANIF_RX_EXTENDED_CAN,
#endif
#if (STD_ON == CANIF_RX_EXTENDED_FD_CAN_SUPPORT)
    CANIF_RX_EXTENDED_FD_CAN,
#endif
#if (STD_ON == CANIF_RX_EXTENDED_NO_FD_CAN_SUPPORT)
    CANIF_RX_EXTENDED_NO_FD_CAN,
#endif
#if (STD_ON == CANIF_RX_STANDARD_CAN_SUPPORT)
    CANIF_RX_STANDARD_CAN,
#endif
#if (STD_ON == CANIF_RX_STANDARD_FD_CAN_SUPPORT)
    CANIF_RX_STANDARD_FD_CAN,
#endif
#if (STD_ON == CANIF_RX_STANDARD_NO_FD_CAN_SUPPORT)
    CANIF_RX_STANDARD_NO_FD_CAN
#endif
} CanIf_RxPduCanIdModeType;

/**
 * @brief type of CanIf HOH type
 */
typedef enum CanIf_HohTypeTag
{
    CANIF_BASIC_CAN,
    CANIF_FULL_CAN
} CanIf_HohTypeType;

/**
 * @brief The PduMode of a channel defines its transmit or receive activity.
 *        Communication direction (transmission and/or reception) of the channel
 *        can be controlled separately or together by upper layers.
 */
typedef enum CanIf_PduModeTag
{
    CANIF_OFFLINE,
    CANIF_TX_OFFLINE,
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
    CANIF_TX_OFFLINE_ACTIVE,
#endif
    CANIF_ONLINE
} CanIf_PduModeType;

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
/**
 * @brief type of CanIf Software Filter type
 */
typedef enum CanIf_SoftwareFilterTypeTag
{
    CANIF_LINEAR,
    CANIF_BINARY
} CanIf_SoftwareFilterTypeType;
#endif

/**
 * @brief type of Can controller's runtime state
 */
typedef struct CanIf_CtrlRuntimeTag
{
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
    boolean PnTxFilter;
#endif
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
    boolean FirstCallRxInd;
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
    CanIf_NotifStatusType TxConfirmationState;
#endif
    CanIf_PduModeType       PduMode;
    Can_ControllerStateType ControllerMode;
} CanIf_CtrlRuntimeType;

/**
 * @brief type of CanIf HTH configuration
 */
typedef struct CanIf_HthConfigTag
{
    /* Reference to controller Id to which the HTH belongs to*/
    uint8 CanIfHthCanCtrlId;
    /*The parameter refers to a particular HTH object in the CanDrv configuration*/
    Can_HwHandleType  CanObjectId;
    CanIf_HohTypeType CanIfHthType;
} CanIf_HthConfigType;

/**
 * @brief type of CanIf HRH configuration
 */
typedef struct CanIf_HrhConfigTag
{
    /* Reference to controller Id to which the HRH belongs to*/
    uint8 CanIfHrhCanCtrlId;
#if (STD_ON == CANIF_HRH_CANID_RANGE_SUPPORT)
    CanIf_HrhRangeNumberType HrhCanIdRangeIndex;
    CanIf_HrhRangeNumberType HrhCanIdRangeNumber;
#endif
    /*The parameter refers to a particular HRH object in the CanDrv configuration*/
    Can_HwHandleType  CanObjectId;
    PduIdType         RxPduIndexMin;
    PduIdType         RxPduIndexMaxExcluded;
    CanIf_HohTypeType CanIfHrhType;
#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
    /* The parameter indicates which way the HRH choose to find RxPdu */
    CanIf_SoftwareFilterTypeType CanIfSoftFilterType;
#endif
} CanIf_HrhConfigType;

/**
 * @brief type of CanIf Dynamic TxPdu runtime state
 */
typedef struct CanIf_DynamicTxPduTag
{
    CanIf_IdType CanId;
} CanIf_DynamicTxPduType;

/**
 * @brief type of User Tx Confirmation runtime state
 */
typedef struct CanIf_UpConfirmTxPduTag
{
    uint8 UpConfirmStatus;
} CanIf_UpConfirmTxPduType;

/**
 * @brief type of CanIf TxPdu configuration
 */
typedef struct CanIf_TxPduConfigTag
{
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
    boolean CanIfTxPduPnFilterPdu;
#endif
    boolean TxMetaDataEnable;
    boolean CanIfTxPduTruncation;
    uint8   UpConfirmTxPduMask;
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
    /*This parameter defines the index for the name of the <User_TriggerTransmit> in array
     * CanIf_UpTriggerTransmitArray*/
    CanIf_UpTrgTxApiNumberType UpTriggerTransmitApiIndex;
#endif
    /*This parameter defines the index for the name of the <User_TxConfirmation> in array CanIf_UpTxConfirmationArray*/
    CanIf_UpTxCfmApiNumberType UpTxConfirmationApiIndex;
    /* Tx buffer index for CanIf_TxPduBuffer, not support the defaut data is 0xffff*/
    CanIf_TxBufferNumberType TxBufferIndex;
    /* from the configuration parameter CanIfTxPduBufferRef */
    Can_HwHandleType CanIfTxPduHthId;
    /*the up layer pdu id.*/
    PduIdType     CanIfUpPduId;
    PduLengthType CanIfTxPduDlc;
#if (STD_ON == CANIF_PARTITION_USED)
    ApplicationType PartitionIndex;
#endif
    /* CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-PDU transmission */
    CanIf_IdType CanIfTxPduCanId; /*if no config,the default data is 0xffffffff*/
    /* Identifier mask which denotes relevant bits in the CAN Identifier.*/
    CanIf_IdType CanIfTxPduCanIdMask; /*when mask is 0x00000000,the tx canid is all metadata,0-0x1fffffff*/
    /* Defines the type of each transmit CAN L-PDU */
    CanIf_TxPduCanIdModeType CanIfTxPduCanIdType;
    CanIf_DynamicTxPduType*  DynamicTxPduPtr;
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
    /* Enables and disables transmit confirmation for each transmit CAN L-SDU for reading its notification status. */
    CanIf_NotifStatusType* TxNotifStatusPtr;
#endif
    CanIf_UpConfirmTxPduType* UpConfirmTxPduPtr;
} CanIf_TxPduConfigType;

/**
 * @brief type of CanIf RxPdu configuration
 */
typedef struct CanIf_RxPduConfigTag
{
    boolean CanIfRxPduForNM;
    boolean RxMetaDataEnable;
    /*This parameter defines the index for the name of the <User_RxIndication> in array CanIf_UpRxIndicationArray*/
    CanIf_UpRxIndApiNumberType UpRxIndicationApiIndex;
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
    CanIf_RxBufferNumberType RxBufferIndex;
#endif
#if (STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT)
    CanIf_RxPduRangeNumberType RxPduCanIdRangeIndex;
#endif
    /* The HRH to which Rx L-PDU belongs to, is referred through this parameter. */
    /*dependency: This information has to be derived from the CAN Driver configuration.*/
    Can_HwHandleType CanIfRxPduHrhId;
    /*the up layer pdu id.*/
    PduIdType CanIfUpPduId;
#if (STD_ON == CANIF_PARTITION_USED)
    ApplicationType PartitionIndex;
#endif
#if (STD_ON == CANIF_PRIVATE_DLC_CHECK)
    /* Data length of the received CAN L-PDUs used by the CAN Interface */
    PduLengthType CanIfRxPduDlc; /*the receive length need >= the Dlc is OK*/
#endif
    /* CAN ID Masked */
    CanIf_IdType CanIfRxPduCanIdMasked;
    /* Identifier mask which denotes relevant bits in the CAN Identifier*/
    CanIf_IdType CanIfRxPduCanIdMask; /*0-0x1fffffff*/
    /* CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN L-PDU reception.*/
    CanIf_RxPduCanIdModeType CanIfRxPduCanIdType;
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
    /* Enables and disables receive indication for each receive CAN L-SDU for reading its notification status. */
    CanIf_NotifStatusType* RxNotifStatusPtr;
#endif
} CanIf_RxPduConfigType;

/**
 * @brief type of CanIf buffer configuration
 */
typedef struct CanIf_BufferConfigTag
{
    uint8 PduLength;
    uint8 BufferSize;
} CanIf_BufferConfigType;

/**
 * @brief type of CanIf buffer configuration for each partition
 */
typedef struct CanIf_BufferConfigPartitionTag
{
    PduIdType                     BufferConfigNumber;
    const CanIf_BufferConfigType* BufferConfigRef;
} CanIf_BufferConfigPartitionType;

/**
 * @brief type of Can controller post-build configuration
 */
typedef struct CanIf_CtrlPbConfigTag
{
    PduIdType              RxPduIndexMin;
    PduIdType              RxPduIndexMaxExcluded;
    PduIdType              TxPduIndexMin;
    PduIdType              TxPduIndexMaxExcluded;
    ApplicationType        PartitionIndex;
    CanIf_CtrlRuntimeType* RuntimePtr;
} CanIf_CtrlPbConfigType;

/**
 * @brief type of CanTrcv post-build configuration
 */
typedef struct CanIf_TrcvPbConfigTag
{
    ApplicationType PartitionIndex;
} CanIf_TrcvPbConfigType;

/**
 * @brief type of CAN HOH-to-HRH mapping
 */
typedef struct CanIf_CanHoh2HrhTag
{
    Can_HwHandleType        CanHrhIdNumber;
    const Can_HwHandleType* CanHrhIdRef;
} CanIf_CanHoh2HrhType;

/**
 * @brief This type defines a data structure for the post build parameters
 *        of the CAN interface for all underlying CAN drivers. At initialization
 *        the CanIf gets a pointer to a structure of this type to get access to
 *        its configuration data, which is necessary for initialization.
 */
typedef struct CanIf_ConfigTag
{
    /* number of HthConfigRef */
    Can_HwHandleType HthConfigNumber;
    /* number of HrhConfigRef */
    Can_HwHandleType HrhConfigNumber;
    /* number of TxPduConfigRef */
    PduIdType TxPduConfigNumber;
    /* number of RxPduConfigRef */
    PduIdType RxPduConfigNumber;
    /* pointer to CanIfHthCfg */
    const CanIf_HthConfigType* HthConfigRef;
    /* pointer to CanIfHrhCfg */
    const CanIf_HrhConfigType* HrhConfigRef;
    /* pointer to CanIfTxPduCfg */
    const CanIf_TxPduConfigType* TxPduConfigRef;
    /* pointer to CanIfRxPduCfg */
    const CanIf_RxPduConfigType* RxPduConfigRef;
    /* pointer to CanIfBufferCfg for Tx */
    const CanIf_BufferConfigPartitionType* TxBufferConfigRef; /* number == CANIF_PARTITION_NUMBER */
    /* pointer to CanIfBufferCfg for Rx */
    const CanIf_BufferConfigPartitionType* RxBufferConfigRef; /* number == CANIF_PARTITION_NUMBER */
    /* pointer to post-build controller configuration */
    const CanIf_CtrlPbConfigType* CtrlPbConfigRef; /* number == CANIF_CANCONTROLLER_NUMBER */
#if (CANIF_TRCV_NUMBER > 0u)
    /* pointer to post-build transceiver configuration */
    const CanIf_TrcvPbConfigType* TrcvPbConfigRef; /* number == CANIF_TRCV_NUMBER */
#endif
#if (STD_OFF == CANIF_RXINDICATION_AUTOSAR_PARAMETER)
    /* pointer to CAN HOH-to-HRH mapping */
    const CanIf_CanHoh2HrhType* CanHoh2HrhRef; /* number == CANIF_CANDRIVER_NUMBER */
#endif
#if (CANIF_VARIANT_NUMBER > 1u)
    /* pointer to TxPdu id-to-index mapping */
    const PduIdType* TxPduIdVariantIndexRef;
    /* pointer to RxPdu id-to-index mapping */
    const PduIdType* RxPduIdVariantIndexRef;
#endif
} CanIf_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
