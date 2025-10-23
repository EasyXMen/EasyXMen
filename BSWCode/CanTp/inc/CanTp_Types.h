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
 **  @file               : CanTp_Types.h
 **  @author             : xue.han
 **  @date               : 2024/08/15
 **  @vendor             : isoft
 **  @description        : CAN Transport Layer Types header
 **
 ***********************************************************************************************************************/

#ifndef CANTP_TYPES_H_
#define CANTP_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "CanTp_Cfg.h"
#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/**
 * @brief This macro definition represents an internal function of the CanTp module
 */
#ifndef CANTP_LOCAL
#define CANTP_LOCAL static /**< represents an internal function */ /* PRQA S 3414 */ /* VL_CanTp_3414 */
#endif

/**
 * @brief This macro definition represents the CanTp module inline function
 */
#ifndef CANTP_LOCAL_INLINE
#define CANTP_LOCAL_INLINE static inline /**< represents the CanTp module inline function  */
#endif

/**
 * @brief This macro definition represents the CanTp module unused parameter
 */
#ifndef CANTP_PARAM_UNUSED
#define CANTP_PARAM_UNUSED(a) (void)(a) /**< represents the CanTp module unused parameter */
#endif

/**
 * @name CanTpDevelopmentErrorsMacros
 * @{
 * @brief Development errors used by the CanTp module.
 */
#define CANTP_E_PARAM_CONFIG  (0x01u) /**< error occurs when the parameter configuration is valid or inconsistent. */
#define CANTP_E_PARAM_ID      (0x02u) /**< error occurs when the parameter ID is invalid. */
#define CANTP_E_PARAM_POINTER (0x03u) /**< error occurs when the parameter is a null pointer. */
#define CANTP_E_INIT_FAILED   (0x04u) /**< error occurs when initialization fails.*/
#define CANTP_E_UNINIT        (0x20u) /**< error occurs without initialization */
#define CANTP_E_INVALID_TX_ID (0x30u) /**< error occurs when TxNSdu ID is invalid. */
#define CANTP_E_INVALID_RX_ID (0x40u) /**< error occurs when RxNSdu ID is invalid */
#define CANTP_E_NO_ERROR      (0xFFu) /**< no error occurs*/
/** @} */

/**
 * @name CanTpRuntimeErrorsMacros
 * @{
 * @brief Runtime errors used by the CanTp module.
 */
#define CANTP_E_PADDING            (0x70u) /**< error occurs with padding byte  */
#define CANTP_E_INVALID_TATYPE     (0x90u) /**< error occurs when segmented Rx / Tx for functional Sdu */
#define CANTP_E_OPER_NOT_SUPPORTED (0xA0u) /**< error occurs when requested operation is not supported */
#define CANTP_E_COM                (0xB0u) /**< error occurs when protocol timeout */
#define CANTP_E_RX_COM             (0xC0u) /**< error occurs during reception */
#define CANTP_E_TX_COM             (0xD0u) /**< error occurs during transmition */
/** @} */

/**
 * @name CanTpServiceIDMacros
 * @{
 * @brief API Service IDs are used to report to Det when an error occurs.
 */
#define CANTP_SERVICEID_INIT                  (0x01u) /**< API CanTp_Init */
#define CANTP_SERVICEID_GETVERSIONINFO        (0x07u) /**< API CanTp_ GetVersionInfo */
#define CANTP_SERVICEID_SHUTDOWN              (0x02u) /**< API CanTp_Shutdown */
#define CANTP_SERVICEID_TRANSMIT              (0x49u) /**< API CanTp_Transmit */
#define CANTP_SERVICEID_CANCELTRANSMITREQUEST (0x4Au) /**< API CanTp_CancelTransmit */
#define CANTP_SERVICEID_CANCELRECEIVEREQUEST  (0x4Cu) /**< API CanTp_CancelReceive */
#define CANTP_SERVICEID_CHANGEPARAMETER       (0x4Bu) /**< API CanTp_ChangeParameter */
#define CANTP_SERVICEID_READPARAMETER         (0x0Bu) /**< API CanTp_ReadParameter */
#define CANTP_SERVICEID_MAINFUNCTION          (0x06u) /**< API CanTp_MainFunction */
#define CANTP_SERVICEID_RXINDICATION          (0x42u) /**< API CanTp_RxIndication */
#define CANTP_SERVICEID_TXCONFIRMATION        (0x40u) /**< API CanTp_TxConfirmation */
/** @} */

/**
 * @name CanTpInvalidValueMacros
 * @{
 * @brief Defines invalid value macros used by the CanTp
 */
