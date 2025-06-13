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
********************************************************************************
**                                                                            **
**  FILENAME    : Xcp_Internal.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Internal Implementation for XCP                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef XCP_INTERNAL_H_
#define XCP_INTERNAL_H_

/*=======[I N C L U D E S]====================================================*/
#include "Xcp.h"
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
#include XCP_FLS_HEADER
#endif
#if (XCP_ON_CAN_ENABLE == STD_ON)
#include "CanIf.h"
#include "XcpOnCan_Cbk.h"
#endif /*XCP_ON_CAN_ENABLE == STD_ON*/
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
#include "SoAd.h"
#include "XcpOnEth_Cbk.h"
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/

/*=======[M A C R O S]========================================================*/
#define XCP_PID_OFFSET          0x00u

#define XCP_AVOID_WARNNING(var) (void)(var)

#define XCP_LOCAL_INLINE        static inline

/*
 * Command Code
 */
/* Standard commands */
#define XCP_CMD_CONNECT             0xFFu
#define XCP_CMD_DISCONNECT          0xFEu
#define XCP_CMD_GET_STATUS          0xFDu
#define XCP_CMD_SYNCH               0xFCu
#define XCP_CMD_GET_COMM_MODE_INFO  0xFBu
#define XCP_CMD_GET_ID              0xFAu
#define XCP_CMD_SET_REQUEST         0xF9u
#define XCP_CMD_GET_SEED            0xF8u
#define XCP_CMD_UNLOCK              0xF7u
#define XCP_CMD_SET_MTA             0xF6u
#define XCP_CMD_UPLOAD              0xF5u
#define XCP_CMD_SHORT_UPLOAD        0xF4u
#define XCP_CMD_BUILD_CHECKSUM      0xF3u
#define XCP_CMD_TRANSPORT_LAYER_CMD 0xF2u
#define XCP_CMD_TL_SUB_CODE_OFFSET  0x01u
#define XCP_CMD_TL_GET_SLAVE_ID     0xFFu
#define XCP_CMD_TL_GET_DAQ_ID       0xFEu
#define XCP_CDM_TL_SET_DAQ_ID       0xFDu
#define XCP_CMD_USER_CMD            0xF1u

/* Calibration commands */
#define XCP_CMD_DOWNLOAD       0xF0u
#define XCP_CMD_DOWNLOAD_NEXT  0xEFu
#define XCP_CMD_DOWNLOAD_MAX   0xEEu
#define XCP_CMD_SHORT_DOWNLOAD 0xEDu
#define XCP_CMD_MODIFY_BITS    0xECu

/* Page switching commands */
#define XCP_CMD_SET_CAL_PAGE           0xEBu
#define XCP_CMD_GET_CAL_PAGE           0xEAu
#define XCP_CMD_GET_PAG_PROCESSOR_INFO 0xE9u
#define XCP_CMD_GET_SEGMENT_INFO       0xE8u
#define XCP_CMD_GET_PAGE_INFO          0xE7u
#define XCP_CMD_SET_SEGMENT_MODE       0xE6u
#define XCP_CMD_GET_SEGMENT_MODE       0xE5u
#define XCP_CMD_COPY_CAL_PAGE          0xE4u

/* Data acquisition and stimulation commands */
#define XCP_CMD_CLEAR_DAQ_LIST          0xE3u
#define XCP_CMD_SET_DAQ_PTR             0xE2u
#define XCP_CMD_WRITE_DAQ               0xE1u
#define XCP_CMD_SET_DAQ_LIST_MODE       0xE0u
#define XCP_CMD_GET_DAQ_LIST_MODE       0xDFu
#define XCP_CMD_START_STOP_DAQ_LIST     0xDEu
#define XCP_CMD_START_STOP_SYNCH        0xDDu
#define XCP_CMD_GET_DAQ_CLOCK           0xDCu
#define XCP_CMD_READ_DAQ                0xDBu
#define XCP_CMD_GET_DAQ_PROCESSOR_INFO  0xDAu
#define XCP_CMD_GET_DAQ_RESOLUTION_INFO 0xD9u
#define XCP_CMD_GET_DAQ_LIST_INFO       0xD8u
#define XCP_CMD_GET_DAQ_EVENT_INFO      0xD7u
#define XCP_CMD_FREE_DAQ                0xD6u
#define XCP_CMD_ALLOC_DAQ               0xD5u
#define XCP_CMD_ALLOC_ODT               0xD4u
#define XCP_CMD_ALLOC_ODT_ENTRY         0xD3u

