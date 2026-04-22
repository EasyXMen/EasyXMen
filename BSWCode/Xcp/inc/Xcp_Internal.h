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
 **  @file               : Xcp_Internal.h
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Internal Implementation for XCP
 **
 ***********************************************************************************************************************/

#ifndef XCP_INTERNAL_H_
#define XCP_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "Xcp.h"
#include "Xcp_Callout.h"
#if (XCP_ON_CAN_ENABLE == STD_ON)
#include "CanIf.h"
#include "XcpOnCan_Cbk.h"
#endif /*XCP_ON_CAN_ENABLE == STD_ON*/
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
#include "SoAd.h"
#include "XcpOnEth_Cbk.h"
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/
#include "IStdLib.h"

/* ===================================================== macros ===================================================== */
#define XCP_CONST_0   0x00u
#define XCP_CONST_1   0x01u
#define XCP_CONST_2   0x02u
#define XCP_CONST_3   0x03u
#define XCP_CONST_4   0x04u
#define XCP_CONST_4L  0x04uL
#define XCP_CONST_5   0x05u
#define XCP_CONST_6   0x06u
#define XCP_CONST_7   0x07u
#define XCP_CONST_8   0x08u
#define XCP_CONST_16  0x10uL
#define XCP_CONST_24  0x18u
#define XCP_CONST_32  0x20u
#define XCP_CONST_64  0x40u
#define XCP_CONST_128 0x80u

#define XCP_U8_MASK 0xFFuL
#define XCP_U8_MAX  0xFFu
#define XCP_U16_MAX 0xFFFFu

#define XCP_PID_OFFSET 0x00u
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

#define XCP_CMD_CODE_MIN 0xC0u

/*
 * End of Command Code
 */

#define XCP_RESP_ERROR_OFFSET 0x01u
#define XCP_RESP_POS_PID      0xFFu
#define XCP_RESP_ERR_PID      0xFEu
#define XCP_RESP_ERR_LEN      0x2u
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
#define XCP_DAQ_SS_OVL  0x08u
#define XCP_DAQ_SS_DONE 0x10u

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
#define XCP_PROGRAM_NEXT_SIZE (XCP_PROGRAM_SIZE * XCP_MAX_BS_PGM)
#define XCP_PROGRAM_MAX_SIZE  ((XCP_MAX_CTO_PGM / XCP_AG) - 0x01u)

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
#define XCP_SHORT_UPLOAD_SIZE  ((XCP_MAX_CTO / XCP_AG))

#define XCP_RAM_PAGE_NUM   0u
#define XCP_FLASH_PAGE_NUM 1u

/* ================================================ type definitions ================================================ */
typedef struct
{
    uint32 TxTimeOutCnt;
    uint32 MTA;
    uint8  MTAExtension;
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
    uint16 EthRxCounter;
    uint16 EthTxCounter;
#endif
    Xcp_CommandStatusType CommandStatus;

#if ((XCP_MASTER_BLOCK_MODE == STD_ON) || (XCP_SLAVE_BLOCK_MODE == STD_ON))
    uint16 BlockBufPos;
    uint16 BlockBuflen;
#endif
#if (XCP_INTERLEAVED_MODE == STD_ON)
    uint8   InterLevPosHead;
    uint8   InterLevPosRear;
    uint8   InterLevCmdBuffer[XCP_QUEUE_SIZE][XCP_MAX_CTO];
    uint8   InterLevCmdLen[XCP_QUEUE_SIZE];
    boolean InterLevRecv;
#endif

#if (XCP_SEED_AND_UNLOCK == STD_ON)
    uint8                    ProtectionStatus;
    Xcp_SeedandKeyStatusType KeyRunStatus;
    uint8                    SeedandKeyRes;
    uint8                    SeedandKeyLen;
    uint8                    SeedandKeyPos;
    uint8                    SeedLen;
    uint8                    KeyLen;
#endif
    uint8 SessionStatus;
    uint8 SendStatus;

    uint16         MaxDto;
    uint8          MaxCto;
    uint8          ConnectionId;
    uint8          CmdLength;
    uint8          RespLength;
    uint8          EvLength;
    uint8          ServLength;
    Xcp_StatusType ConnectionStatus;
} Xcp_ChannelCommonType;

