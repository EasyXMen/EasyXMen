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
/*********************************************************************************
**                                                                            **
**  FILENAME    : FlsTst_Types.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : peng.wu                                                     **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*  <VERSION>    <DATE>      <AUTHOR>         <REVISION LOG>
 *   V1.0.0     20210916     Huangyin         Initial version
 *   V2.0.0     20221221     Peng.Wu          Modified to version R19-11
 */
/*============================================================================*/
#ifndef FLSTST_TYPES_H
#define FLSTST_TYPES_H

/*============================[I N C L U D E S]===============================*/
#include "Std_Types.h"

typedef uint8 FlsTst_BlockIdType;
typedef uint32 FlsTst_IntervalIDType;
typedef uint32 FlsTst_AddressType;
typedef uint32 FlsTst_LengthType;
typedef uint32 FlsTst_SignatureType;

typedef FlsTst_BlockIdType FlsTst_BlockIdFgndType;

typedef P2FUNC(void, FLSTST_CODE, FlsTst_UserNotificationPtrType)(void);
typedef P2FUNC(Std_ReturnType, FLSTST_CODE, FlsTst_ECCFunctionPtrType)(const uint32*, const uint32*);
typedef P2FUNC(void, FLSTST_CODE, FlsTst_ECCInitPtrType)(void);
typedef P2FUNC(void, FLSTST_CODE, FlsTst_ECCDeInitPtrType)(void);

typedef enum
{
    /* The Flash Test is not initialized or not usable. */
    FLSTST_UNINIT = 0x00u,
    /* The Flash Test is initialized and ready to be started. */
    FLSTST_INIT = 0x01u,
    /* The Flash Test is currently running */
    FLSTST_RUNNING = 0x02u,
    /* The Flash Test is aborted. */
    FLSTST_ABORTED = 0x03u,
    /* The Flash Test is waiting to be resumed or is waiting to start foreground mode test */
    FLSTST_SUSPENDED = 0x04u
} FlsTst_StateType;

typedef enum
{
    /* There is no result availabl. */
    FLSTST_RESULT_NOT_TESTED = 0x00u,
    /* The last Flash Test has been tested with OK result. */
    FLSTST_RESULT_OK = 0x01u,
    /* The last Flash Test has been tested with NOT_OK result. */
    FLSTST_RESULT_NOT_OK = 0x02u,
} FlsTst_TestResultType;

typedef enum
{
    /* There is no result availabl. */
    FLSTST_NOT_TESTED = 0x00u,
    /* The last Flash Test has been tested with OK result. */
    FLSTST_OK = 0x01u,
    /* The last Flash Test has been tested with NOT_OK result. */
    FLSTST_NOT_OK = 0x02u,
} FlsTst_TestResultFgndType;

typedef enum
{
    FLSTST_16BIT_CRC,         /* CRC16 Algorithm */
    FLSTST_32BIT_CRC,         /* CRC32 Algorithm */
    FLSTST_8BIT_CRC,          /* CRC8 Algorithm */
    FLSTST_CHECKSUM,          /* CHECKSUM Algorithm */
    FLSTST_DUPLICATED_MEMORY, /* DUPLICATED_MEMORY Algorithm */
    FLSTST_ECC                /* ECC Algorithm */
} FlsTst_AlgorithmType;

typedef struct
{
    /* The Flash ECCTest Init pointer */
    FlsTst_ECCInitPtrType FlsTst_ECCInitPtr;
    /* The Flash ECCTest Deinit pointer */
    FlsTst_ECCDeInitPtrType FlsTst_ECCDeinitPtr;
} FlsTst_ECCInitType;

typedef struct
{
    /* current value of FlsTstTestIntervalId,which is incremented by each new start of an test interval. */
    FlsTst_IntervalIDType FlsTstIntervalID;
    /* Test Result in background flash test*/
    FlsTst_TestResultType result;
} FlsTst_TestResultBgndType;

/*The range is dependent on the number of Foreground
Flash blocks defined in the configuration structure. The
type shall be chosen depending on the MCU platform for
best performance.*/

typedef struct
{
    FlsTst_BlockIdType ErrorBlockID; /* Record the block ID value when the error occurred */
    FlsTst_AlgorithmType Algorithm;  /* The test algorithm used by the current block */
    uint32 SignatureResult;          /* The signature result of the current block */
} FlsTst_ErrorDetailsType;