/* Non-volatile memory programming commands */
#define XCP_CMD_PROGRAM_START          0xD2u
#define XCP_CMD_PROGRAM_CLEAR          0xD1u
#define XCP_CMD_PROGRAM                0xD0u
#define XCP_CMD_PROGRAM_RESET          0xCFu
#define XCP_CMD_GET_PGM_PROCESSOR_INFO 0xCEu
#define XCP_CMD_GET_SECTOR_INFO        0xCDu
#define XCP_CMD_PROGRAM_PREPARE        0xCCu
#define XCP_CMD_PROGRAM_FORMAT         0xCBu
#define XCP_CMD_PROGRAM_NEXT           0xCAu
#define XCP_CMD_PROGRAM_MAX            0xC9u
#define XCP_CMD_PROGRAM_VERIFY         0xC8u

#define XCP_CMD_CODE_MIN               0xC0u

/*
 * End of Command Code
 */

#define XCP_RESP_ERROR_OFFSET 0x01u
#define XCP_RESP_POS_PID      0xFFu
#define XCP_RESP_ERR_PID      0xFEu

/*
 * Error Code
 */
#define XCP_ERR_CMD_SYNCH         0x00u
#define XCP_ERR_CMD_BUSY          0x10u
#define XCP_ERR_DAQ_ACTIVE        0x11u
#define XCP_ERR_PGM_ACTIVE        0x12u
#define XCP_ERR_CMD_UNKNOWN       0x20u
#define XCP_ERR_CMD_SYNTAX        0x21u
#define XCP_ERR_OUT_OF_RANGE      0x22u
#define XCP_ERR_WRITE_PROTECTED   0x23u
#define XCP_ERR_ACCESS_DENIED     0x24u
#define XCP_ERR_ACCESS_LOCKED     0x25u
#define XCP_ERR_PAGE_NOT_VALID    0x26u
#define XCP_ERR_MODE_NOT_VALID    0x27u
#define XCP_ERR_SEGMENT_NOT_VALID 0x28u
#define XCP_ERR_SEQUENCE          0x29u
#define XCP_ERR_DAQ_CONFIG        0x2Au
#define XCP_ERR_MEMORY_OVERFLOW   0x30u
#define XCP_ERR_GENERIC           0x31u
#define XCP_ERR_VERIFY            0x32u
#define XCP_ERR_RES_TEMP_NOT_A    0x33u
/*
 * End of Error Code
 */
#define XCP_EV_PID                0xFDu
#define XCP_EV_CODE_OFFEST        0x01u
#define XCP_EV_RESUME_SSID_OFFEST 0x02u
#define XCP_EV_RESUME_TS_OFFEST   0x04u
/*
 *  Event Code
 */
#define XCP_EV_RESUME_MODE        0x00u
#define XCP_EV_CLEAR_DAQ          0x01u
#define XCP_EV_STORE_DAQ          0x02u
#define XCP_EV_STORE_CAL          0x03u
#define XCP_EV_CMD_PENDING        0x05u
#define XCP_EV_DAQ_OVERLOAD       0x06u
#define XCP_EV_SESSION_TERMINATED 0x07u
#define XCP_EV_TIME_SYNC          0x08u
#define XCP_EV_STIM_TIMEOUT       0x09u
#define XCP_EV_SLEEP              0x0Au
#define XCP_EV_WAKE_UP            0x0Bu
#define XCP_EV_USER               0xFEu
#define XCP_EV_TRANSPORT          0xFFu
/*
 * End of Event Code
 */
/* Bitmask for SessionStatus */
#define XCP_SESSION_STATE_STORE_CAL   0x01u
#define XCP_SESSION_STATE_STORE_DAQ   0x04u
#define XCP_SESSION_STATE_CLEAR_DAQ   0x08u
#define XCP_SESSION_STATE_DAQ_RUNNING 0x40u
#define XCP_SESSION_STATE_RESUME      0x80u

