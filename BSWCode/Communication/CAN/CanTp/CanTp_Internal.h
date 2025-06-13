/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    : CanTp_Internal.h                                           **
**                                                                           **
**  Created on  : 2021/7/30 14:29:43                                         **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Internal header file of CanTp module.                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */

#ifndef CANTP_INTERNAL_H
#define CANTP_INTERNAL_H

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTp_PBcfg.h"
#include "CanTp_Types.h"
#include "Det.h"
#include "PduR_CanTp.h"
#include "CanIf.h"
#include "SchM_CanTp.h"

/*=======[M A C R O S]========================================================*/
#define CANTP_MODULE_ID           35u
#define CANTP_VENDOR_ID           62u
#define CANTP_INSTANCE_ID         0x0u

#define CANTP_CAN20_FRAME_LEN_MAX (PduLengthType)8
#define CANTP_CANFD_FRAME_LEN_MAX (PduLengthType)64

#if !defined(CANTP_LOCAL)
#define CANTP_LOCAL static
#endif

#if !defined(CANTP_INLINE)
#define CANTP_INLINE inline
#endif

#if !defined(CANTP_LOCAL_INLINE)
#define CANTP_LOCAL_INLINE static inline
#endif

#if !defined(CANTP_UNUSED)
#define CANTP_UNUSED(a) (void)(a)
#endif

/** This macro definition specifies whether the Consecutive Frame (CF) should be transmitted immediately during the
 * confirmation phase, without waiting for the completion of a main function cycle, when the STmin value in the received
 * Flow Control (FC) frame is 0. */
#ifndef CANTP_STMIN_IMMEDIATE_TX_CONFIRMATION
#define CANTP_STMIN_IMMEDIATE_TX_CONFIRMATION STD_ON /**< Whether transmitted immediately during the confirmation  */
#endif

/*****************************Development error values ***********************/
#define CANTP_E_PARAM_CONFIG  (0x01u)
#define CANTP_E_PARAM_ID      (0x02u)
#define CANTP_E_PARAM_POINTER (0x03u)
#define CANTP_E_INIT_FAILED   (0x04u)
#define CANTP_E_UNINIT        (0x20u)
#define CANTP_E_INVALID_TX_ID (0x30u)
#define CANTP_E_INVALID_RX_ID (0x40u)
/*****************************Runtime error values ***********************/
#define CANTP_E_PADDING            (0x70u)
#define CANTP_E_INVALID_TATYPE     (0x90u)
#define CANTP_E_OPER_NOT_SUPPORTED (0xA0u)
#define CANTP_E_COM                (0xB0u)
#define CANTP_E_RX_COM             (0xC0u)
#define CANTP_E_TX_COM             (0xD0u)

/*******************************CanTp Module API ID*****************************/
#define CANTP_SERVICEID_RXINDICATION          (0x42u)
#define CANTP_SERVICEID_TXCONFIRMATION        (0x40u)
#define CANTP_SERVICEID_MAINFUNCTION          (0x06u)
#define CANTP_SERVICEID_TRANSMIT              (0x03u)
#define CANTP_SERVICEID_INIT                  (0x01u)
#define CANTP_SERVICEID_CANCELTRANSMITREQUEST (0x08u)
#define CANTP_SERVICEID_CANCELRECEIVEREQUEST  (0x09u)
#define CANTP_SERVICEID_CHANGEPARAMETER       (0x0Au)
#define CANTP_SERVICEID_READPARAMETER         (0x0Bu)
#define CANTP_SERVICEID_GETVERSIONINFO        (0x07u)

#define CANTP_SETFLAG(flag, bit)              (flag |= bit)
#define CANTP_CLEARFLAG(flag, bit)            (flag &= (uint8)(~bit))
#define CANTP_FLAGISSET(flag, bit)            (0u != (flag & bit))
/*type of frame types*/
typedef uint8 CanTp_FramePCIType;

#define CANTP_FTYPE_MASK    (uint8)0xF0

#define CANTP_FTYPE_SF      0x00u

#define CANTP_FTYPE_FF      (uint8)0x10

#define CANTP_FTYPE_CF      (uint8)0x20

#define CANTP_FTYPE_FC      (uint8)0x30

#define CANTP_FTYPE_RESEVED (uint8)0xF0

/*mask of the SF_DL*/
#define CANTP_SF_DL_MASK (uint8)0x0F

/*mask of the upper nibble for FF DL*/
#define CANTP_FF_DL_MASK (uint8)0x0F