/* PRQA S 5004 ++ */ /* VL_Xcp_5004 */
typedef Xcp_ChannelCommonType* Xcp_ChannelCommonPtr;
/* PRQA S 5004 -- */

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
typedef struct
{
    uint8 sendStatus;
    uint8 mode;
    uint8 daqListSize;
    uint8 currentPid;
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
    uint32 timeStamp;
#endif
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
    uint8 prescaler;
    uint8 prescalerCnt;
#endif
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    Xcp_DaqCfgStatusType DynaCfgStatus;
#endif
    uint8        firstPid;
    uint16       eventChannelNum;
    uint32       eventBufferPos;
    uint16       totalEntryLen; /*Record the total length occupied by all ODT entries in DAQ.*/
    Xcp_AGType*  eventBufferPtr;
    Xcp_OdtType* odt;
} Xcp_DaqType;

/* PRQA S 0750 ++ */ /* VL_Xcp_0750 */
typedef union
{
    uint8*       u8ptr;
    Xcp_OdtType* odtPtr;
} Xcp_OdtPtrUnion;

typedef union
{
    uint8*         u8ptr;
    Xcp_EntryType* odtEntryPtr;
} Xcp_OdtEntryPtrUnion;
/* PRQA S 0750 -- */

typedef struct
{
    uint16      SessionCfgId;
    Xcp_DaqType DaqInfo[XCP_MAX_DAQ];

    uint16 PtrDaq;
    uint8  PtrOdt;
    uint8  PtrEntry;

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
    uint8 DynDaqFirstPIDCnt;
#endif
    uint16               DynAllocDaqNum;
    Xcp_DaqCfgStatusType DynDaqSeqStatus;
    uint8*               DynDaqBufPtr;
    uint16               DynDaqBufSize;
    uint16               DynDaqBufPos;
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC*/
#if (XCP_GET_DAQ_CLOCK == STD_ON)
    uint32 RxTimeStamp;
#endif
} Xcp_ChannelDaqType;

/* PRQA S 5004 ++ */ /* VL_Xcp_5004 */
typedef Xcp_ChannelDaqType* Xcp_ChannelDaqPtr;
/* PRQA S 5004 -- */

typedef struct
{
    uint32         EvChBufferUsage[XCP_MAX_EVENT_CHANNEL];
    uint8          EvChActivCnt[XCP_MAX_EVENT_CHANNEL];
    Xcp_DaqNumType DaqListQueue[XCP_MAX_EVENT_CHANNEL][XCP_MAX_DAQ];
    Xcp_DaqNumType DaqListQueuePos[XCP_MAX_EVENT_CHANNEL];
    Xcp_DaqNumType DaqListQueueLength[XCP_MAX_EVENT_CHANNEL];
    boolean        EvChActiveFlag[XCP_MAX_EVENT_CHANNEL]; /* PRQA S 1039 */ /* VL_Xcp_1039 */
} Xcp_ChannelEvChType;
/* PRQA S 5004 ++ */ /* VL_Xcp_5004 */
typedef Xcp_ChannelEvChType* Xcp_ChannelEvChPtr;
/* PRQA S 5004 -- */

#endif /*XCP_PL_DAQ == XCP_PL_DAQ&XCP_RESOURCE*/
typedef enum
{
    XCP_MEM_CAL_RAM,
    XCP_MEM_CAL_ROM,
    XCP_MEM_CAL_ALL,
    XCP_MEM_PGM,
    XCP_MEM_MEA
} Xcp_MemAddrType;

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

