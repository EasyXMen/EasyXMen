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
 ***********************************************************************************************************************
 **
 **  @file               : Xcp.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Implementation for XCP
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"
#include "SchM_Xcp.h"

#if (XCP_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/* ===================================================== macros ===================================================== */
#define XCP_C_AR_MAJOR_VERSION 4u
#define XCP_C_AR_MINOR_VERSION 9u
#define XCP_C_AR_PATCH_VERSION 0u
#define XCP_C_SW_MAJOR_VERSION 3u
#define XCP_C_SW_MINOR_VERSION 0u
#define XCP_C_SW_PATCH_VERSION 3u

#if (XCP_DEV_ERROR_DETECT == STD_ON)
#define XCP_VENDOR_ID (uint16)62
#define XCP_MODULE_ID (uint16)212
#endif

/* ------------------------------------------------- Version  Check ------------------------------------------------- */
#if (XCP_C_AR_MAJOR_VERSION != XCP_H_AR_MAJOR_VERSION)
#error "Xcp.c : Mismatch in Specification Major Version"
#endif
#if (XCP_C_AR_MINOR_VERSION != XCP_H_AR_MINOR_VERSION)
#error "Xcp.c : Mismatch in Specification Major Version"
#endif
#if (XCP_C_AR_PATCH_VERSION != XCP_H_AR_PATCH_VERSION)
#error "Xcp.c : Mismatch in Specification Major Version"
#endif
#if (XCP_C_SW_MAJOR_VERSION != XCP_H_SW_MAJOR_VERSION)
#error "Xcp.c : Mismatch in Specification Major Version"
#endif
#if (XCP_C_SW_MINOR_VERSION != XCP_H_SW_MINOR_VERSION)
#error "Xcp.c : Mismatch in Specification Major Version"
#endif
#if (XCP_C_SW_PATCH_VERSION != XCP_H_SW_PATCH_VERSION)
#error "Xcp.c : Mismatch in Specification Major Version"
#endif

/* ========================================== internal function declarations ======================================== */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
XCP_LOCAL void Xcp_PendingTaskHandle(void);
XCP_LOCAL void Xcp_GenericCommandHandle(void);
XCP_LOCAL void Xcp_RxCommandHal(const PduLengthType len, const uint8* dataPtr);
XCP_LOCAL void Xcp_TxQueueHandler(void);
XCP_LOCAL void Xcp_SendServ(void);
#if (XCP_BYTE_ORDER_CHECK == STD_ON)
XCP_LOCAL boolean Xcp_ByteOrder_Check(void);
#endif

#if (XCP_MASTER_BLOCK_MODE == STD_ON)
XCP_LOCAL void Xcp_RxBlockHal(const uint8 pid, const PduLengthType len, const uint8* dataPtr);
#endif

#if (XCP_INTERLEAVED_MODE == STD_ON)
XCP_LOCAL void    Xcp_InterLeavedHandler(void);
XCP_LOCAL boolean Xcp_IsInterleavedBufFull(void);
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/

XCP_LOCAL void Xcp_Cmd_ReservedCmd(void);

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#define XCP_START_SEC_CONST_PTR
#include "Xcp_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Xcp_TYPE_CAST_001 */
XCP_LOCAL void (*const Xcp_CmdFct[])(void) = {
    Xcp_Disconnect, /* 0xFE = DISCONNECT */
    Xcp_GetStatus,  /* 0xFD = GET_STATUS */
    Xcp_Synch,      /* 0xFC = SYNCH */
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
    Xcp_GetCommModeInfo, /* 0xFB = GET_COMM_MODE_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xFB = GET_COMM_MODE_INFO */
#endif

#if (XCP_GET_ID == STD_ON)
    Xcp_GetId, /* 0xFA = GET_ID */
#else
    Xcp_Cmd_ReservedCmd, /* 0xFA = GET_ID */
#endif

#if (XCP_SET_REQUEST == STD_ON)
    Xcp_SetRequest, /* 0xF9 = SET_REQUEST */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF9 = SET_REQUEST */
#endif

#if (XCP_SEED_AND_UNLOCK == STD_ON)
    Xcp_GetSeed, /* 0xF8 = GET_SEED */
    Xcp_Unlock,  /* 0xF7 = UNLOCK */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF8 = GET_SEED */
    Xcp_Cmd_ReservedCmd, /* 0xF7 = UNLOCK */
#endif

#if (XCP_SET_MTA == STD_ON)
    Xcp_SetMta, /* 0xF6 = SET_MTA */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF6 = SET_MTA */
#endif

#if (XCP_UPLOAD == STD_ON)
    Xcp_Upload, /* 0xF5 = UPLOAD */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF5 = UPLOAD */
#endif

#if (XCP_SHORT_UPLOAD == STD_ON)
    Xcp_ShortUpload, /* 0xF4 = SHORT_UPLOAD */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF4 = SHORT_UPLOAD */
#endif

#if (XCP_BUILD_CHECKSUM == STD_ON)
    Xcp_BuildChecksum, /* 0xF3 = BUILD_CHECKSUM */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF3 = BUILD_CHECKSUM */
#endif

    Xcp_Cmd_ReservedCmd, /* 0xF2 = TRANSPORT_LAYER_CMD */
    Xcp_Cmd_ReservedCmd, /* 0xF1 = USER_CMD */

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
    Xcp_Download, /* 0xF0 = DOWNLOAD */
#else
    Xcp_Cmd_ReservedCmd, /* 0xF0 = DOWNLOAD */
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_DOWNLOAD_NEXT == STD_ON)
    Xcp_DownloadNext, /* 0xEF = DOWNLOAD_NEXT */
#else
    Xcp_Cmd_ReservedCmd, /* 0xEF = DOWNLOAD_NEXT */
#endif

#if (XCP_DOWNLOAD_MAX == STD_ON)
    Xcp_DownloadMax, /* 0xEE = DOWNLOAD_MAX */
#else
    Xcp_Cmd_ReservedCmd, /* 0xEE = DOWNLOAD_MAX */
#endif

#if ((XCP_MAX_CTO > 8u) && (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE)))
    Xcp_ShortDownload, /* 0xED = SHORT_DOWNLOAD */
#else
    Xcp_Cmd_ReservedCmd, /* 0xED = SHORT_DOWNLOAD */
#endif

#if (XCP_MODIFY_BITS == STD_ON)
    Xcp_ModifyBits, /* 0xEC = MODIFY_BITS */
#else
    Xcp_Cmd_ReservedCmd, /* 0xEC = MODIFY_BITS */
#endif

#if (XCP_PAG_SUPPORT == STD_ON)
    Xcp_SetCalPage, /* 0xEB = SET_CAL_PAGE */
    Xcp_GetCalPage, /* 0xEA = GET_CAL_PAGE */
#else
    Xcp_Cmd_ReservedCmd, /* 0xEB = SET_CAL_PAGE */
    Xcp_Cmd_ReservedCmd, /* 0xEA = GET_CAL_PAGE */
#endif

    Xcp_Cmd_ReservedCmd, /* 0xE9 = GET_PAG_PROCESSOR_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xE8 = GET_SEGMENT_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xE7 = GET_PAGE_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xE6 = SET_SEGMENT_MODE */
    Xcp_Cmd_ReservedCmd, /* 0xE5 = GET_SEGMENT_MODE */
    Xcp_Cmd_ReservedCmd, /* 0xE4 = COPY_CAL_PAGE */

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
    Xcp_ClearDaqList, /* 0xE3 = CLEAR_DAQ_LIST */
#else
    Xcp_Cmd_ReservedCmd, /* 0xE3 = CLEAR_DAQ_LIST */
#endif
    Xcp_SetDaqPtr,      /* 0xE2 = SET_DAQ_PTR */
    Xcp_WriteDaq,       /* 0xE1 = WRITE_DAQ */
    Xcp_SetDaqListMode, /* 0xE0 = SET_DAQ_LIST_MODE */
#if (STD_ON == XCP_GET_DAQ_LIST_MODE)
    Xcp_GetDaqListMode, /* 0xDF = GET_DAQ_LIST_MODE */
