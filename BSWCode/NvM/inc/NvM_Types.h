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
 **  @file               : NvM_Types.h
 **  @author             : peng.wu
 **  @date               : 2024/08/27
 **  @vendor             : isoft
 **  @description        : NVRAM manager type definitions
 **
 ***********************************************************************************************************************/
#ifndef NVM_TYPES_H
#define NVM_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Rte_NvM_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/** Defines the used api for different class */
#define NVM_API_CONFIG_CLASS_1 1U /**< All API calls belonging to class 1 */
#define NVM_API_CONFIG_CLASS_2 2U /**< All API calls belonging to class 2 */
#define NVM_API_CONFIG_CLASS_3 3U /**< All API calls belonging to class 3 */

/** Defines used as parameters for the Development Errors */
#define NVM_E_NO_ERROR             (0x0u)  /**< API function called with no det error */
#define NVM_E_PARAM_BLOCK_ID       (0x0Au) /**< API is called with wrong parameter block ID */
#define NVM_E_PARAM_BLOCK_DATA_IDX (0x0Cu) /**< API is called with wrong parameter block data */
#define NVM_E_PARAM_ADDRESS        (0x0Du) /**< API is called with wrong parameter address */
#define NVM_E_PARAM_DATA           (0x0Eu) /**< API is called with wrong parameter data */
#define NVM_E_PARAM_POINTER        (0x0Fu) /**< API is called with wrong parameter pointer */
#define NVM_E_BLOCK_WITHOUT_DEFAULTS                                                                           \
    (0x11u) /**< API is called for a block without defaults when either the NvM_RestoreBlockDeafults or NvM_   \
               RestorePRAMBlockDefaults is called for a valid block ID that has no default data and no NvMInit \
               BlockCallback configured for the block */
#define NVM_E_UNINIT (0x14u) /**< API is called when NVRAM manager is not initialized yet */
#define NVM_E_BLOCK_PENDING \
    (0x15u) /**< read/write/control API is called for a block which is already listed or in progress */
#define NVM_E_BLOCK_CONFIG (0x18u) /**< Service is not possible with this block configuration */
#define NVM_E_BLOCK_LOCKED (0x19u) /**< The block is locked */
#define NVM_E_WRITE_ONCE_STATUS_UNKNOWN                                                                               \
    (0x1Au) /**< write/erase/invalidate API is called for a block with MVM_WRITE_BLOCK_ONCE (TRUE) prior to the first \
               read request for that block */
#define NVM_E_BLOCK_CHIPHER_LENGTH_MISSMATCH                                                                  \
    (0x1Bu) /**< The length resulting from encryption or decription do not match with the given length in the \
               configuration */
#define NVM_E_WRITE_PROTECTED_DET                                                                                  \
    (0x1Cu) /**< NVM_E_BLOCK_CHIPHER_LENGTH_MISSMATCH and NVM_E_WRITE_PROTECTED(_DET) value is conflict in AUTOSAR \
               R22-11, so change NVM_E_WRITE_PROTECTED(_DET) to 0x1C */
#define NVM_E_WRONG_CONDITION (0x1Du) /**< partition info error  */

/** Defines used as parameters for the Runtime Errors */
#define NVM_E_QUEUE_FULL \
    (0xA0u) /**< NvM queue is full so the request cannot bequeued, be the request either standard or immediate */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Defines CRC data width for the NVRAM block.
 */
typedef enum
{
    NVM_CRC_NOT_USED, /**< not use crc @range 0x00*/
    NVM_CRC8,         /**< Crc8 @range 0x01*/
    NVM_CRC16,        /**< Crc16 @range 0x02*/
    NVM_CRC32         /**< Crc32 @range 0x03*/
} NvM_BlockCRCType;

/**
 * @brief     Defines the block management type for the NVRAM block
 */
typedef enum
{
    NVM_BLOCK_NATIVE,    /**< Native block type @range 0x00*/
    NVM_BLOCK_REDUNDANT, /**< Redundant block type @range 0x01*/
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NVM_BLOCK_DATASET /**< Dataset block type @range 0x02*/
#endif
} NvM_BlockManagementType;

/**
 * @brief     Type of multitasking request
 */
typedef enum
{
    NVM_READ_ALL,        /**< NvM_ReadAll was performed @range 0x00*/
    NVM_WRITE_ALL,       /**< NvM_WriteAll was performed @range 0x01*/
    NVM_VALIDATE_ALL,    /**< NvM_ValidateAll was performed @range 0x02*/
    NVM_FIRST_INIT_ALL,  /**< NvM_FirstInitAll was performed @range 0x03*/
    NVM_CANCEL_WRITE_ALL /**< NvM_CancelWriteAll was performed @range 0x04*/
} NvM_MultiBlockRequestType;