/*bit offset of the length information in the low part of first PCI byte*/
#define CANTP_OFFSET_BYTE_PART 8u

/*Offset of FF_DL low nibble for standard addressing*/
#define CANTP_STD_FF_FFDL_LOW_OFFSET_LT4095   1u
#define CANTP_EX_FF_FFDL_LOW_OFFSET_LT4095    2u
#define CANTP_NF_FF_FFDL_LOW_OFFSET_LT4095    1u
#define CANTP_MIX11_FF_FFDL_LOW_OFFSET_LT4095 2u
#define CANTP_MIX29_FF_FFDL_LOW_OFFSET_LT4095 2u

#define CANTP_STD_FF_FFDL_LOW_OFFSET_BT4095   5u
#define CANTP_EX_FF_FFDL_LOW_OFFSET_BT4095    6u
#define CANTP_NF_FF_FFDL_LOW_OFFSET_BT4095    5u
#define CANTP_11MIX_FF_FFDL_LOW_OFFSET_BT4095 6u
#define CANTP_29MIX_FF_FFDL_LOW_OFFSET_BT4095 6u

/*mask of the SN for CF*/
#define CANTP_CF_SN_MASK (uint8)0x0F

/*mask of the FS for FC*/
#define CANTP_FC_FS_MASK (uint8)0x0F

/*CONST values for FS of FC*/
/*continue to send*/
#define CANTP_FC_FS_CTS 0x00u

/*FC_WAIT*/
#define CANTP_FC_FS_WT (uint8)0x01

/*overflow*/
#define CANTP_FC_FS_OVFLW (uint8)0x02

/*offset of FC_BS for standard addressing*/
#define CANTP_FC_OFFSET_BS_STD 1u

/*offset of FC_BS for extended addressing*/
#define CANTP_FC_OFFSET_BS_EX 2u

/*offset of FC_BS for normal fixed addressing*/
#define CANTP_FC_OFFSET_BS_NF 1u

/*offset of FC_BS for mix11 addressing*/
#define CANTP_FC_OFFSET_BS_MIX11 2u

/*offset of FC_BS for mix29 addressing*/
#define CANTP_FC_OFFSET_BS_MIX29 2u

/*FC STmin limits*/
/*ms upper limit*/
#define CANTP_FC_STMIN_MS_LIMIT (uint8)0x7F

/*us lower limit*/
#define CANTP_FC_STMIN_US_LOW (uint8)0xF1

/*us upper limit*/
#define CANTP_FC_STMIN_US_UPPER (uint8)0xF9

/*unit for STmin from 0-7f in us*/
#define CANTP_FC_STMIN_MS_UNIT (uint32)1000

/*unit for STmin from F1-F9 in us*/
#define CANTP_FC_STMIN_US_UNIT (uint32)100

/*offset of FC_STMin for standard addressing*/
#define CANTP_FC_OFFSET_STMIN_STD 2u

/*offset of FC_STmin for extended addressing*/
#define CANTP_FC_OFFSET_STMIN_EX 3u

/*offset of FC_STmin for normal fixed addressing*/
#define CANTP_FC_OFFSET_STMIN_NF 2u

/*offset of FC_STmin for mix11 addressing*/
#define CANTP_FC_OFFSET_STMIN_MIX11 3u

/*offset of FC_STmin for mix29 addressing*/
#define CANTP_FC_OFFSET_STMIN_MIX29 3u

/*frame length of FC for standard addressing*/
#define CANTP_FC_LEN_STD (uint8)0x03

/*frame length of FC for extended addressing*/
#define CANTP_FC_LEN_EX (uint8)0x04

/*frame length of FC for normal fixed addressing*/
#define CANTP_FC_LEN_NF (uint8)0x03

/*frame length of FC for mix11 addressing*/
#define CANTP_FC_LEN_MIX11 (uint8)0x04

/*frame length of FC for mix29 addressing*/
#define CANTP_FC_LEN_MIX29 (uint8)0x04

/*FFDL have 12 bits, its data length*/
#define CANTP_FF_DL_12bit_LENGTH (uint32)4095

/*maximum length of single frame data, if the addressing format is standard*/
#define CANTP_SF_LEN_MAX_STD (uint8)0x07

/*maximum length of single frame data, if the addressing format is extended*/
#define CANTP_SF_LEN_MAX_EX (uint8)0x06

/*maximum length of single frame data, if the addressing format is normal fixed*/
#define CANTP_SF_LEN_MAX_NF (uint8)0x07

