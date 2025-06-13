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
**  FILENAME    : Xcp.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for XCP                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-01-18  qinchun.yang    Modify DAQ consistency bug.
 *  V1.0.1       2020-01-19  qinchun.yang    Increase STIM function.
 *  V1.0.2       2020-01-20  qinchun.yang    Modify DAQ resume function.Modify SET_REQUEST command.
 *               2020-02-12  qinchun.yang
 *   1> Move the Xcp_CanIfRxIndication and Xcp_CanIfTxConfirmation to XcpOnCan.c
 *   2> Modify Xcp_Transmit API (Add Ethernet sending function).
 *   3> Initial version.Distinguish between different transport layers
 *  V1.0.3       2020-05-06  qinchun.yang    Modify function Xcp_EventIndication().
 *  V1.0.4       2020-05-08  qinchun.yang    Solve byte alignment bug(Dynamic allocation daq).
 *  V1.0.5       2021-02-05  qinchun.yang    Add interleaved communication mode.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 *  V2.0.1       2023-02-20  qinchun.yang    Added modification to DAQ measurement length.
 *  V2.0.1       2023-04-12  qinchun.yang    PRD0042022-659 & PRD0042022-782.
 *  V2.0.2       2023-05-09  tong.zhao
 *    1> Delete invalid code in Xcp_RxIndication() when PID OFF is ON (JIRA:CPT-279)
 *  V2.0.3       2023-05-15  qinchun.yang
 *    1> Modify UPLOAD/SHORT_UPLOAD/BULID_CHECKSUM ERR_ACCESS_LOCKED error check.
 *               2023-05-26  tong.zhao       fix Bug CPT-90
 *    2> Modify the bug of API Xcp_GetSeedHal when GET_SEED command to request
 *    the remaining part of the seed, don't care resource.
 *  V2.0.4       2023-08-15  tong.zhao       fix bug CPT-61
 *    1> Modify the bug of API Xcp_CheckAddress ,error generated when addr overflows after calculation.
 *  V2.0.5       2023-11-07  tong.zhao       fix bug CPT-6464
 *    1> add through short_upload command to get measurement data in CANape's polling mode.
 *  V2.0.6       2023-11-21  qinchun.yang       CPD-33590
 *    1> Code execution efficiency optimization.
 *  V2.0.7       2023-12-07  qinchun.yang    fix bug CPT-7804 & CPT-7805
 *    1> CONNECT & DISCONNECT command.
 *  V2.0.8       2024-04-09  qinchun.yang    Remove daq list priority process logic;
 *  V2.0.9       2024-08-02  shaoqiang.Liang    Replace the Frt interface with the internal XCP ReadOutMS interface;
 *  V2.0.10      2024-08-13  shaoqiang.Liang    Modify DAQ consistency bug.
 */
/*============================================================================*/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:XCP<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 1531 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 0306 MISRA Rule 11.4.<br>
    Reason:Necessary type conversions.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:design needs, namelength set to 63 in C99.

    \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:Data information that needs to be stored is extracted in order to facilitate storage mapping.

    \li PRQA S 4558 MISRA Rule 10.1.<br>
    Reason:Necessary type conversions.
 */

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define XCP_C_AR_MAJOR_VERSION 4u
#define XCP_C_AR_MINOR_VERSION 5u
#define XCP_C_AR_PATCH_VERSION 0u
#define XCP_C_SW_MAJOR_VERSION 2u
#define XCP_C_SW_MINOR_VERSION 0u
#define XCP_C_SW_PATCH_VERSION 10u

#include "Xcp_Internal.h"
#include "SchM_Xcp.h"
#include "Xcp_Interface.h"

#if (XCP_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*=======[V E R S I O N  C H E C K]===========================================*/
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

#if (XCP_DEV_ERROR_DETECT == STD_ON)
#define XCP_VENDOR_ID (uint16)62
#define XCP_MODULE_ID (uint16)212
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/
#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(Xcp_StatusType, XCP_VAR_INIT_UNSPECIFIED) Xcp_Status = XCP_UINIT;
VAR(Xcp_CommandStatusType, XCP_VAR_INIT_UNSPECIFIED) Xcp_CommandStatus = XCP_CMD_IDLE;
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
#if ((XCP_MASTER_BLOCK_MODE == STD_ON) || (XCP_SLAVE_BLOCK_MODE == STD_ON))
VAR(Xcp_BlockBufIndxType, XCP_VAR) Xcp_BlockBufferPos;
VAR(Xcp_BlockBufIndxType, XCP_VAR) Xcp_BlockBufferLen;
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON)||(XCP_SLAVE_BLOCK_MODE == STD_ON*/
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR_CLEARED_UNSPECIFIED) Xcp_CmdLength;
VAR(uint8, XCP_VAR_CLEARED_UNSPECIFIED) Xcp_RespLength;
VAR(uint8, XCP_VAR_CLEARED_UNSPECIFIED) Xcp_EvLength;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR_INIT_8) Xcp_ProtectionStatus = (uint8)(XCP_PROTECTION);
#define XCP_STOP_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR) Xcp_TransferTimeOutCnt;
VAR(uint8, XCP_VAR) Xcp_SessionStatus;
VAR(uint8, XCP_VAR) Xcp_SendStatus;
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
VAR(uint32, XCP_VAR) Xcp_UploadInfoLen;
#endif
#define XCP_STOP_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
P2CONST(Xcp_PCConfigType, XCP_VAR_CLEARED_32, XCP_APPL_DATA) Xcp_GlobalCfgPtr;

P2CONST(Xcp_ConfigType, XCP_VAR_CLEARED_32, XCP_APPL_DATA) Xcp_PbCfgPtr;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#if (XCP_INTERLEAVED_MODE == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
static VAR(Xcp_InterlevModeCtrlType, XCP_VAR) Xcp_InterLevModeCtrl;

