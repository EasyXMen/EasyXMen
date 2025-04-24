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
**  FILENAME    : Ea.c                                                        **
**                                                                            **
**  Created on  : 2022/02/15                                                  **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Ea source description                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Ea<br>
  RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 2983 MISRA Rule 2.2 .<br>
    Reason:According to the configuration, the size of the variable after conversion is different, in order to adapt to
  other situations.

   \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:According to the configuration, the size of the variable after conversion is different, in order to adapt to
  other situations.

   \li PRQA S 3415 MISRA Rule 13.5 .<br>
    Reason:Conditional judgment exists in order, design needs

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 3408 MISRA Rule 8.4 .<br>
    Reason:Configuration parameters, design requirements.

    \li PRQA S 1531 MISRA Rule 8.7 .<br>
    Reason:Configuration parameters, design requirements

    \li PRQA S 3332 MISRA Rule 20.9 .<br>
    Reason:Applicable to 16-bit platforms, design requirements
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Ea.h"
#include "Ea_Cfg.h"
#include "Det.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef EA_AR_RELEASE_MAJOR_VERSION
#error "Ea.c : Miss Specification Major Version"
#endif
#ifndef EA_AR_RELEASE_MINOR_VERSION
#error "Ea.c : Miss Specification MINOR Version"
#endif
#ifndef EA_AR_RELEASE_PATCH_VERSION
#error "Ea.c : Miss Specification PATCH Version"
#endif
#if ((2 != EA_CFG_SW_MAJOR_VERSION) || (0 != EA_CFG_SW_MINOR_VERSION) || (0 != EA_CFG_SW_PATCH_VERSION))
#error " cfg version mismatching"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/******** Error Codes in Development-Mode *************************************/
#define EA_E_UNINIT            (0x01u)
#define EA_E_INVALID_BLOCK_NO  (0x02u)
#define EA_E_INVALID_BLOCK_OFS (0x03u)
#define EA_E_PARAM_POINTER     (0x04u)
#define EA_E_INVALID_BLOCK_LEN (0x05u)
#define EA_E_BUSY              (0x06u)
#define EA_E_INVALID_CANCEL    (0x08u)
#define EA_E_INIT_FAILED       (0x09u)

/* Development error codes additionally defined to AUTOSAR */
#define EA_SID_INIT                 (0x00u)
#define EA_SID_SET_MODE             (0x01u)
#define EA_SID_READ                 (0x02u)
#define EA_SID_WRITE                (0x03u)
#define EA_SID_CANCEL               (0x04u)
#define EA_SID_GETSTATUS            (0x05u)
#define EA_SID_GETJOBRESULT         (0x06u)
#define EA_SID_INVALIDATEBLOCK      (0x07u)
#define EA_SID_GETVERSIONINFO       (0x08u)
#define EA_SID_ERASEIMMEDIATE_BLOCK (0x09u)
#define EA_SID_JOBENDNOTIFICATION   (0x10u)
#define EA_SID_JOBERRORNOTIFICATION (0x11u)

#if (EA_DEV_ERROR_DETECT == STD_ON)
static inline void Ea_DetReport(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, ApiId, ErrorId);
}
#endif

static inline void Ea_DetReportRuntime(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(EA_MODULE_ID, EA_INSTANCE_ID, (ApiId), (ErrorId));
}

#define EA_BLOCKNUM_INVALID          (0xFFFFu)

#define EA_BLOCK_INVALID             (0x5A5Au)
#define EA_BLOCK_VALID               (0xA5A5u)

#define EA_BLOCK_STATUS_INVALID      (0u)
#define EA_BLOCK_STATUS_VALID        (1u)
#define EA_BLOCK_STATUS_INCONSISTENT (2u)
#define EA_BLOCK_STATUS_CONSISTENT   (3u)

/*  EA block header info
 *  byte0 - byte1: BlockNumber (uint16)
 *  byte2 - byte3: BlockSize (uint16)
 *  byte4 - byte7: BlockWriteCycle (uint32)
 *  byte8 - byte9: BlockValid  0xA5 0xA5 means valid; 0x5A 0x5A means invalid;
 *                 others means inconsistent
 *  byte10 - byte13 BlockStartAddress (uint32)
 *
 *  total header size: 14byte
 * */
/* PRQA S 3332++ */ /* MISRA Rule 20.9 */
#if (CPU_32_WITH_16_ADR == TRUE)
#define EA_TOTAL_HEADER_LENGTH (10u + (sizeof(Eep_AddressType) * 2u))
#else
#define EA_TOTAL_HEADER_LENGTH (10u + sizeof(Eep_AddressType))
#endif

#define EA_HEADER_ALIGNMENT_LENGTH \
    (((EA_TOTAL_HEADER_LENGTH + (EA_VIRTUAL_PAGE_SIZE - 1U)) / EA_VIRTUAL_PAGE_SIZE) * EA_VIRTUAL_PAGE_SIZE)

#define EA_SET_BLOCK_INFO(blockIndex, isOperated, blockVaild) \
    Ea_BlockInfo[blockIndex].IsOperated = isOperated;         \
    Ea_BlockInfo[blockIndex].BlockVaild = blockVaild;

#if (CPU_32_WITH_16_ADR == TRUE)
#define EA_SERIALIZE(ParamVal, pSerialPtr, ParamType)        \
    do                                                       \
    {                                                        \
        for (uint8 i = 0; i < (sizeof(ParamType) * 2u); i++) \
        {                                                    \
            pSerialPtr[i] = (uint8)(ParamVal >> (i * 8u));   \
        }                                                    \
        pSerialPtr = &pSerialPtr[sizeof(ParamType) * 2u];    \
    } while (0);