typedef struct
{
    uint32 FgndSignature; /* Signature result of foreground test */
} FlsTst_TestSignatureFgndType;

typedef struct
{
    uint32 FlsTstIntervalID; /* current value of FlsTstTestIntervalId */
    uint32 BgndSignature;    /* Signature result of background test */
} FlsTst_TestSignatureBgndType;

typedef struct
{
    FlsTst_StateType State;                          /* Execution status */
    FlsTst_IntervalIDType IntervalID;                /* ID for the background test interval */
    FlsTst_TestResultType BgndResult;                /* Test result of background */
    FlsTst_TestResultFgndType FgndResult;            /* Test result of foreground */
    FlsTst_ErrorDetailsType ErrorInfo;               /* Error details */
    FlsTst_UserNotificationPtrType UserCallBack;     /* Callback function */
    FlsTst_ECCFunctionPtrType FlsTstECCTestFunction; /* Point to the ECC test function after initialzation*/
} FlsTst_ModuleTypes;

typedef struct FlsTst_BlockBgndType
{
    FlsTst_BlockIdType BgndBlockIndex;   /* The block index value of the background test */
    FlsTst_AddressType BlockBaseAddress; /* The block base address of the background test */
    FlsTst_LengthType BlcokLength;       /* The block length of the background test */
    FlsTst_AddressType SignatureAddress; /* The address to store the signature of the background test */
    FlsTst_SignatureType DuplicateAddr;  /* The duplicated address for this block */
    FlsTst_AlgorithmType TestAlgorithm;  /* The test algorithm used in the background test */
} FlsTst_BlockBgndType;

typedef struct FlsTst_BlockFgndType
{
    FlsTst_BlockIdType BgndBlockIndex;   /* The block index value of the foreground test */
    FlsTst_AddressType BlockBaseAddress; /* The block base address of the foreground test */
    FlsTst_LengthType BlcokLength;       /* The block length of the foreground test */
    FlsTst_AddressType SignatureAddress; /* The address to store the signature of the foreground test */
    FlsTst_SignatureType DuplicateAddr;  /* The duplicated address for this block */
    FlsTst_AlgorithmType TestAlgorithm;  /* The test algorithm used in the foreground test */
} FlsTst_BlockFgndType;

typedef struct FlsTst_ConfigType
{
    const FlsTst_BlockBgndType* FlsTst_BgndBlockPtr;   /* Memory block information configured under background test */
    const FlsTst_BlockFgndType* FlsTst_FgndBlockPtr;   /* Memory block information configured under background test */
    FlsTst_UserNotificationPtrType FlsTstUserCallBack; /* Configured callback function */
    FlsTst_ECCFunctionPtrType FlsTst_ECCTestFunc;      /* ECC test function in configuration */
    FlsTst_ECCInitPtrType FlsTst_ECCInitFunc;          /* Configured ECC init function */
    FlsTst_ECCDeInitPtrType FlsTst_ECCDeInitFunc;      /* Configured ECC Deinit function */
} FlsTst_ConfigType;

typedef struct FlsTst_BlockInfoType
{
    FlsTst_AddressType MemBlcokStartAddr;    /* The starting address of this block */
    FlsTst_LengthType MemBlcokLength;        /* The length of the block that needs to be tested */
    FlsTst_SignatureType SignatureStored;    /* The stored signature for this block */
    FlsTst_SignatureType DuplicateAddr;      /* The duplicated address for this block */
    FlsTst_SignatureType SignatueCalculated; /* The signature already calculated for this block */
    FlsTst_BlockIdType MemoryBlockID;        /* The ID value of the block */
    FlsTst_AlgorithmType TestAlgorithm;      /* The test algorithm used by this block */
    FlsTst_TestResultType TestResult;        /* The test result ininof this block */
} FlsTst_BlockInfoType;

typedef struct FlsTst_CurRunningTag
{
    uint32 TestAddr;                    /* The address where the current test is running */
    uint32 DupTestAddr;                 /* The address where the current duplicated test is running */
    uint32 TestLength;                  /* The length of the currently running test that needs to be executed */
    uint32 CalcResult;                  /* Calculated result of the currently running test */
    FlsTst_BlockIdType CurBlockID;      /* The currently running block ID value */
    boolean FirstFlag;                  /*First Calc CRC*/
    FlsTst_AlgorithmType TestAlgorithm; /* The test algorithm for the currently running test */
} FlsTst_CurRunningType;

#endif /* #ifndef FLSTST_TYPES_H */