static VAR(uint8, XCP_VAR) Xcp_InterLevCmdBuffer[XCP_QUEUE_SIZE][XCP_MAX_CTO];
static VAR(uint8, XCP_VAR) Xcp_InterLevCmdLen[XCP_QUEUE_SIZE];

static VAR(boolean, XCP_VAR) Xcp_InterLevRecv;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
static VAR(PduLengthType, XCP_VAR) Xcp_ServLength;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static FUNC(void, XCP_CODE) Xcp_PendingTaskHandle(void);
static FUNC(void, XCP_CODE) Xcp_GenericCommandHandle(void);
static FUNC(void, XCP_CODE) Xcp_RxCommandHal
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    (const uint8 pid, const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#else
    (const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#endif
static FUNC(void, XCP_CODE) Xcp_TxQueueHandler(void);

#if (XCP_MASTER_BLOCK_MODE == STD_ON)
static FUNC(void, XCP_CODE)
    Xcp_RxBlockHal(const uint8 pid, const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#endif

static FUNC(void, XCP_CODE) Xcp_RxCommandHal
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    (const uint8 pid, const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#else
    (const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr);
#endif

#if (XCP_INTERLEAVED_MODE == STD_ON)
static FUNC(void, XCP_CODE) Xcp_InterLeavedHandler(void);
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/

static FUNC(void, XCP_CODE) Xcp_Cmd_ReservedCmd(void);

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#define XCP_START_SEC_CONST_PTR
#include "XCP_MemMap.h"
/* PRQA S 3218 ++*/ /* MISRA Rule 11.4 */ /*Xcp_TYPE_CAST_001*/
static CONSTP2FUNC(void, XCP_CODE, Xcp_CmdFct[])(void) = {
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
    Xcp_SetDaqPtr,        /* 0xE2 = SET_DAQ_PTR */
    Xcp_WriteDaq,         /* 0xE1 = WRITE_DAQ */
    Xcp_SetDaqListMode,   /* 0xE0 = SET_DAQ_LIST_MODE */
    Xcp_GetDaqListMode,   /* 0xDF = GET_DAQ_LIST_MODE */
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
/* PRQA S 3218 --*/ /* MISRA Rule 11.4 */ /*Xcp_TYPE_CAST_001*/
#define XCP_STOP_SEC_CONST_PTR
#include "XCP_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
/******************************************************************************/
/*
 * @brief               <Xcp Module Initialization Function>
 *
 * <This service initializes interfaces and variables of the AUTOSAR XCP layer> .
 * Service ID   :       <XCP_INIT_ID (0x00)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <Xcp_ConfigPtr (IN)>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_Init(CONSTP2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Xcp_Status = XCP_UINIT;
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Xcp_ConfigPtr)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_INIT, XCP_E_INV_POINTER);
    }
    else
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/
    {
        Xcp_GlobalCfgPtr = &Xcp_PConfig;
        Xcp_PbCfgPtr = Xcp_ConfigPtr;
        /* clear the stack status */
        Xcp_ClearStatus();
        /* Clear Command Processor Common Status */
        Xcp_CommandInit();
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        Xcp_ProgramInit();
#endif /*XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE*/
#if (XCP_BUILD_CHECKSUM == STD_ON)
        Xcp_InitCrc16CcittTable();
#endif
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
        Xcp_CALInit();
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
        Xcp_DAQInit();
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/
        Xcp_Status = XCP_DISCONNECT;
    }
    return;
}

#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
uint32 Xcp_ReadOutMS(void)
{
    uint32 OSCurMs;

    (void)GetCounterValue(XCP_SYSTEMTIMER, &OSCurMs);
    /*tick to ms*/
    OSCurMs = XCP_OS_TICKS2MS_SystemTimer(OSCurMs);

    return (OSCurMs);
}
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/

/******************************************************************************/
/*
 * @brief               <Xcp Module scheduled Function>
 *
 * <Scheduled function of the XCP module> .
 * Service ID   :       <(0x04)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <Xcp_ConfigPtr (IN)>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* In case something is in transmission */
    if (0x00U != (Xcp_SendStatus & XCP_SEND_PENDING))
    {
        Xcp_TransferTimeOutCnt++;
        if (Xcp_TransferTimeOutCnt >= XCP_TANSFER_TIME_OUT)
        {
            Xcp_SendStatus &= (uint8)(~XCP_SEND_PENDING);
            if (0x00U != ((XCP_CMD_TRANSF | XCP_CMD_BLOCK_TRANSF) & Xcp_CommandStatus))
            {
                Xcp_CommandInit();
            }
            Xcp_TransferTimeOutCnt = 0x00u;
        }
    }
    /* In case last send request is denied. Restart the tx queue in the main function */
    else if (0x00U != (Xcp_SendStatus & XCP_SEND_REQUEST))
    {
        Xcp_TxQueueHandler();
    }
    else
    {
        /* Do Nothing */
    }
    /* Handle the pending task. */
    Xcp_PendingTaskHandle();
#if (XCP_SET_REQUEST == STD_ON)
    if (0u != Xcp_SessionStatus)
    {
        Xcp_SetRequestHandler();
    }
#endif /*XCP_SET_REQUEST == STD_ON*/
#if (XCP_INTERLEAVED_MODE == STD_ON)
    Xcp_InterLeavedHandler();
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
    /* One Command has been recieved successfully */
    if (XCP_CMD_RECV == Xcp_CommandStatus)
    {
        Xcp_CommandStatus = XCP_CMD_EXCUTE;
        Xcp_GenericCommandHandle();
    }
    return;
}

#if (XCP_INTERLEAVED_MODE == STD_ON)
static FUNC(void, XCP_CODE) Xcp_InterLeavedHandler(void)
{
    uint8 bufIdx;

    if (Xcp_InterLevRecv == TRUE)
    {
        Xcp_CommandStatus = XCP_CMD_RECV;
        Xcp_CmdLength = Xcp_InterLevCmdLen[Xcp_InterLevModeCtrl.posHead];
        for (bufIdx = 0; bufIdx < Xcp_CmdLength; bufIdx++)
        {
            Xcp_CmdBuffer[bufIdx] = Xcp_InterLevCmdBuffer[Xcp_InterLevModeCtrl.posHead][bufIdx];
        }
        Xcp_InterLevModeCtrl.posHead++;
        if (Xcp_InterLevModeCtrl.posHead == XCP_QUEUE_SIZE)
        {
            Xcp_InterLevModeCtrl.posHead = 0;
        }
        if (Xcp_InterLevModeCtrl.posHead == Xcp_InterLevModeCtrl.posRear)
        {
            Xcp_InterLevRecv = FALSE;
        }
    }
}

static FUNC(boolean, XCP_CODE) Xcp_IsInterleavedBufFull(void)
{
    boolean isFull = FALSE;
    uint8 idleBufLen;

    if ((Xcp_InterLevModeCtrl.posRear == Xcp_InterLevModeCtrl.posHead) && (Xcp_InterLevRecv == TRUE))
    {
        isFull = TRUE;
    }
    else
    {
        (Xcp_InterLevModeCtrl.posRear >= Xcp_InterLevModeCtrl.posHead)
            ? (idleBufLen = Xcp_InterLevModeCtrl.posRear - Xcp_InterLevModeCtrl.posHead)
            : (idleBufLen = Xcp_InterLevModeCtrl.posRear + XCP_QUEUE_SIZE - Xcp_InterLevModeCtrl.posHead);
        if (idleBufLen == XCP_QUEUE_SIZE)
        {
            isFull = TRUE;
        }
    }
    return isFull;
}
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/

#if (XCP_VERSION_INFO_API == STD_ON)
/******************************************************************************/
/*
 * @brief               <Get version information.>
 *
 * <Returns the version information of this module.> .
 * Service ID   :       <(0x01)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <None>
 * @param[out]          <versioninfo>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) versioninfo)
{
    Std_VersionInfoType Xcp_VersionInfo = {
        XCP_VENDOR_ID,
        XCP_MODULE_ID,
        XCP_INSTANCE_ID,
        XCP_C_SW_MAJOR_VERSION,
        XCP_C_SW_MINOR_VERSION,
        XCP_C_SW_PATCH_VERSION};
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == versioninfo)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_GET_VERSION_INFO, XCP_E_INV_POINTER);
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
/******************************************************************************/
/*
 * @brief               <Control TX capabilities.>
 *
 * <This API is used to turn on and off of the TX capabilities of used communication
 * bus channel in XCP module..> .
 * Service ID   :       <(0x05)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <Channel>/<Mode>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
FUNC(void, XCP_CODE)
Xcp_SetTransmissionMode(NetworkHandleType Channel, Xcp_TransmissionModeType Mode)
{
}
#endif

FUNC(void, XCP_CODE) Xcp_ClearStatus(void)
{
    Xcp_ProtectionStatus = (uint8)(XCP_PROTECTION);
    Xcp_SessionStatus = 0U;
    Xcp_SendStatus = XCP_SEND_IDLE;
    Xcp_TransferTimeOutCnt = 0U;

    Xcp_CmdLength = 0U;
    Xcp_RespLength = 0U;
    Xcp_EvLength = 0U;
    Xcp_ServLength = 0U;

#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
    Xcp_EthRxCounter = 0u;
    Xcp_EthTxCounter = 0u;
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/

#if (XCP_SEED_AND_UNLOCK == STD_ON)
    Xcp_SeedandKeyStatus = XCP_PRE_SEED;
    Xcp_SeedandKeyRes = 0U;
#endif

#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
    Xcp_UploadInfoLen = 0U;
#endif

    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp Module Command Processor State Machine Init. Function>
 *
 * <Initialize command processor state machine and block buffer,
 * this function used in Xcp_Init,Xcp_Sync and Xcp_MainFunction(in case tx timeout)>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_CommandInit(void)
{
    Xcp_CommandStatus = XCP_CMD_IDLE;
#if ((XCP_MASTER_BLOCK_MODE == STD_ON) || (XCP_SLAVE_BLOCK_MODE == STD_ON))
    Xcp_BlockBufferPos = 0U;
    Xcp_BlockBufferLen = 0U;
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON)||(XCP_SLAVE_BLOCK_MODE == STD_ON*/
#if (XCP_INTERLEAVED_MODE == STD_ON)
    Xcp_InterLevModeCtrl.posRear = 0u;
    Xcp_InterLevModeCtrl.posHead = 0u;
    Xcp_InterLevRecv = FALSE;
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
    if ((Xcp_CmdBuffer[XCP_PID_OFFSET] != XCP_CMD_SYNCH)
        && ((XCP_PRE_CONNECT == Xcp_Status) || (XCP_PRE_USERDEFINE == Xcp_Status)))
    {
        Xcp_Status = XCP_DISCONNECT;
    }
    return;
}

static FUNC(void, XCP_CODE) Xcp_PendingTaskHandle(void)
{
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    Xcp_PgmPendingTask();
#endif
    return;
}

static FUNC(void, XCP_CODE) Xcp_GenericCommandHandle(void)
{
    uint8 u1CommandCode = Xcp_CmdBuffer[XCP_PID_OFFSET];

    /* initialize the PID of RESP frame */
    Xcp_RespBuffer[XCP_PID_OFFSET] = XCP_RESP_POS_PID;
    Xcp_RespLength = 0x01U;

    /* in non-CONNECT status only treat Connect & get_slave_id*/
    if (XCP_CMD_CONNECT == u1CommandCode)
    {
        Xcp_Connect();
    }
    /* other command shall be ignored when status is not connect */
    else if (XCP_CONNECT == Xcp_Status)
    {
        if ((XCP_CMD_CODE_MIN < u1CommandCode) && (XCP_CMD_DISCONNECT >= u1CommandCode))
        {
            Xcp_CmdFct[XCP_CMD_DISCONNECT - u1CommandCode]();
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_CMD_UNKNOWN);
            Xcp_RespLength = 0x02u;
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
    else if (XCP_USERDEFINE == Xcp_Status)
    {
        /* Do nothing Now if needed some upper-level test commands can be
         * located here.
         */
        Xcp_CommandStatus = XCP_CMD_IDLE;
    }
    else
    {
        Xcp_CommandStatus = XCP_CMD_IDLE;
    }
    return;
}

FUNC(Std_ReturnType, XCP_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
Xcp_Transmit(PduIdType pduId, PduLengthType len, P2VAR(uint8, AUTOMATIC, XCP_VAR) dataPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    PduInfoType pduInfo;
    Std_ReturnType ret;
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
    P2CONST(uint8, AUTOMATIC, XCP_VAR_CLEARED_8) srcPtr;
    P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED_8) destPtr;
    uint8 ethFrame[len + 4u];
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
    pduInfo.SduLength = len + 4u;
    pduInfo.SduDataPtr = ethFrame;
    Xcp_CopyU2ToU1Buffer(len, &(ethFrame[0u]), LOW_BYTE_FIRST);
    Xcp_CopyU2ToU1Buffer(Xcp_EthTxCounter, &(ethFrame[2u]), LOW_BYTE_FIRST);
    /*This CTR value is to be increased for each packet regardless of the type
     *  (RES, ERR_EV, SERV, DAQ)*/
    srcPtr = (uint8*)&(dataPtr[0u]);
    destPtr = (uint8*)&(ethFrame[4u]);
    Xcp_BufferCopy((uint32)srcPtr, (uint32)destPtr, len);
    ret = SoAd_IfTransmit(pduId, &pduInfo);
    Xcp_EthTxCounter++;
#else
    /*Don't support.*/
    ret = E_NOT_OK;
#endif
    return ret;
}

/******************************************************************************/
/**
 * @brief               <Put a long into a byte array>
 *
 *      <This Function split the long type data to the byte type array>
 *
 * @param[in]           <u4Data         source long data>
 *                      <byteOrder>
 * @param[out]          <None>
 * @param[in/out]       <pu1BufferPtr   pointer to the byute array>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
Xcp_CopyU4ToU1Buffer(const uint32 u4Data, P2VAR(uint8, AUTOMATIC, XCP_VAR) pu1BufferPtr, uint8 byteOrder)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        pu1BufferPtr[0U] = (uint8)((u4Data) >> 24U);
        pu1BufferPtr[1U] = (uint8)(((u4Data) >> 16U) & 0xffU);
        pu1BufferPtr[2U] = (uint8)(((u4Data) >> 8U) & 0xffU);
        pu1BufferPtr[3U] = (uint8)((u4Data) & 0xffU);
    }
    /*in case of Intel MSB last*/
    else
    {
        pu1BufferPtr[3U] = (uint8)((u4Data) >> 24U);
        pu1BufferPtr[2U] = (uint8)(((u4Data) >> 16U) & 0xffU);
        pu1BufferPtr[1U] = (uint8)(((u4Data) >> 8U) & 0xffU);
        pu1BufferPtr[0U] = (uint8)((u4Data) & 0xffU);
    }
    return;
}

/******************************************************************************/
/**
 * @brief               <Put a word into a byte array>
 *
 *      <This Function split the long type data to the byte type array>
 *
 * @param[in]           <u2Data         source word data>
 *                      <byteOrder>
 * @param[out]          <None>
 * @param[in/out]       <pu1BufferPtr   pointer to the byute array>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
Xcp_CopyU2ToU1Buffer(const uint16 u2Data, P2VAR(uint8, AUTOMATIC, XCP_VAR) pu1BufferPtr, uint8 byteOrder)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        pu1BufferPtr[0U] = (uint8)((u2Data) >> 8U);
        pu1BufferPtr[1U] = (uint8)((u2Data) & 0xffU);
    }
    /*in case of Intel MSB last*/
    else
    {
        pu1BufferPtr[1U] = (uint8)((u2Data) >> 8U);
        pu1BufferPtr[0U] = (uint8)((u2Data) & 0xffU);
    }
    return;
}

