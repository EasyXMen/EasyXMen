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
**  FILENAME    : Fee_InternalTypes.h                                         **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Internal Type definitions of FEE                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef FEE_INTERNALTYPES_H
#define FEE_INTERNALTYPES_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      STRUCTURES AND OTHER TYPEDEFS                         **
*******************************************************************************/
typedef enum
{
    FEE_BLOCK_VALID = 0u,     /*Fee block is valid*/
    FEE_BLOCK_INVALID,        /*Fee block is invalid*/
    FEE_BLOCK_INCONSISTENT,   /*Fee block is inconsistent*/
    FEE_BLOCK_HEADER_INVALID, /*Fee block header is wrong*/
    FEE_BLOCK_HEADER_BLANK    /*Fee block header is blank*/
} Fee_BlockStatusType;

typedef enum
{
    FEE_BANK_VALID = 0u, /*Fee bank is valid*/
    FEE_BANK_INVALID     /*Fee bank is invalid*/
} Fee_BankStatusType;

typedef enum
{
    /*initial job*/
    FEE_JOB_INT_SCAN = 0u,
    /*Fee_ReadBlock*/
    FEE_JOB_READ,
    /*Fee_WriteBlock*/
    FEE_JOB_WRITE,
    /*Fee_InvalidateBlock*/
    FEE_JOB_INVAL_BLOCK,
    /*Fee_EraseImmediateBlock*/
    FEE_JOB_ERASE_IMMEDIATE,
    /*Internal write job*/
    FEE_JOB_WRITE_DATA,
    FEE_JOB_WRITE_VALIDATE,
    FEE_JOB_WRITE_DONE,
    /*Internal invalid job*/
    FEE_JOB_INVAL_BLOCK_DONE,
    /*Internal erase job*/
    FEE_JOB_ERASE_IMMEDIATE_DONE,
    /*Internal initial job*/
    FEE_JOB_INT_SCAN_BANK_HDR_GET,
    FEE_JOB_INT_SCAN_BANK_CLEAR,
    FEE_JOB_INT_SCAN_BANK_CLEAR_DONE,
    FEE_JOB_INT_SCAN_BLOCK_HDR_GET,
    /*Internal swap job*/
    FEE_JOB_INT_SWAP_BANK_CLEAR,
    FEE_JOB_INT_SWAP_BLOCK,
    FEE_JOB_INT_SWAP_DATA_READ,
    FEE_JOB_INT_SWAP_DATA_WRITE,
    FEE_JOB_INT_SWAP_BLOCK_VLD,
    FEE_JOB_INT_SWAP_BANK_VLD_DONE,
    /*No job*/
    FEE_JOB_NONE
} Fee_JobType;

typedef struct
{
    Fls_AddressType Fee_BlockDataAddr;
    Fls_AddressType Fee_BlockHdrAddr;
    uint32 Fee_BankUseID;
    uint8 Fee_ValidBankId;
    uint8 Fee_BankScanId;
} Fee_BankManageType;

typedef struct
{
    Fls_AddressType Fee_DataAddr;
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    Fls_AddressType Fee_HdrAddr;
#else
    Fls_AddressType Fee_ValidHdrAddr;
#endif
    Fee_BlockStatusType Fee_BlockStatus;
} Fee_BlockManageType;

typedef struct
{
    Fee_JobType Swap_PreJob;
    uint16 Swap_BlockId;
    uint8 Swap_BankId;
    Fls_AddressType Swap_HdrAddr;
    Fls_AddressType Swap_DataAddr;
} Fee_SwapJobManageType;

typedef struct
{
    uint16 Fee_BlockId;
    Fee_JobType Fee_Job;
    MemIf_JobResultType Fee_JobResult;
    P2VAR(uint8, FEE_VAR, FEE_APPL_DATA) Fee_JobReadDataDestPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(uint8, FEE_CONST, FEE_APPL_CONST) Fee_JobWriteDataDestPtr;
    Fls_LengthType Fee_JobBlockOffset;
    Fls_LengthType Fee_JobBlockLength;
    Fee_SwapJobManageType Fee_SwapJobInfo;
} Fee_JobManageType;

#endif /* FEE_INTERNALTYPES_H */