/* Bitmasks for sendStatus */
#define XCP_RESP_REQUEST 0x01u
#define XCP_EV_REQUEST   0x02u
#define XCP_DAQ_REQUEST  0x04u
#define XCP_SERV_REQUEST 0x08u
#define XCP_SEND_REQUEST 0x0fu
#define XCP_RESP_PENDING 0x10u
#define XCP_EV_PENDING   0x20u
#define XCP_DAQ_PENDING  0x40u
#define XCP_SERV_PENDING 0x80u
#define XCP_SEND_PENDING 0xf0u
#define XCP_SEND_IDLE    0x00u

/* Flash area selection */
#define XCP_FLASHAREA_CAL   0x01u
#define XCP_FLASHAREA_CODE  0x02u
#define XCP_FLASHAREA_NVRAM 0x04u

/* Bitmask for DAQ sendStatus */
/*ALTERNATING*/
#define XCP_DAQ_SS_AL      0x01u
#define XCP_DAQ_SS_QUEUE   0x02u
#define XCP_DAQ_SS_SENDING 0x04u
/* OVERLOAD */
#define XCP_DAQ_SS_OVL      0x08u
#define XCP_DAQ_SS_DONE     0x10u

#define XCP_DAQ_MOD_RESUME  0x80u
#define XCP_DAQ_MOD_RUNNING 0x40u
/* Bit 4 is Time Stamp */
#define XCP_DAQ_MOD_PIDOFF 0x20u
/* Bit 4 is Time Stamp */
#define XCP_DAQ_MOD_TS 0x10u
/* Bit 1 is Direction */
#define XCP_DAQ_MOD_DIR_BIT  0x02u /* DIRECTION */
#define XCP_DAQ_MOD_DIR_DAQ  0X00u
#define XCP_DAQ_MOD_DIR_STIM 0x02u
/* Bit 0 is Alternating */
#define XCP_DAQ_MOD_SELE   0x01u /* SELECTED */
#define XCP_DAQ_MOD_AL_BIT 0x01u

/* XCP checksum types */
#define XCP_CHECKSUM_TYPE_ADD_11      0x01u
#define XCP_CHECKSUM_TYPE_ADD_12      0x02u
#define XCP_CHECKSUM_TYPE_ADD_14      0x03u
#define XCP_CHECKSUM_TYPE_ADD_22      0x04u
#define XCP_CHECKSUM_TYPE_ADD_24      0x05u
#define XCP_CHECKSUM_TYPE_ADD_44      0x06u
#define XCP_CHECKSUM_TYPE_CRC_16      0x07u
#define XCP_CHECKSUM_TYPE_CRC_16_CITT 0x08u
#define XCP_CHECKSUM_TYPE_CRC_32      0x09u
#define XCP_CHECKSUM_TYPE_USER        0xFFu

#if (XCP_ADDR_GRANULARITY == XCP_AG_BYTE)
#define XCP_UPLOAD_DATA_OFFSET (0x01u)
#elif (XCP_ADDR_GRANULARITY == XCP_AG_WORD)
#define XCP_UPLOAD_DATA_OFFSET (0x02u)
#elif (XCP_ADDR_GRANULARITY == XCP_AG_DWORD)
#define XCP_UPLOAD_DATA_OFFSET (0x04u)
#endif

#define XCP_PROGRAM_LEN_OFFSET 0x01u
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
#define XCP_PROGRAM_DATA_OFFSET 0x02u
#else
#define XCP_PROGRAM_DATA_OFFSET 0x04u
#endif
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
#define XCP_PROGRAM_SIZE ((XCP_MAX_CTO_PGM - 2u) / XCP_AG)
#else
#define XCP_PROGRAM_SIZE ((XCP_MAX_CTO_PGM / XCP_AG) - 0x01u)
#endif
#define XCP_PROGRAM_NEXT_SIZE   (XCP_PROGRAM_SIZE * XCP_MAX_BS_PGM)
#define XCP_PROGRAM_MAX_SIZE    ((XCP_MAX_CTO_PGM / XCP_AG) - 0x01u)