#define EA_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                              \
    do                                                                                 \
    {                                                                                  \
        ParamVal = 0;                                                                  \
        for (uint8 i = 0; i < (sizeof(ParamType) * 2u); i++)                           \
        {                                                                              \
            ParamVal = ParamVal | (ParamType)((ParamType)pDeserialPtr[i] << (i * 8u)); \
        }                                                                              \
        pDeserialPtr = &pDeserialPtr[sizeof(ParamType) * 2u];                          \
    } while (0);
#else
#define EA_SERIALIZE(ParamVal, pSerialPtr, ParamType)      \
    do                                                     \
    {                                                      \
        for (uint8 i = 0; i < sizeof(ParamType); i++)      \
        {                                                  \
            pSerialPtr[i] = (uint8)(ParamVal >> (i * 8u)); \
        }                                                  \
        pSerialPtr = &pSerialPtr[sizeof(ParamType)];       \
    } while (0);

#define EA_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                              \
    do                                                                                 \
    {                                                                                  \
        ParamVal = 0;                                                                  \
        for (uint8 i = 0; i < sizeof(ParamType); i++)                                  \
        {                                                                              \
            ParamVal = ParamVal | (ParamType)((ParamType)pDeserialPtr[i] << (i * 8u)); \
        }                                                                              \
        pDeserialPtr = &pDeserialPtr[sizeof(ParamType)];                               \
    } while (0);
#endif
/* PRQA S 3332-- */ /* MISRA Rule 20.9 */
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef struct
{
    uint8* DataBufferPtr;
    const uint8* DataWriteBufferPtr;
    uint16 BlockNumber;
    uint16 BlockIndex;
    uint16 BlockOffset;
    uint16 Length;
    uint16 DealSize;
    uint16 DealOffset;
    uint8* InterDataBufferPtr;
} Ea_PendReqType;

typedef struct
{
    MemIf_JobResultType EaLastJobResult;
    Ea_RequestJobType EaRequestJob;
    Ea_SubJobStatusType EaSubJobStatus;
    Ea_EepStateType EaEepState;
    Ea_PendReqType EaPendReq;
} Ea_RunTimeDataType;

typedef struct
{
    MemIf_JobResultType BlockVaild;
    boolean IsOperated;
    uint32 BlockWriteCycle;
} Ea_BlockInfoType;
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/*---------- FAST INIT VARIABLES ----------*/
#define EA_START_SEC_VAR_INIT_8
#include "Ea_MemMap.h"
static VAR(MemIf_StatusType, EA_APPL_DATA) Ea_State = MEMIF_UNINIT;
#define EA_STOP_SEC_VAR_INIT_8
#include "Ea_MemMap.h"

#define EA_START_SEC_VAR_NO_INIT_8
#include "Ea_MemMap.h"
static VAR(uint8, EA_APPL_DATA) Ea_ReadWritBuffer[EA_BLOCK_MAX_SIZE + EA_TOTAL_HEADER_LENGTH];
#define EA_STOP_SEC_VAR_NO_INIT_8
#include "Ea_MemMap.h"

#define EA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Ea_MemMap.h"
static VAR(Ea_RunTimeDataType, EA_APPL_DATA) Ea_RunTimeData;
#define EA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Ea_MemMap.h"

#define EA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Ea_MemMap.h"
static VAR(Ea_BlockInfoType, EA_APPL_DATA) Ea_BlockInfo[EA_NUMBER_OF_BLOCKS];
#define EA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Ea_MemMap.h"

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/* get the block BlockNumber index. */
static FUNC(uint16, EA_PUBLIC_CODE) Ea_GetBlockIndex(uint16 BlockNumber);

/* job finish, set runtime data*/
static FUNC(void, EA_PUBLIC_CODE) Ea_JobFinish(MemIf_JobResultType JobResult);

/* Service to report to this module the successful end of an asynchronous operation.*/
static FUNC(void, EA_PUBLIC_CODE) Ea_CallJobEndNotification(void);

/* Service to report to this module the failure of an asynchronous operation. */
static FUNC(void, EA_PUBLIC_CODE) Ea_CallJobErrorNotification(void);

/* Service to read */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterRead(void);

/* Service to write */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterWrite(void);

/* Service to invalidate */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterInvalidate(void);

/* Service to erase */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterErase(void);

/* Service to cancel */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterCancel(void);

/* read job finish, copy data to upper buffer */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterCopyReadData(void);

/* Service to call MemAcc Api */
static FUNC(void, EA_PUBLIC_CODE) Ea_MemAccReq(Ea_RequestJobType Ea_RequestJob);

/* read job finish, check read header */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterCheckReadHeader(void);

/* write or invalidate job process, prepare write header */
static FUNC(void, EA_PUBLIC_CODE) Ea_InterPrepareWriteHeader(uint8 status);