/******************************************************************************/
/**
 * @brief               <Put a byte array into a long>
 *
 *      <This Function split the byte type array to the long type data>
 *
 * @param[in]           <pu1BufferPtr   pointer to the byte array>
 *                      <byteOrder>
 * @param[out]          <None>
 * @param[in/out]       <pu4Data        pointer to the long>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
Xcp_CopyU1BufferToU4(
    P2CONST(uint8, AUTOMATIC, XCP_VAR) pu1BufferPtr,
    P2VAR(uint32, AUTOMATIC, XCP_VAR) pu4Data,
    uint8 byteOrder)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        *pu4Data = ((uint32)pu1BufferPtr[0U] << 24U) + ((uint32)pu1BufferPtr[1U] << 16U)
                   + ((uint32)pu1BufferPtr[2U] << 8U) + ((uint32)pu1BufferPtr[3U]);
    }
    /*in case of Intel MSB last*/
    else
    {
        *pu4Data = ((uint32)pu1BufferPtr[3U] << 24U) + ((uint32)pu1BufferPtr[2U] << 16U)
                   + ((uint32)pu1BufferPtr[1U] << 8U) + ((uint32)pu1BufferPtr[0U]);
    }
    return;
}

/******************************************************************************/
/**
 * @brief               <Put a byte array into a word>
 *
 *      <This Function split the byte type array to the word type data>
 *
 * @param[in]           <pu1BufferPtr   pointer to the byte array>
 *                      <byteOrder>
 * @param[out]          <None>
 * @param[in/out]       <pu2Data        pointer to the word>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
Xcp_CopyU1BufferToU2(
    P2CONST(uint8, AUTOMATIC, XCP_VAR) pu1BufferPtr,
    P2VAR(uint16, AUTOMATIC, XCP_VAR) pu2Data,
    uint8 byteOrder)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    /*in case of Motorola MSB first*/
    if ((uint8)HIGH_BYTE_FIRST == byteOrder)
    {
        *pu2Data = (uint16)((uint16)pu1BufferPtr[0U] << 8U) + (uint16)pu1BufferPtr[1U];
    }
    /*in case of Intel MSB last*/
    else
    {
        *pu2Data = (uint16)((uint16)pu1BufferPtr[1U] << 8U) + (uint16)pu1BufferPtr[0U];
    }
    return;
}

