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
 **  @file               : Fee_InternalTypes.h
 **  @author             : peng.wu
 **  @date               : 2024/09/15
 **  @vendor             : isoft
 **  @description        : Internal Type definitions of FEE
 **
 ***********************************************************************************************************************/
#ifndef FEE_INTERNALTYPES_H
#define FEE_INTERNALTYPES_H

/* =================================================== inclusions =================================================== */
#include "Fee_Cfg.h"
#include "MemIf_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/* the Private Macro Definition */
#define FEE_UNUSED_UINT8  (0xFFu)
#define FEE_UNUSED_UINT16 (0xFFFFu)
#define FEE_UNUSED_UINT32 (0xFFFFFFFFu)
#define FEE_UNUSED_UINT64 (0xFFFFFFFFFFFFFFFFu)

/* Change in special circumstances, all Cluster data can be discarded */
#define FEE_VALID_VALUE   0xAAu
#define FEE_INVALID_VALUE 0x55u

/* ================================================ type definitions ================================================ */
/**
 * @brief     Defines status for the Fee block
 */
typedef enum
{
    FEE_BLOCK_VALID,          /* Fee block is valid */
    FEE_BLOCK_INVALID,        /* Fee block is invalid */
    FEE_BLOCK_INCONSISTENT,   /* Fee block is inconsistent */
    FEE_BLOCK_HEADER_INVALID, /* Fee block header is wrong */
    FEE_BLOCK_HEADER_BLANK    /* Fee block header is blank */
} Fee_BlockStatusType;

/**
 * @brief     Defines status for the Fee Cluster
 */
typedef enum
{
    FEE_CLUSTER_VALID,  /* Fee Cluster is valid */
    FEE_CLUSTER_INVALID /* Fee Cluster is invalid */
} Fee_ClusterStatusType;

/**
 * @brief     Type of Fee job requested
 */
typedef enum
{
    /* initial job */
    FEE_JOB_INT_SCAN,
    /* Fee_ReadBlock */
    FEE_JOB_READ,
    /* Fee_WriteBlock */
    FEE_JOB_WRITE,
    /* Fee_InvalidateBlock */
    FEE_JOB_INVAL_BLOCK,
    /* Fee_EraseImmediateBlock */
    FEE_JOB_ERASE_IMMEDIATE,
    /* Internal write job */
    FEE_JOB_WRITE_DATA,
    FEE_JOB_WRITE_VALIDATE,
    FEE_JOB_WRITE_DONE,
    /* Internal invalid job */
    FEE_JOB_INVAL_BLOCK_DONE,
    /* Internal erase job */
    FEE_JOB_ERASE_IMMEDIATE_DONE,
    /* Internal initial job */
    FEE_JOB_INT_SCAN_CLUSTER_HDR_GET,
    FEE_JOB_INT_SCAN_CLUSTER_CLEAR,
    FEE_JOB_INT_SCAN_CLUSTER_CLEAR_DONE,
    FEE_JOB_INT_SCAN_BLOCK_HDR_GET,
    /* Internal swap job */
    FEE_JOB_INT_SWAP_CLUSTER_CLEAR,
    FEE_JOB_INT_SWAP_BLOCK,
    FEE_JOB_INT_SWAP_DATA_READ,
    FEE_JOB_INT_SWAP_DATA_WRITE,
    FEE_JOB_INT_SWAP_BLOCK_VLD,
    FEE_JOB_INT_SWAP_CLUSTER_VLD_DONE,
    /* No job */
    FEE_JOB_NONE
} Fee_JobType;

/**
 * @brief     Fee Module Cluster managed Information Type
 */
typedef struct
{
    Fls_AddressType Fee_BlockDataAddr;  /* The address to store data */
    Fls_AddressType Fee_BlockHdrAddr;   /* The address to store header info */
    uint32          Fee_ClusterUseID;   /* Currently used Cluster ID */
    uint8           Fee_ValidClusterId; /* The index of the currently used Cluster in the configuration */
} Fee_ClusterManageType;

/**
 * @brief     Fee Module block managed Information Type
 */
typedef struct
{
    Fee_BlockStatusType Fee_BlockStatus; /* Status of the block */
    Fls_AddressType     Fee_DataAddr;    /* Data address */
#if (STD_ON == FEE_PAGE_DIRECT_WRITE_SUPPORT)
    Fls_AddressType Fee_HdrAddr; /* Directly write header address */
#else
    Fls_AddressType Fee_ValidHdrAddr; /* Valid header address */
#endif
} Fee_BlockManageType;

/**
 * @brief     Fee Module swap job managed Information Type
 */
typedef struct
{
    uint8           Swap_ClusterId; /* Cluster Id when swapping */
    Fee_JobType     Swap_PreJob;    /* Store type of original job */
    uint16          Swap_BlockId;   /* Block Id when swapping */
    Fls_AddressType Swap_HdrAddr;   /* Header address when swapping */
    Fls_AddressType Swap_DataAddr;  /* Data address when swapping */
} Fee_SwapJobManageType;

/**
 * @brief     Fee Module job managed Information Type
 */
typedef struct
{
    Fee_JobType           Fee_Job;                 /* The requested job type */
    MemIf_JobResultType   Fee_JobResult;           /* The requested job result */
    uint16                Fee_BlockId;             /* Block ID of the requested job */
    uint8*                Fee_JobReadDataDestPtr;  /* Pointer to the destination address of the read request job */
    const uint8*          Fee_JobWriteDataDestPtr; /* Pointer to the destination address of the write request job */
    Fls_LengthType        Fee_JobBlockOffset;      /* Offset of the current request job */
    Fls_LengthType        Fee_JobBlockLength;      /* Block length of the current request job */
    Fee_SwapJobManageType Fee_SwapJobInfo;         /* Swap information of the Job */
} Fee_JobManageType;

/* PRQA S 5004 ++ */ /* VL_Fee_5004 */
/**
 * @brief     Function Pointer type define, with void parameter and MemIf_JobResultType return value
 */
typedef MemIf_JobResultType (*Fee_JobFuncPtr)(void);

/**
 * @brief     Function Pointer type define, with void parameter and void return value
 */
typedef void (*Fee_VoidFuncVoidPtr)(void);
/* PRQA S 5004 -- */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