typedef struct
{
    uint32             PgmAreaSelect;
    uint32             PgmTargetAddr;
    Xcp_FlsAddressType PgmClrStartAddr; /* clear start address for program_clear command and pending task */
    Xcp_FlsLengthType  PgmClrSize;      /* clear size for program_clear command and pending task */
    uint16             PgmBufSize;
    uint8*             PgmBufAlignPtr;
    Xcp_PgmStautsType  PgmStauts;
    uint8              PgmPendingFlag;

    uint8   PgmClrAccessMode; /*Clear Access Mode(Absolute or Functional)*/
    uint8   PgmAccessMode;    /*Program Access Mode(Absolute or Functional)*/
    uint8   PgmBlkSeqCnt;     /* block sequence counter used for program in functional access mode */
    uint8   PgmRetryCnt;      /* program_clear and program pending task retry counters */
    boolean PgmLastWriteFlag;
} Xcp_ChannelPgmType;

/* PRQA S 5004 ++ */ /* VL_Xcp_5004 */
typedef Xcp_ChannelPgmType* Xcp_ChannelPgmPtr;
/* PRQA S 5004 -- */
#endif

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
extern Xcp_ChannelCommonType Xcp_ChannelCommonData;
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
extern Xcp_ChannelDaqType  Xcp_ChannelDaqData;
extern Xcp_ChannelEvChType Xcp_ChannelEvChData;
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
extern Xcp_ChannelPgmType Xcp_ChannelPtgmData;
#endif
extern boolean Xcp_ModuleInit;
/*
 * Block transfer
 * */
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
extern Xcp_AGType Xcp_BlockBuffer[XCP_BLOCK_BUFFER_SIZE];
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

extern uint8 Xcp_CmdBuffer[XCP_MAX_CTO];
extern uint8 Xcp_RespBuffer[XCP_MAX_CTO];
extern uint8 Xcp_EvBuffer[XCP_MAX_CTO];
extern uint8 Xcp_ServBuffer[XCP_MAX_CTO];
#if (STD_ON == XCP_CHECK_MEA_ADDR)
/* PRQA S 1512 ++ */ /* VL_Xcp_1512 */
extern const Xcp_MeaAddrCheckType Xcp_MeaAddrCfgSet[XCP_CHECK_MEASET_NUM];
/* PRQA S 1512 -- */
#endif
/*
 * DAQ
 *  */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))

#if (XCP_GET_DAQ_CLOCK == STD_ON)
extern uint32 Xcp_RxTimeStamp;
#endif /* XCP_GET_DAQ_CLOCK == STD_ON */

#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

/* Global initial point */
extern const Xcp_ConfigType* Xcp_PbCfgPtr;

/* ========================================= external function declarations ========================================= */
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)

/**
 * @brief       Reads the current system time in milliseconds from the system timer.
 * @return      uint32
 * @retval      Current system time in milliseconds.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63913
 */
extern uint32 Xcp_ReadOutMS(void);

#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP*/

/**
 * @brief       Check the validity of the address range.
 * @param[in]   addr   : The address to be checked for validity.
 * @param[in]   size : The size of the address to be checked.
 * @param[in]   start : The start of the configured address range.
 * @param[in]   end : The end of the configured address range.
 * @return      boolean
 * @retval      TRUE    : Address is valid.
 * @retval      FALSE: Address is invalid.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63962
 */
XCP_LOCAL_INLINE boolean Xcp_CheckMemVaild(uint32 addr, uint32 size, uint32 start, uint32 end)
{
    return ((addr >= start) && (addr <= end) && (size <= (end - addr + 1uL)));
}

/*
 * common services
 */

/**
 * @brief       Initialize the status of the command.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63922
 */
extern void Xcp_CommandInit(void);

/**
 * @brief       Reset all internal states of XCP to their initial states.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63921
 */
extern void Xcp_ClearStatus(void);

/**
 * @brief       Store a four-byte long data into a single byte.
 * @param[in]   u4Data   : The four-byte source data to be copied.
 * @param[in]   byteOrder : Motorola mode or Intel mode.
 * @param[out]  pu1BufferPtr : The target address where the data will be stored.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63927
 */