/******************************************************************************/
/**
 * @brief               <Copy Memory>
 *
 *      <This Function copy the data from source address to the destination address>
 *
 * @param[in]           <u4SourceAddr   source address
                         u4DestAddr     destination address
                         u4Length       memory length>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, XCP_CODE) Xcp_BufferCopy(uint32 u4SourceAddr, uint32 u4DestAddr, uint32 u4Length)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint32 i;

    /*copy SourceAddr to DsetAddr*/
    for (i = 0u; i < u4Length; i++)
    {
        *(uint8*)(u4DestAddr + i) = *(uint8*)(u4SourceAddr + i); /* PRQA S 0306 */ /* MISRA Rule 11.4 */
    }
    return;
}

/******************************************************************************/
/**
 * @brief               <Set ErrorCode of the CRM>
 *
 *      <This Function set the error and the error PID>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <u1ErrorCode    Error Code>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_SetErrorCode(uint8 u1ErrorCode)
{
    /*set the Errorcode in the CRM buffer*/
    Xcp_RespBuffer[XCP_PID_OFFSET] = XCP_RESP_ERR_PID;
    Xcp_RespBuffer[XCP_RESP_ERROR_OFFSET] = u1ErrorCode;
    return;
}
/******************************************************************************/
/**
 * @brief               <Send a command response.>
 *
 * <Send a command response> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_SendResp(void)
{
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
    P2CONST(Xcp_AGType, AUTOMATIC, XCP_CONST) sourPtr;
    P2VAR(Xcp_AGType, AUTOMATIC, BSWM_VAR_CLEARED) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 pos;
    uint8 len;
#endif
    Std_ReturnType ret;

    Xcp_SendStatus |= XCP_RESP_REQUEST;

#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
    if (XCP_RESP_PENDING != (XCP_RESP_PENDING & Xcp_SendStatus))
#endif
    {
/* Block transfer mode */
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        if ((XCP_CMD_BLOCK_TRANSF == Xcp_CommandStatus) && (Xcp_BlockBufferPos != Xcp_BlockBufferLen))
        {
#if (XCP_BUS_TX_POLLING_MODE == STD_ON)
            do
            {
#endif
                /* Set pointer */
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
                sourPtr = (Xcp_AGType*)Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                destPtr = (Xcp_AGType*)(&Xcp_RespBuffer[XCP_UPLOAD_DATA_OFFSET]);
                /* Get data length */
                len = (XCP_MAX_CTO / XCP_AG) - 1U;
                if (len > (Xcp_BlockBufferLen - Xcp_BlockBufferPos))
                {
                    len = (uint8)(Xcp_BlockBufferLen - Xcp_BlockBufferPos);
                }
                /* copy data into frame buffer */
                for (pos = 0; pos < len; pos++)
                {
                    destPtr[pos] = sourPtr[pos];
                }
                /* Set frame length */
                Xcp_RespLength = (len * XCP_AG) + XCP_UPLOAD_DATA_OFFSET;

                if ((Std_ReturnType)E_OK
                    == Xcp_Transmit(
                        (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                        (PduLengthType)Xcp_RespLength,
                        &Xcp_RespBuffer[0U]))
                {
                    /* updata buffer status */
                    Xcp_SendStatus |= XCP_RESP_PENDING;
                    Xcp_TransferTimeOutCnt = 0U;
                    Xcp_BlockBufferPos += len;
                    Xcp_UpdateMTA(((uint32)len * XCP_AG));
                }
#if (XCP_BUS_TX_POLLING_MODE == STD_ON)
                else
                {
                    /* in polling mode continuously send the frame until CAN buffer is full */
                    break;
                }
                /* or until all data has been sent */
            } while (Xcp_BlockBufferPos != Xcp_BlockBufferLen);
#endif /*XCP_BUS_TX_POLLING_MODE == STD_ON*/
            /* block upload command has been sent */
            if (Xcp_BlockBufferPos == Xcp_BlockBufferLen)
            {
                Xcp_SendStatus &= (uint8)(~XCP_RESP_REQUEST);
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
                Xcp_CommandStatus = XCP_CMD_TRANSF;
#else
                /*
                 * in polling mode we do not wait for txConfirmation
                 * thus, the command status shall goto idle immediately
                 */
                Xcp_CommandStatus = XCP_CMD_IDLE;
#endif
            }
/* if not in polling mode the time cost by the block transfer shall be
 * take care when calculate the timeout threshold T1
 */
#if (XCP_BUS_TX_POLLING_MODE == STD_ON)
            else
            {
                /* in polling mode we may need a ev_cmd_pending
                 * when can't finish send all frame here
                 */
                Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
                Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_CMD_PENDING;
                Xcp_EvLength = 0x02;
                Xcp_SendEv();
            }
#endif /*XCP_BUS_TX_POLLING_MODE == STD_ON*/
        }
        else
#endif /* XCP_SLAVE_BLOCK_MODE == STD_ON */
        {
            /* Standard transfer mode */
            ret = Xcp_Transmit(
                (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                (PduLengthType)Xcp_RespLength,
                &Xcp_RespBuffer[0U]);
            if ((Std_ReturnType)E_OK == ret)
            {
                Xcp_SendStatus &= (uint8)(~XCP_RESP_REQUEST);
                Xcp_SendStatus |= XCP_RESP_PENDING;
                Xcp_TransferTimeOutCnt = 0;
                {
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
                    Xcp_CommandStatus = XCP_CMD_TRANSF;
#else
                    /*
                     * in polling mode we do not wait for txConfirmation
                     * thus, the command status shall goto idle immediately
                     */
                    Xcp_CommandStatus = XCP_CMD_IDLE;
                    /*
                     * In polling mode we do not wait for the txconfirmation
                     */
                    if (XCP_PRE_CONNECT == Xcp_Status)
                    {
                        Xcp_Status = XCP_CONNECT;
                    }
                    else if (XCP_PRE_USERDEFINE == Xcp_Status)
                    {
                        Xcp_Status = XCP_USERDEFINE;
                    }
#endif
                }
            }
            else
            {
                /* Do Nothing */
            }
        }
    }
    return;
}
/******************************************************************************/
/**
 * @brief               <Send a Event frame.>
 *
 * <Send a Event frame> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_SendEv(void)
{

    Xcp_SendStatus |= XCP_EV_REQUEST;
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
    if (XCP_EV_PENDING != (XCP_EV_PENDING & Xcp_SendStatus))
#endif
    {
        if ((Std_ReturnType)E_OK
            == Xcp_Transmit(
                (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                (PduLengthType)Xcp_EvLength,
                &Xcp_EvBuffer[0]))
        {
            Xcp_SendStatus &= (uint8)(~XCP_EV_REQUEST);
            Xcp_SendStatus |= XCP_EV_PENDING;
            Xcp_TransferTimeOutCnt = 0U;
        }
    }

    return;
}

/******************************************************************************/
/**
 * @brief               <Send a Service frame.>
 *
 * <Send a Service frame>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_SendServ(void)
{
    Xcp_SendStatus |= XCP_SERV_REQUEST;
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
    if (XCP_SERV_PENDING != (XCP_SERV_PENDING & Xcp_SendStatus))
#endif
    {
        if ((Std_ReturnType)E_OK
            == Xcp_Transmit(
                (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId,
                (PduLengthType)Xcp_ServLength,
                &Xcp_ServBuffer[0]))
        {
            Xcp_SendStatus &= (uint8)(~XCP_SERV_REQUEST);
            Xcp_SendStatus |= XCP_SERV_PENDING;
            Xcp_TransferTimeOutCnt = 0U;
        }
    }
    return;
}
#if (XCP_PID_OFF_SUPPORT == STD_OFF)
/******************************************************************************/
/*
 * @brief               <Xcp Module RxIndication Function>
 *
 * <This function will call associated handler to treat a STIM frame or a command
 * frame>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <len, dataPtr, pduId(in case PID_OFF supported)>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Xcp_RxIndication(const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 pid = dataPtr[XCP_PID_OFFSET];
    /* STIM is not currently supported */

    /* A command has been received */
    if (len <= XCP_MAX_CTO)
    {
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
        if (XCP_CMD_CODE_MIN > pid)
        {
            /* Receive STIM,Cache into buffer */
            /* Not testing */
            Xcp_RxStimHal(dataPtr);
        }
        else
        {
#endif
            /* idle or synch command */
            if (((Xcp_CommandStatusType)XCP_CMD_IDLE == Xcp_CommandStatus) || (XCP_CMD_SYNCH == pid))
            {
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
                Xcp_RxCommandHal(pid, len, dataPtr);
#else
            Xcp_RxCommandHal(len, dataPtr);
#endif
            }
            else
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
                /* Block Recv */
                if (XCP_CMD_BLOCK_RECV == Xcp_CommandStatus)
                {
                    Xcp_RxBlockHal(pid, len, dataPtr);
                }
                else
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/
                /* cmd busy */
                {
                    Xcp_SetErrorCode(XCP_ERR_CMD_BUSY);
                    Xcp_RespLength = 0x02u;
                    Xcp_SendResp();
                }
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
        }
#endif
    }
    else
    {
        /* Do Nothing */
    }
    return;
}
#endif