#else
    Xcp_Cmd_ReservedCmd, /* 0xDF = GET_DAQ_LIST_MODE */
#endif
    Xcp_StartStopDaqList, /* 0xDE = START_STOP_DAQ_LIST */
    Xcp_StartStopSynch,   /* 0xDD = START_STOP_SYNCH */
#if (XCP_GET_DAQ_CLOCK == STD_ON)
    Xcp_GetDaqClock, /* 0xDC = GET_DAQ_CLOCK */
#else
    Xcp_Cmd_ReservedCmd, /* 0xDC = GET_DAQ_CLOCK */
#endif

#if (XCP_READ_DAQ == STD_ON)
    Xcp_ReadDaq, /* 0xDB = READ_DAQ */
#else
    Xcp_Cmd_ReservedCmd, /* 0xDB = READ_DAQ */
#endif

#if (XCP_GET_DAQ_PROCESSOR_INFO == STD_ON)
    Xcp_GetDaqProcessorInfo, /* 0xDA = GET_DAQ_PROCESSOR_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xDA = GET_DAQ_PROCESSOR_INFO */
#endif

#if (XCP_GET_DAQ_RESOLUTION_INFO == STD_ON)
    Xcp_GetDaqResolutionInfo, /* 0xD9 = GET_DAQ_RESOLUTION_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xD9 = GET_DAQ_RESOLUTION_INFO */
#endif

#if (XCP_GET_DAQ_LIST_INFO == STD_ON)
    Xcp_GetDaqListInfo, /* 0xD8 = GET_DAQ_LIST_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xD8 = GET_DAQ_LIST_INFO */
#endif

#if (XCP_GET_DAQ_EVENT_INFO == STD_ON)
    Xcp_GetDaqEventInfo, /* 0xD7 = GET_DAQ_EVENT_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xD7 = GET_DAQ_EVENT_INFO */
#endif

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    Xcp_FreeDaq,       /* 0xD6 = FREE_DAQ */
    Xcp_AllocDaq,      /* 0xD5 = ALLOC_DAQ */
    Xcp_AllocOdt,      /* 0xD4 = ALLOC_ODT */
    Xcp_AllocOdtEntry, /* 0xD3 = ALLOC_ODT_ENTRY */
#else
    Xcp_Cmd_ReservedCmd, /* 0xD6 = FREE_DAQ */
    Xcp_Cmd_ReservedCmd, /* 0xD5 = ALLOC_DAQ */
    Xcp_Cmd_ReservedCmd, /* 0xD4 = ALLOC_ODT */
    Xcp_Cmd_ReservedCmd, /* 0xD3 = ALLOC_ODT_ENTRY */
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC*/

#else
    Xcp_Cmd_ReservedCmd, /* 0xE3 = CLEAR_DAQ_LIST */
    Xcp_Cmd_ReservedCmd, /* 0xE2 = SET_DAQ_PTR */
    Xcp_Cmd_ReservedCmd, /* 0xE1 = WRITE_DAQ */
    Xcp_Cmd_ReservedCmd, /* 0xE0 = SET_DAQ_LIST_MODE */
    Xcp_Cmd_ReservedCmd, /* 0xDF = GET_DAQ_LIST_MODE */
    Xcp_Cmd_ReservedCmd, /* 0xDE = START_STOP_DAQ_LIST */
    Xcp_Cmd_ReservedCmd, /* 0xDD = START_STOP_SYNCH */
    Xcp_Cmd_ReservedCmd, /* 0xDC = GET_DAQ_CLOCK */
    Xcp_Cmd_ReservedCmd, /* 0xDB = READ_DAQ */
    Xcp_Cmd_ReservedCmd, /* 0xDA = GET_DAQ_PROCESSOR_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xD9 = GET_DAQ_RESOLUTION_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xD8 = GET_DAQ_LIST_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xD7 = GET_DAQ_EVENT_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xD6 = FREE_DAQ */
    Xcp_Cmd_ReservedCmd, /* 0xD5 = ALLOC_DAQ */
    Xcp_Cmd_ReservedCmd, /* 0xD4 = ALLOC_ODT */
    Xcp_Cmd_ReservedCmd, /* 0xD3 = ALLOC_ODT_ENTRY */
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    Xcp_ProgramStart, /* 0xD2 = PROGRAM_START */
    Xcp_ProgramClear, /* 0xD1 = PROGRAM_CLEAR */
    Xcp_Program,      /* 0xD0 = PROGRAM */
    Xcp_ProgramReset, /* 0xCF = PROGRAM_RESET */
#if (XCP_GET_PGM_PROCESSOR_INFO == STD_ON)
    Xcp_GetPgmProcessorInfo, /* 0xCE = GET_PGM_PROCESSOR_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xCE = GET_PGM_PROCESSOR_INFO */
#endif

#if (XCP_GET_SECTOR_INFO == STD_ON)
    Xcp_GetSectorInfo, /* 0xCD = GET_SECTOR_INFO */
#else
    Xcp_Cmd_ReservedCmd, /* 0xCD = GET_SECTOR_INFO */
#endif

#if (XCP_PROGRAM_PREPARE == STD_ON)
    Xcp_ProgramPrepare, /* 0xCC = PROGRAM_PREPARE */
#else
    Xcp_Cmd_ReservedCmd, /* 0xCC = PROGRAM_PREPARE */
#endif

#if (XCP_PROGRAM_FORMAT == STD_ON)
    Xcp_ProgramFormat, /* 0xCB = PROGRAM_FORMAT */
#else
    Xcp_Cmd_ReservedCmd, /* 0xCB = PROGRAM_FORMAT */
#endif

#if (XCP_PROGRAM_NEXT == STD_ON)
    Xcp_ProgramNext, /* 0xCA = PROGRAM_NEXT */
#else
    Xcp_Cmd_ReservedCmd, /* 0xCA = PROGRAM_NEXT */
#endif

#if (XCP_PROGRAM_MAX == STD_ON)
    Xcp_ProgramMax, /* 0xC9 = PROGRAM_MAX */
#else
    Xcp_Cmd_ReservedCmd, /* 0xC9 = PROGRAM_MAX */
#endif

#if (XCP_PROGRAM_VERIFY == STD_ON)
    Xcp_ProgramVerify, /* 0xC8 = PROGRAM_VERIFY */
#else
    Xcp_Cmd_ReservedCmd, /* 0xC8 = PROGRAM_VERIFY */
#endif

#else                    /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/
    Xcp_Cmd_ReservedCmd, /* 0xD2 = PROGRAM_START */
    Xcp_Cmd_ReservedCmd, /* 0xD1 = PROGRAM_CLEAR */
    Xcp_Cmd_ReservedCmd, /* 0xD0 = PROGRAM */
    Xcp_Cmd_ReservedCmd, /* 0xCF = PROGRAM_RESET */
    Xcp_Cmd_ReservedCmd, /* 0xCE = GET_PGM_PROCESSOR_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xCD = GET_SECTOR_INFO */
    Xcp_Cmd_ReservedCmd, /* 0xCC = PROGRAM_PREPARE */
    Xcp_Cmd_ReservedCmd, /* 0xCB = PROGRAM_FORMAT */
    Xcp_Cmd_ReservedCmd, /* 0xCA = PROGRAM_NEXT */
    Xcp_Cmd_ReservedCmd, /* 0xC9 = PROGRAM_MAX */
    Xcp_Cmd_ReservedCmd, /* 0xC8 = PROGRAM_VERIFY */
#endif                   /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/
    Xcp_Cmd_ReservedCmd, /* 0xC7 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC6 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC5 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC4 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC3 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC2 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC1 = RESERVED */
    Xcp_Cmd_ReservedCmd, /* 0xC0 = RESERVED */
};
/* PRQA S 3218 -- */
#define XCP_STOP_SEC_CONST_PTR
#include "Xcp_MemMap.h"

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
Xcp_ChannelCommonType Xcp_ChannelCommonData;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
boolean Xcp_ModuleInit = FALSE;
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
const Xcp_ConfigType* Xcp_PbCfgPtr;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
uint8 Xcp_CmdBuffer[XCP_MAX_CTO];
uint8 Xcp_RespBuffer[XCP_MAX_CTO];
uint8 Xcp_EvBuffer[XCP_MAX_CTO];
/* PRQA S 1514 ++ */ /* VL_Xcp_IndirectUse */
uint8 Xcp_ServBuffer[XCP_MAX_CTO];
/* PRQA S 1514 -- */
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