#define XCP_DOWNLOAD_LEN_OFFSET 0x01u
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
#define XCP_DOWNLOAD_DATA_OFFSET 0X02u
#else
#define XCP_DOWNLOAD_DATA_OFFSET 0X04u
#endif

#define XCP_DOWNLOADMAX_SIZE    ((XCP_MAX_CTO / XCP_AG) - 0x01u)
#define XCP_SHORT_DOWNLOAD_SIZE ((XCP_MAX_CTO - 8u) / XCP_AG)

#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
#define XCP_DOWNLOAD_SIZE ((XCP_MAX_CTO - 2u) / XCP_AG)
#else
#define XCP_DOWNLOAD_SIZE ((XCP_MAX_CTO / XCP_AG) - 0x01u)
#endif
#define XCP_DOWNLOAD_NEXT_SIZE (XCP_DOWNLOAD_SIZE * XCP_MAX_BS)
#define XCP_DOWNLOAD_MAX_SIZE  ((XCP_MAX_CTO / XCP_AG) - 0x01u)
#define XCP_UPLOAD_SIZE        ((XCP_MAX_CTO / XCP_AG) - 0x01u)
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
typedef struct
{
    uint8 sendStatus;
    uint8 mode;
    uint8 daqListSize;
    uint8 currentPid;
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
    uint32 timeStamp;
#endif
#if (XCP_DAQ_PRIORITY_SUPPORT == STD_ON)
    uint8 priority;
#endif
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
    uint8 prescaler;
    uint8 prescalerCnt;
#endif
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    Xcp_DaqCfgStatusType DynaCfgStatus;
#endif
    uint8 firstPid;
    uint16 eventChannelNum;
    uint32 eventBufferPos;
    P2VAR(Xcp_AGType, AUTOMATIC, XCP_VAR) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    eventBufferPtr;
    P2VAR(Xcp_OdtType, AUTOMATIC, XCP_VAR) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    odt;
} Xcp_DaqType;

#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
typedef struct
{
    uint32 bufferSize;
    uint32 bufferPos;
} Xcp_DynamicalDaqQueueType;
#endif /*XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE*/
#endif /*XCP_PL_DAQ == XCP_PL_DAQ&XCP_RESOURCE*/
typedef enum
{
    XCP_MEM_CAL_RAM = 0u,
    XCP_MEM_CAL_ROM,
    XCP_MEM_CAL_ALL,
    XCP_MEM_PGM,
    XCP_MEM_MEA
} Xcp_MemAddrType;

#if ((STD_ON == XCP_MASTER_BLOCK_MODE) || (STD_ON == XCP_SLAVE_BLOCK_MODE))
typedef VAR(uint16, TYPEDEF) Xcp_BlockBufIndxType;
#endif

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
/* Xcp program Status */
typedef enum
{
    XCP_PGM_IDLE,
    XCP_PGM_START,
    XCP_PGM_CLEARED,
    XCP_PGM_PROGRAMMING,
    XCP_PGM_PROGRAMMED
} Xcp_PgmStautsType;
#endif

#if (XCP_INTERLEAVED_MODE == STD_ON)
typedef struct
{
    uint8 posHead;
    uint8 posRear;
} Xcp_InterlevModeCtrlType;
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/

/*=======[E X T E R N A L   D A T A]==========================================*/
extern VAR(uint16, XCP_VAR) Xcp_EthRxCounter;
extern VAR(uint16, XCP_VAR) Xcp_EthTxCounter;

/*
 * Block transfer
 * */
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
extern VAR(Xcp_AGType, XCP_VAR) Xcp_BlockBuffer[XCP_BLOCK_BUFFER_SIZE];
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

#if ((XCP_MASTER_BLOCK_MODE == STD_ON) || (XCP_SLAVE_BLOCK_MODE == STD_ON)) /* Var for Block transfer */
extern VAR(Xcp_BlockBufIndxType, XCP_VAR) Xcp_BlockBufferPos;
extern VAR(Xcp_BlockBufIndxType, XCP_VAR) Xcp_BlockBufferLen;
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON || XCP_SLAVE_BLOCK_MODE == STD_ON*/