#if (XCP_DEV_ERROR_DETECT == STD_ON)
FUNC(boolean, XCP_CODE)
Xcp_RxIndCheckDetErr(PduIdType XcpRxPduId, P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr)
{
    boolean errorFlag = FALSE;
    boolean pduIdError = FALSE;

    if (XCP_UINIT == Xcp_Status)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_RXINDICATION, XCP_E_NOT_INITIALIZED);
        errorFlag = TRUE;
    }
    else if (NULL_PTR == XcpRxPduPtr)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_RXINDICATION, XCP_E_INV_POINTER);
        errorFlag = TRUE;
    }
    else
    {
        if (Xcp_PbCfgPtr->numOfRxPdu <= XcpRxPduId)
        {
            pduIdError = TRUE;
        }
        if ((boolean)TRUE == pduIdError)
        {
            /* PduId check failed */
            (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_RXINDICATION, XCP_E_INVALID_PDUID);
            errorFlag = TRUE;
        }
    }
    return errorFlag;
}
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/

/******************************************************************************/
/*
 * @brief               <Xcp Module Command Rx Handeler>
 *
 * <This function copy command frame data into asosciated buffer>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <len, dataPtr, pid(in case Master_Block supported)>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE)
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    Xcp_RxCommandHal(const uint8 pid, const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr)
#else
    Xcp_RxCommandHal(const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr)
#endif
{
#if ((0 != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE)) && (XCP_MASTER_BLOCK_MODE == STD_ON))
    const Xcp_AGType* sourPtr;
    Xcp_AGType* destPtr;
    Xcp_BlockBufIndxType pos;
#endif
    uint16 lenCounter;
    SchM_Enter_Xcp_Exclusive_Common();
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
#if (XCP_GET_DAQ_CLOCK == STD_ON)
    /* Get Recieve Time */
    Xcp_RxTimeStamp = Xcp_ReadOutMS();