/*
 * Block transfer
 * */
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
Xcp_AGType Xcp_BlockBuffer[XCP_BLOCK_BUFFER_SIZE];
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
#endif

/* ========================================== external function definitions ========================================= */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
/**
 * Initialize the XCP module with the provided configuration.This function
 * sets up the XCP communication protocol by configuring thephysical layer,
 * calibration, DAQ measurement,PGM flash, and initializes the internal state
 *  machines of XCP.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void Xcp_Init(const Xcp_ConfigType* Xcp_ConfigPtr)
{
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Xcp_ConfigPtr)
    {
        (void)Det_ReportError(
            (uint16)XCP_MODULE_ID,
            (uint8)XCP_INSTANCE_ID,
            (uint8)XCP_SERVICE_ID_INIT,
            (uint8)XCP_E_PARAM_POINTER);
    }
    else
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/

#if (XCP_BYTE_ORDER_CHECK == STD_ON)
        if (Xcp_ByteOrder_Check() == FALSE)
    {
        Xcp_ModuleInit = FALSE;
#if (XCP_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(
            (uint16)XCP_MODULE_ID,
            (uint8)XCP_INSTANCE_ID,
            (uint8)XCP_SERVICE_ID_INIT,
            (uint8)XCP_E_INIT_FAILED);
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/
    }
    else
#endif
    {
        Xcp_PbCfgPtr = Xcp_ConfigPtr;
        (void)IStdLib_MemSet(&Xcp_ChannelCommonData, 0, sizeof(Xcp_ChannelCommonData));
        /*Initialize runtime state variables*/
        Xcp_ChannelCommonData.ProtectionStatus = (uint8)(XCP_PROTECTION);
        Xcp_ChannelCommonData.ConnectionStatus = XCP_DISCONNECT;
        Xcp_ChannelCommonData.SendStatus       = XCP_SEND_IDLE;
#if (XCP_SEED_AND_UNLOCK == STD_ON)
        Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_SEED;
#endif
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
        Xcp_CALInit();
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        Xcp_ProgramInit();
#endif /*XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE*/

#if (XCP_BUILD_CHECKSUM == STD_ON)
        Xcp_InitCrc16CcittTable();
#endif

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
        Xcp_DAQInit();
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/
        Xcp_ModuleInit = TRUE;
    }
    return;
}
/* PRQA S 1503 -- */

#if (XCP_BYTE_ORDER_CHECK == STD_ON)
XCP_LOCAL boolean Xcp_ByteOrder_Check(void)
{
    uint32 testVar = 1;
    uint8  Order   = *(uint8*)&testVar;

    /*Check if the endianness matches*/
    if (Order != (uint8)CPU_BYTE_ORDER)
    {
        return FALSE;
    }

    return TRUE;
}
#endif

#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
/**
 * Reads the current system timer value and converts it to milliseconds.This
 * function retrieves the current value of the system timer and converts it
 * from ticks to milliseconds,providing the elapsed time since the system
 * timer was last reset.
 */
uint32 Xcp_ReadOutMS(void)
{
    uint32 OSCurMs;

    (void)GetCounterValue((uint16)XCP_SYSTEMTIMER, &OSCurMs);
    /*tick to ms*/
    OSCurMs = (uint32)(XCP_OS_TICKS2MS(OSCurMs));

    return (OSCurMs);
}
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP*/

/**
 * Main function for XCP protocol handling.Manages transmission status,
 * timeout, and command execution.Handles pending tasks, set requests,
 * and interleaved mode operations.Processes received commands and
 * executes them accordingly.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_MainFunction(void)
/* PRQA S 1532 -- */
{
    /* In case something is in transmission */
    if (0x00U != (Xcp_ChannelCommonData.SendStatus & XCP_SEND_PENDING))
    {
        Xcp_ChannelCommonData.TxTimeOutCnt++;
        if (Xcp_ChannelCommonData.TxTimeOutCnt >= XCP_TANSFER_TIME_OUT)
        {
            Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_SEND_PENDING);
            if (0x00U != ((XCP_CMD_TRANSF | XCP_CMD_BLOCK_TRANSF) & Xcp_ChannelCommonData.CommandStatus))
            {
                Xcp_CommandInit();
            }
            Xcp_ChannelCommonData.TxTimeOutCnt = 0x00u;
        }
    }
    /* In case last send request is denied. Restart the tx queue in the main function */
    /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
    else if (0x00U != (Xcp_ChannelCommonData.SendStatus & XCP_SEND_REQUEST))
    /* PRQA S 2985 -- */
    {
        Xcp_TxQueueHandler();
    }
    else
    {
        /* Do Nothing */
    }
    /* Handle the pending task. */
    Xcp_PendingTaskHandle();
#if ((XCP_SET_REQUEST == STD_ON) && (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE)))
    if (0u != Xcp_ChannelCommonData.SessionStatus)
    {
        Xcp_SetRequestHandler();
    }
#endif /*XCP_SET_REQUEST == STD_ON*/
#if (XCP_INTERLEAVED_MODE == STD_ON)
    Xcp_InterLeavedHandler();
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
    /* One Command has been recieved successfully */
    if (XCP_CMD_RECV == Xcp_ChannelCommonData.CommandStatus)
    {
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_EXECUTE;
        Xcp_GenericCommandHandle();
    }
    return;
}

#if (XCP_INTERLEAVED_MODE == STD_ON)
/**
 * Handles interleaved data reception in XCP protocol.
 * This function processes incoming interleaved commands,
 * extracts command data, and updates the command reception status.
 */

XCP_LOCAL void Xcp_InterLeavedHandler(void)
{
    if (Xcp_ChannelCommonData.InterLevRecv == TRUE)
    {
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_RECV;
        Xcp_ChannelCommonData.CmdLength = (Xcp_ChannelCommonData.InterLevCmdLen)[Xcp_ChannelCommonData.InterLevPosHead];
        for (uint8 bufIdx = 0u; bufIdx < Xcp_ChannelCommonData.CmdLength; bufIdx++)
        {
            Xcp_CmdBuffer[bufIdx] =
                (Xcp_ChannelCommonData.InterLevCmdBuffer)[Xcp_ChannelCommonData.InterLevPosHead][bufIdx];
        }
        Xcp_ChannelCommonData.InterLevPosHead++;
        if (Xcp_ChannelCommonData.InterLevPosHead == XCP_QUEUE_SIZE)
        {
            Xcp_ChannelCommonData.InterLevPosHead = 0u;
        }
        if (Xcp_ChannelCommonData.InterLevPosHead == Xcp_ChannelCommonData.InterLevPosRear)
        {
            Xcp_ChannelCommonData.InterLevRecv = FALSE;
        }
    }
}
/**
 * Checks if the interleaved buffer is full.This function determines
 * whether the interleaved buffer is full by comparing the rear and
 * head positions of the buffer. If the buffer is full, it returns
 * TRUE; otherwise, it returns FALSE.
 */
XCP_LOCAL boolean Xcp_IsInterleavedBufFull(void)
{
    boolean isFull = FALSE;
    uint8   idleBufLen;

    if ((Xcp_ChannelCommonData.InterLevPosRear == Xcp_ChannelCommonData.InterLevPosHead)
        && (Xcp_ChannelCommonData.InterLevRecv == TRUE))
    {
        isFull = TRUE;
    }
    else
    {
        if (Xcp_ChannelCommonData.InterLevPosRear >= Xcp_ChannelCommonData.InterLevPosHead)
        {
            idleBufLen = Xcp_ChannelCommonData.InterLevPosRear - Xcp_ChannelCommonData.InterLevPosHead;
        }
        else
        {
            idleBufLen = Xcp_ChannelCommonData.InterLevPosRear + XCP_QUEUE_SIZE - Xcp_ChannelCommonData.InterLevPosHead;
        }
        if (idleBufLen == XCP_QUEUE_SIZE)
        {
            isFull = TRUE;
        }
    }
    return isFull;
}
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/