/*maximum length of single frame data, if the addressing format is mixed11*/
#define CANTP_SF_LEN_MAX_MIX11 (uint8)0x06

/*maximum length of single frame data, if the addressing format is mixed29*/
#define CANTP_SF_LEN_MAX_MIX29 (uint8)0x06

/*maximum length of the CF data for standard SDU*/
#define CANTP_CF_DATALEN_STD (uint8)0x07

/*maximum length of the CF data for extended SDU*/
#define CANTP_CF_DATALEN_EX    (uint8)0x06

#define CANTP_CF_DATALEN_NF    (uint8)0x07

#define CANTP_CF_DATALEN_MIX11 (uint8)0x06

#define CANTP_CF_DATALEN_MIX29 (uint8)0x06

/*CF data offset for CF of standard addressing*/
#define CANTP_CF_DATA_OFFSET_STD   (uint8)0x01
#define CANTP_CF_DATA_OFFSET_EX    (uint8)0x02
#define CANTP_CF_DATA_OFFSET_NF    (uint8)0x01
#define CANTP_CF_DATA_OFFSET_MIX11 (uint8)0x02
#define CANTP_CF_DATA_OFFSET_MIX29 (uint8)0x02

/*data offset of FF*/
#define CANTP_FF_DATA_OFFSET_LT4095_STD   (uint8)0x02
#define CANTP_FF_DATA_OFFSET_LT4095_EX    (uint8)0x03
#define CANTP_FF_DATA_OFFSET_LT4095_NF    (uint8)0x02
#define CANTP_FF_DATA_OFFSET_LT4095_MIX11 (uint8)0x03
#define CANTP_MIX29_FF_DATA_OFFSET_LT4095 (uint8)0x03

#define CANTP_FF_DATA_OFFSET_BT4095_STD   (uint8)0x06
#define CANTP_FF_DATA_OFFSET_BT4095_EX    (uint8)0x07
#define CANTP_FF_DATA_OFFSET_BT4095_NF    (uint8)0x06
#define CANTP_FF_DATA_OFFSET_BT4095_MIX11 (uint8)0x07
#define CANTP_MIX29_FF_DATA_OFFSET_BT4095 (uint8)0x07

/*data offset of SF for standard addressing*/
#define CANTP_SF_DATA_OFFSET_STD       (uint8)0x01
#define CANTP_SF_DATA_OFFSET_CANFD_STD (uint8)0x02

/*data offset of SF for extended addressing*/
#define CANTP_SF_DATA_OFFSET_EX       (uint8)0x02
#define CANTP_SF_DATA_OFFSET_CANFD_EX (uint8)0x03

/*data offset of SF for normal fix addressing*/
#define CANTP_SF_DATA_OFFSET_NF       (uint8)0x01
#define CANTP_SF_DATA_OFFSET_CANFD_NF (uint8)0x02

/*data offset of SF for mix11 addressing*/
#define CANTP_SF_DATA_OFFSET_MIX11       (uint8)0x02
#define CANTP_SF_DATA_OFFSET_CANFD_MIX11 (uint8)0x03

/*data offset of SF for mix29 addressing*/
#define CANTP_SF_DATA_OFFSET_MIX29       (uint8)0x02
#define CANTP_SF_DATA_OFFSET_CANFD_MIX29 (uint8)0x03

/*offset of PCI for standard addressing*/
#define CANTP_PCI_OFFSET_STD (uint8)0x00

/*offset of PCI for extended addressing*/
#define CANTP_PCI_OFFSET_EX (uint8)0x01

/*offset of PCI for normal fixed addressing*/
#define CANTP_PCI_OFFSET_NF (uint8)0x00

/*offset of PCI for mixed11 addressing*/
#define CANTP_PCI_OFFSET_MIX11 (uint8)0x01

/*offset of PCI for mixed29 addressing*/
#define CANTP_PCI_OFFSET_MIX29 (uint8)0x01

/*MetaData information*/
/*extended meta data TA*/
#define CANTP_TA_OFFSET_META_EX 0u
/*mix11 meta data AE*/
#define CANTP_AE_OFFSET_META_MIX 0u
/*mix29 meta data AE*/
#define CANTP_AE_OFFSET_META_MIX29 0u

#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#define CANTP_SA_OFFSET_META_IF 3u
#define CANTP_TA_OFFSET_META_IF 2u
#else
#define CANTP_SA_OFFSET_META_IF 0u
#define CANTP_TA_OFFSET_META_IF 1u
#endif