extern void Xcp_CopyU4ToU1Buffer(const uint32 u4Data, uint8* pu1BufferPtr, uint8 byteOrder);

/**
 * @brief       Copy two-byte data and store it in a single byte.
 * @param[in]   u2Data   : The two-byte source data to be copied.
 * @param[in]   byteOrder : Motorola mode or Intel mode.
 * @param[out]  pu1BufferPtr : The target address where the data will be stored.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63928
 */
extern void Xcp_CopyU2ToU1Buffer(const uint16 u2Data, uint8* pu1BufferPtr, uint8 byteOrder);

/**
 * @brief       Copy a single byte and store it in four-byte data.
 * @param[in]   pu1BufferPtr   : The source data of a single byte to be copied.
 * @param[in]   byteOrder : Motorola mode or Intel mode.
 * @param[out]  pu4Data : The target address for the four-byte data where the data will be stored.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63930
 */
extern void Xcp_CopyU1BufferToU4(const uint8* pu1BufferPtr, uint32* pu4Data, uint8 byteOrder);

/**
 * @brief       Copies a single byte buffer to a two-byte data element, considering the byte order.
 * @param[in]   pu1BufferPtr : Pointer to the source buffer containing single byte data.
 * @param[out]  pu2Data      : Pointer to the destination where the two-byte data will be stored.
 * @param[in]   byteOrder    : Byte order to be used for copying. Motorola mode is MSB first, Intel mode is MSB last.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63931
 */
extern void Xcp_CopyU1BufferToU2(
    const uint8* pu1BufferPtr,
    uint16*      pu2Data,

    uint8 byteOrder);

/**
 * @brief       Copies data from a source buffer to a destination buffer.
 * @param[in]   srcPtr       : Pointer to the source buffer.
 * @param[out]  destPtr      : Pointer to the destination buffer.
 * @param[in]   u4Length     : The length of data to be copied in bytes.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63932
 */
extern void Xcp_BufferCopy(const void* srcPtr, void* destPtr, uint32 u4Length);

/**
 * @brief       Sets an error code in the XCP response buffer.
 * @param[in]   u1ErrorCode : The error code to be set.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63933
 */
extern void Xcp_SetErrorCode(uint8 u1ErrorCode);

/**
 * @brief       Sends a response from the XCP layer.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63935
 */
extern void Xcp_SendResp(void);

/**
 * @brief       Sends an event from the XCP layer.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63944
 */
extern void Xcp_SendEv(void);

/**
 * @brief       Transmits data over the XCP communication channel.
 * @param[in]   pduId       : The identifier of the PDU to be used for transmission.
 * @param[in]   len         : The length of the data to be transmitted.
 * @param[in]   dataPtr     : Pointer to the data buffer to be transmitted.
 * @return      Std_ReturnType
 * @retval      E_OK        : Data transmission request has been accepted
 * @retval      E_NOT_OK    : Data transmission request has not been accepted
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63926
 */
extern Std_ReturnType Xcp_Transmit(PduIdType pduId, PduLengthType len, uint8* dataPtr);

/**
 * @brief       Handles the indication of received data in the XCP communication channel.
 * @param[in]   len         : The length of the received data.
 * @param[in]   dataPtr     : Pointer to the buffer containing the received data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63926
 */
extern void Xcp_RxIndication(const PduLengthType len, const uint8* dataPtr);

/**
 * @brief       Provides confirmation that the transmission of a PDU (Protocol Data Unit) has been completed.
 * @param[in]   pduId          : Identifier of the PDU for which the transmission confirmation is provided.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63952
 */
extern void Xcp_TxConfirmation(const PduIdType pduId);

/**
 * @brief       Checks for detection errors during the reception indication phase of XCP communication.
 * @param[in]   XcpRxPduId    : Identifier of the received PDU for which the reception indication is being processed.
 * @param[in]   XcpRxPduPtr   : Pointer to the PDU information structure containing the received data.
 * @return      boolean
 * @retval      FALSE         : No error detected.
 * @retval      TRUE          : An error was detected.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63948
 */