#define CANTP_INVALID_U8    (uint8)0xFFu        /**< u8 invalid value definition */
#define CANTP_STMIN_INVALID (uint8)0x80u        /**< parameter STmin invalid value definition */
#define CANTP_INVALID_U16   (uint16)0xFFu       /**< u16 invalid value definition */
#define CANTP_INVALID_U32   (uint32)0xFFFFFFFFu /**< u32 invalid value definition */

/**
 * @name  CanTpFrameTypesMacros
 * @{
 * @brief definition for the CAN Transport Protocol frame PCI type
 */
#define CANTP_FTYPE_MASK    (CanTp_FramePCIType)0xF0u /**< mask to extract the frame type from the PCI byte */
#define CANTP_FTYPE_SF      (CanTp_FramePCIType)0x00u /**< single frame type */
#define CANTP_FTYPE_FF      (CanTp_FramePCIType)0x10u /**< first frame type */
#define CANTP_FTYPE_CF      (CanTp_FramePCIType)0x20u /**< consecutive frame type */
#define CANTP_FTYPE_FC      (CanTp_FramePCIType)0x30u /**< flow control frame type */
#define CANTP_FTYPE_RESEVED (CanTp_FramePCIType)0xF0u /**< reserved frame type */
/** @} */

/**
 * @name  CanTpFlowControlStatusMacros
 * @{
 * @brief definition for the flow control frame
 */
#define CANTP_FC_FS_CTS  (uint8)0x00u /**< indicates the receiver is ready to accept the next frame */
#define CANTP_FC_FS_WT   (uint8)0x01u /**< indicates the receiver is not ready to accept and will be retrying*/
#define CANTP_FC_FS_OVFL (uint8)0x02u /**< indicates the receiver's buffer has overflowed */
/** @} */

/**
 * @name CanTpMaskMacros
 * @{
 * @brief macros group of various mask
 */
#define CANTP_SF_DL_MASK (uint8)0x0Fu /**< mask of the SF_DL*/
#define CANTP_FF_DL_MASK (uint8)0x0Fu /**< mask of the upper nibble for FF DL*/
#define CANTP_CF_SN_MASK (uint8)0x0Fu /**< mask of the SN for CF*/
#define CANTP_FC_FS_MASK (uint8)0x0Fu /**< mask of the FS for FC */
/** @} */

/**
 * @name CanTpPCILengthMacros
 * @{
 * @brief macros group of various frame PCI length
 */
#define CANTP_PCI_LENGTH_SF       (uint8)1u /**< SF PCI length */
#define CANTP_PCI_LENGTH_FF       (uint8)2u /**< FF PCI length */
#define CANTP_PCI_LENGTH_CF       (uint8)1u /**< CF PCI length */
#define CANTP_PCI_LENGTH_FC       (uint8)3u /**< FC PCI length */
#define CANTP_PCI_LENGTH_SF_FD    (uint8)2u /**< SF PCI length with CANFD*/
#define CANTP_PCI_LENGTH_FF_12BIT (uint8)6u /**< FF PCI length with 12bit*/
/** @} */

/**
 * @name CanTpSpecifiedBitsMacros
 * @{
 * @brief macros group of various specified bits
 */
#define CANTP_EVENT_RXNOTIFIUPPER    0x01u /**< Eventflags,whether to notify upperlayer that rx is E_OK*/
#define CANTP_EVENT_CAN20_WITH_CANFD 0x02u /**< Eventflags,whether to enbale transmission of Can2.0 under CanFD*/
#define CANTP_METADATA_LENGTH_MASK   0x0Fu /**< CanTpPduFlag,bit0-bit3 indicates metadata data length*/
#define CANTP_CAN20_CANFD_MASK       0x10u /**< CanTpPduFlag,whether support transmission of Can2.0 under CanFD*/
#define CANTP_TC_MASK                0x20u /**< CanTpPduFlag,whether support transmission cancel*/
/** @} */

/* PRQA S 3472 ++ */ /* VL_CanTp_3472 */
/**
 * @name CanTpOperateFlagBit&CalculateMacros
 * @{
 * @brief These macros are a set of utility functions for manipulating and querying flag bits.
 */
#define CANTP_SETFLAG(flag, bit)     ((flag) |= (bit))           /**< Set flag bit */
#define CANTP_CLEARFLAG(flag, bit)   ((flag) &= (uint8)(~(bit))) /**< Clear flag bit */
#define CANTP_FLAGISSET(flag, bit)   (0u != ((flag) & (bit)))    /**< Check if the specified bit is set*/
#define CANTP_FLAG_ISNSET(flag, bit) (0u == ((flag) & (bit)))    /**< Check if the specified bit is not set*/
#define CEIL_DIV(data, denominator)  (((data) + (denominator) - 1u) / (denominator)) /**< Ceiling division macro */
/** @} */
/* PRQA S 3472 -- */