#endif
#endif

#if (XCP_INTERLEAVED_MODE == STD_ON)
    if ((boolean)FALSE == Xcp_IsInterleavedBufFull())
    {
        for (lenCounter = 0; lenCounter < len; lenCounter++)
        {
            Xcp_InterLevCmdBuffer[Xcp_InterLevModeCtrl.posRear][lenCounter] = dataPtr[lenCounter];
        }
        Xcp_InterLevCmdLen[Xcp_InterLevModeCtrl.posRear] = (uint8)len;
        Xcp_InterLevModeCtrl.posRear++;
        Xcp_InterLevModeCtrl.posRear %= XCP_QUEUE_SIZE;
        Xcp_InterLevRecv = TRUE;
    }
#else
    /* copy data to the local buffer */
    for (lenCounter = 0; lenCounter < len; lenCounter++)
    {
        Xcp_CmdBuffer[lenCounter] = dataPtr[lenCounter];
    }
    Xcp_CmdLength = (uint8)len;
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
        Xcp_CommandStatus = XCP_CMD_EXCUTE;
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
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
            Xcp_BlockBufferLen = dataPtr[XCP_DOWNLOAD_LEN_OFFSET];
            /* set pointer to the sour and dest [AG] */
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_DOWNLOAD_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[0];
            /* copy data until the end of the frame */
            lenCounter = (XCP_DOWNLOAD_DATA_OFFSET + XCP_AG);
            pos = 0u;
            while (lenCounter <= len)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            /* updata some state and set the pid to download_next */
            Xcp_BlockBufferPos = pos;
            Xcp_CmdBuffer[XCP_PID_OFFSET] = XCP_CMD_DOWNLOAD_NEXT;
            Xcp_CommandStatus = XCP_CMD_BLOCK_RECV;
        }
        /* block length out of range */
        else
        {
            Xcp_CommandStatus = XCP_CMD_EXCUTE;
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
            Xcp_SendResp();
        }
    }
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    else if (
        (XCP_CMD_PROGRAM == pid) && (dataPtr[XCP_PROGRAM_LEN_OFFSET] > XCP_PROGRAM_SIZE)
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        && (XCP_PGM_IDLE != Xcp_PgmStauts) && (XCP_PGM_START != Xcp_PgmStauts)
#endif
    )
    {
        /* the length of the *_next shall be check here to prevent the overload
         * of the BlockBuffer
         */
        if (dataPtr[XCP_PROGRAM_LEN_OFFSET] <= XCP_PROGRAM_NEXT_SIZE)
        {
            /* get total length */
            Xcp_BlockBufferLen = dataPtr[XCP_PROGRAM_LEN_OFFSET];
            /* set pointer to the sour and dest [AG] */
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_PROGRAM_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[0];
            /* copy data until the end of the frame */
            lenCounter = (XCP_PROGRAM_DATA_OFFSET + XCP_AG);
            pos = 0u;
            while (lenCounter <= len)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            /* updata some state and set the pid to download_next */
            Xcp_BlockBufferPos = pos;
            Xcp_CmdBuffer[XCP_PID_OFFSET] = XCP_CMD_PROGRAM_NEXT;
            Xcp_CommandStatus = XCP_CMD_BLOCK_RECV;
        }
        /* block length out of range */
        else
        {
            Xcp_CommandStatus = XCP_CMD_EXCUTE;
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
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
        Xcp_CommandStatus = XCP_CMD_RECV;
#endif /*XCP_INTERLEAVED_MODE != STD_ON*/
    }
    SchM_Exit_Xcp_Exclusive_Common();
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp Module Rx Block Handeler>
 *
 * <This function will check and copy the data into the buffer in block transfer mode>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <len, dataPtr, pid>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
static FUNC(void, XCP_CODE)
    Xcp_RxBlockHal(const uint8 pid, const PduLengthType len, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr)
{
#if (0 != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
    const Xcp_AGType* sourPtr;
    Xcp_AGType* destPtr;
    uint16 lenCounter;
    uint16 lenMax = len;
#endif
    Xcp_BlockBufIndxType pos = 0u;

    /* pid shall be the associated *_NEXT */
    if (pid == Xcp_CmdBuffer[XCP_PID_OFFSET])
    {
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
        if ((XCP_CMD_DOWNLOAD_NEXT == pid)
            && (dataPtr[XCP_DOWNLOAD_LEN_OFFSET]
                == (Xcp_BlockBufferLen - Xcp_BlockBufferPos))) /* Determine whether the length is consistent */
        {
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_DOWNLOAD_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[Xcp_BlockBufferPos];
            lenCounter = XCP_DOWNLOAD_DATA_OFFSET + (uint16)(XCP_AG * (Xcp_BlockBufferLen - Xcp_BlockBufferPos));
            if (lenMax > lenCounter)
            {
                lenMax = lenCounter;
            }
            lenCounter = XCP_DOWNLOAD_DATA_OFFSET + XCP_AG;
            pos = 0u;
            while (lenCounter <= lenMax)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            Xcp_BlockBufferPos += pos;
            if (Xcp_BlockBufferPos == Xcp_BlockBufferLen)
            {
                Xcp_CommandStatus = XCP_CMD_RECV;
            }
        }
#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if ((XCP_CMD_PROGRAM_NEXT == pid)
            && (dataPtr[XCP_PROGRAM_LEN_OFFSET] == (Xcp_BlockBufferLen - Xcp_BlockBufferPos)))
        {
            sourPtr = (const Xcp_AGType*)&dataPtr[XCP_PROGRAM_DATA_OFFSET];
            destPtr = &Xcp_BlockBuffer[Xcp_BlockBufferPos];
            lenCounter = XCP_PROGRAM_DATA_OFFSET + (uint16)(XCP_AG * (Xcp_BlockBufferLen - Xcp_BlockBufferPos));
            if (lenMax > lenCounter)
            {
                lenMax = (uint16)lenCounter;
            }
            lenCounter = XCP_PROGRAM_DATA_OFFSET + XCP_AG;
            pos = 0u;
            while (lenCounter <= lenMax)
            {
                destPtr[pos] = sourPtr[pos];
                lenCounter += XCP_AG;
                pos++;
            }
            Xcp_BlockBufferPos += pos;
            if (Xcp_BlockBufferPos == Xcp_BlockBufferLen)
            {
                Xcp_CommandStatus = XCP_CMD_RECV;
            }
        }
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/

        if (pos == 0u)
        {
            Xcp_CommandStatus = XCP_CMD_EXCUTE;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespBuffer[XCP_RESP_ERROR_OFFSET + 1U] = (uint8)(Xcp_BlockBufferLen - Xcp_BlockBufferPos);
            Xcp_RespLength = 0x03u;
            Xcp_SendResp();
        }
    }
    /* with wrong pid in the sequence an error shall be reported */
    else
    {
        Xcp_CommandStatus = XCP_CMD_EXCUTE;
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
        Xcp_SendResp();
    }
    return;
}
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

