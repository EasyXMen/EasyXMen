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
/*********************************************************************************
**                                                                            **
**  FILENAME    : RamTst_Types.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : peng.wu                                                     **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

#ifndef RAMTST_TYPES_H_
#define RAMTST_TYPES_H_

/*============================[I N C L U D E S]===============================*/
#include "RamTst_Cfg.h"
#include "Std_Types.h"

typedef uint8 RamTst_AlgParamsIdType;
typedef uint32 RamTst_NumberOfTestedCellsType;
typedef uint16 RamTst_NumberOfBlocksType;

/**********************************  enum   ****************************************/
typedef enum
{
    RAMTST_EXECUTION_UNINIT = 0x0u, /* The RAM Test is not initialized or not usable */
    RAMTST_EXECUTION_STOPPED,       /* The RAM Test is stopped and ready to be started in foreground or to be allowed in
                                       background */
    RAMTST_EXECUTION_RUNNING,       /* The RAM Test is currently running. */
    RAMTST_EXECUTION_SUSPENDED,     /* The RAM Test is currently running. */
} RamTst_ExecutionStatusType;

typedef enum
{
    RAMTST_RESULT_NOT_TESTED = 0x0u, /* The RAM Test is not executed. */
    RAMTST_RESULT_OK,                /* The RAM Test has been tested with OK result */
    RAMTST_RESULT_NOT_OK,            /* The RAM Test has been tested with NOTOK result. */
    RAMTST_RESULT_UNDEFINED,         /* The RAM Test is currently running. */
} RamTst_TestResultType;

typedef enum
{
    RAMTST_ALGORITHM_UNDEFINED = 0x0u, /* Undefined algorithm (uninitialized value) */
    RAMTST_CHECKERBOARD_TEST,          /* Checkerboard test algorithm */
    RAMTST_MARCH_TEST,                 /* March test algorithm */
    RAMTST_WALK_PATH_TEST,             /* Walk path test algorithm */
    RAMTST_GALPAT_TEST,                /* Galpat test algorithm */
    RAMTST_TRANSP_GALPAT_TEST,         /* Transparent Galpat test algorithm */
    RAMTST_ABRAHAM_TEST,               /* Abraham test algorithm */
} RamTst_AlgorithmType;

typedef enum
{
    RAMTEST_DESTRUCTIVE = 0x0u, /* RAM test does not restore memory content. */
    RAMTEST_NON_DESTRUCTIVE,    /* RAM test restores memory content. */
} RamTst_Policy;

typedef enum
{
    /*RamTst Uninit*/
    RAMTST_INIT_NOT,
    /*RamTst init*/
    RAMTST_INIT_OK
} RamTst_ModeInitStatusType;

typedef enum
{
    BLOCK_TEST_USED_METHOD_NONE = 0x0u, /* no method use */
    BLOCK_TEST_USED_METHOD_FOREGROUND,  /* only foreground */
    BLOCK_TEST_USED_METHOD_BACKGROUND,  /* only background */
    BLOCK_TEST_USED_METHOD_BOTH,        /* both foreground and background */
} RamTst_BlockTestUseMethod;

typedef enum
{
    BLOCK_NOT_TEST = 0x0u, /* RAM block test not complete */
    BLOCK_HAS_TESTED,      /* RAM block test completed */
} RamTst_IsBlockTestedFlag;

typedef enum
{
    UPDATE_RESULT_IDLE = 0x0u,   /* Initial status */
    UPDATE_RESULT_ALLTESTED,     /* All blocks are tested */
    UPDATE_RESULT_NOT_TEST,      /* Some block is not tested */
    UPDATE_RESULT_FAIL_NOT_TEST, /* Update test result if block is not tested */
    UPDATE_RESULT_FAIL_TESTED,   /* Update test result if blocks are all tested */
    UPDATE_RESULT_TEST_RUNNING,  /* Test is running */
} RamTst_UpdateResultFlag;

/**********************************  config types   ****************************************/

typedef struct
{
    uint32 NowTestedAddr;                            /* The address to which the block is currently tested */
    uint32 FinalEndAddr;                             /* The end address of the current test block */
    RamTst_IsBlockTestedFlag IsBlockTestedFlag;      /* Whether the currently tested block is complete */
    RamTst_TestResultType BlockTestResult;           /* Test result of the current block */
    RamTst_Policy TestPolicyUse;                     /* Test policy for the current test block */
    RamTst_AlgParamsIdType RamTstAlgParamsIdUse;     /* The algorithm used by the current test block */
    RamTst_BlockTestUseMethod BlockTestUseMethodUse; /* Block use foreground test or background test */
} RamTst_BlockInfoType;

typedef struct
{
    uint16 RamTstBlockId;                         /* ID value of the Block */
    uint32 RamTstEndAddress;                      /* The end address of the Block */
    uint32 RamTstStartAddress;                    /* The start address of the Block */
    uint32 RamTstFillPattern;                     /* The pattern value of the Block */
    RamTst_Policy RamTstTestPolicy;               /* The test policy of the Block */
    RamTst_BlockTestUseMethod BlockTestUseMethod; /* Block use foreground test or background test */
} RamTst_BlockParams;

typedef struct
{
    RamTst_AlgorithmType RamTstAlgorithm; /* This is the algorithm for which this RamTstAlgParams set is defined.  */
    RamTst_AlgParamsIdType
        RamTstAlgParamsId; /* This is the identifier by which this RamTstAlgParams set can be selected */
    RamTst_NumberOfTestedCellsType
        RamTstExtNumberOfTestedCells; /* The number of RAM units that can be executed at a time */
    RamTst_NumberOfTestedCellsType
        RamTstMaxNumberOfTestedCells;               /* The maximum number of RAM units that can be executed at a time */
    RamTst_NumberOfBlocksType RamTstNumberOfBlocks; /* Number of blocks to test */
    RamTst_NumberOfTestedCellsType RamTstNumberOfTestedCells; /* This is the initial value for a RAM variable */
    const RamTst_BlockParams* RamTstBlockParams;
} RamTst_AlgParams;

typedef struct
{
    boolean RamTstAbrahamTestSelected;      /* Select the Abraham test algorithm */
    boolean RamTstCheckerboardTestSelected; /* Select the Checkerboard test algorithm */
    boolean RamTstGalpatTestSelected;       /* Select the Galpat test algorithm */
    boolean RamTstMarchTestSelected;        /* Select the March test algorithm */
    boolean RamTstTranspGalpatTestSelected; /* Select the TranspGalpat test algorithm */
    boolean RamTstWalkPathTestSelected;     /* Select the WalkPath test algorithm */
} RamTst_Algorithms;

typedef struct
{
    RamTst_AlgParamsIdType RamTstDefaultAlgParamsId;             /* Default algorithm */
    RamTst_NumberOfTestedCellsType RamTstMinNumberOfTestedCells; /* The minimum number of RAM test */
    uint8 RamTstNumberOfAlgParamSets;                            /* Number of test algorithm sets */
    const RamTst_AlgParams* RamTstAlgParams;                     /* Test algorithm set */
#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
    void (*RamTst_TestCompletedNotificationPtr)(void); /* Test completion notification function pointer */
#endif
#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
    void (*RamTst_ErrorNotificationPtr)(void); /* Pointer to the notification function when an error occurs */
#endif
} RamTst_ConfigParams;

typedef struct
{
    RamTst_ConfigParams RamTstConfigParams; /* RAM test configuration parameters */
    RamTst_Algorithms RamTstAlgorithms;     /* Selection of test algorithm */
} RamTst_ConfigType;

#endif /* RAMTST_TYPES_H_ */