#if (XCP_VERSION_INFO_API == STD_ON)
/**
 * Retrieves version information of the XCP module.
 * This function fills a provided structure with version information of the XCP
 * module, including vendor ID, module ID, and software version numbers. If the
 * provided pointer is NULL, an error is reported if error detection is enabled.
 */
void Xcp_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    Std_VersionInfoType Xcp_VersionInfo = {
        XCP_VENDOR_ID,
        XCP_MODULE_ID,

        XCP_C_SW_MAJOR_VERSION,
        XCP_C_SW_MINOR_VERSION,
        XCP_C_SW_PATCH_VERSION};
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == versioninfo)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_GET_VERSION_INFO, XCP_E_PARAM_POINTER);
    }
    else
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/
    {
        versioninfo->moduleID = Xcp_VersionInfo.moduleID;
        versioninfo->vendorID = Xcp_VersionInfo.vendorID;

        versioninfo->sw_major_version = Xcp_VersionInfo.sw_major_version;
        versioninfo->sw_minor_version = Xcp_VersionInfo.sw_minor_version;
        versioninfo->sw_patch_version = Xcp_VersionInfo.sw_patch_version;
    }
    return;
}
#endif

#if (XCP_SUPPRESS_TX_SUPPORTED == STD_ON)
/**
 * Set whether the XCP transfer mode is enabled or disabled.
 */
void Xcp_SetTransmissionMode(NetworkHandleType Channel, Xcp_TransmissionModeType Mode)
{
    (void)Channel;
    (void)Mode;
}
#endif

/**
 * Clears the current status of the XCP channel. This function sets the protection
 * status of the XCP channel to a predefined protection level, resets the session
 * status, send status, and timeout counter. It also clears the command length,
 * response length, event length, and service length. If Ethernet functionality
 * is enabled, this function will also reset the Ethernet receive and transmit counters.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_ClearStatus(void)
/* PRQA S 1532 -- */
{
#if (XCP_SEED_AND_UNLOCK == STD_ON)
    Xcp_ChannelCommonData.ProtectionStatus = (uint8)(XCP_PROTECTION);
#endif
    Xcp_ChannelCommonData.SessionStatus = 0U;
    Xcp_ChannelCommonData.SendStatus    = XCP_SEND_IDLE;
    Xcp_ChannelCommonData.TxTimeOutCnt  = 0U;

    Xcp_ChannelCommonData.CmdLength  = 0U;
    Xcp_ChannelCommonData.RespLength = 0U;
    Xcp_ChannelCommonData.EvLength   = 0U;
    Xcp_ChannelCommonData.ServLength = 0U;

    return;
}

/**
 * Initializes the command processing for the XCP channel. This function sets the
 * command status to idle. If either master or slave block mode is enabled, it
 * initializes the block buffer position and length to zero. In interleaved mode,
 * if enabled, it also resets the positions of the rear and head of the interleave
 * buffer and sets the interleave receive flag to false.
 */
void Xcp_CommandInit(void)
{
    Xcp_ChannelCommonData.CommandStatus = XCP_CMD_IDLE;
#if ((XCP_MASTER_BLOCK_MODE == STD_ON) || (XCP_SLAVE_BLOCK_MODE == STD_ON))
    Xcp_ChannelCommonData.BlockBufPos = 0U;
    Xcp_ChannelCommonData.BlockBuflen = 0U;
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON)||(XCP_SLAVE_BLOCK_MODE == STD_ON*/
#if (XCP_INTERLEAVED_MODE == STD_ON)
    Xcp_ChannelCommonData.InterLevPosRear = 0u;
    Xcp_ChannelCommonData.InterLevPosHead = 0u;
    Xcp_ChannelCommonData.InterLevRecv    = FALSE;
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
    return;
}

/**
 * Handles the pending tasks for the XCP protocol layer. This function checks if
 * the programming resource is enabled and, if so, calls the function responsible
 * for handling pending programming tasks.
 */
XCP_LOCAL void Xcp_PendingTaskHandle(void)
{
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    Xcp_PgmPendingTask();
#endif
    return;
}

/**
 * Handles generic commands for the XCP protocol. This function reads the command
 * code from the command buffer, initializes the response buffer, and processes
 * the command based on the current connection status. It supports commands like
 * connect, disconnect, and get slave ID, among others, while ignoring unsupported
 * commands or those received when not connected.
 */
XCP_LOCAL void Xcp_GenericCommandHandle(void)
{
    uint8 u1CommandCode = Xcp_CmdBuffer[XCP_PID_OFFSET];

    /* initialize the PID of RESP frame */
    Xcp_RespBuffer[XCP_PID_OFFSET]   = XCP_RESP_POS_PID;
    Xcp_ChannelCommonData.RespLength = 0x01U;

    /* in non-CONNECT status only treat Connect & get_slave_id*/
    if (XCP_CMD_CONNECT == u1CommandCode)
    {
        Xcp_Connect();
    }
    /* other command shall be ignored when status is not connect */
    else if (XCP_CONNECT == Xcp_ChannelCommonData.ConnectionStatus)
    {
        if ((XCP_CMD_CODE_MIN < u1CommandCode))
        {
            Xcp_CmdFct[XCP_CMD_DISCONNECT - u1CommandCode]();
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_CMD_UNKNOWN);
            Xcp_SendResp();
        }
    }
#if (XCP_GET_SLAVE_ID == STD_ON)
    else if (
        (XCP_CMD_TRANSPORT_LAYER_CMD == u1CommandCode)
        && (XCP_CMD_TL_GET_SLAVE_ID == Xcp_CmdBuffer[XCP_CMD_TL_SUB_CODE_OFFSET]))
    {
        Xcp_GetSlaveId();
    }
#endif /*XCP_GET_SLAVE_ID == STD_ON*/
    else if (XCP_USERDEFINE == Xcp_ChannelCommonData.ConnectionStatus)
    {
        /* Do nothing Now if needed some upper-level test commands can be
         * located here.
         */
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_IDLE;
    }
    else
    {
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_IDLE;
    }
    return;
}

/**
 * Transmits data over the communication channel used by the XCP protocol. This
 * function prepares the data to be sent, sets the length and data pointer, and
 * calls the appropriate transmit function based on whether CAN or Ethernet is
 * being used. For Ethernet, it also handles the framing of the data. The function
 * returns the status of the transmission operation.
 */
Std_ReturnType Xcp_Transmit(PduIdType pduId, PduLengthType len, uint8* dataPtr)
{
    PduInfoType    pduInfo;
    Std_ReturnType ret;
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
    const uint8* srcPtr;
    uint8*       destPtr;
    uint8        ethFrame[len + 4u];
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/

    pduInfo.MetaDataPtr = NULL_PTR;
#if (XCP_ON_CAN_ENABLE == STD_ON)
/* set length */
#if (XCP_CAN_MAX_DLC_REQUIRED == STD_ON)
    pduInfo.SduLength = XCP_CAN_MAX_DLC;
    XCP_AVOID_WARNNING(len);
#else
    pduInfo.SduLength = len;
#endif /* XCP_CAN_MAX_DLC == STD_ON */
    /* set data ptr */
    pduInfo.SduDataPtr = dataPtr;
    /* call API and return the result */
    ret = CanIf_Transmit(pduId, &pduInfo);
#elif (XCP_ON_ETHERNET_ENABLE == STD_ON)
    pduInfo.SduLength  = len + 4u;
    pduInfo.SduDataPtr = ethFrame;
    Xcp_CopyU2ToU1Buffer(len, &(ethFrame[0u]), LOW_BYTE_FIRST);
    Xcp_CopyU2ToU1Buffer(Xcp_ChannelCommonData.EthTxCounter, &(ethFrame[2u]), LOW_BYTE_FIRST);
    /*This CTR value is to be increased for each packet regardless of the type
     *  (RES, ERR_EV, SERV, DAQ)*/
    srcPtr  = (uint8*)&(dataPtr[0u]);
    destPtr = (uint8*)&(ethFrame[4u]);
    Xcp_BufferCopy(srcPtr, destPtr, len);
    ret = SoAd_IfTransmit(pduId, &pduInfo);
    Xcp_ChannelCommonData.EthTxCounter++;
#else
    /*Don't support.*/
    ret = E_NOT_OK;
#endif
    return ret;
}