extern boolean Xcp_RxIndCheckDetErr(PduIdType XcpRxPduId, const PduInfoType* XcpRxPduPtr);

/**
 * @brief       Checks for detection errors during the transmission indication phase of XCP communication.
 * @return      boolean
 * @retval      FALSE         : No error detected.
 * @retval      TRUE          : An error was detected.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63953
 */
extern boolean Xcp_TxIndCheckDetErr(void);

/*
 * STD
 */
/* Xcp STD mandatory commands */
/**
 * @brief       Handles the XCP Connect command to establish a connection with the slave.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64056
 */
extern void Xcp_Connect(void);

/**
 * @brief       Processes the XCP Disconnect command to terminate the connection with the slave.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64058
 */
extern void Xcp_Disconnect(void);

/**
 * @brief       Executes the hardware abstraction layer (HAL) specific actions for disconnecting from the XCP slave.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64057
 */
extern void Xcp_DisconnectHal(void);

/**
 * @brief       Retrieves the current status of the XCP connection.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64059
 */
extern void Xcp_GetStatus(void);

/**
 * @brief       Processes the XCP Synchronization command to synchronize the state of the slave with the master.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64060
 */
extern void Xcp_Synch(void);

/* Xcp STD optional commands */
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
/**
 * @brief       Provides communication mode information as part of the XCP protocol.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64061
 */
extern void Xcp_GetCommModeInfo(void);
#endif
#if (XCP_GET_ID == STD_ON)
/**
 * @brief       Retrieves identification information according to the XCP protocol.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64062
 */
extern void Xcp_GetId(void);
#endif
#if (XCP_SET_REQUEST == STD_ON)
/**
 * @brief       Sets various request types for the XCP session such as storing calibration data or managing DAQ lists.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64063
 */
extern void Xcp_SetRequest(void);
/**
 * @brief       Handles the execution of requests set by the `Xcp_SetRequest` function,
 *              such as storing calibration data or DAQ configurations.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63977
 */
extern void Xcp_SetRequestHandler(void);
#endif

#if (XCP_SEED_AND_UNLOCK == STD_ON)
/**
 * @brief       Retrieves a seed for unlocking protection mechanisms in the XCP protocol.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64066
 */
extern void Xcp_GetSeed(void);
/**
 * @brief       Attempts to unlock a protected resource in the XCP protocol by providing a key.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64068
 */
extern void Xcp_Unlock(void);
#endif
/* Xcp STD optional commands */
#if (XCP_SET_MTA == STD_ON)
/**
 * @brief       Sets the Memory Transfer Address (MTA) used for subsequent data transfer operations.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64069
 */
extern void Xcp_SetMta(void);
/**
 * @brief       Converts the Memory Transfer Address (MTA) and its extension into a pointer for memory access.
 * @param[in]   u1AddExt : Extension byte for the address.
 * @param[in]   u4Add    : Base address for the memory transfer.
 * @return      uint32
 * @retval      uint32   : The calculated pointer address.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64070
 */
uint32 Xcp_Mta2Ptr(const uint8 u1AddExt, const uint32 u4Add);

/**
 * @brief       Validates an address range against the allowed memory areas based on the specified memory type.
 * @param[in]   addr   : The base address to check.
 * @param[in]   size   : The size of the memory area to check.
 * @param[in]   type   : The type of memory area to validate against.
 * @return      boolean
 * @retval      TRUE   : The address range is within the allowed memory area.
 * @retval      FALSE  : The address range is outside the allowed memory area.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64079
 */
extern boolean Xcp_CheckAddress(uint32 addr, uint32 size, Xcp_MemAddrType type);

/**
 * @brief       Updates the Memory Transfer Address (MTA) by a specified length.
 * @param[in]   u4Length : The length to increment the MTA by.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64080
 */
extern void Xcp_UpdateMTA(uint32 u4Length);
#endif