#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#if (EA_POLLING_MODE == STD_OFF)
#define EA_START_SEC_CONST_UNSPECIFIED
#include "Ea_MemMap.h"
static const Ea_VoidFunctionVoidPtr EA_InterServFunc[] = {
    NULL_PTR,           /* EA_JOB_IDLE */
    Ea_InterRead,       /* EA_JOB_READ */
    Ea_InterWrite,      /* EA_JOB_WRITE */
    Ea_InterCancel,     /* EA_JOB_CANCEL */
    Ea_InterInvalidate, /* EA_JOB_INVALIDATE */
    Ea_InterErase,      /* EA_JOB_ERASE */
};
#define EA_STOP_SEC_CONST_UNSPECIFIED
#include "Ea_MemMap.h"
#endif /* EA_POLLING_MODE == STD_OFF */

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the EEPROM abstraction module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the selected configuration set.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, EA_PUBLIC_CODE) Ea_Init(P2CONST(Ea_ConfigType, EA_VAR, EA_CONST) ConfigPtr)
{
    uint16 iloop;
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR != ConfigPtr)
    {
        Ea_DetReport(EA_SID_INIT, EA_E_INIT_FAILED);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        (void)ConfigPtr;
        /* init run time data of Ea*/
        Ea_RunTimeData.EaLastJobResult = MEMIF_BLOCK_INCONSISTENT;
        Ea_RunTimeData.EaRequestJob = EA_JOB_IDLE;
        Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_IDLE;
        Ea_RunTimeData.EaEepState = EA_EEP_JOB_IDLE;
        Ea_RunTimeData.EaPendReq.BlockNumber = 0;
        Ea_RunTimeData.EaPendReq.BlockOffset = 0;
        Ea_RunTimeData.EaPendReq.BlockIndex = EA_BLOCKNUM_INVALID;
        Ea_RunTimeData.EaPendReq.Length = 0;
        Ea_RunTimeData.EaPendReq.DealOffset = 0;
        Ea_RunTimeData.EaPendReq.DealSize = 0;
        Ea_RunTimeData.EaPendReq.DataBufferPtr = NULL_PTR;
        Ea_RunTimeData.EaPendReq.DataWriteBufferPtr = NULL_PTR;
        Ea_RunTimeData.EaPendReq.InterDataBufferPtr = NULL_PTR;

        /* init internal buffer of Ea*/
        for (iloop = 0; iloop < (EA_BLOCK_MAX_SIZE + EA_TOTAL_HEADER_LENGTH); iloop++)
        {
            Ea_ReadWritBuffer[iloop] = 0;
        }

        /* init block information of Ea*/
        for (iloop = 0; iloop < Ea_Config.Ea_BlockCfg->BlockNum; iloop++)
        {
            Ea_BlockInfo[iloop].IsOperated = FALSE;
            Ea_BlockInfo[iloop].BlockVaild = MEMIF_BLOCK_INCONSISTENT;
            Ea_BlockInfo[iloop].BlockWriteCycle = 0;
        }
        Ea_State = MEMIF_IDLE;
    }
}

#if (EA_SETMODE_API == STD_ON)
/******************************************************************************/
/*
 * Brief               Sets the mode.
                       Sets the mode Mode for the underlying EEPROM driver.
 * ServiceId           0x01
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Mode: Desired mode for the underlying EEPROM driver
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None :
 */
/******************************************************************************/
FUNC(void, EA_PUBLIC_CODE) Ea_SetMode(MemIf_ModeType Mode)
{
    uint8 iloop;
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_SET_MODE, EA_E_UNINIT);
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_DetReport(EA_SID_SET_MODE, EA_E_BUSY);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        for (iloop = 0; iloop < Ea_Config.EaDeviceNum; iloop++)
        {
            /* Set mode of EEP driver */
            if (NULL_PTR != Ea_Config.Ea_EepApi[iloop].Eep_SetModeApi)
            {
                Ea_Config.Ea_EepApi[iloop].Eep_SetModeApi(Mode);
            }
        }
    }
}
#endif /* EA_SETMODE_API == STD_ON */

/******************************************************************************/
/*
 * Brief               Reads Length bytes of block BlockNumber at offset
 *                     BlockOffset into the buffer DataBufferPtr.
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start
 *                                  address of that block in EEPROM.
                       BlockOffset: Read address offset inside the block(read offset within block)
                       Length: Number of bytes to read(Length of read job)
 * Param-Name[out]     DataBufferPtr: Pointer to data buffer
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: Returns if requested job has been accepted by the EA module.
                       E_OK: The requested job has been accepted by the module.
                       E_NOT_OK: The requested job has not been accepted by the EA module.
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE)
Ea_Read(uint16 BlockNumber, uint16 BlockOffset, P2VAR(uint8, EA_VAR, EA_VAR) DataBufferPtr, uint16 Length)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_READ, EA_E_UNINIT);
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_DetReport(EA_SID_READ, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        Ea_DetReport(EA_SID_READ, EA_E_INVALID_BLOCK_NO);
    }
    else if (Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize < (BlockOffset + EA_BLOCK_OVER_HEAD))
    {
        Ea_DetReport(EA_SID_READ, EA_E_INVALID_BLOCK_OFS);
    }
    else if (Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize < (Length + BlockOffset + EA_BLOCK_OVER_HEAD))
    {
        Ea_DetReport(EA_SID_READ, EA_E_INVALID_BLOCK_LEN);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        Ea_DetReport(EA_SID_READ, EA_E_PARAM_POINTER);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        Ea_State = MEMIF_BUSY;
        Ea_RunTimeData.EaLastJobResult = MEMIF_JOB_PENDING;
        Ea_RunTimeData.EaRequestJob = EA_JOB_READ;
        Ea_RunTimeData.EaPendReq.BlockNumber = BlockNumber;
        Ea_RunTimeData.EaPendReq.BlockIndex = blockIndex;
        Ea_RunTimeData.EaPendReq.Length = Length;
        Ea_RunTimeData.EaPendReq.BlockOffset = BlockOffset;
        Ea_RunTimeData.EaPendReq.DataBufferPtr = DataBufferPtr;
        Ea_RunTimeData.EaPendReq.InterDataBufferPtr = Ea_ReadWritBuffer;
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Writes the contents of the DataBufferPtr to the block BlockNumber.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address
 *                                  of that block in EEPROM.
                       DataBufferPtr: Pointer to data buffer
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: Returns if requested job has been accepted by the EA module.
                       E_OK: The requested job has been accepted by the module.
                       E_NOT_OK: The requested job has not been accepted by the EA module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, EA_PUBLIC_CODE)
Ea_Write(uint16 BlockNumber, P2CONST(uint8, EA_VAR, EA_CONST) DataBufferPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_WRITE, EA_E_UNINIT);
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_DetReport(EA_SID_WRITE, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        Ea_DetReport(EA_SID_WRITE, EA_E_INVALID_BLOCK_NO);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        Ea_DetReport(EA_SID_WRITE, EA_E_PARAM_POINTER);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
        if ((0u == Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaNumberOfWriteCycles)
            || (Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaNumberOfWriteCycles
                > Ea_BlockInfo[blockIndex].BlockWriteCycle))
        {
            Ea_State = MEMIF_BUSY;
            Ea_RunTimeData.EaLastJobResult = MEMIF_JOB_PENDING;
            Ea_RunTimeData.EaRequestJob = EA_JOB_WRITE;
            Ea_RunTimeData.EaPendReq.BlockNumber = BlockNumber;
            Ea_RunTimeData.EaPendReq.BlockIndex = blockIndex;
            Ea_RunTimeData.EaPendReq.Length = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize;
            Ea_RunTimeData.EaPendReq.DataWriteBufferPtr = DataBufferPtr;
            Ea_RunTimeData.EaPendReq.InterDataBufferPtr = Ea_ReadWritBuffer;
            if (Ea_BlockInfo[blockIndex].BlockWriteCycle < 0xFFFFFFFFUL)
            {
                Ea_BlockInfo[blockIndex].BlockWriteCycle++;
            }
            ret = E_OK;
        }
        else
        {
            /*idle*/
        }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Cancels the ongoing asynchronous operation.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, EA_PUBLIC_CODE) Ea_Cancel(void)
{
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_CANCEL, EA_E_UNINIT);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
        if (MEMIF_BUSY != Ea_State)
        {
            Ea_DetReportRuntime(EA_SID_CANCEL, EA_E_INVALID_CANCEL);
        }
        else
        {
            Ea_RunTimeData.EaRequestJob = EA_JOB_CANCEL;
        }
}