/******************************************************************************/
/**
 * @brief               <Xcp module transmission confirmation>
 *
 * <It update the flag and try to send next frame if there is one when last frame
 *  has been reported sent successfully> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <pduId: PduId of last DTO/CTO>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_TxConfirmation(const PduIdType pduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* Since last frame has been sent, clear the pending flag */
    Xcp_SendStatus &= (uint8)(~XCP_SEND_PENDING);
    Xcp_TransferTimeOutCnt = 0x00u;

/* In case the bus driver is working in polling mode the commandstatus
 * will be set to Idle immediately when the <lower_layer>_Transmit is return E_OK
 */
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
    /* in case interrupt mode is used, we change status here, otherwise
     * this shall be done in Xcp_SendResp()
     */
    /* After the CRM sent, reset the command status */
    if ((XCP_CMD_TRANSF == Xcp_CommandStatus) && (pduId == Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->XcpLocalTxPduId))
    {
        Xcp_CommandStatus = XCP_CMD_IDLE;
#if (XCP_INTERLEAVED_MODE == STD_ON)
        if (Xcp_InterLevModeCtrl.posRear != Xcp_InterLevModeCtrl.posHead)
        {
            Xcp_CommandStatus = XCP_CMD_RECV;
        }
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
        /* stack will go into the connected status when the positive ack sent successfully */
        if (XCP_PRE_CONNECT == Xcp_Status)
        {
            Xcp_Status = XCP_CONNECT;
        }
        else if (XCP_PRE_USERDEFINE == Xcp_Status)
        {
            Xcp_Status = XCP_USERDEFINE;
        }
        else
        {
            /* do nothing */
        }
    }