/**
 * @brief       Checks if a given address range is valid for measurement memory.
 * @param[in]   addr   : The base address to check.
 * @param[in]   size   : The size of the memory area to check.
 * @return      boolean
 * @retval      TRUE   : The address range is valid for measurement memory.
 * @retval      FALSE  : The address range is not valid for measurement memory.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64078
 */
boolean Xcp_CheckMeaMem(uint32 addr, uint32 size);

#if (XCP_UPLOAD == STD_ON)
/**
 * @brief       Uploads data from the target system's memory to the master using the current MTA.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64071
 */
extern void Xcp_Upload(void);
#endif
#if (XCP_SHORT_UPLOAD == STD_ON)

/**
 * @brief       Performs a short upload operation, transferring a small amount of data from the
 *              target system's memory to the master.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64073
 */
extern void Xcp_ShortUpload(void);
#endif
#if (XCP_BUILD_CHECKSUM == STD_ON)
/**
 * @brief       Computes a checksum for a specified block of memory.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64076
 */
extern void Xcp_BuildChecksum(void);
#endif
#if (XCP_GET_SLAVE_ID == STD_ON)
/**
 * @brief       Retrieves the slave ID of the XCP device, either through an echo or an inverse echo mechanism.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64077
 */
extern void Xcp_GetSlaveId(void);
#endif

/*
 * CAL/PAG local Function
 */

/**
 * @brief       Initializes the calibration pages for the XCP protocol, copying data from read-only
 *              pages to writable pages.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63961
 */
extern void Xcp_CALInit(void);

/**
 * @brief       Validates an address range against the calibration memory areas (ROM or RAM).
 * @param[in]   addr   : The base address to check.
 * @param[in]   size   : The size of the memory area to check.
 * @param[in]   type   : The type of memory area to validate against.
 * @return      boolean
 * @retval      TRUE   : The address range is valid for the specified memory area.
 * @retval      FALSE  : The address range is not valid for the specified memory area.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63962
 */
extern boolean Xcp_CheckCalMem(uint32 addr, uint32 size, Xcp_MemAddrType type);
/*
 * CAL/PAG Mandatory Function
 */

/**
 * @brief       Downloads data from the master to the target system's memory.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63963
 */
extern void Xcp_Download(void);

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
#if (XCP_MAX_CTO > 8u)
/**
 * @brief       Performs a short download operation, transferring a small amount of data from the
 *              master to the target system's memory.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63965
 */
/* PRQA S 1501 ++ */ /* VL_Xcp_1501 */
extern void Xcp_ShortDownload(void);
/* PRQA S 1501 -- */
#endif
#endif
/*
 * CAL/PAG Optional Function
 */
#if (XCP_DOWNLOAD_NEXT == STD_ON)
/**
 * @brief       Downloads the next block of data from the master to the target system's memory,
 *              continuing from the last MTA.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63966
 */
extern void Xcp_DownloadNext(void);
#endif
#if (XCP_DOWNLOAD_MAX == STD_ON)
/**
 * @brief       Downloads the maximum block of data from the master to the target system's memory.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63968
 */
extern void Xcp_DownloadMax(void);
#endif
#if (XCP_MODIFY_BITS == STD_ON)
/**
 * @brief       Modifies bits at a specific memory location using AND and XOR masks.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63969
 */
extern void Xcp_ModifyBits(void);
#endif
#if (XCP_PAG_SUPPORT == STD_ON)
/**
 * @brief       Sets the calibration page for the XCP protocol.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63972
 */
extern void Xcp_SetCalPage(void);

/**
 * @brief       Retrieves the currently active calibration page for a specific segment.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63974
 */
extern void Xcp_GetCalPage(void);
#endif
/*
 * DAQ
 */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
/**
 * @brief       Initializes the DAQ (Data Acquisition) process, optionally resuming previous sessions.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63976
 */
extern void Xcp_DAQInit(void);

/**
 * @brief       Reinitializes the DAQ (Data Acquisition) process, resetting all DAQ-related structures and counters.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63978
 */
extern void Xcp_DAQReinit(void);