/******************************************************************************/
/*
 * Brief               Service to return the status.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_StatusType: MEMIF_UNINIT: The EA module has not been initialized(yet).
                       MEMIF_IDLE: The EA module is currently idle.
                       MEMIF_BUSY: The EA module is currently busy.
                       MEMIF_BUSY_INTERNAL: The EA module is currently busy with
                                            nternal management operations.
 */
/******************************************************************************/
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) Ea_GetStatus(void)
{
    return Ea_State;
}

/******************************************************************************/
/*
 * Brief               Service to return the job result.
                       Returns current job result of EA module.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_JobResultType: MEMIF_JOB_OK: The last job has been
 *                     finished successfully.
                       MEMIF_JOB_PENDING: The last job is waiting for execution or currently being
                       executed.
                       MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed).
                       MEMIF_JOB_FAILED: The last job was not finished successfully (it failed).
                       MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain
                       corrupted data.
                       MEMIF_BLOCK_INVALID: The requested block has been invalidated, the requested
                       operation can not be performed.
 */
/******************************************************************************/
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) Ea_GetJobResult(void)
{
    MemIf_JobResultType ret = MEMIF_JOB_FAILED;
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_GETJOBRESULT, EA_E_UNINIT);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        ret = Ea_RunTimeData.EaLastJobResult;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Invalidates the block BlockNumber.
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of
 *                      that block in EEPROM.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * pre                 EA component status is MEMIF_IDLE.
 * Return              Std_ReturnType: E_OK: The requested job has been accepted by the module.
                       E_NOT_OK - only if DET is enabled: The requested job has not been accepted
                        by the EA module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_InvalidateBlock(uint16 BlockNumber)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_INVALIDATEBLOCK, EA_E_UNINIT);
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_DetReport(EA_SID_INVALIDATEBLOCK, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        Ea_DetReport(EA_SID_INVALIDATEBLOCK, EA_E_INVALID_BLOCK_NO);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        Ea_State = MEMIF_BUSY;
        Ea_RunTimeData.EaLastJobResult = MEMIF_JOB_PENDING;
        Ea_RunTimeData.EaRequestJob = EA_JOB_INVALIDATE;
        Ea_RunTimeData.EaPendReq.BlockNumber = BlockNumber;
        Ea_RunTimeData.EaPendReq.BlockIndex = blockIndex;
        Ea_RunTimeData.EaPendReq.InterDataBufferPtr = Ea_ReadWritBuffer;
        ret = E_OK;
    }
    return ret;
}

#if (EA_VERSION_INFO_API == STD_ON)
/******************************************************************************/
/*
 * Brief               Service to get the version information of EA module.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     VersionInfo: Pointer to standard version information structure
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, EA_PUBLIC_CODE)
Ea_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, EA_APPL_DATA) VersionInfoPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if (EA_DEV_ERROR_DETECT == STD_ON)
    /* Development Error Checks */
    if (NULL_PTR == VersionInfoPtr)
    {
        Ea_DetReport(EA_SID_GETVERSIONINFO, EA_E_PARAM_POINTER);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        /*----------- Implementation -------------*/
        VersionInfoPtr->vendorID = EA_VENDOR_ID;
        VersionInfoPtr->moduleID = EA_MODULE_ID;
        VersionInfoPtr->sw_major_version = EA_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = EA_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = EA_SW_PATCH_VERSION;
    }
}
#endif