extern VAR(uint16, XCP_VAR) Xcp_SessionCfgId;

#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)

extern uint32 Xcp_ReadOutMS(void);

#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/

extern VAR(uint8, XCP_VAR_NO_INIT_8) Xcp_CmdBuffer[XCP_MAX_CTO];
extern VAR(uint8, XCP_VAR_NO_INIT_8) Xcp_RespBuffer[XCP_MAX_CTO];
extern VAR(uint8, XCP_VAR) Xcp_EvBuffer[XCP_CTO_BUFFER_SIZE];
extern VAR(uint8, XCP_VAR) Xcp_ServBuffer[XCP_CTO_BUFFER_SIZE];

extern VAR(uint8, XCP_VAR) Xcp_CmdLength;
extern VAR(uint8, XCP_VAR) Xcp_RespLength;
extern VAR(uint8, XCP_VAR) Xcp_EvLength;

extern VAR(uint8, XCP_VAR_INIT_8) Xcp_ProtectionStatus;
extern VAR(uint8, XCP_VAR_INIT_8) Xcp_SessionStatus;
extern VAR(uint8, XCP_VAR_INIT_8) Xcp_SendStatus;

extern VAR(uint8, XCP_VAR) Xcp_TransferTimeOutCnt;

extern VAR(Xcp_StatusType, XCP_VAR_INIT_UNSPECIFIED) Xcp_Status;
extern VAR(Xcp_CommandStatusType, XCP_VAR_INIT_UNSPECIFIED) Xcp_CommandStatus;

#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)

extern uint32 Xcp_ReadOutMS(void);

#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/

#if (XCP_SET_MTA == STD_ON)
extern VAR(Xcp_AddressType, XCP_VAR_INIT_UNSPECIFIED) Xcp_MTA;
#endif

/*
 * Std Seed&key
 *  */
extern VAR(Xcp_SeedandKeyStatusType, XCP_VAR_INIT_UNSPECIFIED) Xcp_SeedandKeyStatus;

#if (XCP_SEED_AND_UNLOCK == STD_ON)
extern VAR(uint8, XCP_VAR_INIT_8) Xcp_SeedandKeyRes;
#endif

/*
 * DAQ
 *  */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
extern VAR(Xcp_DaqType, XCP_VAR) Xcp_Daq[XCP_MAX_DAQ];

#if (XCP_GET_DAQ_CLOCK == STD_ON)
extern VAR(uint32, XCP_VAR) Xcp_RxTimeStamp;
#endif /* XCP_GET_DAQ_CLOCK == STD_ON */

#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
extern VAR(uint32, XCP_VAR) Xcp_UploadInfoLen;
#endif

/* Global initial point */
extern P2CONST(Xcp_PCConfigType, XCP_VAR_CLEARED_32, XCP_APPL_DATA) Xcp_GlobalCfgPtr;
extern P2CONST(Xcp_ConfigType, XCP_VAR_CLEARED_32, XCP_APPL_DATA) Xcp_PbCfgPtr;

/*
 * CAL
 * */
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
#if (XCP_PAG_SUPPORT == STD_ON)
extern VAR(uint8, XCP_VAR) Xcp_ActivPagNum[XCP_MAX_SEGMENT];
#endif
#endif
/*
 * PGM
 * */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
extern VAR(Xcp_PgmStautsType, XCP_VAR) Xcp_PgmStauts;
#endif /*XCP_PL_PGM == XCP_PL_PGM&XCP_RESOURCE*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*Check the validity of the address range*/
XCP_LOCAL_INLINE FUNC(boolean, XCP_CODE) Xcp_CheckMemVaild(uint32 addr, uint32 size, uint32 start, uint32 end)
{
    return ((addr >= start) && (addr <= end) && (size <= (end - addr + 1u)));
}

/*
 * common services
 */