#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#define CANTP_SA_OFFSET_META_UPPER 1u
#define CANTP_TA_OFFSET_META_UPPER 3u
#else
#define CANTP_SA_OFFSET_META_UPPER 0u
#define CANTP_TA_OFFSET_META_UPPER 2u
#endif

#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#define CANTP_SA_OFFSET_META_UPPER 1u
#define CANTP_TA_OFFSET_META_UPPER 3u
#else

#define CANTP_SA_OFFSET_META_UPPER 0u
#define CANTP_TA_OFFSET_META_UPPER 2u
#endif

/*Event flags*/
#define CANTP_EVENT_RXBSCHANGED      (uint8)0x01

#define CANTP_EVENT_RXSTMINCHANGED   (uint8)0x02

#define CANTP_EVENT_RXNOTIFIUPPER    (uint8)0x04

#define CANTP_EVENT_COPYANDWAITSTMIN (uint8)0x08

/*CanTpPduFlag*/
#define CANTP_METADATA_LENGTH_MASK     (uint8)0x0F

#define CANTP_ADDRESS_EXTENSION_8_MASK (uint8)0x10

#define CANTP_TC_MASK                  (uint8)0x20

#define CANTP_CAN_ID_32_MASK           (uint8)0x80

#define CANTP_SA16_AND_TA16_MASK       (uint8)0x40

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* State of the CanTp module. Initialized or not. */
typedef enum
{
    CANTP_OFF, /* CanTp module not initialized */
    CANTP_ON   /* CanTp module initialized */
} CanTp_StateType;

/* current enabled ABC timer for receiver or sender  */
typedef enum
{
    CANTP_TIMER_NONE, /* No timer started */
    CANTP_NA,         /* Nar or Nas started */
    CANTP_NB,         /* Nbs or Nbr started */
    CANTP_NC          /* Ncs or Ncr started */
} CanTp_ABCTimerType;

/* running control of the ABC timer for receiver and sender */
typedef struct
{
    CanTp_ABCTimerType EnabledTimer;
    uint32 StartTime;
    uint32 RemainTime;
} CanTp_ChannelTimerType;

/* running control for STmin for sender */
typedef struct
{
    boolean Started;
    uint32 RemainTime;
    uint32 StartTime;
    uint32 FcSTMin;
} CanTp_STminType;

/* running control for diag gateway for sender */
typedef struct
{
    boolean Started;
    uint32 RemainTime;
    uint32 StartTime;
    uint8 ResponseTimer;
    uint8 Sid;
    uint8 PendingSid;
} CanTp_GatewayTimerType;

/*root states of communication channel*/
typedef enum
{
    CANTP_IDLE,              /*the channel is idle*/
    CANTP_OCCUPIED,          /*the channel is occupied*/
    CANTP_SF_RECEIVING,      /*the channel is handling SF receiving*/
    CANTP_SF_TRANSIMITTING,  /*the channel is handling SF transmitting*/
    CANTP_LARGE_RECEIVING,   /*the channel is handling large SDU receiving*/
    CANTP_LARGE_TRANSMITTING /*the channel is handling large SDU transmitting*/
} CanTp_ChannelRootStateType;

/*Sub-states of communication channel*/
typedef enum
{
    CANTP_SUBSTATE_NONE,

    CANTP_RX_SF_START, /*SF Receiving*/

    CANTP_RX_FF_START,
    CANTP_RX_LARGE_START,       /*Large receiving start*/
    CANTP_RX_LARGE_V4FCWT_CFM,  /*wait for confirmation of FC_WAIT*/
    CANTP_RX_LARGE_V4FCCTS_CFM, /*wait for confirmation of FC_CTS*/
    CANTP_RX_LARGE_V4CF,        /*wait for the following CFs*/
    CANTP_RX_LARGE_RXCF,        /*receiving received CF*/

    CANTP_TX_SF_START,    /*SF transmitting start*/
    CANTP_TX_SF_V4SF_CFM, /*wait for confirmation of SF transmitting*/

    CANTP_TX_LARGE_START,      /*large transmitting start*/
    CANTP_TX_LARGE_V4DATA_CFM, /*wait for confirmation of transmitted frame*/
    CANTP_TX_LARGE_V4FC        /*wait for FC*/
} CanTp_ChannelSubstateType;