/******************************************************************************/
/*
 * Brief               Erases the block BlockNumber.
 * ServiceId           0x09
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting
 *                      start address of that block in EEPROM.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: E_OK: The requested job has been accepted by the module.
                       E_NOT_OK - only if DET is enabled: The requested job has not been accepted
                        by the EA module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_EraseImmediateBlock(uint16 BlockNumber)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_ERASEIMMEDIATE_BLOCK, EA_E_UNINIT);
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_DetReport(EA_SID_ERASEIMMEDIATE_BLOCK, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        Ea_DetReport(EA_SID_ERASEIMMEDIATE_BLOCK, EA_E_INVALID_BLOCK_NO);
    }
    else if (FALSE == Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaImmediateData)
    {
        Ea_DetReport(EA_SID_ERASEIMMEDIATE_BLOCK, EA_E_INVALID_BLOCK_NO);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
        Ea_State = MEMIF_BUSY;
        Ea_RunTimeData.EaLastJobResult = MEMIF_JOB_PENDING;
        Ea_RunTimeData.EaRequestJob = EA_JOB_ERASE;
        Ea_RunTimeData.EaPendReq.BlockNumber = BlockNumber;
        Ea_RunTimeData.EaPendReq.BlockIndex = blockIndex;
        ret = E_OK;
    }
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"

#define EA_START_SEC_JOBENDNOTIFICATIONCALLBACK_CODE
#include "Ea_MemMap.h"
/******************************************************************************/
/*
 * Brief               Service to report to this module the successful end of
                        an asynchronous operation.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, EA_PUBLIC_CODE) Ea_JobEndNotification(void)
{
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_JOBENDNOTIFICATION, EA_E_UNINIT);
    }
    else
#endif /* EA_DEV_ERROR_DETECT == STD_ON */
    {
#if (EA_POLLING_MODE == STD_OFF)
        if (MEMIF_JOB_PENDING == Ea_RunTimeData.EaLastJobResult)
        {
            EA_InterServFunc[Ea_RunTimeData.EaRequestJob]();
        }
        else
        {
            Ea_CallJobEndNotification();
        }
#endif /* EA_POLLING_MODE == STD_OFF */
    }
}
#define EA_STOP_SEC_JOBENDNOTIFICATIONCALLBACK_CODE
#include "Ea_MemMap.h"

#define EA_START_SEC_JOBERRORNOTIFICATIONCALLBACK_CODE
#include "Ea_MemMap.h"
/******************************************************************************/
/*
 * Brief               Service to report to this module the failure of an asynchronous operation.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, EA_PUBLIC_CODE) Ea_JobErrorNotification(void)
{
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_DetReport(EA_SID_JOBERRORNOTIFICATION, EA_E_UNINIT);
    }
    else
#endif
    {
#if (EA_POLLING_MODE == STD_OFF)
        if (MEMIF_JOB_PENDING == Ea_RunTimeData.EaLastJobResult)
        {
            EA_InterServFunc[Ea_RunTimeData.EaRequestJob]();
        }
        else
        {
            Ea_CallJobErrorNotification();
        }
#endif /* EA_POLLING_MODE == STD_OFF */
    }
}
#define EA_STOP_SEC_JOBERRORNOTIFICATIONCALLBACK_CODE
#include "Ea_MemMap.h"

#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/******************************************************************************/
/*
 * Brief               Service to handle the requested jobs and the internal management operations.
 * ServiceId           0x12
 * Sync/Async          --
 * Reentrancy          --
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, EA_PUBLIC_CODE) Ea_MainFunction(void)
{
    /* MainFunction is only processed if busy */
    if (Ea_State > MEMIF_IDLE)
    {
#if (EA_POLLING_MODE == STD_ON)
        const Ea_VoidFunctionVoidPtr EA_InterServFunc[] = {
            NULL_PTR,           /* EA_JOB_IDLE */
            Ea_InterRead,       /* EA_JOB_READ */
            Ea_InterWrite,      /* EA_JOB_WRITE */
            Ea_InterCancel,     /* EA_JOB_CANCEL */
            Ea_InterInvalidate, /* EA_JOB_INVALIDATE */
            Ea_InterErase,      /* EA_JOB_ERASE */
        };
#endif /* EA_POLLING_MODE == STD_ON */
        (*EA_InterServFunc[Ea_RunTimeData.EaRequestJob])();
    }
}

#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/******************************************************************************/
/*
 * Brief               get the block BlockNumber index.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start
 *                     address of that block in EEPROM.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              uint16
 */
/******************************************************************************/
static FUNC(uint16, EA_PUBLIC_CODE) Ea_GetBlockIndex(uint16 BlockNumber)
{
    uint16 ret = EA_BLOCKNUM_INVALID;
    uint16 iloop;
    if ((0u != BlockNumber) && (EA_BLOCKNUM_INVALID != BlockNumber))
    {
        for (iloop = 0; iloop < Ea_Config.Ea_BlockCfg->BlockNum; iloop++)
        {
            if (BlockNumber == Ea_Config.Ea_BlockCfg->BlockCfg[iloop].EaBlockNumber)
            {
                ret = iloop;
                break;
            }
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               job finish, set runtime data
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      JobResult
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              uint16
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_JobFinish(MemIf_JobResultType JobResult)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;

    switch (JobResult)
    {
    case MEMIF_JOB_OK:
        switch (Ea_RunTimeData.EaRequestJob)
        {
        case EA_JOB_READ:
        case EA_JOB_WRITE:
            EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_JOB_OK))
            break;
        case EA_JOB_ERASE:
            EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INCONSISTENT))
            break;
        case EA_JOB_INVALIDATE:
            EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INVALID))
            break;
        default:
            /*never happen*/
            break;
        }
        Ea_CallJobEndNotification();
        break;
    case MEMIF_JOB_FAILED:
        EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_JOB_FAILED))
        Ea_CallJobErrorNotification();
        break;
    case MEMIF_BLOCK_INCONSISTENT:
        EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INCONSISTENT))
        Ea_CallJobErrorNotification();
        break;
    case MEMIF_BLOCK_INVALID:
        EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INVALID))
        Ea_CallJobErrorNotification();
        break;
    case MEMIF_JOB_CANCELED:
        if (Ea_RunTimeData.EaRequestJob != EA_JOB_READ)
        {
            EA_SET_BLOCK_INFO((blockIndex), (FALSE), (MEMIF_BLOCK_INCONSISTENT))
        }
        break;
    default:
        /*never happen*/
        break;
    }

    Ea_RunTimeData.EaLastJobResult = JobResult;
    Ea_RunTimeData.EaRequestJob = EA_JOB_IDLE;
    Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_IDLE;
    Ea_RunTimeData.EaEepState = EA_EEP_JOB_IDLE;
    Ea_RunTimeData.EaPendReq.BlockNumber = 0;
    Ea_RunTimeData.EaPendReq.BlockOffset = 0;
    Ea_RunTimeData.EaPendReq.BlockIndex = EA_BLOCKNUM_INVALID;
    Ea_RunTimeData.EaPendReq.Length = 0;
    Ea_RunTimeData.EaPendReq.DealOffset = 0;
    Ea_RunTimeData.EaPendReq.DealSize = 0;
    Ea_RunTimeData.EaPendReq.DataBufferPtr = NULL_PTR;
    Ea_RunTimeData.EaPendReq.DataWriteBufferPtr = NULL_PTR;
    Ea_RunTimeData.EaPendReq.InterDataBufferPtr = NULL_PTR;

    Ea_State = MEMIF_IDLE;
}

