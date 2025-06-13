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
**  FILENAME    : Ea.h                                                        **
**                                                                            **
**  Created on  : 2022/02/15                                                  **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Ea header description                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2022-04-15     tao.yu         initial version.
 *  V1.0.1     2022-05-13     tao.yu         code review.
 *  V2.0.0     2022-12-13     tao.yu         update version info
 *  V2.0.1     2023-02-16     tao.yu         fix PRD0042022-619/PRD0042022-610/PRD0042022-631
 *  V2.0.2     2023-05-09     tao.yu         fix some bugs and update version info
 *  V2.0.3     2024-01-22     tao.yu         QAC
 *  V2.0.4     2024-05-14     haibin.shao    adj Det restriction and Storage algorithm
 *             2024-06-15     tao.yu         update memmap

 */
#ifndef EA_H
#define EA_H

/*==============================================================================
*                             INCLUDE FILES
==============================================================================*/
#include "Ea_Types.h"
#include "MemIf_Types.h"

/*==============================================================================
*                              SOURCE FILE VERSION INFORMATION
==============================================================================*/
#define EA_VENDOR_ID                62U
#define EA_MODULE_ID                40U
#define EA_INSTANCE_ID              0U
#define EA_AR_RELEASE_MAJOR_VERSION 4U
#define EA_AR_RELEASE_MINOR_VERSION 5U
#define EA_AR_RELEASE_PATCH_VERSION 0U
#define EA_SW_MAJOR_VERSION         2U
#define EA_SW_MINOR_VERSION         0U
#define EA_SW_PATCH_VERSION         4U

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(Ea_ConfigType, EA_CONST) Ea_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
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
extern FUNC(void, EA_PUBLIC_CODE) Ea_Init(P2CONST(Ea_ConfigType, EA_VAR, EA_CONST) ConfigPtr);

#if (EA_SETMODE_API == STD_ON)
/******************************************************************************/
/*
 * Brief               Function to switch the mode of the underlying EEPROM Driver.
 * ServiceId           0x01
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Mode: Desired mode for the underlying EEPROM driver
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None :
 */
/******************************************************************************/
extern FUNC(void, EA_PUBLIC_CODE) Ea_SetMode(MemIf_ModeType Mode);
#endif

/******************************************************************************/
/*
 * Brief               Reads Length bytes of block BlockNumber at offset BlockOffset into the buffer DataBufferPtr.
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
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
extern FUNC(Std_ReturnType, EA_PUBLIC_CODE)
    Ea_Read(uint16 BlockNumber, uint16 BlockOffset, P2VAR(uint8, EA_VAR, EA_VAR) DataBufferPtr, uint16 Length);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/******************************************************************************/
/*
 * Brief               Writes the contents of the DataBufferPtr to the block BlockNumber.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
                       DataBufferPtr: Pointer to data buffer
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: Returns if requested job has been accepted by the EA module.
                       E_OK: The requested job has been accepted by the module.
                       E_NOT_OK: The requested job has not been accepted by the EA module.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, EA_PUBLIC_CODE)
    Ea_Write(uint16 BlockNumber, P2CONST(uint8, EA_VAR, EA_CONST) DataBufferPtr);

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
extern FUNC(void, EA_PUBLIC_CODE) Ea_Cancel(void);

/******************************************************************************/
/*
 * Brief               Service to return the status.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_StatusType: MEMIF_UNINIT: The EA module has not been initialized (yet).
                       MEMIF_IDLE: The EA module is currently idle.
                       MEMIF_BUSY: The EA module is currently busy.
                       MEMIF_BUSY_INTERNAL: The EA module is currently busy with internal management operations.
 */
/******************************************************************************/
extern FUNC(MemIf_StatusType, EA_PUBLIC_CODE) Ea_GetStatus(void);

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
 * Return              MemIf_JobResultType: MEMIF_JOB_OK: The last job has been finished successfully.
                       MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed.
                       MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed).
                       MEMIF_JOB_FAILED: The last job was not finished successfully (it failed).
                       MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data.
                       MEMIF_BLOCK_INVALID: The requested block has been invalidated, the requested operation can not be
 performed.
 */
/******************************************************************************/
extern FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) Ea_GetJobResult(void);

/******************************************************************************/
/*
 * Brief               Invalidates the block BlockNumber.
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * pre                 EA component status is MEMIF_IDLE.
 * Return              Std_ReturnType: E_OK: The requested job has been accepted by the module.
                       E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_InvalidateBlock(uint16 BlockNumber);

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
extern FUNC(void, EA_PUBLIC_CODE) Ea_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, EA_APPL_DATA) VersionInfoPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

/******************************************************************************/
/*
 * Brief               Erases the block BlockNumber.
 * ServiceId           0x09
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType: E_OK: The requested job has been accepted by the module.
                       E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, EA_PUBLIC_CODE) Ea_EraseImmediateBlock(uint16 BlockNumber);

/******************************************************************************/
/*
 * Brief               Service to report to this module the successful end of an asynchronous operation.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
extern FUNC(void, EA_PUBLIC_CODE) Ea_JobEndNotification(void);

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
extern FUNC(void, EA_PUBLIC_CODE) Ea_JobErrorNotification(void);

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
extern FUNC(void, EA_PUBLIC_CODE) Ea_MainFunction(void);

#endif /* EA_H */