/**
 * @brief       Sends data from a DAQ (Data Acquisition) list to the master, potentially enabling the event channel.
 * @param[in]   evChNum   : Event channel number to send data from.
 * @param[in]   rqstEnable: Boolean indicating whether to enable the event channel.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63985
 */
extern void Xcp_SendDaq(uint16 evChNum, boolean rqstEnable);

/*
 * DAQ mandatory
 */

/**
 * @brief       Sets the pointer to a specific DAQ (Data Acquisition) list, ODT (Object Description Table), and ODT
 * entry.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63991
 */
extern void Xcp_SetDaqPtr(void);

/**
 * @brief       Writes data to a specified DAQ (Data Acquisition) list entry.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63993
 */
extern void Xcp_WriteDaq(void);

/**
 * @brief       Sets the mode for a DAQ (Data Acquisition) list, affecting how data is acquired and transmitted.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63995
 */
extern void Xcp_SetDaqListMode(void);

/**
 * @brief       Starts or stops a DAQ (Data Acquisition) list, controlling the acquisition of data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63997
 */
extern void Xcp_StartStopDaqList(void);

/**
 * @brief       Starts or stops synchronization, affecting the timing of data acquisition processes.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63999
 */
extern void Xcp_StartStopSynch(void);
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
/**
 * @brief       Processes incoming stimulation data, updating the values of specified DAQ (Data Acquisition) entries.
 * @param[in]   dataPtr : Pointer to the received data containing the stimulation information.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63986
 */
extern void Xcp_RxStimHal(const uint8* dataPtr);
#endif
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
/**
 * @brief       Clears a specified DAQ (Data Acquisition) list, stopping its operation if it is running.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64010
 */
extern void Xcp_ClearDaqList(void);
#if (XCP_GET_DAQ_LIST_INFO == STD_ON)
/**
 * @brief       Retrieves information about a specified DAQ (Data Acquisition) list, including its
 *               configuration and associated event channel.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64011
 */
extern void Xcp_GetDaqListInfo(void);
#endif
#else
/**
 * @brief       Frees all DAQ (Data Acquisition) lists, resetting their states and releasing associated resources.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64012
 */
extern void Xcp_FreeDaq(void);

/**
 * @brief       Allocates a DAQ (Data Acquisition) list, preparing it for data acquisition.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64013
 */
extern void Xcp_AllocDaq(void);

/**
 * @brief       Allocates an ODT (Object Description Table) within a DAQ (Data Acquisition) list,
 *              preparing it for data acquisition.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64016
 */
extern void Xcp_AllocOdt(void);

/**
 * @brief       Allocates an ODT (Object Description Table) entry within a DAQ (Data Acquisition) list,
 *              preparing it for data acquisition.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64018
 */
extern void Xcp_AllocOdtEntry(void);
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC*/

/*
 * DAQ optional
 */
#if (XCP_GET_DAQ_CLOCK == STD_ON)
/**
 * @brief       Retrieves the clock or timestamp associated with the DAQ (Data Acquisition) system.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64005
 */
extern void Xcp_GetDaqClock(void);
#endif
#if (XCP_GET_DAQ_PROCESSOR_INFO == STD_ON)
/**
 * @brief       Retrieves processor-specific information related to DAQ (Data Acquisition) capabilities.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64006
 */
extern void Xcp_GetDaqProcessorInfo(void);
#endif
#if (XCP_GET_DAQ_RESOLUTION_INFO == STD_ON)
/**
 * @brief       Retrieves resolution information for DAQ (Data Acquisition) and STIM (Stimulation) operations.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64007
 */
extern void Xcp_GetDaqResolutionInfo(void);
#endif
#if (XCP_GET_DAQ_LIST_MODE == STD_ON)
/**
 * @brief       Retrieves the current mode and configuration details of a specified DAQ (Data Acquisition) list.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64008
 */