/******************************************************************************/
/*
 * Brief               Service to report to this module the successful
 *                      end of an asynchronous operation.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_CallJobEndNotification(void)
{
    if (Ea_Config.NvmJobEndNotification != NULL_PTR)
    {
        Ea_Config.NvmJobEndNotification();
    }
}

/******************************************************************************/
/*
 * Brief               Service to report to this module the failure of an asynchronous operation.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_CallJobErrorNotification(void)
{
    if (Ea_Config.NvmJobErrorNotification != NULL_PTR)
    {
        Ea_Config.NvmJobErrorNotification();
    }
}

/******************************************************************************/
/*
 * Brief               Service to read
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterRead(void)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint8 EaDeviceIndex = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaDeviceIndex;
    MemIf_JobResultType ret;

    if ((TRUE == Ea_BlockInfo[blockIndex].IsOperated) && (MEMIF_JOB_OK != Ea_BlockInfo[blockIndex].BlockVaild))
    {
        Ea_JobFinish(Ea_BlockInfo[blockIndex].BlockVaild);
    }
    else
    {
        if (EA_EEP_JOB_IDLE == Ea_RunTimeData.EaEepState)
        {
            /*first to read header*/
            if (EA_SUB_JOB_IDLE == Ea_RunTimeData.EaSubJobStatus)
            {
                Ea_RunTimeData.EaPendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
                Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_READ_HEADER;
                Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
            }
            /*second to read data*/
            else if (EA_SUB_JOB_READ_HEADER_OK == Ea_RunTimeData.EaSubJobStatus)
            {
                Ea_RunTimeData.EaPendReq.DealOffset = EA_HEADER_ALIGNMENT_LENGTH;
                Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_READ_DATA;
                if (Ea_RunTimeData.EaPendReq.Length > (EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH))
                {
                    Ea_RunTimeData.EaPendReq.DealSize =
                        Ea_RunTimeData.EaPendReq.Length - (EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH);
                    Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
                }
                else
                {
                    /* data is already read out */
                    Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_PENDING;
                }
            }
            else
            {
                /*idle*/
            }
        }
        Ea_MemAccReq(EA_JOB_READ);
        /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
        if ((EA_EEP_JOB_ASYNC_PENDING == Ea_RunTimeData.EaEepState)
            && (MEMIF_IDLE == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetStatusApi()))
        /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
        {
            ret = Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetJobResultApi();
            switch (ret)
            {
            case MEMIF_JOB_OK:
                if (EA_SUB_JOB_READ_HEADER == Ea_RunTimeData.EaSubJobStatus)
                {
                    /* check header */
                    Ea_InterCheckReadHeader();
                }
                else if (EA_SUB_JOB_READ_DATA == Ea_RunTimeData.EaSubJobStatus)
                {
                    /* copy data */
                    Ea_InterCopyReadData();
                    Ea_JobFinish(MEMIF_JOB_OK);
                }
                else
                {
                    /*idle,never happen*/
                }
                break;
            case MEMIF_BLOCK_INCONSISTENT:
                Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
                break;
            case MEMIF_JOB_CANCELED:
                Ea_JobFinish(MEMIF_JOB_CANCELED);
                break;
            default:
                Ea_JobFinish(MEMIF_JOB_FAILED);
                break;
            }
            Ea_RunTimeData.EaEepState = EA_EEP_JOB_IDLE;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Service to write
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterWrite(void)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint8 EaDeviceIndex = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaDeviceIndex;
    MemIf_JobResultType ret;
    uint16 iloop;
    uint16 size;
    const uint8* src;
    uint8* Dest;

    if (EA_EEP_JOB_IDLE == Ea_RunTimeData.EaEepState)
    {
        /*first to write invalid header*/
        if (EA_SUB_JOB_IDLE == Ea_RunTimeData.EaSubJobStatus)
        {
            Ea_InterPrepareWriteHeader(EA_BLOCK_STATUS_INCONSISTENT);
            Ea_RunTimeData.EaPendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
            Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_INVALID_HEADER;
        }
        /*second to write data*/
        else if (EA_SUB_JOB_INVALID_HEADER_OK == Ea_RunTimeData.EaSubJobStatus)
        {
            Ea_RunTimeData.EaPendReq.DealOffset = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.EaPendReq.DealSize =
                Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize - EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_WRITE_DATA;
            Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
            size = Ea_RunTimeData.EaPendReq.DealSize;
            Dest = &Ea_RunTimeData.EaPendReq.InterDataBufferPtr[EA_HEADER_ALIGNMENT_LENGTH];
            src = &Ea_RunTimeData.EaPendReq.DataWriteBufferPtr[EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH];
            for (iloop = 0; iloop < size; iloop++)
            {
                Dest[iloop] = src[iloop];
            }
        }
        /*third to write valid header*/
        else if (EA_SUB_JOB_WRITE_DATA_OK == Ea_RunTimeData.EaSubJobStatus)
        {
            Ea_InterPrepareWriteHeader(EA_BLOCK_STATUS_CONSISTENT);
            Ea_RunTimeData.EaPendReq.DealOffset = 0;
            Ea_RunTimeData.EaPendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
            Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_WRITE_HEADER_VALID;
        }
        else
        {
            /*idle*/
        }
    }
    Ea_MemAccReq(EA_JOB_WRITE);
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if ((EA_EEP_JOB_ASYNC_PENDING == Ea_RunTimeData.EaEepState)
        && (MEMIF_IDLE == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetStatusApi()))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        ret = Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetJobResultApi();
        switch (ret)
        {
        case MEMIF_JOB_OK:
            if (EA_SUB_JOB_INVALID_HEADER == Ea_RunTimeData.EaSubJobStatus)
            {
                /* write invalid header ok */
                Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_INVALID_HEADER_OK;
            }
            else if (EA_SUB_JOB_WRITE_DATA == Ea_RunTimeData.EaSubJobStatus)
            {
                /* write data ok */
                Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_WRITE_DATA_OK;
            }
            else if (EA_SUB_JOB_WRITE_HEADER_VALID == Ea_RunTimeData.EaSubJobStatus)
            {
                /* write valid header ok */
                Ea_JobFinish(MEMIF_JOB_OK);
            }
            else
            {
                /*idle,never happen*/
            }
            break;
        case MEMIF_BLOCK_INCONSISTENT:
            Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
            break;
        case MEMIF_JOB_CANCELED:
            Ea_JobFinish(MEMIF_JOB_CANCELED);
            break;
        default:
            Ea_JobFinish(MEMIF_JOB_FAILED);
            break;
        }
        Ea_RunTimeData.EaEepState = EA_EEP_JOB_IDLE;
    }
}