/**
 * Copies a 32-bit unsigned integer to a byte buffer according to the specified
 * byte order. This function takes the 32-bit data, the destination buffer pointer,
 * and the byte order (high byte first or high byte last), then populates the buffer
 * with the bytes of the 32-bit data in the correct order.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void Xcp_CopyU4ToU1Buffer(const uint32 u4Data, uint8* pu1BufferPtr, uint8 byteOrder)
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        pu1BufferPtr[0u]          = (uint8)((u4Data) >> XCP_CONST_24);
        pu1BufferPtr[XCP_CONST_1] = (uint8)(((u4Data) >> XCP_CONST_16) & XCP_U8_MASK);
        pu1BufferPtr[XCP_CONST_2] = (uint8)(((u4Data) >> XCP_CONST_8) & XCP_U8_MASK);
        pu1BufferPtr[XCP_CONST_3] = (uint8)((u4Data)&XCP_U8_MASK);
    }
    /*in case of Intel MSB last*/
    else
    {
        pu1BufferPtr[XCP_CONST_3] = (uint8)((u4Data) >> XCP_CONST_24);
        pu1BufferPtr[XCP_CONST_2] = (uint8)(((u4Data) >> XCP_CONST_16) & XCP_U8_MASK);
        pu1BufferPtr[XCP_CONST_1] = (uint8)(((u4Data) >> XCP_CONST_8) & XCP_U8_MASK);
        pu1BufferPtr[0u]          = (uint8)((u4Data)&XCP_U8_MASK);
    }
    return;
}
/* PRQA S 1503 -- */

/**
 * Copies a 16-bit unsigned integer to a byte buffer according to the specified
 * byte order. This function takes the 16-bit data, the destination buffer pointer,
 * and the byte order (high byte first or high byte last), then populates the buffer
 * with the bytes of the 16-bit data in the correct order.
 */
void Xcp_CopyU2ToU1Buffer(const uint16 u2Data, uint8* pu1BufferPtr, uint8 byteOrder)
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        pu1BufferPtr[0u]          = (uint8)((u2Data) >> XCP_CONST_8);
        pu1BufferPtr[XCP_CONST_1] = (uint8)((u2Data)&XCP_U8_MASK);
    }
    /*in case of Intel MSB last*/
    else
    {
        pu1BufferPtr[XCP_CONST_1] = (uint8)((u2Data) >> XCP_CONST_8);
        pu1BufferPtr[0u]          = (uint8)((u2Data)&XCP_U8_MASK);
    }
    return;
}

/**
 * Copies a 32-bit unsigned integer from a byte buffer according to the specified
 * byte order. This function takes the source buffer pointer, the destination
 * data pointer, and the byte order (high byte first or high byte last), then
 * constructs the 32-bit data from the bytes in the buffer in the correct order.
 */
void Xcp_CopyU1BufferToU4(const uint8* pu1BufferPtr, uint32* pu4Data, uint8 byteOrder)
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        *pu4Data = ((uint32)pu1BufferPtr[0u] << XCP_CONST_24) + ((uint32)pu1BufferPtr[XCP_CONST_1] << XCP_CONST_16)
                   + ((uint32)pu1BufferPtr[XCP_CONST_2] << XCP_CONST_8) + ((uint32)pu1BufferPtr[XCP_CONST_3]);
    }
    /*in case of Intel MSB last*/
    else
    {
        *pu4Data = ((uint32)pu1BufferPtr[XCP_CONST_3] << XCP_CONST_24)
                   + ((uint32)pu1BufferPtr[XCP_CONST_2] << XCP_CONST_16)
                   + ((uint32)pu1BufferPtr[XCP_CONST_1] << XCP_CONST_8) + ((uint32)pu1BufferPtr[0u]);
    }
    return;
}

/**
 * Copies a 16-bit unsigned integer from a byte buffer according to the specified
 * byte order. This function takes the source buffer pointer, the destination
 * data pointer, and the byte order (high byte first or high byte last), then
 * constructs the 16-bit data from the bytes in the buffer in the correct order.
 */
void Xcp_CopyU1BufferToU2(const uint8* pu1BufferPtr, uint16* pu2Data, uint8 byteOrder)
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        *pu2Data = (uint16)((uint16)pu1BufferPtr[0u] << XCP_CONST_8) + (uint16)pu1BufferPtr[XCP_CONST_1];
    }
    /*in case of Intel MSB last*/
    else
    {
        *pu2Data = (uint16)((uint16)pu1BufferPtr[XCP_CONST_1] << XCP_CONST_8) + (uint16)pu1BufferPtr[0u];
    }
    return;
}

/**
 * Copies data from a source buffer to a destination buffer. This function performs
 * the copy operation using the most efficient method based on the data length and
 * alignment. It supports optimized copying for 32-bit and 16-bit aligned data, as
 * well as a fallback for byte-wise copying. The function ensures compliance with
 * MISRA coding standards and considers the byte order for unaligned data.
 */
/*PRQA S 3673 ++*/ /* VL_QAC_3673 */
void Xcp_BufferCopy(const void* srcPtr, void* destPtr, uint32 u4Length)
/*PRQA S 3673 --*/
{
    uint32 i;
    /* PRQA S 0306,0309,0326 ++ */ /* VL_Xcp_0306,VL_Xcp_0309,VL_Xcp_0326 */
    uint32 u4SourceAddr = (uint32)srcPtr;
    uint32 u4DestAddr   = (uint32)destPtr;
    /* PRQA S 0306,0309,0326 -- */
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
    /* PRQA S 0306,0309,0326 ++ */ /* VL_Xcp_0306,VL_Xcp_0309,VL_Xcp_0326 */
    const uint8* u8Src = (uint8*)u4SourceAddr;
    /* PRQA S 0306,0309,0326 -- */
#endif

#if (XCP_COPY_SPEED_UP == STD_ON)
    /*1.4-byte Data Copy.*/
    if (((u4Length % sizeof(uint32)) == (uint32)0uL)
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
        /*Address alignment(4 byte)*/
        && ((u4DestAddr % sizeof(uint32)) == (uint32)0uL)
#endif /*XCP_ACCESS_ALIGN_ENABLE == STD_ON*/
    )
    {
        /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
        uint32* u32dest = (uint32*)u4DestAddr;
        /* PRQA S 0306,0309 -- */
        const uint32* u32Src;
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
        if ((u4SourceAddr % sizeof(uint32)) == (uint32)0uL)
        {
            /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
            u32Src = (uint32*)u4SourceAddr;
            /* PRQA S 0306,0309 -- */
            for (i = 0u; i < (u4Length / sizeof(uint32)); i++)
            {
                u32dest[i] = u32Src[i];
            }
        }
        else
        {
            Xcp_CopyU1BufferToU4(u8Src, u32dest, (uint8)CPU_BYTE_ORDER);
        }
#else  /*XCP_ACCESS_ALIGN_ENABLE == STD_OFF*/
        u32Src = (uint32*)u4SourceAddr;
        for (i = 0u; i < (u4Length / sizeof(uint32)); i++)
        {
            u32dest[i] = u32Src[i];
        }
#endif /*XCP_ACCESS_ALIGN_ENABLE == STD_ON*/
    }
    /*2.2-byte Data Copy.*/
    else if (
        ((u4Length % sizeof(uint16)) == (uint32)0uL)
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
        /*Address alignment(2 byte)*/
        && ((u4DestAddr % sizeof(uint16)) == (uint32)0uL)
#endif /*XCP_ACCESS_ALIGN_ENABLE == STD_ON*/
    )
    {
        /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
        uint16* u16dest = (uint16*)u4DestAddr;
        /* PRQA S 0306,0309 -- */
        const uint16* u16Src;
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
        if ((u4SourceAddr % sizeof(uint16)) == (uint32)0uL)
        {
            /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
            u16Src = (uint16*)u4SourceAddr;
            /* PRQA S 0306,0309 -- */
            for (i = 0u; i < (u4Length / sizeof(uint16)); i++)
            {
                u16dest[i] = u16Src[i];
            }
        }
        else
        {
            Xcp_CopyU1BufferToU2(u8Src, u16dest, (uint8)CPU_BYTE_ORDER);
        }
#else  /*XCP_ACCESS_ALIGN_ENABLE == STD_OFF*/
        u16Src = (uint16*)u4SourceAddr;
        for (i = 0u; i < (u4Length / sizeof(uint16)); i++)
        {
            u16dest[i] = u16Src[i];
        }
#endif /*XCP_ACCESS_ALIGN_ENABLE == STD_ON*/
    }
    else
#endif /*XCP_COPY_SPEED_UP == STD_ON*/
    {
        /*3.1-byte Data Copy.*/
        /*copy SourceAddr to DsetAddr*/
        for (i = 0u; i < u4Length; i++)
        {
            *(uint8*)(u4DestAddr + i) = *(uint8*)(u4SourceAddr + i); /* PRQA S 0306 */ /* VL_Xcp_0306 */
        }
    }
}

