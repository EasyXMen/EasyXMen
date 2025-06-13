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
**  FILENAME    : NvM.c                                                      **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :function declare                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
#ifndef NVM_H
#define NVM_H

/*====================[I N C L U D E S]====================*/
#include "NvM_Types.h"

/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *   V1.0.0     2020/5/9    tao.yu        Create
 *   V2.0.0     2021-4-8    tao.yu      release version
 *   V2.0.1     2023-6-7    peng.wu     Fix CPT-5670:Modify the parameter type
 *   V2.0.2     2023-8-7    tao.yu      Fix CPT-6128:Modify read length when no crc;
 *                                      Change uppercase variable names to lowercase,remove no use include
 *   V2.0.3     2023-8-10   xue.han     QAC Change
 *   V2.0.4     2023-9-8    tao.yu      fix CPT-6831 multiblock result bug when NVM_REQ_BLOCK_SKIPPED
 *   V2.0.5     2023-10-17  tao.yu      Optimize code, add comments
 *              2023-10-23  tao.yu      CPT-7315, Transient error in single block result when recovering from rom
 *              2023-11-7   peng.wu     CPT-7503, Fix the ReadReq problem when NVM_WRITE_BLOCK_ONCE is enabled
 *                                      CPT-7477, Verifies the NvMDatasetSelectionBits in the configuration
 *   V2.0.6     2023-12-25  tao.yu      change SchM
 *              2024-01-13  tao.yu      CPT-8123 Remove precompile condition for NvM_EraseNvBlock
 *   V2.0.7     2024-02-02  xue.han     QAC
 *   V2.0.8     2024-03-12  tao.yu      change Det
 *   V2.0.9     2024-04-17  tao.yu      CPT-8826, fix cancel error, CPT-8830 fix memifinvalid result
 *              2024-04-24  tao.yu      CPT-8895, Remove redundant Det errors
 *                                      CPT-8898, Remove redundant cancel condition
 *                                      CPT-8926, change NvM_RestorePRAMBlockDefaults
 *                                      adapte TI C2xx
 *                                      CPT-8738, avoid warning in NvM_Init
 *                                      update NvM_GetVersionInfo
 *   V2.0.10    2024-04-17  tao.yu      CPT-9089, remove turn off write protection on extend mode
 *   V2.0.11    2024-06-01  tao.yu      CPT-9128, fix Multi Req Result
 *              2024-06-01  tao.yu      CPT-9152, change NvM_MemIfInvalid
 *              2024-06-11  tao.yu      CPT-9207,CPT-9202, fix NvMWriteBlockOnce error in read rom, fix
 *                                      NvMWriteVerification retry not copy new data
 *              2024-06-20  tao.yu      CPT-9219, fix call SingleCallback
 *              2024-06-24  tao.yu      CPT-9295, fix NVM_CANCEL_WRITE_ALL notification
 *              2024-07-10  peng.wu     Fix exclusive zone protection
 *              2024-08-13  peng.wu     Append QAC remarks and remove main function declarations from header files
 *              2024-08-14  tao.yu      implementation deal NvMBlockUseSetRamBlockStatus is off
 *              2024-10-14  tao.yu      CPT-10775, fix call SingleCallback when read
 *                                      CPT-10763, fix Repaire redundant block
 *              2024-10-14  xue.han     QAC
 *   V2.0.12    2025-03-10  peng.wu     Update the external API comment
 *              2025-03-25  peng.wu     CPT-13643,Fix the logic for reading the first block during power-on
============================================================================*/

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#define NVM_VENDOR_ID                62U
#define NVM_MODULE_ID                20U
#define NVM_INSTANCE_ID              0U
#define NVM_AR_RELEASE_MAJOR_VERSION 4U
#define NVM_AR_RELEASE_MINOR_VERSION 5U
#define NVM_AR_RELEASE_PATCH_VERSION 0U
#define NVM_SW_MAJOR_VERSION         2U
#define NVM_SW_MINOR_VERSION         0U
#define NVM_SW_PATCH_VERSION         12U

/*====================[P R O T O T Y P E S]====================*/

extern CONST(NvM_BlockDescriptorType, NVM_CONST) NvM_BlockDescriptor[NVM_BLOCK_NUM_ALL];

extern CONST(NvM_MultiBlockCallbackType, NVM_CONST) NvmMultiBlockCallback;

#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)

extern VAR(uint8, NVM_VAR_NOINIT) NVM_RamMirror[NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR];

#endif