/******************************************************************************/
/*
 * Brief               Service to invalidate
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterInvalidate(void)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint8 EaDeviceIndex = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaDeviceIndex;
    MemIf_JobResultType ret;

    if (EA_EEP_JOB_IDLE == Ea_RunTimeData.EaEepState)
    {
        Ea_InterPrepareWriteHeader(EA_BLOCK_STATUS_INVALID);
        Ea_RunTimeData.EaPendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
        Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
    }
    Ea_MemAccReq(EA_JOB_INVALIDATE);
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if ((EA_EEP_JOB_ASYNC_PENDING == Ea_RunTimeData.EaEepState)
        && (MEMIF_IDLE == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetStatusApi()))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        ret = Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetJobResultApi();
        switch (ret)
        {
        case MEMIF_JOB_OK:
            Ea_JobFinish(MEMIF_JOB_OK);
            break;
        case MEMIF_BLOCK_INCONSISTENT:
            Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
            break;
        case MEMIF_JOB_CANCELED:
            Ea_JobFinish(MEMIF_JOB_CANCELED);
            break;
        default:
            Ea_JobFinish(MEMIF_JOB_FAILED);
            break;
        }
        Ea_RunTimeData.EaEepState = EA_EEP_JOB_IDLE;
    }
}

/******************************************************************************/
/*
 * Brief               Service to erase
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterErase(void)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint8 EaDeviceIndex = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaDeviceIndex;
    MemIf_JobResultType ret;

    if (EA_EEP_JOB_IDLE == Ea_RunTimeData.EaEepState)
    {
        Ea_RunTimeData.EaPendReq.DealSize = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize;
        Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_READY;
    }
    Ea_MemAccReq(EA_JOB_ERASE);
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if ((EA_EEP_JOB_ASYNC_PENDING == Ea_RunTimeData.EaEepState)
        && (MEMIF_IDLE == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetStatusApi()))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        ret = Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetJobResultApi();
        switch (ret)
        {
        case MEMIF_JOB_OK:
            Ea_JobFinish(MEMIF_JOB_OK);
            break;
        case MEMIF_BLOCK_INCONSISTENT:
            Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
            break;
        case MEMIF_JOB_CANCELED:
            Ea_JobFinish(MEMIF_JOB_CANCELED);
            break;
        default:
            Ea_JobFinish(MEMIF_JOB_FAILED);
            break;
        }
        Ea_RunTimeData.EaEepState = EA_EEP_JOB_IDLE;
    }
}

/******************************************************************************/
/*
 * Brief               read job finish, copy data to upper buffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterCopyReadData(void)
{
    uint16 iloop;
    uint16 size = Ea_RunTimeData.EaPendReq.Length;
    const uint8* src = &Ea_RunTimeData.EaPendReq.InterDataBufferPtr[EA_TOTAL_HEADER_LENGTH];
    uint8* Dest = Ea_RunTimeData.EaPendReq.DataBufferPtr;
    for (iloop = 0; iloop < size; iloop++)
    {
        Dest[iloop] = src[iloop];
    }
}

/******************************************************************************/
/*
 * Brief               read job finish, check read header
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterCheckReadHeader(void)
{
    /*  EA block header info
     *  byte0 - byte1: BlockNumber (uint16)
     *  byte2 - byte3: BlockSize (uint16)
     *  byte4 - byte7: BlockWriteCycle (uint32)
     *  byte8 - byte9: BlockValid  0xA5 0xA5 means valid; 0x5A 0x5A means invalid;
     *                   others means inconsistent
     *  byte10 - byte13  BlockStartAddress (uint32)
     *
     *  total header size: 14byte
     * */

    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint16 EaBlockNumber = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockNumber;
    uint16 EaBlockSize = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize;
    Eep_AddressType EaBlockStartAddress = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].BlockStartAddress;

    const uint8* src = Ea_RunTimeData.EaPendReq.InterDataBufferPtr;
    uint16 BlockNumber;
    uint16 BlockSize;
    uint32 BlockWriteCycle;
    uint16 BlockValid;
    Eep_AddressType BlockStartAddress;

    EA_DESERIALIZE((src), (BlockNumber), uint16)
    EA_DESERIALIZE((src), (BlockSize), uint16)
    EA_DESERIALIZE((src), (BlockWriteCycle), uint32)
    EA_DESERIALIZE((src), (BlockValid), uint16)
    EA_DESERIALIZE((src), (BlockStartAddress), Eep_AddressType) /* PRQA S 2983 */ /* MISRA Rule 2.2 */

    if (EA_BLOCK_INVALID == BlockValid)
    {
        Ea_JobFinish(MEMIF_BLOCK_INVALID);
    }
    else if ((EaBlockNumber != BlockNumber) || (EaBlockSize != BlockSize) || (EaBlockStartAddress != BlockStartAddress))
    {
        Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
    }
    else if (EA_BLOCK_VALID == BlockValid)
    {
        Ea_BlockInfo[blockIndex].BlockWriteCycle = BlockWriteCycle;
        Ea_RunTimeData.EaSubJobStatus = EA_SUB_JOB_READ_HEADER_OK;
    }
    else
    {
        Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
    }
}