extern FUNC(void, XCP_CODE) Xcp_CommandInit(void);
extern FUNC(void, XCP_CODE) Xcp_ClearStatus(void);
extern FUNC(void, XCP_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Xcp_CopyU4ToU1Buffer(const uint32 u4Data, P2VAR(uint8, AUTOMATIC, AUTOMATIC) pu1BufferPtr, uint8 byteOrder);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
extern FUNC(void, XCP_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Xcp_CopyU2ToU1Buffer(const uint16 u2Data, P2VAR(uint8, AUTOMATIC, AUTOMATIC) pu1BufferPtr, uint8 byteOrder);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
extern FUNC(void, XCP_CODE) Xcp_CopyU1BufferToU4(
    P2CONST(uint8, AUTOMATIC, AUTOMATIC) pu1BufferPtr,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, AUTOMATIC) pu4Data,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint8 byteOrder);
extern FUNC(void, XCP_CODE) Xcp_CopyU1BufferToU2(
    P2CONST(uint8, AUTOMATIC, AUTOMATIC) pu1BufferPtr,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, AUTOMATIC) pu2Data,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint8 byteOrder);
extern FUNC(void, XCP_CODE) Xcp_BufferCopy(uint32 u4SourceAddr, uint32 u4DestAddr, uint32 u4Length);