/**
 * Sets an error code in the XCP response buffer. This function updates the response
 * buffer with the error PID and the specified error code, setting the response length
 * to indicate the presence of an error response.
 */
void Xcp_SetErrorCode(uint8 u1ErrorCode)
{
    /*set the Errorcode in the CRM buffer*/
    Xcp_RespBuffer[XCP_PID_OFFSET]        = XCP_RESP_ERR_PID;
    Xcp_RespBuffer[XCP_RESP_ERROR_OFFSET] = u1ErrorCode;
    Xcp_ChannelCommonData.RespLength      = XCP_RESP_ERR_LEN;
    return;
}
/**
 * Sends a response over the XCP channel. This function sets the send status to
 * indicate a response request and checks if the response is pending. It supports
 * both standard and block transfer modes. In block transfer mode, it copies data
 * from the memory address pointed to by MTA into the response buffer, updates the
 * buffer position, and sends the response. In standard mode, it directly sends the
 * prepared response. The function also updates the command and send statuses after
 * the response is sent.
 */
void Xcp_SendResp(void)
{
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint8             pos;
    uint8             len;
#endif
    Std_ReturnType ret;

    Xcp_ChannelCommonData.SendStatus |= XCP_RESP_REQUEST;

    if (XCP_RESP_PENDING != (XCP_RESP_PENDING & Xcp_ChannelCommonData.SendStatus))
    {
/* Block transfer mode */
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        if ((XCP_CMD_BLOCK_TRANSF == Xcp_ChannelCommonData.CommandStatus)
            && (Xcp_ChannelCommonData.BlockBufPos != Xcp_ChannelCommonData.BlockBuflen))
        {
            /* Set pointer */
            /* PRQA S 0306 ++ */ /* VL_Xcp_0306 */
            sourPtr = (Xcp_AGType*)Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
            /* PRQA S 0306 -- */
            destPtr = (Xcp_AGType*)(&Xcp_RespBuffer[XCP_UPLOAD_DATA_OFFSET]);
            /* Get data length */
            len = (XCP_MAX_CTO / XCP_AG) - 1U;
            if (len > (Xcp_ChannelCommonData.BlockBuflen - Xcp_ChannelCommonData.BlockBufPos))
            {
                len = (uint8)(Xcp_ChannelCommonData.BlockBuflen - Xcp_ChannelCommonData.BlockBufPos);
            }
            /* copy data into frame buffer */
            for (pos = 0u; pos < len; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
            /* Set frame length */
            /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
            Xcp_ChannelCommonData.RespLength = (len * XCP_AG) + XCP_UPLOAD_DATA_OFFSET;
            /* PRQA S 2985 -- */

            if ((Std_ReturnType)E_OK
                == Xcp_Transmit(
                    (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                    (PduLengthType)Xcp_ChannelCommonData.RespLength,
                    &Xcp_RespBuffer[0U]))
            {
                /* updata buffer status */
                Xcp_ChannelCommonData.SendStatus |= XCP_RESP_PENDING;
                Xcp_ChannelCommonData.BlockBufPos += len;
                /* PRQA S 2985,1252 ++ */ /* VL_Xcp_2985 */
                Xcp_UpdateMTA(((uint32)len * XCP_AG));
                /* PRQA S 2985,1252 -- */
            }

            /* block upload command has been sent */
            if (Xcp_ChannelCommonData.BlockBufPos == Xcp_ChannelCommonData.BlockBuflen)
            {
                Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_RESP_REQUEST);
                Xcp_ChannelCommonData.CommandStatus = XCP_CMD_TRANSF;
            }
        }
        else
#endif /* XCP_SLAVE_BLOCK_MODE == STD_ON */
        {
            /* Standard transfer mode */
            Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_RESP_REQUEST);
            Xcp_ChannelCommonData.SendStatus |= XCP_RESP_PENDING;
            ret = Xcp_Transmit(
                (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                (PduLengthType)Xcp_ChannelCommonData.RespLength,
                &Xcp_RespBuffer[0U]);
            if ((Std_ReturnType)E_OK == ret)
            {
                Xcp_ChannelCommonData.CommandStatus = XCP_CMD_TRANSF;
            }
            else
            {
                /* In the event of transmission failure, continue the transmission */
                Xcp_ChannelCommonData.SendStatus |= XCP_RESP_REQUEST;
                Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_RESP_PENDING);
            }
        }
    }
    return;
}
/**
 * Sends an event over the XCP channel. This function sets the send status to
 * indicate an event request, prepares the event buffer with the event PID, and
 * checks if the event is pending. If not, it transmits the event buffer and
 * updates the send status to reflect the completion of the event transmission.
 */
void Xcp_SendEv(void)
{
    Xcp_ChannelCommonData.SendStatus |= XCP_EV_REQUEST;
    Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
    if (XCP_EV_PENDING != (XCP_EV_PENDING & Xcp_ChannelCommonData.SendStatus))
    {
        if ((Std_ReturnType)E_OK
            == Xcp_Transmit(
                (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                (PduLengthType)Xcp_ChannelCommonData.EvLength,
                &Xcp_EvBuffer[0]))
        {
            Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_EV_REQUEST);
            Xcp_ChannelCommonData.SendStatus |= XCP_EV_PENDING;
        }
    }

    return;
}

/**
 * Sends a service message over the XCP channel. This function sets the send status
 * to indicate a service request, checks if the service is pending, and if not, it
 * transmits the service buffer. After successful transmission, it updates the send
 * status to reflect that the service message has been sent.
 */
XCP_LOCAL void Xcp_SendServ(void)
{
    Xcp_ChannelCommonData.SendStatus |= XCP_SERV_REQUEST;
    if (XCP_SERV_PENDING != (XCP_SERV_PENDING & Xcp_ChannelCommonData.SendStatus))
    {
        if ((Std_ReturnType)E_OK
            == Xcp_Transmit(
                (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                (PduLengthType)Xcp_ChannelCommonData.ServLength,
                &Xcp_ServBuffer[0]))
        {
            Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_SERV_REQUEST);
            Xcp_ChannelCommonData.SendStatus |= XCP_SERV_PENDING;
        }
    }
    return;
}
/**
 *  This function processes the received data based on the current connection status
 *  and the received PID. It checks if the received command is a connect command or
 *  if the connection is already established. It also checks the command length.
 *  If the conditions are met, it processes the command or STIM frame. If the
 *  command is in an idle state or a synch command, it calls the command handler.
 *  If in block receive mode, it calls the block receive handler. Otherwise, it
 *  sets a command busy error code and sends a response. If a STIM is received
 *  and the resource is enabled, it caches the STIM into the buffer.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_RxIndication(const PduLengthType len, const uint8* dataPtr)
/* PRQA S 1532 -- */
{
    uint8 pid = dataPtr[XCP_PID_OFFSET];

    /*Receiving command processing conditions:
     * 1.It's a connect command;
     * 2.It's a other command but now already in a connected state;
     * 3.Command length check.
     * */
    if (((Xcp_ChannelCommonData.ConnectionStatus == XCP_CONNECT) || (pid == XCP_CMD_CONNECT)) && (len <= XCP_MAX_CTO))

    {
        /*CMD or a STIM frame*/
        if (pid >= XCP_CMD_CODE_MIN)
        {
            /* idle or synch command */
            if (((Xcp_CommandStatusType)XCP_CMD_IDLE == Xcp_ChannelCommonData.CommandStatus) || (XCP_CMD_SYNCH == pid))
            {
                Xcp_RxCommandHal(len, dataPtr);
            }
            else
            {
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
                /* Block Recv */
                if (XCP_CMD_BLOCK_RECV == Xcp_ChannelCommonData.CommandStatus)
                {
                    Xcp_RxBlockHal(pid, len, dataPtr);
                }
                else
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/
                {
                    Xcp_SetErrorCode(XCP_ERR_CMD_BUSY);
                    Xcp_SendResp();
                }
            }
        }
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
        else
        {
            /* Receive STIM,Cache into buffer */
            Xcp_RxStimHal(dataPtr);
        }
#endif
    }
    else
    {
        /* Do Nothing */
    }
    return;
}

