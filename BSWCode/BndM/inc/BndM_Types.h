
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
 **  @file               : BndM_Types.h
 **  @author             : haibin.shao
 **  @date               : 2025/06/20
 **  @vendor             : isoft
 **  @description        : Declaration and type define for Bulk NvData Manager
 **
 ***********************************************************************************************************************/

#ifndef BNDM_TYPES_H_
#define BNDM_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "MemAcc_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief Used as result of the asynchronous job finish notifications @range 0..255
 */
typedef uint8 BndM_ResultType;
/**
 * @brief Used as unique identification of an bulk nv block; the BndM_BlockId is assigned by the BndM.
 * @range 0..65535
 */
typedef uint16 BndM_BlockIdType;

/**
 * @brief This function pointer type is used to describe the link that raise 'BndM_WriteStart' to Upper layer.
 * @range NA
 */
typedef void (*BndM_WriteStartFinishFct)(BndM_BlockIdType BlockId, BndM_ResultType result);
/**
 * @brief This function pointer type is used to describe the link that raise 'BndM_WriteFinalize' to Upper layer.
 * @range NA
 */
typedef void (*BndM_WriteFinalizeFinishFct)(BndM_BlockIdType BlockId, BndM_ResultType result);
/**
 * @brief This function pointer type is used to describe the link that raise 'BndM_WriteBlock' to Upper layer.
 * @range NA
 */
typedef void (*BndM_WriteBlockFinishFct)(BndM_BlockIdType BlockId, BndM_ResultType result);

/**
 * @brief This type is used to describe the parameter list of bank in BndM
 */
typedef struct BndM_SwapBlockParamListTypeTag
{
    MemAcc_AddressAreaIdType AddressAreaId;   /**< Indicates the address area id of per block @range 0..65535 */
    MemAcc_AddressType BankFirstBaseAddress;  /**< Indicates the base address of the first bank @range 0..4294967295 */
    MemAcc_AddressType BankSecondBaseAddress; /**< Indicates the base address of the second bank @range 0..4294967295 */
    MemAcc_LengthType  TotalSizeOfPerBank;    /**< Indicates the total size of the per bank @range 0..4294967295 */
} BndM_BankParamListType;

/**
 * @brief This type is used to describe the header information of block descriptor in BndM
 */
typedef struct BndM_BlockHeaderInfoTypeTag
{
    uint32             ActivateCode;        /**< Indicates the activate code of block header @range 0..4294967295 */
    MemAcc_AddressType HeaderOffsetAddress; /**< Indicates the offset address of per block @range 0..4294967295 */
    MemAcc_LengthType  HeaderSize;          /**< Indicates the size of per block @range 0..4294967295 */
} BndM_BlockHeaderInfoType;

/**
 * @brief This type is used to describe the parameter list of block descriptor in BndM
 */
typedef struct BndM_BlockDescriptorTypeTag
{
    BndM_BlockIdType   BlockId;            /**< Indicates the unique identification of per block @range 0..65535 */
    MemAcc_AddressType BlockOffsetAddress; /**< Indicates the offset address of per block @range 0..4294967295 */
    MemAcc_LengthType  BlockSize;          /**< Indicates the size of per block @range 0..4294967295 */
    const BndM_WriteBlockFinishFct
        WriteBlockFinish; /**< Indicates the 'WriteBlock' callback function of per block @range NA */
} BndM_BlockDescriptorType;

/**
 * @brief This type is used to describe the block configuration information in BndM
 */
typedef struct BndM_BlockConfigurationTypeTag
{
    const BndM_BlockHeaderInfoType* BlockHeaderInfoPtr; /**< used bank header info configuration of BndM @range NA*/
    uint16                          BlockDescriptorNbr; /**< Indicates the number of block descriptor @range 0..65535 */
    const BndM_BlockDescriptorType* BlockDescriptorPtr; /**< used block configuration of BndM @range NA*/
} BndM_BlockConfigurationType;

/**
 * @brief This type is used to describe the configuration information structure of BndM
 */
typedef struct BndM_ConfigTypeTag
{
    const BndM_BankParamListType*      BankParamListPtr;      /**< used parameter list configuration of Bank @range NA*/
    const BndM_BlockConfigurationType* BlockConfigurationPtr; /**< used block configuration of BndM @range NA*/
    const BndM_WriteStartFinishFct     WriteStartFinish; /**< Indicates the 'WriteStart' callback function @range NA */
    const BndM_WriteFinalizeFinishFct
        WriteFinalizeFinish; /**< Indicates the 'WriteFinalize' callback function @range NA */
} BndM_ConfigType;

#ifdef __cplusplus
}
#endif

#endif /* BNDM_TYPES_H_ */