#endif /* XCP_BUS_TX_POLLING_MODE == STD_OFF */
    /* Send next frame if necessary */
    Xcp_TxQueueHandler();
    return;
}

#if (XCP_DEV_ERROR_DETECT == STD_ON)
FUNC(boolean, XCP_CODE) Xcp_TxIndCheckDetErr(void)
{
    boolean errFlag = FALSE;

    if (XCP_UINIT == Xcp_Status)
    {
        (void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, XCP_SERVICE_ID_TXCONFIRMATION, XCP_E_NOT_INITIALIZED);
        errFlag = TRUE;
    }
    return errFlag;
}
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/

static FUNC(void, XCP_CODE) Xcp_TxQueueHandler(void)
{
    if (XCP_RESP_REQUEST == (XCP_RESP_REQUEST & Xcp_SendStatus))
    {
        Xcp_SendResp();
    }
    if (XCP_SERV_REQUEST == (XCP_SERV_REQUEST & Xcp_SendStatus))
    {
        Xcp_SendServ();
    }
    if (XCP_EV_REQUEST == (XCP_EV_REQUEST & Xcp_SendStatus))
    {
        Xcp_SendEv();
    }
/* here we put Daq below the Ev cause there is some Ev has high priority than DAQ
 * for example Ev_Sleep
 */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
    if (XCP_DAQ_REQUEST == (XCP_DAQ_REQUEST & Xcp_SendStatus))
    {
        Xcp_SendDaq();
    }
#endif
    return;
}

static FUNC(void, XCP_CODE) Xcp_Cmd_ReservedCmd(void)
{
    Xcp_SetErrorCode(XCP_ERR_CMD_UNKNOWN);
    Xcp_RespLength = 0x02u;
    Xcp_SendResp();
}
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