#if (XCP_DEV_ERROR_DETECT == STD_ON)
/**
 * Checks for errors in the XCP receive indication. This function verifies that the
 * module is initialized, the PDU pointer is valid, and the PDU ID is within the
 * expected range. If any of these checks fail, it reports the error using the
 * Detection and Reporting mechanism and sets an error flag.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
boolean Xcp_RxIndCheckDetErr(PduIdType XcpRxPduId, const PduInfoType* XcpRxPduPtr)
/* PRQA S 1532 -- */
{
    boolean errorFlag  = FALSE;
    boolean pduIdError = FALSE;

    if (!Xcp_ModuleInit)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_RXINDICATION, XCP_E_UNINIT);
        errorFlag = TRUE;
    }
    else if (NULL_PTR == XcpRxPduPtr)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_RXINDICATION, XCP_E_PARAM_POINTER);
        errorFlag = TRUE;
    }
    else
    {
        if (Xcp_PbCfgPtr->numOfRxPdu <= XcpRxPduId)
        {
            pduIdError = TRUE;
        }
        if (pduIdError)
        {
            /* PduId check failed */
            (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_RXINDICATION, XCP_E_INVALID_PDUID);
            errorFlag = TRUE;
        }
    }
    return errorFlag;
}
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/

/**
 * Processes received XCP commands. This function handles the reception of commands
 * in both interleaved and non-interleaved modes. It checks the command PID and
 * processes block commands like `DOWNLOAD` and `PROGRAM`, ensuring the data length
 * is within the allowed range. For valid commands, it updates the command status
 * and buffers the data. If an error occurs, it sets an error code and sends a
 * response. The function also manages the synchronization of critical sections.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
XCP_LOCAL void Xcp_RxCommandHal(const PduLengthType len, const uint8* dataPtr)
/* PRQA S 6030 -- */
{
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    uint8 pid = dataPtr[XCP_PID_OFFSET];
#endif
#if ((0u != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE)) && (XCP_MASTER_BLOCK_MODE == STD_ON))
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint16            pos;
#endif
    uint16 lenCounter;
    SchM_Enter_Xcp_Exclusive_Common();
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
#if (XCP_GET_DAQ_CLOCK == STD_ON)
    /* Get Recieve Time */
    Xcp_ChannelDaqData.RxTimeStamp = Xcp_ReadOutMS();
#endif
#endif

#if (XCP_INTERLEAVED_MODE == STD_ON)
    if ((boolean)FALSE == Xcp_IsInterleavedBufFull())
    {
        for (lenCounter = 0u; lenCounter < len; lenCounter++)
        {
            (Xcp_ChannelCommonData.InterLevCmdBuffer)[Xcp_ChannelCommonData.InterLevPosRear][lenCounter] =
                dataPtr[lenCounter];
        }
        (Xcp_ChannelCommonData.InterLevCmdLen)[Xcp_ChannelCommonData.InterLevPosRear] = (uint8)len;
        Xcp_ChannelCommonData.InterLevPosRear++;
        Xcp_ChannelCommonData.InterLevPosRear %= XCP_QUEUE_SIZE;
        Xcp_ChannelCommonData.InterLevRecv = TRUE;
    }
#else
    /* copy data to the local buffer */
    for (lenCounter = 0u; lenCounter < len; lenCounter++)
    {
        Xcp_CmdBuffer[lenCounter] = dataPtr[lenCounter];
    }
    Xcp_ChannelCommonData.CmdLength = (uint8)len;
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
/*
 * Change Command Status
 */
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    /*
     * Block Commands
     */
    if ((XCP_CMD_DOWNLOAD_NEXT == pid) || (XCP_CMD_PROGRAM_NEXT == pid))
    {
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_EXECUTE;
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_SendResp();
    }
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
    else if ((XCP_CMD_DOWNLOAD == pid) && (dataPtr[XCP_DOWNLOAD_LEN_OFFSET] > XCP_DOWNLOAD_SIZE))
    {
        /* the length of the *_next shall be check here to prevent the overload
         * of the BlockBuffer
         */
        if (dataPtr[XCP_DOWNLOAD_LEN_OFFSET] <= XCP_DOWNLOAD_NEXT_SIZE)
        {
            /* get total length */
            Xcp_ChannelCommonData.BlockBuflen = dataPtr[XCP_DOWNLOAD_LEN_OFFSET];
            /* set pointer to the sour and dest [AG] */
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_DOWNLOAD_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[0];
            /* copy data until the end of the frame */
            lenCounter = (XCP_DOWNLOAD_DATA_OFFSET + XCP_AG);
            pos        = 0u;
            while (lenCounter <= len)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            /* updata some state and set the pid to download_next */
            Xcp_ChannelCommonData.BlockBufPos   = pos;
            Xcp_CmdBuffer[XCP_PID_OFFSET]       = XCP_CMD_DOWNLOAD_NEXT;
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_BLOCK_RECV;
        }
        /* block length out of range */
        else
        {
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_EXECUTE;
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_SendResp();
        }
    }
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    else if (
        (XCP_CMD_PROGRAM == pid) && (dataPtr[XCP_PROGRAM_LEN_OFFSET] > XCP_PROGRAM_SIZE)
        && (XCP_PGM_IDLE != Xcp_ChannelPtgmData.PgmStauts) && (XCP_PGM_START != Xcp_ChannelPtgmData.PgmStauts))
    {
        /* the length of the *_next shall be check here to prevent the overload
         * of the BlockBuffer
         */
        if (dataPtr[XCP_PROGRAM_LEN_OFFSET] <= XCP_PROGRAM_NEXT_SIZE)
        {
            /* get total length */
            Xcp_ChannelCommonData.BlockBuflen = dataPtr[XCP_PROGRAM_LEN_OFFSET];
            /* set pointer to the sour and dest [AG] */
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_PROGRAM_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[0];
            /* copy data until the end of the frame */
            lenCounter = (XCP_PROGRAM_DATA_OFFSET + XCP_AG);
            pos        = 0u;
            while (lenCounter <= len)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            /* updata some state and set the pid to download_next */
            Xcp_ChannelCommonData.BlockBufPos   = pos;
            Xcp_CmdBuffer[XCP_PID_OFFSET]       = XCP_CMD_PROGRAM_NEXT;
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_BLOCK_RECV;
        }
        /* block length out of range */
        else
        {
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_EXECUTE;
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_SendResp();
        }
    }
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/

    else
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/
    /*
     * Non-Block Commands
     */
    {
#if (XCP_INTERLEAVED_MODE != STD_ON)
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_RECV;
#endif /*XCP_INTERLEAVED_MODE != STD_ON*/
    }
    SchM_Exit_Xcp_Exclusive_Common();
    return;
}