extern FUNC(void, XCP_CODE) Xcp_SetErrorCode(uint8 u1ErrorCode);
extern FUNC(void, XCP_CODE) Xcp_SendResp(void);
extern FUNC(void, XCP_CODE) Xcp_SendEv(void);
extern FUNC(Std_ReturnType, XCP_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Xcp_Transmit(PduIdType pduId, PduLengthType len, P2VAR(uint8, AUTOMATIC, XCP_VAR) dataPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#if (XCP_PID_OFF_SUPPORT == STD_OFF)
extern FUNC(void, XCP_CODE) Xcp_RxIndication(const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#endif
extern FUNC(void, XCP_CODE) Xcp_TxConfirmation(const PduIdType pduId);
extern FUNC(boolean, XCP_CODE)
    Xcp_RxIndCheckDetErr(PduIdType XcpRxPduId, P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr);
extern FUNC(boolean, XCP_CODE) Xcp_TxIndCheckDetErr(void);

/*
 * STD
 */
/* Xcp STD mandatory commands */
extern FUNC(void, XCP_CODE) Xcp_Connect(void);
extern FUNC(void, XCP_CODE) Xcp_Disconnect(void);
extern FUNC(void, XCP_CODE) Xcp_DisconnectHal(void);
extern FUNC(void, XCP_CODE) Xcp_GetStatus(void);
extern FUNC(void, XCP_CODE) Xcp_Synch(void);

/* Xcp STD optional commands */
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetCommModeInfo(void);
#endif
#if (XCP_GET_ID == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetId(void);
#endif
#if (XCP_SET_REQUEST == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_SetRequest(void);
extern FUNC(void, XCP_CODE) Xcp_SetRequestHandler(void);
#endif

#if (XCP_SEED_AND_UNLOCK == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetSeed(void);
extern FUNC(void, XCP_CODE) Xcp_Unlock(void);
#endif
/* Xcp STD optional commands */
#if (XCP_SET_MTA == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_SetMta(void);
FUNC(uint32, XCP_CODE) Xcp_Mta2Ptr(const uint8 u1AddExt, const uint32 u4Add);
extern FUNC(boolean, XCP_CODE) Xcp_CheckAddress(uint32 addr, uint32 size, Xcp_MemAddrType type);
extern FUNC(void, XCP_CODE) Xcp_UpdateMTA(uint32 u4Length);
#endif

#if (XCP_UPLOAD == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_Upload(void);
#endif
#if (XCP_SHORT_UPLOAD == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ShortUpload(void);
#endif
#if (XCP_BUILD_CHECKSUM == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_BuildChecksum(void);
#endif
#if (XCP_GET_SLAVE_ID == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetSlaveId(void);
#endif

/*
 * CAL/PAG local Function
 */
extern FUNC(void, XCP_CODE) Xcp_CALInit(void);

extern FUNC(boolean, XCP_CODE) Xcp_CheckCalMem(uint32 addr, uint32 size, Xcp_MemAddrType type);
/*
 * CAL/PAG Mandatory Function
 */
extern FUNC(void, XCP_CODE) Xcp_Download(void);

extern FUNC(void, XCP_CODE) Xcp_ShortDownload(void);
/*
 * CAL/PAG Optional Function
 */
#if (XCP_DOWNLOAD_NEXT == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_DownloadNext(void);
#endif
#if (XCP_DOWNLOAD_MAX == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_DownloadMax(void);
#endif
#if (XCP_MODIFY_BITS == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ModifyBits(void);
#endif
#if (XCP_PAG_SUPPORT == STD_ON)
#if (XCP_SET_CAL_PAGE == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_SetCalPage(void);
#endif
#if (XCP_GET_CAL_PAGE == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetCalPage(void);
#endif
#endif
/*
 * DAQ
 */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
extern FUNC(void, XCP_CODE) Xcp_DAQInit(void);
extern FUNC(void, XCP_CODE) Xcp_DAQReinit(void);
extern FUNC(void, XCP_CODE) Xcp_SendDaq(void);

/*
 * DAQ mandatory
 */
extern FUNC(void, XCP_CODE) Xcp_SetDaqPtr(void);
extern FUNC(void, XCP_CODE) Xcp_WriteDaq(void);
extern FUNC(void, XCP_CODE) Xcp_SetDaqListMode(void);
extern FUNC(void, XCP_CODE) Xcp_StartStopDaqList(void);
extern FUNC(void, XCP_CODE) Xcp_StartStopSynch(void);
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
extern FUNC(void, XCP_CODE) Xcp_RxStimHal(P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#endif
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
extern FUNC(void, XCP_CODE) Xcp_ClearDaqList(void);
#if (XCP_GET_DAQ_LIST_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetDaqListInfo(void);
#endif
#else
extern FUNC(void, XCP_CODE) Xcp_FreeDaq(void);
extern FUNC(void, XCP_CODE) Xcp_AllocDaq(void);
extern FUNC(void, XCP_CODE) Xcp_AllocOdt(void);
extern FUNC(void, XCP_CODE) Xcp_AllocOdtEntry(void);
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC*/

/*
 * DAQ optional
 */
#if (XCP_GET_DAQ_CLOCK == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetDaqClock(void);
#endif
#if (XCP_GET_DAQ_PROCESSOR_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetDaqProcessorInfo(void);
#endif
#if (XCP_GET_DAQ_RESOLUTION_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetDaqResolutionInfo(void);
#endif
#if (XCP_GET_DAQ_LIST_MODE == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetDaqListMode(void);
#endif
#if (XCP_GET_DAQ_EVENT_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetDaqEventInfo(void);
#endif
#if (XCP_READ_DAQ == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ReadDaq(void);
#endif
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

/*
 * PGM
 */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
extern FUNC(void, XCP_CODE) Xcp_ProgramInit(void);
extern FUNC(boolean, XCP_CODE) Xcp_CheckPgmMem(uint32 addr, uint32 size);
extern FUNC(void, XCP_CODE) Xcp_PgmPendingTask(void);
/*
 * PGM mandatory
 */
extern FUNC(void, XCP_CODE) Xcp_ProgramStart(void);
extern FUNC(void, XCP_CODE) Xcp_ProgramClear(void);
extern FUNC(void, XCP_CODE) Xcp_Program(void);
extern FUNC(void, XCP_CODE) Xcp_ProgramReset(void);
/*
 * PGM optional
 */
#if (XCP_GET_PGM_PROCESSOR_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetPgmProcessorInfo(void);
#endif
#if (XCP_GET_SECTOR_INFO == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_GetSectorInfo(void);
#endif
#if (XCP_PROGRAM_PREPARE == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ProgramPrepare(void);
#endif
#if (XCP_PROGRAM_FORMAT == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ProgramFormat(void);
#endif
#if (XCP_PROGRAM_NEXT == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ProgramNext(void);
#endif
#if (XCP_PROGRAM_MAX == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ProgramMax(void);
#endif
#if (XCP_PROGRAM_VERIFY == STD_ON)
extern FUNC(void, XCP_CODE) Xcp_ProgramVerify(void);
#endif
/*
 * End of PGM
 */
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/
#endif /* XCP_INTERNAL_H_ */