typedef struct
{
    /* parent channel Index */
    uint8 ChannelId;
    /*runtime timer for channel*/
    CanTp_ChannelTimerType ChannelTimer;

    /*STmin timer runtime information*/
    CanTp_STminType STminTimer;

    /*root state of channel*/
    CanTp_ChannelRootStateType RootState;

    /*sub-state of channel*/
    CanTp_ChannelSubstateType Substate;

    /*handled frame type in current phase*/
    CanTp_FramePCIType HandleType;

    /*identifier of N-SDU which is using this channel*/
    PduIdType NSduId;

    PduLengthType CanFrameLength;

    /*data in bytes need to deal with of this N-SDU*/
    PduLengthType SduDataRemaining;

    /*total count of N-SDU data in bytes*/
    PduLengthType SduDataTotalCount;

    /*length of receive data, include pci and metaData*/
    PduLengthType DataLen;

    /*current BS value*/
    uint8 CurrentBs;

#if (STD_ON == CANTP_FD)
    PduLengthType FDDlc;
#endif /* STD_ON == CANTP_FD */

    uint8 ChangedBs;

    uint8 ChangedSTmin;

    /*count of handled CF in a block*/
    uint8 HandledCfCount;

    /*current CF serial number*/
    uint8 CurrentCfSn;

    /*number of already sent FCwait consecutively*/
    uint8 SentWftCount;

    /*Event Flags*/
    uint8 EventFlags;

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    /*The addressing information in the MedataData depends on the addressing format*/
    uint8 MetaDataN_TA;

    /*The addressing information in the MedataData depends on the addressing format*/
    uint8 MetaDataN_SA;

    /*The addressing information in the MedataData depends on the addressing format*/
    uint8 MetaDataN_AE;

    uint8 MetaDataLength;
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
} CanTp_ConnectionType;

typedef struct
{
    CanTp_ConnectionType Connection;
    /*local buffer of channel*/
#if (STD_ON == CANTP_FD)
    uint8 LocalBuf[CANTP_CANFD_FRAME_LEN_MAX];
#else  /* STD_ON == CANTP_FD */
    uint8 LocalBuf[CANTP_CAN20_FRAME_LEN_MAX];
#endif /* STD_ON == CANTP_FD */
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[5];
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
    /* diag gateway need to response */
    CanTp_GatewayTimerType NeedRes;
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */
} CanTp_TxConnectionType;
/* Channel parameters during CanTp module running. Channel is a logic
 * connection between sender and receiver. */
typedef struct
{
    CanTp_ConnectionType RxConnection;
    CanTp_TxConnectionType TxConnection;
} CanTp_ConnectionChannelType;

/*structure for FC frame information*/
typedef struct
{
    uint8 FcFs;
    uint8 FcBS;
    uint32 FcSTMin; /*STmin in tick unit*/
} CanTp_FCInfoType;

BEGIN_C_DECLS
/*=======[E X T E R N A L   D A T A]==========================================*/
/*CANTP cfg ptr*/
extern P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CanTp_ConfigPtr;
/*CANTP global variable for runtime channel controlling information*/
extern VAR(CanTp_ConnectionChannelType, CANTP_VAR_NOINIT) CanTp_Channels[CANTP_CHANNEL_NUMBER];
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

CANTP_LOCAL_INLINE void CANTP_DET_REPORTRUNTIMEERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId), (ErrorId));
}
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
CANTP_LOCAL_INLINE void CANTP_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */

/* get the Rx specific channel */
CANTP_LOCAL_INLINE CanTp_ConnectionType* CANTP_RXCHANNEL(uint8 ChannelId)
{
    return &CanTp_Channels[ChannelId].RxConnection;
}

/* get the Tx specific channel */
CANTP_LOCAL_INLINE CanTp_TxConnectionType* CANTP_TXCHANNEL(uint8 ChannelId)
{
    return &CanTp_Channels[ChannelId].TxConnection;
}

/* get the Tx specific channel Connection */
CANTP_LOCAL_INLINE CanTp_ConnectionType* CANTP_TXSUBCHANNEL(uint8 ChannelId)
{
    return &CanTp_Channels[ChannelId].TxConnection.Connection;
}

/*
 * Brief               Get RX-SDU configuration by received NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE) CanTp_GetRxSduCfgByNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(CanTp_FramePCIType, AUTOMATIC, AUTOMATIC) SduFrameType,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ChannelId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief               Get SFF_DL information from the data content of SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(PduLengthType, CANTP_CODE) CanTp_RxGetSFDl(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#if (STD_ON == CANTP_FD)
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) dataoffset,
#endif /* STD_ON == CANTP_FD */
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) totaloffset);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               store Rx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_SaveRxMetaDataInfo(
    CanTp_ConnectionType* RxchannelPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Get FF_DL information from the data content of FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(PduLengthType, CANTP_CODE) CanTp_RxGetFFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_CONST) offset,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr);