/**
 * @brief       A pointer to initial callback function
 * @param[in]   InitBlockRequest: Block request type during initial
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76731
 */
/* PRQA S 5004 ++ */ /* VL_NvM_5004 */
typedef Std_ReturnType (*NvM_InitBlockCallbackType)(NvM_InitBlockRequestType InitBlockRequest);

/**
 * @brief       A pointer to single block callback function
 * @param[in]   BlockRequest: Block request type
 * @param[in]   JobResult: Block request result
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76732
 */
typedef Std_ReturnType (
    *NvM_SingleBlockCallbackType)(NvM_BlockRequestType BlockRequest, NvM_RequestResultType JobResult);

/**
 * @brief       A pointer to copy ram to ram mirror read callback function
 * @param[in]   NvMBuffer: Pointer to read buffer
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76733
 */
typedef Std_ReturnType (*NvM_ReadRamBlockFromNvmCallbackType)(const void* NvMBuffer);

/**
 * @brief       A pointer to copy ram to ram mirror write callback function
 * @param[in]   NvMBuffer: Pointer to write buffer
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76734
 */
typedef Std_ReturnType (*NvM_WriteRamBlockToNvmCallbackType)(void* NvMBuffer);

/**
 * @brief       A pointer to multi block callback function
 * @param[in]   MultiBlockRequest: Multi block request type
 * @param[in]   JobResult: Block request result
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76735
 */
typedef void (
    *NvM_MultiBlockCallbackType)(NvM_MultiBlockRequestType MultiBlockRequest, NvM_RequestResultType JobResult);
/* PRQA S 5004 -- */

/**
 * @brief     Service Type Indicates the Id of a function service
 */
typedef enum
{
    NVM_INIT_SERV_ID                        = 0,  /**< Init Api Id @range 0x00*/
    NVM_SET_DATA_INDEX_SERV_ID              = 1,  /**< SetDataIndex Api Id @range 0x01*/
    NVM_GET_DATA_INDEX_SERV_ID              = 2,  /**< GetDataIndex Api Id @range 0x02*/
    NVM_SET_BLOCK_PROTECTION_SERV_ID        = 3,  /**< SetBlockProtection Api Id @range 0x03*/
    NVM_GET_ERROR_STATUS_SERV_ID            = 4,  /**< GetErrorStatus Api Id @range 0x04*/
    NVM_SET_RAM_BLOCK_STATUS_SERV_ID        = 5,  /**< SetRamBlockStatus Api Id @range 0x05*/
    NVM_READ_BLOCK_SERV_ID                  = 6,  /**< ReadBlock Api Id @range 0x06*/
    NVM_WRITE_BLOCK_SERV_ID                 = 7,  /**< WriteBlock Api Id @range 0x07*/
    NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID      = 8,  /**< RestoreBlockDefaults Api Id @range 0x08*/
    NVM_ERASE_NV_BLOCK_SERV_ID              = 9,  /**< EraseNvBlock Api Id @range 0x09*/
    NVM_CANCEL_WRITE_ALL_SERV_ID            = 10, /**< CancelWriteAll Api Id @range 0x0A*/
    NVM_INVALIDATE_NV_BLOCK_SERV_ID         = 11, /**< InvalidateNvBlock Api Id @range 0x0B*/
    NVM_READ_ALL_SERV_ID                    = 12, /**< ReadAll Api Id @range 0x0C*/
    NVM_WRITE_ALL_SERV_ID                   = 13, /**< WriteAll Api Id @range 0x0D*/
    NVM_GET_VERSIONINFO_ID                  = 15, /**< NvM_GetVersionInfo Api Id @range 0x0E*/
    NVM_CANCEL_JOBS_SERV_ID                 = 16, /**< CancelJobs Api Id @range 0x0F*/
    NVM_FIRSTINIT_ALL_SERV_ID               = 20, /**< FirstInitAll Api Id @range 0x10*/
    NVM_READ_PRAM_BLOCK_SERV_ID             = 22, /**< ReadPRAMBlock Api Id @range 0x11*/
    NVM_WRITE_PRAM_BLOCK_SERV_ID            = 23, /**< WritePRAMBlock Api Id @range 0x12*/
    NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID = 24, /**< RestorePRAMBlockDefaults Api Id @range 0x13*/
    NVM_VALIDATE_ALL_SERV_ID                = 25, /**< ValidateAll Api Id @range 0x14*/
    NVM_NONE_SERV_ID                        = 26  /**< No service @range 0x15*/
} NvM_ServiceIdType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