/**
 * @name CanTpMetaDataInfoMacros
 * @{
 * @brief macros group of various metaData information
 */
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
#define CANTP_METADATA_MAXLENGTH  (uint8)4u /**< metadata length */
#define CANTP_METADATA_LENGTHMASK (uint8)4u /**< metadata length mask*/
#endif
#define CANTP_TA_OFFSET_META_EX    0u /**< the offset for TA information */
#define CANTP_AE_OFFSET_META_MIX   0u /**< the offset for AE information with mixed11 addressing*/
#define CANTP_AE_OFFSET_META_MIX29 0u /**< the offset for AE information with mixed29 addressing*/
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#define CANTP_SA_OFFSET_META_IF      3u /**< position of SA transmited to CanIf*/
#define CANTP_TA_OFFSET_META_IF      2u /**< position of TA transmited to CanIf*/
#define CANTP_RESERVE_OFFSET_META_IF 1u /**< position of reserved byte transmited to CanIf*/
#define CANTP_ID_OFFSET_META_IF      0u /**< position of CanID highest byte transmited to CanIf*/
#else
#define CANTP_SA_OFFSET_META_IF      0u /**< position of SA transmited to CanIf*/
#define CANTP_TA_OFFSET_META_IF      1u /**< position of TA transmited to CanIf*/
#define CANTP_RESERVE_OFFSET_META_IF 2u /**< position of reserved byte transmited to CanIf*/
#define CANTP_ID_OFFSET_META_IF      3u /**< position of CanID highest byte transmited to CanIf*/
#endif
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#define CANTP_SA_OFFSET_META_UPPER 2u /**< position of SA transmited to Upperlayer*/
#define CANTP_TA_OFFSET_META_UPPER 0u /**< position of TA transmited to Upperlayer*/
#else
#define CANTP_SA_OFFSET_META_UPPER 1u /**< position of SA transmited to Upperlayer*/
#define CANTP_TA_OFFSET_META_UPPER 3u /**< position of TA transmited to Upperlayer*/
#endif
/** @} */

/**
 * @name CanTpInternalLength&OffsetMacros
 * @{
 * @brief macros group contain various CanTp Internal length and offset information
 */
#define CANTP_CANID_INIT           0x00u                /**< CanID highest byte init value*/
#define CANTP_PARALLEL_LENGTH      0x02u                /**< 3E80 Length*/
#define CANTP_PARALLEL_SID         0x3Eu                /**< parallel services id*/
#define CANTP_PARALLEL_SUBFUNCTION 0x80u                /**< parallel services subfunction*/
#define CANTP_CANID_CANFD_MASK     0x40u                /**< whether CANFD frame in CanID highest byte*/
#define CANTP_CANID_EXTERNED_MASK  0x80u                /**< whether extended frame in CanID highest byte*/
#define CANTP_MATCH_DLC_NUMBER     0x08u                /**< 8-15 DLC discrete range number*/
#define CANTP_MAX_SEQUENC_NUMBER   0x0Fu                /**< max SequenceNumber*/
#define CANTP_MAX_SEPARATE_TIME    0x7Fu                /**< max SeparationTime minimum*/
#define CANTP_PCI_OFFSET_STD_NF    0x00u                /**< offset of PCI with normal addressing */
#define CANTP_PCI_OFFSET_EX_MIX    0x01u                /**< offset of PCI with extended or mixed addressing*/
#define CANTP_STMIN_MICROSEC_MIN   0xF1u                /**< the lower limit of microseconds range */
#define CANTP_STMIN_MICROSEC_MAX   0xF9u                /**< the upper limit of microseconds range */
#define CANTP_FF_DL_12BIT_LENGTH   (PduLengthType)4095u /**< data length which FFDL have 12 bits */
#define CANTP_CAN20_FRAME_LEN_MAX  (PduLengthType)8u    /**< Can2.0 frame max length */
#define CANTP_CANFD_FRAME_LEN_MAX  (PduLengthType)64u   /**< CanFD frame max length */
/** @} */

/* ================================================ type definitions ================================================ */
/**
 * @brief Type definition for the CAN Transport Protocol frame PCI type
 */
typedef uint8 CanTp_FramePCIType;
/**
 * @brief  State of the CanTp module. Initialized or not.
 */
typedef enum
{
    CANTP_OFF, /**< CanTp module not initialized @range 0x00*/
    CANTP_ON   /**< CanTp module initialized @range 0x01*/
} CanTp_StateType;

/**
 * @brief  Enumerated type is used to identify the type of Can frame
 */
typedef enum
{
    CANTP_CAN_20, /**< standard CAN 2.0 frame @range 0x00*/
    CANTP_CAN_FD  /**< CAN Flexible Data-rate frame @range 0x01*/
} CanTp_CanFrameType;

/**
 * @brief  Enumerated type to define the padding activation states for CanTp
 */