/*
 * Brief               Calculate the BS value according to the buffer length and remain data length
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE)
    CanTp_CalcBS(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr, CanTp_ConnectionType* RxchannelPtr);

/*
 * Brief               Compare Buffer With Bs
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CompareBufferWithBs(
    const CanTp_RxNSduType* RxNSduCfgPtr,
    const CanTp_ConnectionType* RxchannelPtr,
    PduLengthType bufferSize);

/*
 * Brief               send FC
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_SendFC(
    const CanTp_RxNSduType* RxNSduCfgPtr,
    const CanTp_ConnectionType* RxchannelPtr,
    CanTp_FCInfoType CanTp_FC);

/*
 * Brief               Utility function: Set memory with specific value
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    CanTp_MemorySet(P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Address, uint8 Value, uint16 Length);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               check RxCF meta data
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CheckRxCFMetaData(
    CanTp_ConnectionType* RxchannelPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Get CF Offset
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE) CanTp_RxGetCFOffset(
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) dataoffset,
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) pcioffset);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief               Get TX-SDU configuration by received FC NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE) CanTp_GetTxSduCfgByFCNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ChannelId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               check RxFc meta data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CheckRxFCMetaData(
    CanTp_ConnectionType* TxSubchannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*Get the information of received FC frame.*/
extern FUNC(void, CANTP_CODE) CanTp_GetChannelFCInfo(
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(CanTp_FCInfoType, AUTOMATIC, AUTOMATIC) FcInfoPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief               check Tx Length In Address Mode.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CheckTxLengthInAddressMode(
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               store Tx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE) CanTp_SaveTxMetaDataInfo(
    CanTp_ConnectionType* TxSubchannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Handle transmit request.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE) CanTp_TxHandleTransmitReq(
    CanTp_ConnectionType* TxSubchannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr
#if (CANTP_SYNCHRONOUS_TRANSMIT == STD_ON)
    ,
    uint8 ChannelId
#endif
);

/*handling for SF transmitting.*/
extern FUNC(void, CANTP_CODE) CanTp_TxHandleSFStart(CanTp_ConnectionChannelType* connectionChannel);

/* handling for large SDU transmitting, trying to get TX buffer.*/
extern FUNC(void, CANTP_CODE) CanTp_TxHandleLargeStart(CanTp_ConnectionChannelType* connectionChannel);

/*construct SF PCI information in local buffer for channel.*/
extern FUNC(void, CANTP_CODE) CanTp_ConstructSFPci(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    uint8* dataPtr,
    uint8* totalOffset);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*construct CF PCI information in local buffer for channel.*/
extern FUNC(void, CANTP_CODE) CanTp_ConstructCFPci(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    uint8* dataPtr,
    uint8* totalOffset);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*construct FF PCI information in local buffer for channel.*/
FUNC(void, CANTP_CODE)
CanTp_ConstructFFPci(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    uint8* dataPtr,
    uint8* totalOffset);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               Construct Tx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE) CanTp_ConstructTxMetaDataInfo(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* TxNSduCfgPtr,
    const PduInfoType* pduInfoPtr);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Tx SF Padding.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE) CanTp_TxSFPadding(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txSduCfgPtr,
    PduInfoType* pduInfoPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (STD_ON == CANTP_FD)
/*match CanFd Data length.*/
extern FUNC(PduLengthType, CANTP_CODE) CanTp_MatchDLC(PduLengthType frameDL);
#endif /* STD_ON == CANTP_FD */

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
/*
 * Brief               Construct Rx meta data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_CODE) CanTp_ConstructForwardRxMetaData(
    const CanTp_ConnectionType* RxchannelPtr,
    const CanTp_RxNSduType* RxNSduCfgPtr,
    const PduInfoType* CanTpRxInfoPtr);
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */

/* Reset Timer to start timing */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, CANTP_CODE) CanTp_ResetTime(P2VAR(uint32, AUTOMATIC, CANTP_APPL_DATA) TimerPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/* Get the time passed since last reset */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, CANTP_CODE) CanTp_GetTimeSpan(uint32 TimerPtr, P2VAR(uint32, AUTOMATIC, CANTP_APPL_DATA) TimeSpanPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

END_C_DECLS

#endif /* CANTP_INTERNAL_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