/* PRQA S 3449,3451++ */ /* MISRA Rule 8.5,Rule 8.5*/
/*************************************************************************/
/*
 * Brief               Service for resetting all internal variables
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the selected configuration set
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, NVM_CODE) NvM_Init(const NvM_ConfigType* ConfigPtr);

#if (STD_ON == NVM_VERSION_INFO_API)
/*
 * Brief               This service returns the version information of this module
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Versioninfo: Pointer to where to store the version
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
FUNC(void, NVM_CODE)
NvM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif

/*************************************************************************/
/*
 * Brief               Service to cancel a running NvM_WriteAll request
 * ServiceId           0x0a
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        This request shall only be used by the ECU state manager
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, NVM_CODE) NvM_CancelWriteAll(void);

/*************************************************************************/
/*
 * Brief               Initiates a multi block read request
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        This request may be triggered only by the ECU state manager at system startup
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, NVM_CODE) NvM_ReadAll(void);
/*************************************************************************/
/*
 * Brief               Initiates a multi block write request
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        This request must only be triggered by the ECU state manager at shutdown of the system
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, NVM_CODE) NvM_WriteAll(void);

/*************************************************************************/
/*
 * Brief               Service to copy the data of the permanent RAM block to its corresponding NV block
 * ServiceId           0x17
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                            A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_WritePRAMBlock(NvM_BlockIdType BlockId);

/*************************************************************************/
/*
 * Brief               Service to restore the default data to its corresponding permanent RAM block
 * ServiceId           0x18
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                            A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_RestorePRAMBlockDefaults(NvM_BlockIdType BlockId);

/*************************************************************************/
/*
 * Brief               Service for setting the lock status of a permanent RAM block or of the explicit synchronization
 * of a NVRAM block. ServiceId           0x13 Sync/Async          Synchronous Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                            A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                     BlockLocked: TRUE: Mark the RAM.block as locked FALSE: Mark the RAM.block as unlocked
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, NVM_CODE) NvM_SetBlockLockStatus(NvM_BlockIdType BlockId, boolean BlockLocked);

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/*************************************************************************/
/*
 * Brief               Service for setting the DataIndex of a dataset NVRAM block
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                            A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                     DataIndex: Index position (association) of a NV/ROM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex);

/*************************************************************************/
/*
 * Brief               Service for getting the currently set DataIndex of a dataset NVRAM block
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     DataIndexPtr: Pointer to where to store the current dataset index (0..255)
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, NVM_CODE)
    NvM_GetDataIndex(NvM_BlockIdType BlockId, P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DataIndexPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to copy the data of the NV block to its corresponding permanent RAM block
 * ServiceId           0x16
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ReadPRAMBlock(NvM_BlockIdType BlockId);

/*************************************************************************/
/*
 * Brief               Service to copy the data of the NV block to its corresponding RAM block
 * ServiceId           0x06
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     NvM_DstPtr: Pointer to the RAM data block
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ReadBlock(NvM_BlockIdType BlockId, void* NvM_DstPtr);

/*************************************************************************/
/*
 * Brief               Service to copy the data of the RAM block to its corresponding NV block
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                     NvM_SrcPtr: Pointer to the RAM data block
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_WriteBlock(NvM_BlockIdType BlockId, const void* NvM_SrcPtr);

/*************************************************************************/
/*
 * Brief               Service to restore the default data to its corresponding RAM block
 * ServiceId           0x08
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     NvM_DestPtr: Pointer to the RAM data block
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, void* NvM_DestPtr);

/*************************************************************************/
/*
 * Brief               Service to read the block dependent error/status information
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     RequestResultPtr: Pointer to where to store the request result
 * Param-Name[in/out]  None
 * Return              E_OK: The block dependent error/status information was read successfully
 *                     E_NOT_OK: An error occured
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE)
    NvM_GetErrorStatus(NvM_BlockIdType BlockId, NvM_RequestResultType* RequestResultPtr);

/*************************************************************************/
/*
 * Brief               Service to cancel all jobs pending for a NV block
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The job was successfully removed from queue
 *                     E_NOT_OK: The job could not be found in the queue
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_CancelJobs(NvM_BlockIdType BlockId);

/*************************************************************************/
/*
 * Brief               Initiates a multi block validation request
 * ServiceId           0x19
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(void, NVM_CODE) NvM_ValidateAll(void);

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/*************************************************************************/
/*
 * Brief               Service for setting the RAM block status of an NVRAM block
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                     BlockChanged: TRUE: Validate the RAM block and mark block as changed
 *                                 FALSE: Invalidate the RAM block and mark block as unchanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The status of the permanent RAM block or the explicit synchronization was changed as
 * requested E_NOT_OK: An error occurred PreCondition        None CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged);

#endif
#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/*************************************************************************/
/*
 * Brief               Service for setting/resetting the write protection for a NV block
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                     ProtectionEnabled: TRUE: Write protection shall be enabled
 *                                      FALSE: Write protection shall be disabled
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The block was enabled/disabled as requested
 *                     E_NOT_OK: An error occurred
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* PRQA S 0624++ */ /* MISRA Rule 8.3*/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_SetBlockProtection(NvM_BlockIdType BlockId, boolean ProtectionEnabled);
/* PRQA S 0624-- */ /* MISRA Rule 8.3*/

/*************************************************************************/
/*
 * Brief               Service to invalidate a NV block
 * ServiceId           0x0b
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_InvalidateNvBlock(NvM_BlockIdType BlockId);

/*************************************************************************/
/*
 * Brief               Service to erase a NV block
 * ServiceId           0x09
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                     E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_EraseNvBlock(NvM_BlockIdType BlockId);
#endif
/* PRQA S 3449,3451-- */ /* MISRA Rule 8.5,Rule 8.5*/

#endif /* End of NVM_H*/

/*====================[E N D   O F   F I L E]====================*/