/**
 * Processes received block commands in XCP. This function handles `DOWNLOAD_NEXT`
 * and `PROGRAM_NEXT` commands, verifying the command PID and the data length. It
 * copies the data into the block buffer and updates the buffer position. If the
 * buffer is full, it updates the command status. If the PID or data length is
 * incorrect, it sets an error code and sends a response indicating the error.
 */
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
/* PRQA S 5017, 6010, 6080 ++ */ /* VL_Xcp_5017, VL_MTR_Xcp_STCYC, VL_MTR_Xcp_STPTH */
XCP_LOCAL void Xcp_RxBlockHal(const uint8 pid, const PduLengthType len, const uint8* dataPtr)
/* PRQA S 5017, 6010, 6080 -- */
{
#if (0u != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint16            lenCounter;
    PduLengthType     lenMax = len;
#endif
    uint16 pos = 0u;

    /* pid shall be the associated *_NEXT */
    if (pid == Xcp_CmdBuffer[XCP_PID_OFFSET])
    {
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
        if ((XCP_CMD_DOWNLOAD_NEXT == pid)
            && (dataPtr[XCP_DOWNLOAD_LEN_OFFSET]
                == (Xcp_ChannelCommonData.BlockBuflen
                    - Xcp_ChannelCommonData.BlockBufPos))) /* Determine whether the length is consistent */
        {
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_DOWNLOAD_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[Xcp_ChannelCommonData.BlockBufPos];
            /* PRQA S 2986 ++ */ /* VL_Xcp_2986 */
            lenCounter = XCP_DOWNLOAD_DATA_OFFSET
                         + (uint16)(XCP_AG * (Xcp_ChannelCommonData.BlockBuflen - Xcp_ChannelCommonData.BlockBufPos));
            /* PRQA S 2986 -- */
            if (lenMax > lenCounter)
            {
                lenMax = lenCounter;
            }
            lenCounter = XCP_DOWNLOAD_DATA_OFFSET + XCP_AG;
            pos        = 0u;
            while (lenCounter <= lenMax)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            Xcp_ChannelCommonData.BlockBufPos += pos;
            if (Xcp_ChannelCommonData.BlockBufPos == Xcp_ChannelCommonData.BlockBuflen)
            {
                Xcp_ChannelCommonData.CommandStatus = XCP_CMD_RECV;
            }
        }
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if ((XCP_CMD_PROGRAM_NEXT == pid)
            && (dataPtr[XCP_PROGRAM_LEN_OFFSET]
                == (Xcp_ChannelCommonData.BlockBuflen - Xcp_ChannelCommonData.BlockBufPos)))
        {
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_PROGRAM_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[Xcp_ChannelCommonData.BlockBufPos];
            /* PRQA S 2986 ++ */ /* VL_Xcp_2986 */
            lenCounter = XCP_PROGRAM_DATA_OFFSET
                         + (uint16)(XCP_AG * (Xcp_ChannelCommonData.BlockBuflen - Xcp_ChannelCommonData.BlockBufPos));
            /* PRQA S 2986 -- */
            if (lenMax > lenCounter)
            {
                lenMax = (uint16)lenCounter;
            }
            lenCounter = XCP_PROGRAM_DATA_OFFSET + XCP_AG;
            pos        = 0u;
            while (lenCounter <= lenMax)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            Xcp_ChannelCommonData.BlockBufPos += pos;
            if (Xcp_ChannelCommonData.BlockBufPos == Xcp_ChannelCommonData.BlockBuflen)
            {
                Xcp_ChannelCommonData.CommandStatus = XCP_CMD_RECV;
            }
        }
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/

        if (pos == 0u)
        {
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_EXECUTE;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespBuffer[XCP_RESP_ERROR_OFFSET + XCP_CONST_1] =
                (uint8)(Xcp_ChannelCommonData.BlockBuflen - Xcp_ChannelCommonData.BlockBufPos);
            Xcp_ChannelCommonData.RespLength = XCP_CONST_3;
            Xcp_SendResp();
        }
    }
    /* with wrong pid in the sequence an error shall be reported */
    else
    {
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_EXECUTE;
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);

        Xcp_SendResp();
    }
    return;
}
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

/**
 * Handles the transmission confirmation in XCP. This function clears the send pending
 * flag and resets the transmit timeout counter. It updates the command status based
 * on the current state and the transmission result. If the command is complete, it
 * resets the command status to idle and updates the connection status if necessary.
 * It also calls the transmit queue handler to send the next frame if needed.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_TxConfirmation(const PduIdType pduId)
/* PRQA S 1532 -- */
{
    /* Since last frame has been sent, clear the pending flag */
    Xcp_ChannelCommonData.SendStatus &= (uint8)(~XCP_SEND_PENDING);
    Xcp_ChannelCommonData.TxTimeOutCnt = 0x00u;

    /* In case the bus driver is working in polling mode the commandstatus
     * will be set to Idle immediately when the <lower_layer>_Transmit is return E_OK
     */
    /* in case interrupt mode is used, we change status here, otherwise
     * this shall be done in Xcp_SendResp()
     */
    /* After the CRM sent, reset the command status */
    if ((XCP_CMD_TRANSF == Xcp_ChannelCommonData.CommandStatus)
        && (pduId == Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->XcpLocalTxPduId))
    {
        Xcp_ChannelCommonData.CommandStatus = XCP_CMD_IDLE;
#if (XCP_INTERLEAVED_MODE == STD_ON)
        if (Xcp_ChannelCommonData.InterLevPosRear != Xcp_ChannelCommonData.InterLevPosHead)
        {
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_RECV;
        }
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
        /* stack will go into the connected status when the positive ack sent successfully */
        if (XCP_PRE_CONNECT == Xcp_ChannelCommonData.ConnectionStatus)
        {
            Xcp_ChannelCommonData.ConnectionStatus = XCP_CONNECT;
        }
        else if (XCP_PRE_USERDEFINE == Xcp_ChannelCommonData.ConnectionStatus)
        {
            Xcp_ChannelCommonData.ConnectionStatus = XCP_USERDEFINE;
        }
        else
        {
            /* do nothing */
        }
    }
    /* Send next frame if necessary */
    Xcp_TxQueueHandler();
    return;
}

#if (XCP_DEV_ERROR_DETECT == STD_ON)
/**
 * Checks for errors in the XCP transmit indication. This function verifies that
 * the module is initialized. If the module is not initialized, it reports an error
 * using the Detection and Reporting mechanism and sets an error flag.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
boolean Xcp_TxIndCheckDetErr(void)
/* PRQA S 1532 -- */
{
    boolean errFlag = FALSE;

    if (!Xcp_ModuleInit)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_TXCONFIRMATION, XCP_E_UNINIT);
        errFlag = TRUE;
    }
    return errFlag;
}
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/

/**
 * Manages the transmit queue for XCP. This function checks the send status for
 * response, service, and event requests and calls the respective send functions
 * to handle them. It also processes active event channels and sends DAQ data if
 * applicable, prioritizing events over DAQ data.
 */
XCP_LOCAL void Xcp_TxQueueHandler(void)
{
    if (XCP_RESP_REQUEST == (XCP_RESP_REQUEST & Xcp_ChannelCommonData.SendStatus))
    {
        Xcp_SendResp();
    }
    if (XCP_SERV_REQUEST == (XCP_SERV_REQUEST & Xcp_ChannelCommonData.SendStatus))
    {
        Xcp_SendServ();
    }
    if (XCP_EV_REQUEST == (XCP_EV_REQUEST & Xcp_ChannelCommonData.SendStatus))
    {
        Xcp_SendEv();
    }
/* here we put Daq below the Ev cause there is some Ev has high priority than DAQ
 * for example Ev_Sleep
 */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
    for (uint8 evCh = 0u; evCh < XCP_MAX_EVENT_CHANNEL; evCh++)
    {
        if (Xcp_ChannelEvChData.EvChActiveFlag[evCh])
        {
            Xcp_SendDaq(evCh, FALSE);
            break;
        }
    }
#endif
    return;
}

/**
 * Handles reserved or unknown XCP commands. This function sets an error code
 * indicating an unknown command and sends a response to notify the sender of
 * the error.
 */
XCP_LOCAL void Xcp_Cmd_ReservedCmd(void)
{
    Xcp_SetErrorCode(XCP_ERR_CMD_UNKNOWN);
    Xcp_SendResp();
}
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/* ========================================== internal function definitions ========================================= */