/******************************************************************************/
/*
 * Brief               write or invalidate job process, prepare write header
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Valid:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterPrepareWriteHeader(uint8 status)
{
    /*  EA block header info
     *  byte0 - byte1: BlockNumber (uint16)
     *  byte2 - byte3: BlockSize (uint16)
     *  byte4 - byte7: BlockWriteCycle (uint32)
     *  byte8 - byte9: BlockVaild  0xA5 0xA5 means valid; 0x5A 0x5A means invalid;
     *                  others means inconsistent
     *  byte10 - byte13BlockStartAddress (uint32)
     *
     *  total header size: 14byte
     * */
    uint16 iloop;
    uint8* Dest = Ea_RunTimeData.EaPendReq.InterDataBufferPtr;
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    const uint8* src;

    uint16 BlockNumber = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockNumber;
    uint16 BlockSize = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaBlockSize;
    uint32 BlockWriteCycle = Ea_BlockInfo[blockIndex].BlockWriteCycle;
    uint16 BlockVaild = EA_BLOCK_VALID;
    Eep_AddressType BlockStartAddress = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].BlockStartAddress;

    if (EA_BLOCK_STATUS_INVALID == status)
    {
        BlockVaild = EA_BLOCK_INVALID;
    }
    if (EA_BLOCK_STATUS_INCONSISTENT == status)
    {
        BlockSize = 0;
    }
    EA_SERIALIZE((BlockNumber), (Dest), uint16)
    EA_SERIALIZE((BlockSize), (Dest), uint16)
    EA_SERIALIZE((BlockWriteCycle), (Dest), uint32)
    EA_SERIALIZE((BlockVaild), (Dest), uint16)
    EA_SERIALIZE((BlockStartAddress), (Dest), Eep_AddressType)

    /* if write job process, copy some write data to buffer*/
    if (NULL_PTR != Ea_RunTimeData.EaPendReq.DataWriteBufferPtr)
    {
        src = Ea_RunTimeData.EaPendReq.DataWriteBufferPtr;
        for (iloop = 0; iloop < (EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH); iloop++)
        {
            Dest[iloop] = src[iloop];
        }
    }
}

/******************************************************************************/
/*
 * Brief               Service to cancel
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_InterCancel(void)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint8 EaDeviceIndex = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaDeviceIndex;

    if (MEMIF_JOB_PENDING == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetJobResultApi())
    {
        Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_CancelApi();
    }
    Ea_JobFinish(MEMIF_JOB_CANCELED);
}

/******************************************************************************/
/*
 * Brief               Service to call MemAcc Api
 * ServiceId           None
 * Sync/Async          Non Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
static FUNC(void, EA_PUBLIC_CODE) Ea_MemAccReq(Ea_RequestJobType Ea_RequestJob)
{
    uint16 blockIndex = Ea_RunTimeData.EaPendReq.BlockIndex;
    uint8* DataBuffer = &Ea_RunTimeData.EaPendReq.InterDataBufferPtr[Ea_RunTimeData.EaPendReq.DealOffset];
    Eep_LengthType DealLength = (Eep_LengthType)Ea_RunTimeData.EaPendReq.DealSize;
    Eep_AddressType address = (Eep_AddressType)(Ea_RunTimeData.EaPendReq.DealOffset
                                                + Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].BlockStartAddress);
    uint8 EaDeviceIndex = Ea_Config.Ea_BlockCfg->BlockCfg[blockIndex].EaDeviceIndex;

    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if ((EA_EEP_JOB_ASYNC_READY == Ea_RunTimeData.EaEepState)
        && (MEMIF_IDLE == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_GetStatusApi()))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        switch (Ea_RequestJob)
        {
        case EA_JOB_READ:
            if ((Std_ReturnType)E_OK == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_ReadApi(address, DataBuffer, DealLength))
            {
                Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_PENDING;
            }
            else
            {
                Ea_JobFinish(MEMIF_JOB_FAILED);
            }
            break;
        case EA_JOB_WRITE:
        case EA_JOB_INVALIDATE:
            if ((Std_ReturnType)E_OK
                == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_WriteApi(address, DataBuffer, DealLength))
            {
                Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_PENDING;
            }
            else
            {
                Ea_JobFinish(MEMIF_JOB_FAILED);
            }
            break;
        case EA_JOB_ERASE:
            if ((Std_ReturnType)E_OK == Ea_Config.Ea_EepApi[EaDeviceIndex].Eep_EraseApi(address, DealLength))
            {
                Ea_RunTimeData.EaEepState = EA_EEP_JOB_ASYNC_PENDING;
            }
            else
            {
                Ea_JobFinish(MEMIF_JOB_FAILED);
            }
            break;
        default:
            /*idle*/
            break;
        }
    }
}

#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