extern void Xcp_GetDaqListMode(void);
#endif
#if (XCP_GET_DAQ_EVENT_INFO == STD_ON)
/**
 * @brief       Retrieves information about a specified event channel, including its type, consistency,
 *              maximum number of DAQ lists it can trigger, name length, time cycle, time unit, and priority.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64009
 */
extern void Xcp_GetDaqEventInfo(void);
#endif
#if (XCP_READ_DAQ == STD_ON)
/**
 * @brief       Reads the configuration of a DAQ (Data Acquisition) element, including its size,
 *              address extension, and address.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64001
 */
extern void Xcp_ReadDaq(void);
#endif
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

/*
 * PGM
 */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
/**
 * @brief       Initializes the programming process, clearing buffers and setting initial
 *              conditions for programming operations.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64040
 */
extern void Xcp_ProgramInit(void);

/**
 * @brief       Checks if the given memory range for programming is valid within any of the defined sectors.
 * @param[in]   addr   : Starting address of the memory range to be checked.
 * @param[in]   size   : Size of the memory range to be checked.
 * @return      boolean
 * @retval      TRUE   : The memory range is valid within at least one sector.
 * @retval      FALSE  : The memory range is not valid within any sector.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64026
 */
extern boolean Xcp_CheckPgmMem(uint32 addr, uint32 size);

/**
 * @brief       Handles pending programming tasks based on the current state of the programming process.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64041
 */
extern void Xcp_PgmPendingTask(void);
/*
 * PGM mandatory
 */

/**
 * @brief       Starts the programming session, checking for active DAQ sessions and protection status before
 * proceeding.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64042
 */
extern void Xcp_ProgramStart(void);

/**
 * @brief       Clears a specified memory range for programming, ensuring that the operation is
 *              within allowed boundaries and that no DAQ sessions are active.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64043
 */
extern void Xcp_ProgramClear(void);

/**
 * @brief       Executes a programming operation, writing data to the target memory according to the command received.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64046
 */
extern void Xcp_Program(void);

/**
 * @brief       Resets the programming session, transitioning the system back to an idle state
 *              and disconnecting the hardware abstraction layer.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64047
 */
extern void Xcp_ProgramReset(void);
/*
 * PGM optional
 */
#if (XCP_GET_PGM_PROCESSOR_INFO == STD_ON)
/**
 * @brief       Retrieves information about the programming processor, including its properties
 *              and the maximum number of sectors.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64048
 */
extern void Xcp_GetPgmProcessorInfo(void);
#endif
#if (XCP_GET_SECTOR_INFO == STD_ON)
/**
 * @brief       Retrieves information about a specific sector, such as its start address, length,
 *              or name length, based on the requested mode.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64049
 */
extern void Xcp_GetSectorInfo(void);
#endif
#if (XCP_PROGRAM_PREPARE == STD_ON)
/**
 * @brief       Prepares the system for a programming operation, currently not supported by the implementation.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64050
 */
extern void Xcp_ProgramPrepare(void);
#endif
#if (XCP_PROGRAM_FORMAT == STD_ON)
/**
 * @brief       Sets the format for the upcoming programming operation, including compression, encryption,
 *              programming, and access methods.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64051
 */
extern void Xcp_ProgramFormat(void);
#endif
#if (XCP_PROGRAM_NEXT == STD_ON)
/**
 * @brief       Writes the next block of data to the target memory, handling alignment and buffer management
 *              to ensure efficient programming.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64052
 */
extern void Xcp_ProgramNext(void);
#endif
#if (XCP_PROGRAM_MAX == STD_ON)
/**
 * @brief       Programs a large block of data to the target memory, verifying command syntax,
 *              protection status, and current programming state before proceeding.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64054
 */
extern void Xcp_ProgramMax(void);
#endif
#if (XCP_PROGRAM_VERIFY == STD_ON)
/**
 * @brief       Verifies the programmed data against a specified value or condition, ensuring the
 *              integrity of the programming process.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64055
 */
extern void Xcp_ProgramVerify(void);
#endif
/*
 * End of PGM
 */
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/

#endif /* XCP_INTERNAL_H_ */