typedef enum
{
    CANTP_PADDING_OFF, /**< padding inactivation @range 0x00*/
    CANTP_PADDING_ON   /**< padding activation @range 0x01*/
} CanTp_PaddingActivationType;

/**
 * @brief  Enumerated type to define the addressing types for CanTp requests.
 */
typedef enum
{
    CANTP_FUNCTIONAL, /**< functional request type @range 0x00*/
    CANTP_PHYSICAL,   /**< physical request type @range 0x01*/
} CanTp_TaTypeType;

/**
 * @brief   Enumerated type to define the addressing format for NSdu
 */
typedef enum
{
    CANTP_STANDARD,    /**< Standard addressing format @range 0x00*/
    CANTP_EXTENDED,    /**< Extended addressing format @range 0x01*/
    CANTP_NORMALFIXED, /**< Normal fixed addressing format @range 0x02*/
    CANTP_MIXED,       /**< Mixed11 addressing format @range 0x03*/
    CANTP_MIXED29BIT   /**< Mixed29 addressing format @range 0x04*/
} CanTp_AddressingFormatType;

/**
 * @brief  Enumerated type is used to record direction and occupancy state of pre txPdu
 */
typedef enum
{
    CANTP_DIR_IDLE, /**< the txPdu is in an idle state. @range 0x00*/
    CANTP_DIR_TX,   /**< the txPdu is occupied by rxNSdu @range 0x01*/
    CANTP_DIR_RX,   /**< the txPdu is occupied by txNSdu @range 0x02*/
} CanTp_TxDirectionType;

/**
 * @brief  This data type is used to record the state of pre txPdu and corresponding connection
 */
typedef struct CanTp_ConfirmDirectionTypeTag /* PRQA S 1536 */ /* VL_CanTp_1536 */
{
    PduIdType             ConnectionId; /**< corresponding connection @range 0x00*/
    CanTp_TxDirectionType TxDirection;  /**< the state of the txPdu @range 0x01*/
} CanTp_ConfirmDirectionType;

/**
 * @brief   Txchannel states switch
 */
typedef enum
{
    CANTP_TX_IDLE,            /**< No transmition process on the current channel @range 0x00*/
    CANTP_TX_TRANSMIT_SF,     /**< SingleFrame transmition is processing @range 0x01*/
    CANTP_TX_TRANSMIT_FF,     /**< FirstFrame transmition is processing @range 0x02*/
    CANTP_TX_TRANSMIT_CF,     /**< ConsecutiveFrame transmition is processing @range 0x03*/
    CANTP_TX_WAIT_CF_STMIN,   /**< wait STmin timer before transmitCF @range 0x04*/
    CANTP_TX_WAIT_RECEIVE_FC, /**< wait to receive FlowContrlFrame @range 0x05*/
    CANTP_TX_WAIT_SFCONF,     /**< wait to SF confirmation @range 0x06*/
    CANTP_TX_WAIT_FFCONF,     /**< wait to FF confirmation  @range 0x07*/
    CANTP_TX_WAIT_CFCONF,     /**< wait to CF confirmation  @range 0x08*/
} CanTp_TxChannelType;

/**
 * @brief  Rxchannel states switch
 */
typedef enum
{
    CANTP_RX_IDLE,             /**< No reception process on the current channel @range 0x00*/
    CANTP_RX_RECEIVE_SF,       /**< SingleFrame reception is processing @range 0x01*/
    CANTP_RX_RECEIVE_FF,       /**< FirstFrame reception is processing  @range 0x02*/
    CANTP_RX_RECEIVE_CF,       /**< ConsecutiveFrame reception is processing @range 0x03*/
    CANTP_RX_WAIT_RECEIVE_CF,  /**< wait to receive CF after FC/CF confirmed @range 0x04*/
    CANTP_RX_WAIT_BUFFER_CF,   /**< wait for sufficient buffersize for CF receiving @range 0x05*/
    CANTP_RX_TRANSMIT_FC_CFS,  /**< Transmit FC indicating the request to continue to send  @range 0x06*/
    CANTP_RX_TRANSMIT_FC_OVFL, /**< Transmit FC indicating buffer overflow  @range 0x07*/
    CANTP_RX_TRANSMIT_FC_WT,   /**< Transmit FC indicating the request to continue to wait @range 0x08*/
    CANTP_RX_WAIT_FCCONF_CFS,  /**< wait to FC.CTS confirmation @range 0x09*/
    CANTP_RX_WAIT_FCCONF_WT,   /**< wait to FC.WAIT confirmation @range 0x0A*/
    CANTP_RX_WAIT_FCCONF_OVFL, /**< wait to FC.OVFLW confirmation @range 0x0B*/
} CanTp_RxChannelType;

#ifdef __cplusplus
}
#endif
#endif
