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
**  FILENAME    : RamTst.c                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : peng.wu                                                     **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:RamTst<br>
  RuleSorce:puhua-rule.rcf 2.3.5

   \li PRQA S 0306 MISRA Rule 11.4 .<br>Reason:Cast between a pointer to object and an integral type.

    Reason: autosar require.
 */

/*=======[I N C L U D E S]====================================================*/
#include "RamTst.h"
#include "Schm_RamTst.h"
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*STD_ON == RAMTST_DEV_ERROR_DETECT*/
#include "Dem.h"
#if (RAMTST_TEST_TIMEOUT == STD_ON)
#include "FreeRTimer.h"
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */

/*******************************************************************/
#define RAMTST_AR_RELEASE_MAJOR_VERSION 4u /*Autosar Major Version*/
#define RAMTST_AR_RELEASE_MINOR_VERSION 5u /*Autosar Minor Version*/
#define RAMTST_AR_RELEASE_PATCH_VERSION 0u /*Autosar Patch Version*/
#define RAMTST_SW_MAJOR_VERSION         2u /*Major Version*/
#define RAMTST_SW_MINOR_VERSION         0u /*Minor Version*/
#define RAMTST_SW_PATCH_VERSION         1u /*Patch Version*/

/*=======================[V E R S I O N  C H E C K]====================================*/
#ifndef RAMTST_AR_RELEASE_MAJOR_VERSION
#error " RAMTST version miss"
#endif
#ifndef RAMTST_AR_RELEASE_MINOR_VERSION
#error " RAMTST version miss"
#endif
#ifndef RAMTST_AR_RELEASE_PATCH_VERSION
#error " RAMTST version miss"
#endif
#if ((2 != RAMTST_CFG_SW_MAJOR_VERSION) || (0 != RAMTST_CFG_SW_MINOR_VERSION) || (1 != RAMTST_CFG_SW_PATCH_VERSION))
#error " cfg version mismatching"
#endif

/***************************Static variable********************************************/
#define RAMTST_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "RamTst_MemMap.h"
static VAR(RamTst_BlockInfoType, RAMTST_VAR) RamTstAllBlockInfo[RAMTST_BLOCK_NUM + 1u];
#define RAMTST_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_INIT_PTR
#include "RamTst_MemMap.h"
static P2CONST(RamTst_ConfigType, AUTOMATIC, RAMTST_APPL_DATA) RamTstConfig = NULL_PTR;
#define RAMTST_STOP_SEC_VAR_INIT_PTR
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_INIT_32
#include "RamTst_MemMap.h"
static VAR(uint32, RAMTST_VAR) NowBackTestBlockIndex = 1u;
static VAR(RamTst_NumberOfTestedCellsType, RAMTST_VAR) RamTstNumberOfTestedCellsUse = 1u;
static VAR(uint32, RAMTST_VAR) NowAlgParamTestedBlockNumber = 0u;
#define RAMTST_STOP_SEC_VAR_INIT_32
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_INIT_8
#include "RamTst_MemMap.h"
/* Data type used to identify a set of configuration parameters for a test algorithm */
static VAR(RamTst_AlgParamsIdType, RAMTST_VAR) CurRamTstAlgParamsId = 1u;
static VAR(RamTst_ModeInitStatusType, RAMTST_VAR) RamTstDeInitFlag = RAMTST_INIT_NOT;
static VAR(RamTst_ExecutionStatusType, RAMTST_VAR) CurRamTstExecutionStatus = RAMTST_EXECUTION_UNINIT;
static VAR(RamTst_TestResultType, RAMTST_VAR) RamTstOverallTestResult = RAMTST_RESULT_NOT_TESTED;
#define RAMTST_STOP_SEC_VAR_INIT_8
#include "RamTst_MemMap.h"

/***************************I********************************************************/

/***************************Internal  API********************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"

/* Execute  the March C Algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoMarchAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

/* Execute the Checker board Algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoCheckerboardAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

/* Execute the Walk Path Algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoWalkPathAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

/* Execute the Galpat Algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoGalpatAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

/* Execute  the TransGalpat Algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoTransGalpatAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

/* Execute  the Abraham Algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoAbrahamAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

/* Select the test algorithm and fill in the specified value after the test */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTst_ExecTest(
    RamTst_AlgorithmType UsedAlgorithm,
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy);

#if (STD_ON == RAMTST_SELF_CHECK_ENABLE)
/* Implement RAM self-check algorithm */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTst_DoSelfCheckTestAction(uint32 u32CStartAddr, uint32 u32CEndAddr);
#endif /* STD_ON == RAMTST_SELF_CHECK_ENABLE */

/* Initializes the state of all blocks */
static FUNC(void, RAMTST_CODE) RamTst_InitAllBlockStatus(void);

/* Check whether the ID of the Block is out of range */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTst_CheckBlockIDIsOutOfRange(RamTst_NumberOfBlocksType BlockID);

/* Check whether the Condition of the Background is valid */
static FUNC(boolean, RAMTST_CODE) RamTst_CheckBgndCondIsValid(void);

/* Update overall test results */
static FUNC(void, RAMTST_CODE) RamTst_UpdateOverallTestResult(void);
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Execute Abraham algorithm step one
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      u32RamTstNow:RAM address of the test
 * Param-Name[out]     None
 * Param-Name[in/out]  DoTestRamResult:E_OK:The RAM area tested is successful
 *                     E_NOT_OK:The RAM area tested is failed
 * Return              None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(void, RAMTST_CODE) RamTest_AbrahamAlgorithmStepOne(uint32* u32RamTstNow, Std_ReturnType* DoTestRamResult)
{
    uint8 u8RamTstCellIndex;

    /* Check whether the value is 0 in ascending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (0u == RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            /*  Write 1 if the data of the cell is right */
            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 1 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE
            != RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 1 in ascending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE == RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            /*  Write 0 if the data of the cell is right */
            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, u8RamTstCellIndex);
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 0 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO
            != RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Execute Abraham algorithm step two
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      u32RamTstNow:RAM address of the test
 * Param-Name[out]     None
 * Param-Name[in/out]  DoTestRamResult:E_OK:The RAM area tested is successful
 *                     E_NOT_OK:The RAM area tested is failed
 * Return              None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(void, RAMTST_CODE) RamTest_AbrahamAlgorithmStepTwo(uint32* u32RamTstNow, Std_ReturnType* DoTestRamResult)
{
    uint8 u8RamTstCellIndex;

    /* Check whether the value is 0 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO
            == RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            /* Write 1 if the data of the cell is right */
            *u32RamTstNow =
                RAMTST_SET_ONE(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U));
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 1 in ascending order */
    for (u8RamTstCellIndex = 0x01U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE != RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 1 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE
            == RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            /* Write 0 if the data of the cell is right */
            *u32RamTstNow =
                RAMTST_SET_ZERO(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U));
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 0 in ascending order */
    for (u8RamTstCellIndex = 0x01U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO != RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Execute Abraham algorithm step three
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      u32RamTstNow:RAM address of the test
 * Param-Name[out]     None
 * Param-Name[in/out]  DoTestRamResult:E_OK:The RAM area tested is successful
 *                     E_NOT_OK:The RAM area tested is failed
 * Return              None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(void, RAMTST_CODE) RamTest_AbrahamAlgorithmStepThree(uint32* u32RamTstNow, Std_ReturnType* DoTestRamResult)
{
    uint8 u8RamTstCellIndex;

    /* Check whether the value is 0 in ascending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO == RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            /* Write 1 if the data of the cell is right */
            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);
            /* Write 0 if the data of the cell is right */
            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, u8RamTstCellIndex);
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 0 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO
            != RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }

    /* Check whether the value is 0 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO
            == RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            /* Write 1 if the data of the cell is right */
            *u32RamTstNow =
                RAMTST_SET_ONE(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U));
            /* Write 0 if the data of the cell is right */
            *u32RamTstNow =
                RAMTST_SET_ZERO(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U));
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 0 in ascending order */
    for (u8RamTstCellIndex = 0x01U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ALL_ZERO != RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Execute Abraham algorithm step four
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      u32RamTstNow:RAM address of the test
 * Param-Name[out]     None
 * Param-Name[in/out]  DoTestRamResult:E_OK:The RAM area tested is successful
 *                     E_NOT_OK:The RAM area tested is failed
 * Return              None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(void, RAMTST_CODE) RamTest_AbrahamAlgorithmStepFour(uint32* u32RamTstNow, Std_ReturnType* DoTestRamResult)
{
    uint8 u8RamTstCellIndex;

    /* Check whether the value is 1 in ascending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE == RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            /* Write 0 if the data of the cell is right */
            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, u8RamTstCellIndex);
            /* Write 1 if the data of the cell is right */
            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
    /* Check whether the value is 1 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE
            != RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }

    /* Check whether the value is 1 in descending order */
    for (u8RamTstCellIndex = 0x00U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE
            == RAMTST_VERIFY(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U)))
        {
            /* Write 0 if the data of the cell is right */
            *u32RamTstNow =
                RAMTST_SET_ZERO(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U));
            /* Write 1 if the data of the cell is right */
            *u32RamTstNow =
                RAMTST_SET_ONE(*u32RamTstNow, (uint8)(RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 0x01U));
        }
        else
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }

    /* Check whether the value is 1 in ascending order */
    for (u8RamTstCellIndex = 0x01U;
         ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == *DoTestRamResult));
         u8RamTstCellIndex++)
    {
        if (RAMTST_ONE != RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
        {
            *DoTestRamResult = E_NOT_OK;
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Do March Algorithm
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
/* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* RAMTST_CAST_OPERATORS_001 */
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoMarchAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    uint8 u8RamTstCellIndex;
    uint32 u32RamTstDataSaved;
    uint32* u32RamTstNow;

    Std_ReturnType DoTestRamResult = E_OK;

    for (u32RamTstNow = (uint32*)u32CRamStartAddr;
         ((u32RamTstNow <= (uint32*)u32CRamEndAddr) && (E_OK == DoTestRamResult));
         u32RamTstNow++)
    {
        SchM_Enter_RamTst_Queue();

        u32RamTstDataSaved = *u32RamTstNow;

        /* Write the pattern 0x00000000 */
        *u32RamTstNow = RAMTST_ALL_ZERO;

        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            if (RAMTST_ALL_ZERO != RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
            {
                DoTestRamResult = E_NOT_OK;
            }
            else
            {
                *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);
            }
        }

        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            if (RAMTST_ALL_ZERO == RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == u8TestPolicy)
        {
            *u32RamTstNow = u32CRamTstFillPattern;
        }
        else
        {
            *u32RamTstNow = u32RamTstDataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }
    return DoTestRamResult;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Do Checkerboard Algorithm
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoCheckerboardAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    uint8 u8RamTstCellIndex;
    uint32 u32RamTstDataSaved;
    uint32* u32RamTstNow;
    Std_ReturnType DoTestRamResult;

    DoTestRamResult = E_OK;

    for (u32RamTstNow = (uint32*)u32CRamStartAddr;
         ((u32RamTstNow <= (uint32*)u32CRamEndAddr) && (E_OK == DoTestRamResult));
         u32RamTstNow++)
    {
        SchM_Enter_RamTst_Queue();

        u32RamTstDataSaved = *u32RamTstNow;

        /* Write checkerboard pattern 0xAAAAAAAAu */
        for (u8RamTstCellIndex = 0; u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS; u8RamTstCellIndex += 2u)
        {
            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, u8RamTstCellIndex);
            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, (u8RamTstCellIndex + 1u));
        }

        /* Check the cells from the lower to the higher */
        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex += 2u)
        {
            if ((RAMTST_ALL_ZERO != RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
                || (0u == RAMTST_VERIFY(*u32RamTstNow, (u8RamTstCellIndex + 1u))))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        /* Check from the higher to the lower cell. */
        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex += 2u)
        {
            if ((RAMTST_ALL_ZERO
                 != RAMTST_VERIFY(*u32RamTstNow, (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 2u)))
                || (0u == RAMTST_VERIFY(*u32RamTstNow, (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 1u))))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        /* Write checkerboard pattern 0x55555555u */
        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex += 2u)
        {
            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);
            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, (u8RamTstCellIndex + 1u));
        }

        /* Check from the lower to the higher cell. */
        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex += 2u)
        {
            if ((RAMTST_ALL_ZERO == RAMTST_VERIFY(*u32RamTstNow, u8RamTstCellIndex))
                || (0u != RAMTST_VERIFY(*u32RamTstNow, (u8RamTstCellIndex + 1u))))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        /* Check from the higher to the lower cell. */
        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex += 2u)
        {
            if ((RAMTST_ALL_ZERO
                 == RAMTST_VERIFY(*u32RamTstNow, (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 2u)))
                || (0u != RAMTST_VERIFY(*u32RamTstNow, (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex - 1u))))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == u8TestPolicy)
        {
            *u32RamTstNow = u32CRamTstFillPattern;
        }
        else
        {
            *u32RamTstNow = u32RamTstDataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return DoTestRamResult;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Do WalkPath Algorithm
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoWalkPathAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    uint8 u8RamTstCellIndex;
    uint32 u32RamTstDataSaved;
    uint32* u32RamTstNow;
    Std_ReturnType DoTestRamResult;

    DoTestRamResult = E_OK;

    for (u32RamTstNow = (uint32*)u32CRamStartAddr;
         ((u32RamTstNow <= (uint32*)u32CRamEndAddr) && (E_OK == DoTestRamResult));
         u32RamTstNow++)
    {
        SchM_Enter_RamTst_Queue();

        u32RamTstDataSaved = *u32RamTstNow;

        /* Write the pattern 0x00000000 */
        *u32RamTstNow = RAMTST_ALL_ZERO;

        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);

            /* Check the remaining memory area. */
            if (RAMTST_ALL_ZERO != RAMTST_VERIFY_WALKPATH(*u32RamTstNow, u8RamTstCellIndex))
            {
                DoTestRamResult = E_NOT_OK;
                break;
            }

            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, u8RamTstCellIndex);
        }

        /* Write the pattern 0xFFFFFFFF */
        *u32RamTstNow = RAMTST_ALL_ONE;

        for (u8RamTstCellIndex = 0;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            *u32RamTstNow = RAMTST_SET_ZERO(*u32RamTstNow, u8RamTstCellIndex);

            /* Check the remaining memory area. */
            if (RAMTST_ALL_ONE != RAMTST_VERIFY_WALKPATH(*u32RamTstNow, u8RamTstCellIndex))
            {
                DoTestRamResult = E_NOT_OK;
                break;
            }

            *u32RamTstNow = RAMTST_SET_ONE(*u32RamTstNow, u8RamTstCellIndex);
        }

        if (RAMTEST_DESTRUCTIVE == u8TestPolicy)
        {
            *u32RamTstNow = u32CRamTstFillPattern;
        }
        else
        {
            *u32RamTstNow = u32RamTstDataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return DoTestRamResult;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Do Galpat Algorithm
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoGalpatAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    uint8 u8RamTstCellIndex;
    uint32 u32RamTstDataSaved;
    Std_ReturnType DoTestRamResult;
    uint32* u32RamTstNow;

    DoTestRamResult = E_OK;

    for (u32RamTstNow = (uint32*)u32CRamStartAddr;
         ((u32RamTstNow <= (uint32*)u32CRamEndAddr) && (E_OK == DoTestRamResult));
         u32RamTstNow++)
    {
        SchM_Enter_RamTst_Queue();

        u32RamTstDataSaved = *u32RamTstNow;

        /* Write the pattern 0x00000000 */
        *u32RamTstNow = RAMTST_ALL_ZERO;

        for (u8RamTstCellIndex = 1u;
             ((u8RamTstCellIndex <= RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            /* For example. Write the test cell as follow sequence: 1. 0000; 2. 0001; 3. 0011; ... */
            *u32RamTstNow = RAMTST_GALPAT_ONE_PATTERN(*u32RamTstNow, u8RamTstCellIndex);

            /* Check the remaining cells. */
            if ((RAMTST_ALL_ZERO != RAMTST_GALPAT_CHECK_ZERO(*u32RamTstNow, u8RamTstCellIndex))
                || (RAMTST_ALL_ONE != RAMTST_GALPAT_CHECK_ONES(*u32RamTstNow, u8RamTstCellIndex)))
            {
                DoTestRamResult = E_NOT_OK;
                break;
            }

            /* Check the inverted Cell. */
            if (RAMTST_ALL_ZERO == (RAMTST_VERIFY(*u32RamTstNow, (u8RamTstCellIndex - 1u))))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        for (u8RamTstCellIndex = 1u;
             ((u8RamTstCellIndex <= RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            /* For example. Write the test cell as follow sequence: 1. 1111; 2. 0111; 3. 0011 ... */
            *u32RamTstNow = RAMTST_GALPAT_ZERO_PATTERN(*u32RamTstNow, u8RamTstCellIndex);

            /* Check the remaining cells. */
            if ((RAMTST_ALL_ONE != RAMTST_GALPAT_CHECK_ALL_ONE(*u32RamTstNow, u8RamTstCellIndex))
                || (RAMTST_ALL_ZERO != RAMTST_GALPAT_CHECK_ALL_ZERO(*u32RamTstNow, u8RamTstCellIndex)))
            {
                DoTestRamResult = E_NOT_OK;
                break;
            }

            /* Check the inverted Cell. */
            if (RAMTST_ALL_ZERO
                != (RAMTST_VERIFY(*u32RamTstNow, (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - u8RamTstCellIndex))))
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == u8TestPolicy)
        {
            *u32RamTstNow = u32CRamTstFillPattern;
        }
        else
        {
            *u32RamTstNow = u32RamTstDataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return DoTestRamResult;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Do TransGalpat Algorithm
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoTransGalpatAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    uint8 u8RamTstCellIndex;
    uint32 u32RamTstDataSaved;
    uint32 u32_Signature1;
    uint32 u32_Signature2;
    uint32 u32_Signature3;
    Std_ReturnType DoTestRamResult;
    uint32* u32RamTstNow;

    DoTestRamResult = E_OK;

    for (u32RamTstNow = (uint32*)u32CRamStartAddr;
         ((u32RamTstNow <= (uint32*)u32CRamEndAddr) && (E_OK == DoTestRamResult));
         u32RamTstNow++)
    {
        SchM_Enter_RamTst_Queue();

        u32RamTstDataSaved = *u32RamTstNow;

        for (u8RamTstCellIndex = 0u;
             ((u8RamTstCellIndex < RAMTST_MIN_NUMBER_OF_TESTED_CELLS) && (E_OK == DoTestRamResult));
             u8RamTstCellIndex++)
        {
            u32_Signature1 =
                (uint32)(((*u32RamTstNow) ^ RAMTST_SIGNATURE_TransGalpat) & ~(uint32)(1uL << u8RamTstCellIndex));
            *u32RamTstNow ^= (uint32)(1uL << u8RamTstCellIndex);

            u32_Signature2 =
                (uint32)(((*u32RamTstNow) ^ RAMTST_SIGNATURE_TransGalpat) & ~(uint32)(1uL << u8RamTstCellIndex));
            if (u32_Signature1 != u32_Signature2)
            {
                DoTestRamResult = E_NOT_OK;
                break;
            }
            *u32RamTstNow ^= (uint32)(1uL << u8RamTstCellIndex);

            u32_Signature3 =
                (uint32)(((*u32RamTstNow) ^ RAMTST_SIGNATURE_TransGalpat) & ~(uint32)(1uL << u8RamTstCellIndex));
            if (u32_Signature1 != u32_Signature3)
            {
                DoTestRamResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == u8TestPolicy)
        {
            *u32RamTstNow = u32CRamTstFillPattern;
        }
        else
        {
            *u32RamTstNow = u32RamTstDataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return DoTestRamResult;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Do Abraham Algorithm
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTest_DoAbrahamAlgorithm(
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    uint32 u32RamTstDataSaved;
    Std_ReturnType DoTestRamResult;
    uint32* u32RamTstNow;

    DoTestRamResult = E_OK;

    for (u32RamTstNow = (uint32*)u32CRamStartAddr;
         ((u32RamTstNow <= (uint32*)u32CRamEndAddr) && (E_OK == DoTestRamResult));
         u32RamTstNow++)
    {
        SchM_Enter_RamTst_Queue();

        u32RamTstDataSaved = *u32RamTstNow;

        /* Write the pattern 0x00000000 */
        *u32RamTstNow = RAMTST_ALL_ZERO;

        RamTest_AbrahamAlgorithmStepOne(u32RamTstNow, &DoTestRamResult);
        RamTest_AbrahamAlgorithmStepTwo(u32RamTstNow, &DoTestRamResult);
        RamTest_AbrahamAlgorithmStepThree(u32RamTstNow, &DoTestRamResult);

        /* Write the pattern 0xFFFFFFFF */
        *u32RamTstNow = RAMTST_ALL_ONE;

        RamTest_AbrahamAlgorithmStepFour(u32RamTstNow, &DoTestRamResult);

        if (RAMTEST_DESTRUCTIVE == u8TestPolicy)
        {
            *u32RamTstNow = u32CRamTstFillPattern;
        }
        else
        {
            *u32RamTstNow = u32RamTstDataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }
    return DoTestRamResult;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Execute test
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      UsedAlgorithm:used Algorithm
 *                     u32CRamStartAddr:RAM start addr
 *                     u32CRamEndAddr:RAM end addr
 *                     u32CRamTstFillPattern:the bit pattern value
 *                     u8TestPolicy:the test policy
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTst_ExecTest(
    RamTst_AlgorithmType UsedAlgorithm,
    uint32 u32CRamStartAddr,
    uint32 u32CRamEndAddr,
    uint32 u32CRamTstFillPattern,
    RamTst_Policy u8TestPolicy)
{
    Std_ReturnType Rtn;

    Rtn = E_NOT_OK;

    switch (UsedAlgorithm)
    {
    case RAMTST_CHECKERBOARD_TEST: {
        if ((boolean)TRUE == RamTstConfig->RamTstAlgorithms.RamTstCheckerboardTestSelected)
        {
            Rtn =
                RamTest_DoCheckerboardAlgorithm(u32CRamStartAddr, u32CRamEndAddr, u32CRamTstFillPattern, u8TestPolicy);
        }
        break;
    }

    case RAMTST_MARCH_TEST: {
        if ((boolean)TRUE == RamTstConfig->RamTstAlgorithms.RamTstMarchTestSelected)
        {
            Rtn = RamTest_DoMarchAlgorithm(u32CRamStartAddr, u32CRamEndAddr, u32CRamTstFillPattern, u8TestPolicy);
        }
        break;
    }

    case RAMTST_WALK_PATH_TEST: {
        if ((boolean)TRUE == RamTstConfig->RamTstAlgorithms.RamTstWalkPathTestSelected)
        {
            Rtn = RamTest_DoWalkPathAlgorithm(u32CRamStartAddr, u32CRamEndAddr, u32CRamTstFillPattern, u8TestPolicy);
        }
        break;
    }

    case RAMTST_GALPAT_TEST: {
        if ((boolean)TRUE == RamTstConfig->RamTstAlgorithms.RamTstGalpatTestSelected)
        {
            Rtn = RamTest_DoGalpatAlgorithm(u32CRamStartAddr, u32CRamEndAddr, u32CRamTstFillPattern, u8TestPolicy);
        }
        break;
    }

    case RAMTST_TRANSP_GALPAT_TEST: {
        if ((boolean)TRUE == RamTstConfig->RamTstAlgorithms.RamTstTranspGalpatTestSelected)
        {
            Rtn = RamTest_DoTransGalpatAlgorithm(u32CRamStartAddr, u32CRamEndAddr, u32CRamTstFillPattern, u8TestPolicy);
        }
        break;
    }

    case RAMTST_ABRAHAM_TEST: {
        if ((boolean)TRUE == RamTstConfig->RamTstAlgorithms.RamTstAbrahamTestSelected)
        {
            Rtn = RamTest_DoAbrahamAlgorithm(u32CRamStartAddr, u32CRamEndAddr, u32CRamTstFillPattern, u8TestPolicy);
        }
        break;
    }

    default:
        /* empty */
        break;
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

#if (STD_ON == RAMTST_SELF_CHECK_ENABLE)
/*************************************************************************/
/*
 * Brief               Do ram self check
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      u32CStartAddr: RAM start addr
 *                     u32CEndAddr: RAM end addr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTst_DoSelfCheckTestAction(uint32 u32CStartAddr, uint32 u32CEndAddr)
{
    Std_ReturnType Rtn;
    uint32* pRamPtr;
    const uint32 u32WriteData = 0xA5A5A5A5u;
    uint32 u32Storeata;

    Rtn = E_OK;

    /* Verify data with addresses increasing */
    for (pRamPtr = (uint32*)u32CStartAddr; ((pRamPtr < (uint32*)u32CEndAddr) && (E_OK == Rtn)); (pRamPtr)++)
    {
        u32Storeata = *pRamPtr;
        *pRamPtr = u32WriteData;
        if (*pRamPtr != u32WriteData)
        {
            Rtn = E_NOT_OK;
        }
        else
        {
            *pRamPtr = u32Storeata;
        }
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif               /* STD_ON == RAMTST_SELF_CHECK_ENABLE */
/* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

/*************************************************************************/
/*
 * Brief               Init All config Block Status
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(void, RAMTST_CODE) RamTst_InitAllBlockStatus(void)
{
    uint32 Index1;
    uint32 Index2;
    uint32 TmpId;
    const RamTst_BlockParams* pTmpBlock;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (NULL_PTR == RamTstConfig)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_INIT_ID, RAMTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        for (Index2 = 0; Index2 < RamTstConfig->RamTstConfigParams.RamTstNumberOfAlgParamSets; Index2++)
        {
            for (Index1 = 0; Index1 < RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index2].RamTstNumberOfBlocks;
                 Index1++)
            {
                /* use block id as the buffer index */
                TmpId =
                    RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index2].RamTstBlockParams[Index1].RamTstBlockId;
                pTmpBlock = &RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index2].RamTstBlockParams[Index1];

                RamTstAllBlockInfo[TmpId].BlockTestResult = RAMTST_RESULT_NOT_TESTED;
                RamTstAllBlockInfo[TmpId].IsBlockTestedFlag = BLOCK_NOT_TEST;
                RamTstAllBlockInfo[TmpId].NowTestedAddr = pTmpBlock->RamTstStartAddress;
                RamTstAllBlockInfo[TmpId].FinalEndAddr = pTmpBlock->RamTstStartAddress;
                RamTstAllBlockInfo[TmpId].TestPolicyUse = pTmpBlock->RamTstTestPolicy;
                RamTstAllBlockInfo[TmpId].RamTstAlgParamsIdUse =
                    RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index2].RamTstAlgParamsId;
                RamTstAllBlockInfo[TmpId].BlockTestUseMethodUse = pTmpBlock->BlockTestUseMethod;
            }
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Check BlockID Is Out Of Range
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockID:to be checked BlockID
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(Std_ReturnType, RAMTST_CODE) RamTst_CheckBlockIDIsOutOfRange(RamTst_NumberOfBlocksType BlockID)
{
    Std_ReturnType Rtn;
    uint32 Index1;
    uint32 Index2;

    Rtn = E_NOT_OK;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (NULL_PTR == RamTstConfig)
    {
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_GET_TEST_RESULT_PER_BLOCK_ID,
            RAMTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        for (Index2 = 0; Index2 < RamTstConfig->RamTstConfigParams.RamTstNumberOfAlgParamSets; Index2++)
        {
            for (Index1 = 0; Index1 < RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index2].RamTstNumberOfBlocks;
                 Index1++)
            {
                if (BlockID
                    == RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index2].RamTstBlockParams[Index1].RamTstBlockId)
                {
                    Rtn = E_OK;
                    break;
                }
            }
        }
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Update Overall Test Result
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(void, RAMTST_CODE) RamTst_UpdateOverallTestResult(void)
{
    RamTst_UpdateResultFlag UpdateResultFuncUseFlag;
    uint32 UpdateResultIndex;
    const RamTst_AlgParams* UpdateOverallFuncAlgUsePtr;

    UpdateResultFuncUseFlag = UPDATE_RESULT_IDLE;

    /* check if no test started */
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (NULL_PTR == RamTstConfig)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_TEST_RESULT_ID, RAMTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        UpdateOverallFuncAlgUsePtr =
            &RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)];
        for (UpdateResultIndex = 0u; UpdateResultIndex < UpdateOverallFuncAlgUsePtr->RamTstNumberOfBlocks;
             UpdateResultIndex++)
        {
            if (BLOCK_NOT_TEST
                != RamTstAllBlockInfo[UpdateOverallFuncAlgUsePtr->RamTstBlockParams[UpdateResultIndex].RamTstBlockId]
                       .IsBlockTestedFlag)
            {
                UpdateResultFuncUseFlag = UPDATE_RESULT_ALLTESTED;
                break;
            }
        }

        if (UPDATE_RESULT_IDLE == UpdateResultFuncUseFlag)
        {
            /* no block has tested ,but check if has a block current running */
            for (UpdateResultIndex = 0u; UpdateResultIndex < UpdateOverallFuncAlgUsePtr->RamTstNumberOfBlocks;
                 UpdateResultIndex++)
            {
                if (RAMTST_RESULT_UNDEFINED
                    == RamTstAllBlockInfo[UpdateOverallFuncAlgUsePtr->RamTstBlockParams[UpdateResultIndex]
                                              .RamTstBlockId]
                           .BlockTestResult)
                {
                    UpdateResultFuncUseFlag = UPDATE_RESULT_TEST_RUNNING;
                    break;
                }
            }
            if (UPDATE_RESULT_TEST_RUNNING == UpdateResultFuncUseFlag)
            {
                RamTstOverallTestResult = RAMTST_RESULT_UNDEFINED;
            }
            else
            {
                /* no block start test */
                RamTstOverallTestResult = RAMTST_RESULT_NOT_TESTED;
            }
        }
        else
        {
            for (UpdateResultIndex = 0u; UpdateResultIndex < UpdateOverallFuncAlgUsePtr->RamTstNumberOfBlocks;
                 UpdateResultIndex++)
            {
                if (BLOCK_NOT_TEST
                    == RamTstAllBlockInfo[UpdateOverallFuncAlgUsePtr->RamTstBlockParams[UpdateResultIndex]
                                              .RamTstBlockId]
                           .IsBlockTestedFlag)
                {
                    /* check if has block not tested */
                    UpdateResultFuncUseFlag = UPDATE_RESULT_NOT_TEST;
                    break;
                }
            }

            if (UPDATE_RESULT_NOT_TEST == UpdateResultFuncUseFlag)
            {
                /* a test was started, not all blocks have yet been tested */
                for (UpdateResultIndex = 0u; UpdateResultIndex < UpdateOverallFuncAlgUsePtr->RamTstNumberOfBlocks;
                     UpdateResultIndex++)
                {
                    if (RAMTST_RESULT_NOT_OK
                        == RamTstAllBlockInfo[UpdateOverallFuncAlgUsePtr->RamTstBlockParams[UpdateResultIndex]
                                                  .RamTstBlockId]
                               .BlockTestResult)
                    {
                        UpdateResultFuncUseFlag = UPDATE_RESULT_FAIL_NOT_TEST;
                        break;
                    }
                }

                if (UPDATE_RESULT_FAIL_NOT_TEST != UpdateResultFuncUseFlag)
                {
                    /* a test was started, not all blocks have yet been tested and no block result is
                     * RAMTST_RESULT_NOT_OK */
                    RamTstOverallTestResult = RAMTST_RESULT_UNDEFINED;
                }
                else
                {
                    RamTstOverallTestResult = RAMTST_RESULT_NOT_OK;
                }
            }
            else
            {
                /* all block has tested */
                /* check ok or not ok */
                for (UpdateResultIndex = 0u; UpdateResultIndex < UpdateOverallFuncAlgUsePtr->RamTstNumberOfBlocks;
                     UpdateResultIndex++)
                {
                    if (RAMTST_RESULT_NOT_OK
                        == RamTstAllBlockInfo[UpdateOverallFuncAlgUsePtr->RamTstBlockParams[UpdateResultIndex]
                                                  .RamTstBlockId]
                               .BlockTestResult)
                    {
                        UpdateResultFuncUseFlag = UPDATE_RESULT_FAIL_TESTED;
                        break;
                    }
                }

                if (UPDATE_RESULT_FAIL_TESTED != UpdateResultFuncUseFlag)
                {
                    RamTstOverallTestResult = RAMTST_RESULT_OK;
                }
                else
                {
                    RamTstOverallTestResult = RAMTST_RESULT_NOT_OK;
                }
            }
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Check the Background test condition
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
static FUNC(boolean, RAMTST_CODE) RamTst_CheckBgndCondIsValid(void)
{
    const RamTst_AlgParams* RamTstAlgParamsPtr;
    boolean ret = FALSE;

    RamTstAlgParamsPtr = &RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)];
    if ((CurRamTstAlgParamsId == RamTstAllBlockInfo[NowBackTestBlockIndex].RamTstAlgParamsIdUse)
        && (((uint16)(NowAlgParamTestedBlockNumber + 1u) <= RamTstAlgParamsPtr->RamTstNumberOfBlocks)))
    {
        if (BLOCK_NOT_TEST == RamTstAllBlockInfo[NowBackTestBlockIndex].IsBlockTestedFlag)
        {
            /* Only matched algorithms can be tested  */
            if ((BLOCK_TEST_USED_METHOD_BACKGROUND == RamTstAllBlockInfo[NowBackTestBlockIndex].BlockTestUseMethodUse)
                || (BLOCK_TEST_USED_METHOD_BOTH == RamTstAllBlockInfo[NowBackTestBlockIndex].BlockTestUseMethodUse))
            {
                /* back test must be none destuctive and need save and recover */
                if (RAMTEST_DESTRUCTIVE == RamTstAllBlockInfo[NowBackTestBlockIndex].TestPolicyUse)
                {
                    ret = FALSE;
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
                    (void)Det_ReportError(
                        RAMTST_MODULE_ID,
                        RAMTST_INSTANCE_ID,
                        RAMTST_GET_MAIN_FUNCTION_ID,
                        RAMTST_E_OUT_OF_RANGE);
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
                }
                else
                {
                    ret = TRUE;
                }
            }
        }
    }

    return ret;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/**********************************                 extern api                  *************************************/
/*************************************************************************/
/* RamTst
 * Brief               Service for RAM Test initialization
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr:Pointer to the selected configuration set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_Init(P2CONST(RamTst_ConfigType, AUTOMATIC, RAMTST_APPL_CONST) ConfigPtr)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_INIT_ID, RAMTST_E_PARAM_POINTER);
    }
    else if (RAMTST_EXECUTION_UNINIT != CurRamTstExecutionStatus)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_INIT_ID, RAMTST_E_STATUS_FAILURE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        /* self check */
#if (STD_ON == RAMTST_SELF_CHECK_ENABLE)
        if (E_OK != RamTst_DoSelfCheckTestAction(RAM_LOCAL_VAR_AREA_START, RAM_LOCAL_VAR_AREA_END))
        {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
            (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_INIT_ID, RAMTST_E_SELF_CHECK_ERR);
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
            while (1)
            {
                /* empty */
            }
        }
#endif /* STD_ON == RAMTST_SELF_CHECK_ENABLE */
        RamTstConfig = ConfigPtr;
        RamTstOverallTestResult = RAMTST_RESULT_NOT_TESTED;
        CurRamTstAlgParamsId = RamTstConfig->RamTstConfigParams.RamTstDefaultAlgParamsId;
        CurRamTstExecutionStatus = RAMTST_EXECUTION_STOPPED;
        RamTstDeInitFlag = RAMTST_INIT_OK;
        RamTstNumberOfTestedCellsUse =
            RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)]
                .RamTstNumberOfTestedCells;
        NowBackTestBlockIndex = RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)]
                                    .RamTstBlockParams[0]
                                    .RamTstBlockId;

        RamTst_InitAllBlockStatus();
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
/*************************************************************************/
/* RamTst
 * Brief               Service for RAM Test deinitialization
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_DeInit(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_DEINIT_ID, RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstAlgParamsId = 1u;
        RamTstOverallTestResult = RAMTST_RESULT_NOT_TESTED;
        CurRamTstExecutionStatus = RAMTST_EXECUTION_UNINIT;
        RamTstDeInitFlag = RAMTST_INIT_NOT;
        RamTst_InitAllBlockStatus();
        NowBackTestBlockIndex = 1u;
        RamTstNumberOfTestedCellsUse = 1u;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#if RAMTST_STOP_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for stopping the RAM Test
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_Stop(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_STOP_ID, RAMTST_E_UNINIT);
    }
    else if (
        (RAMTST_EXECUTION_RUNNING != CurRamTstExecutionStatus)
        && (RAMTST_EXECUTION_SUSPENDED != CurRamTstExecutionStatus))
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_STOP_ID, RAMTST_E_STATUS_FAILURE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstExecutionStatus = RAMTST_EXECUTION_STOPPED;
        /* The test result is retained, but test parameters and loop data are not.  SWS_RamTst_00014 */
        NowBackTestBlockIndex = RamTstConfig->RamTstConfigParams.RamTstAlgParams[CurRamTstAlgParamsId - 1u]
                                    .RamTstBlockParams[0]
                                    .RamTstBlockId;
        RamTst_InitAllBlockStatus();
        CurRamTstAlgParamsId = RamTstConfig->RamTstConfigParams.RamTstDefaultAlgParamsId;
        RamTstOverallTestResult = RAMTST_RESULT_NOT_TESTED;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_STOP_API_ENABLE */

#if RAMTST_ALLOW_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for continuing the RAM Test after calling 'RamTst_Stop.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_Allow(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_ALLOW_ID, RAMTST_E_UNINIT);
    }
    else if (RAMTST_EXECUTION_STOPPED != CurRamTstExecutionStatus)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_ALLOW_ID, RAMTST_E_STATUS_FAILURE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstExecutionStatus = RAMTST_EXECUTION_RUNNING;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_ALLOW_API_ENABLE */

#if RAMTST_SUSPEND_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for suspending current operation of background RAM Test, until RESUME is called
 * ServiceId           0x0D
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_Suspend(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_SUSPEND_ID, RAMTST_E_UNINIT);
    }
    else if (RAMTST_EXECUTION_RUNNING != CurRamTstExecutionStatus)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_SUSPEND_ID, RAMTST_E_STATUS_FAILURE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstExecutionStatus = RAMTST_EXECUTION_SUSPENDED;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_SUSPEND_API_ENABLE */

#if RAMTST_RESUME_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for allowing to continue the background RAM Test at the point is was suspended.
 * ServiceId           0x0E
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_Resume(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RESUME_ID, RAMTST_E_UNINIT);
    }
    else if (RAMTST_EXECUTION_SUSPENDED != CurRamTstExecutionStatus)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RESUME_ID, RAMTST_E_STATUS_FAILURE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstExecutionStatus = RAMTST_EXECUTION_RUNNING;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_RESUME_API_ENABLE */

#if RAMTST_GET_EXECUTION_STATUS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test execution status.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_ExecutionStatusType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(RamTst_ExecutionStatusType, RAMTST_CODE)
RamTst_GetExecutionStatus(void)
{
    RamTst_ExecutionStatusType Rtn;

    Rtn = RAMTST_EXECUTION_UNINIT;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_EXECUTION_STATUS_ID, RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        Rtn = CurRamTstExecutionStatus;
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_GET_EXECUTION_STATUS_API_ENABLE */

#if RAMTST_GET_TEST_RESULT_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test result.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_TestResultType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(RamTst_TestResultType, RAMTST_CODE)
RamTst_GetTestResult(void)
{
    RamTst_TestResultType Rtn;

    Rtn = RAMTST_RESULT_NOT_TESTED;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_TEST_RESULT_ID, RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        Rtn = RamTstOverallTestResult;
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_GET_TEST_RESULT_API_ENABLE */

#if RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test result for the specified block
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockID:Identifies the block
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_TestResultType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(RamTst_TestResultType, RAMTST_CODE)
RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID)
{
    RamTst_TestResultType Rtn;

    Rtn = RAMTST_RESULT_NOT_TESTED;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    /* SWS_RamTst_00039 */
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)
            Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_TEST_RESULT_PER_BLOCK_ID, RAMTST_E_UNINIT);
    }
    else if ((Std_ReturnType)E_NOT_OK == RamTst_CheckBlockIDIsOutOfRange(BlockID))
    {
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_GET_TEST_RESULT_PER_BLOCK_ID,
            RAMTST_E_OUT_OF_RANGE);

        Rtn = RAMTST_RESULT_UNDEFINED;
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        Rtn = RamTstAllBlockInfo[BlockID].BlockTestResult;
    }

    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE */

#if RAMTST_VERSIONINFO_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the version information of this module
 * ServiceId           0x0A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  The location address where to store the version information of this module.
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_VERSION_INFO_ID, RAMTST_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
    {
        VersionInfo->vendorID = RAMTST_VENDOR_ID;
        VersionInfo->moduleID = RAMTST_MODULE_ID;
        VersionInfo->sw_major_version = RAMTST_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = RAMTST_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = RAMTST_SW_PATCH_VERSION;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_VERSIONINFO_API_ENABLE */

#if RAMTST_GET_ALGPARAMS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the ID of the current RAM Test algorithm parameter set.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_AlgParamsIdType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(RamTst_AlgParamsIdType, RAMTST_CODE)
RamTst_GetAlgParams(void)
{
    RamTst_AlgParamsIdType Rtn;

    Rtn = 0u;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_ALG_PARAMS_ID, RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        Rtn = CurRamTstAlgParamsId;
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_GET_ALGPARAMS_API_ENABLE */

#if RAMTST_GET_TEST_ALGORITHM_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test algorithm.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_AlgorithmType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(RamTst_AlgorithmType, RAMTST_CODE)
RamTst_GetTestAlgorithm(void)
{
    RamTst_AlgorithmType Rtn;

    Rtn = RAMTST_ALGORITHM_UNDEFINED;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_TEST_ALGORITHM_ID, RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        Rtn = RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)].RamTstAlgorithm;
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_GET_TEST_ALGORITHM_API_ENABLE */

#if RAMTST_GET_NUMBER_OF_TESTED_CELLS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current number of tested cells per main-function cycle.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_NumberOfTestedCellsType:Number of currently tested cells per cycle
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(RamTst_NumberOfTestedCellsType, RAMTST_CODE)
RamTst_GetNumberOfTestedCells(void)
{
    RamTst_NumberOfTestedCellsType Rtn;

    Rtn = 0u;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_GET_NUMBER_OF_TESTED_CELLS_ID,
            RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        Rtn = RamTstNumberOfTestedCellsUse;
    }
    return Rtn;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_GET_NUMBER_OF_TESTED_CELLS_API_ENABLE */

#if RAMTST_SELECT_ALGPARAMS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service used to set the test algorithm and its parameter set.
 * ServiceId           0x0B
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NewAlgParamsId:Identifies the parameter set to be used.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_SelectAlgParams(RamTst_AlgParamsIdType NewAlgParamsId)
{
    boolean Flag;
    uint32 Index;

    Flag = FALSE;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_SELECT_ALGPARAMS_ID, RAMTST_E_UNINIT);
    }
    else if (RAMTST_EXECUTION_STOPPED != CurRamTstExecutionStatus)
    {
        (void)
            Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_SELECT_ALGPARAMS_ID, RAMTST_E_STATUS_FAILURE);
    }
    else
    {
        /* MISRC */
    }

    if (NULL_PTR != RamTstConfig)
    {
#if (1u < RAMTST_ALGPARAMS_NUM)
        for (Index = 0u; Index < RAMTST_ALGPARAMS_NUM; Index++)
#else
        Index = 0;
#endif /*1u < RAMTST_ALGPARAMS_NUM*/
        {
            if (NewAlgParamsId == RamTstConfig->RamTstConfigParams.RamTstAlgParams[Index].RamTstAlgParamsId)
            {
                Flag = TRUE;
#if (1u < RAMTST_ALGPARAMS_NUM)
                break;
#endif /*1u < RAMTST_ALGPARAMS_NUM*/
            }
        }
    }
    else
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_SELECT_ALGPARAMS_ID, RAMTST_E_PARAM_POINTER);
    }

    if (FALSE == Flag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_SELECT_ALGPARAMS_ID, RAMTST_E_OUT_OF_RANGE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstAlgParamsId = NewAlgParamsId;
        /* SWS_RamTst_00085 */
        RamTst_InitAllBlockStatus();
        RamTstNumberOfTestedCellsUse =
            RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)]
                .RamTstNumberOfTestedCells;
        NowBackTestBlockIndex = RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)]
                                    .RamTstBlockParams[0]
                                    .RamTstBlockId;
        NowAlgParamTestedBlockNumber = 0;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_SELECT_ALGPARAMS_API_ENABLE */

#if RAMTST_CHANGE_NUM_OF_TESTED_CELLS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service changes the current number of tested cells
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RamTst_NumberOfTestedCellsType
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID,
            RAMTST_E_UNINIT);
    }
    else if (RAMTST_EXECUTION_STOPPED != CurRamTstExecutionStatus)
    {
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID,
            RAMTST_E_STATUS_FAILURE);
    }
    else if (
        (RamTstConfig->RamTstConfigParams.RamTstMinNumberOfTestedCells > NewNumberOfTestedCells)
        || (RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)]
                .RamTstMaxNumberOfTestedCells
            < NewNumberOfTestedCells))
    {
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID,
            RAMTST_E_OUT_OF_RANGE);
    }
    else if (0u != (NewNumberOfTestedCells % 4u))
    {
        /* NewNumberOfTestedCells Must be Multiple of 4 */
        (void)Det_ReportError(
            RAMTST_MODULE_ID,
            RAMTST_INSTANCE_ID,
            RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID,
            RAMTST_E_OUT_OF_RANGE);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        RamTstNumberOfTestedCellsUse = NewNumberOfTestedCells;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_CHANGE_NUM_OF_TESTED_CELLS_API_ENABLE */

#if RAMTST_RUN_FULL_TEST_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for executing the full RAM Test in the foreground.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_RunFullTest(void)
{
    Std_ReturnType FullTestRet;
    uint32 FullTestIndex;
    const RamTst_AlgParams* RamTstAlgParamsPtr;
    const RamTst_BlockParams* RamTstBlockParamsPtr;
#if (RAMTST_TEST_TIMEOUT == STD_ON)
    uint32 RamTstTimeStart_Fgnd;
    uint32 RamTstTimeGap_Fgnd;
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_FULL_TEST_ID, RAMTST_E_UNINIT);
    }
    else if (RAMTST_EXECUTION_STOPPED != CurRamTstExecutionStatus)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_FULL_TEST_ID, RAMTST_E_STATUS_FAILURE);
    }
    else if (NULL_PTR == RamTstConfig)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_FULL_TEST_ID, RAMTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        CurRamTstExecutionStatus = RAMTST_EXECUTION_RUNNING;
        RamTstOverallTestResult = RAMTST_RESULT_UNDEFINED;
        RamTstAlgParamsPtr = &RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)];
        /* just test CurRamTstAlgParamsId indicate blocks */
        for (FullTestIndex = 0; FullTestIndex < RamTstAlgParamsPtr->RamTstNumberOfBlocks; (FullTestIndex)++)
        {
            RamTstBlockParamsPtr = &RamTstAlgParamsPtr->RamTstBlockParams[FullTestIndex];
            /* Only matched algorithms can be tested  */
            if (((BLOCK_TEST_USED_METHOD_FOREGROUND == RamTstBlockParamsPtr->BlockTestUseMethod)
                 || (BLOCK_TEST_USED_METHOD_BOTH == RamTstBlockParamsPtr->BlockTestUseMethod))
                && (BLOCK_HAS_TESTED != RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].IsBlockTestedFlag))
            {
                RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_UNDEFINED;
#if (RAMTST_TEST_TIMEOUT == STD_ON)
                RamTstTimeStart_Fgnd = Frt_ReadOutMS();
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */
                FullTestRet = RamTst_ExecTest(
                    RamTstAlgParamsPtr->RamTstAlgorithm,
                    RamTstBlockParamsPtr->RamTstStartAddress,
                    RamTstBlockParamsPtr->RamTstEndAddress,
                    RamTstBlockParamsPtr->RamTstFillPattern,
                    RamTstBlockParamsPtr->RamTstTestPolicy);
#if (RAMTST_TEST_TIMEOUT == STD_ON)
                RamTstTimeGap_Fgnd = Frt_CalculateElapsedMS(RamTstTimeStart_Fgnd);
                if (RamTstTimeGap_Fgnd >= RAMTST_TIMEOUTVALUE_FGND)
                {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
                    (void)Det_ReportError(
                        RAMTST_MODULE_ID,
                        RAMTST_INSTANCE_ID,
                        RAMTST_RUN_FULL_TEST_ID,
                        RAMTST_E_TIMEOUT);
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
                    RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
                    break;
                }
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */

                /* check test result */
                if (E_OK == FullTestRet)
                {
                    RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_OK;
                }
                else
                {
                    RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
                    RamTstOverallTestResult = RAMTST_RESULT_NOT_OK;
#if RAMTST_DEM_RUNFL_RAM_FAILURE_ENABLE
                    /* report the production error RAMTST_RUNFL_RAM_FAILURE to the DEM */
                    Dem_ReportErrorStatus(RAMTST_RUNFL_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif /* RAMTST_DEM_RUNFL_RAM_FAILURE_ENABLE */
                }
                RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].IsBlockTestedFlag = BLOCK_HAS_TESTED;
            }
        }
        RamTst_UpdateOverallTestResult();
    }
    /* set it back to RAMTST_EXECUTION_STOPPED before returning */
    CurRamTstExecutionStatus = RAMTST_EXECUTION_STOPPED;
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_RUN_FULL_TEST_API_ENABLE */

#if RAMTST_RUN_PARTIAL_TEST_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for testing one RAM block in the foreground
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId锛欼dentifies the single RAM block to be tested in the selected set of RamTstAlgParams.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_RunPartialTest(RamTst_NumberOfBlocksType BlockId)
{
    RamTst_ExecutionStatusType PartialTestStatus;
    Std_ReturnType PartialTestRet;
    const RamTst_AlgParams* RamTstAlgParamsPtr;
    const RamTst_BlockParams* RamTstBlockParamsPtr;
#if (RAMTST_TEST_TIMEOUT == STD_ON)
    uint32 RamTstTimeStart_Fgnd;
    uint32 RamTstTimeGap_Fgnd;
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_PARTIAL_TEST_ID, RAMTST_E_UNINIT);
    }
    else if (
        (RAMTST_EXECUTION_STOPPED != CurRamTstExecutionStatus)
        && (RAMTST_EXECUTION_SUSPENDED != CurRamTstExecutionStatus))
    {
        (void)
            Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_PARTIAL_TEST_ID, RAMTST_E_STATUS_FAILURE);
    }
    else if ((Std_ReturnType)E_NOT_OK == RamTst_CheckBlockIDIsOutOfRange(BlockId))
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_PARTIAL_TEST_ID, RAMTST_E_OUT_OF_RANGE);
    }
    else if (NULL_PTR == RamTstConfig)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_PARTIAL_TEST_ID, RAMTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        /* save the old test exec status */
        PartialTestStatus = CurRamTstExecutionStatus;
        CurRamTstExecutionStatus = RAMTST_EXECUTION_RUNNING;

        RamTstAlgParamsPtr = &RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)];
        RamTstBlockParamsPtr = &RamTstAlgParamsPtr->RamTstBlockParams[(uint16)(BlockId - 1u)];
        if ((BLOCK_HAS_TESTED != RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].IsBlockTestedFlag)
            && ((BLOCK_TEST_USED_METHOD_FOREGROUND == RamTstBlockParamsPtr->BlockTestUseMethod)
                || (BLOCK_TEST_USED_METHOD_BOTH == RamTstBlockParamsPtr->BlockTestUseMethod)))
        {
            /* Only matched algorithms can be tested  */
            RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_UNDEFINED;
            /* just test CurRamTstAlgParamsId indicate blocks */
#if (RAMTST_TEST_TIMEOUT == STD_ON)
            RamTstTimeStart_Fgnd = Frt_ReadOutMS();
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */
            PartialTestRet = RamTst_ExecTest(
                RamTstAlgParamsPtr->RamTstAlgorithm,
                RamTstBlockParamsPtr->RamTstStartAddress,
                RamTstBlockParamsPtr->RamTstEndAddress,
                RamTstBlockParamsPtr->RamTstFillPattern,
                RamTstBlockParamsPtr->RamTstTestPolicy);

            if (E_OK == PartialTestRet)
            {
                RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_OK;
            }
            else
            {
                RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
                RamTstOverallTestResult = RAMTST_RESULT_NOT_OK;
#if RAMTST_DEM_PART_RAM_FAILURE_ENABLE
                /*  report the production error RAMTST_PART_RAM_FAILURE to the DEM */
                Dem_ReportErrorStatus(RAMTST_PART_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif /* RAMTST_DEM_PART_RAM_FAILURE_ENABLE */
            }

            RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].IsBlockTestedFlag = BLOCK_HAS_TESTED;
#if (RAMTST_TEST_TIMEOUT == STD_ON)
            RamTstTimeGap_Fgnd = Frt_CalculateElapsedMS(RamTstTimeStart_Fgnd);
            if (RamTstTimeGap_Fgnd >= RAMTST_TIMEOUTVALUE_FGND)
            {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
                (void)
                    Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_RUN_PARTIAL_TEST_ID, RAMTST_E_TIMEOUT);
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
                RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
            }
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */
        }
        /* set it back to the previous state (the state when the function was called) */
        CurRamTstExecutionStatus = PartialTestStatus;
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_RUN_PARTIAL_TEST_API_ENABLE */

/*************************************************************************/
/* RamTst
 * Brief               Scheduled function for executing the RAM Test in the background
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_MainFunction(void)
{
    boolean AllTestDone;
    boolean TestCondMatch;
    Std_ReturnType RamTestFlag;
    const RamTst_AlgParams* RamTstAlgParamsPtr;
    const RamTst_BlockParams* RamTstBlockParamsPtr;
#if (RAMTST_TEST_TIMEOUT == STD_ON)
    uint32 RamTstTimeStart_Bgnd;
    uint32 RamTstTimeGap_Bgnd;
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */

    AllTestDone = FALSE;
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_INIT_NOT == RamTstDeInitFlag)
    {
        (void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, RAMTST_GET_MAIN_FUNCTION_ID, RAMTST_E_UNINIT);
    }
    else
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
    {
        RamTstAlgParamsPtr = &RamTstConfig->RamTstConfigParams.RamTstAlgParams[(uint8)(CurRamTstAlgParamsId - 1u)];

        if (RAMTST_EXECUTION_RUNNING == CurRamTstExecutionStatus)
        {
            TestCondMatch = RamTst_CheckBgndCondIsValid();
            /* match the test conditions */
            if (TRUE == TestCondMatch)
            {
                RamTstBlockParamsPtr = &RamTstAlgParamsPtr->RamTstBlockParams[(uint16)(NowAlgParamTestedBlockNumber)];
                /* calc now test final addr */
                if ((RamTstBlockParamsPtr->RamTstEndAddress)
                    < (uint32)(RamTstAllBlockInfo[NowBackTestBlockIndex].FinalEndAddr + RamTstNumberOfTestedCellsUse
                               - 1u))
                {
                    RamTstAllBlockInfo[NowBackTestBlockIndex].FinalEndAddr = RamTstBlockParamsPtr->RamTstEndAddress;
                }
                else
                {
                    RamTstAllBlockInfo[NowBackTestBlockIndex].FinalEndAddr +=
                        (uint32)(RamTstNumberOfTestedCellsUse - 1u);
                }
                /* exec test */
                RamTstAllBlockInfo[NowBackTestBlockIndex].BlockTestResult = RAMTST_RESULT_UNDEFINED;

#if (RAMTST_TEST_TIMEOUT == STD_ON)
                RamTstTimeStart_Bgnd = Frt_ReadOutMS();
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */
                RamTestFlag = RamTst_ExecTest(
                    RamTstAlgParamsPtr->RamTstAlgorithm,
                    RamTstAllBlockInfo[NowBackTestBlockIndex].NowTestedAddr,
                    RamTstAllBlockInfo[NowBackTestBlockIndex].FinalEndAddr,
                    RamTstBlockParamsPtr->RamTstFillPattern,
                    RamTstAllBlockInfo[NowBackTestBlockIndex].TestPolicyUse);

                if (E_NOT_OK == RamTestFlag)
                {
                    RamTstAllBlockInfo[NowBackTestBlockIndex].BlockTestResult = RAMTST_RESULT_NOT_OK;
                    RamTstOverallTestResult = RAMTST_RESULT_NOT_OK;
                    /* has error, this block not test continue */
                    if ((uint16)(NowAlgParamTestedBlockNumber + 1u) <= RamTstAlgParamsPtr->RamTstNumberOfBlocks)
                    {
                        NowAlgParamTestedBlockNumber++;
                        NowBackTestBlockIndex++;
                    }
                    else
                    {
                        AllTestDone = TRUE;
                    }
                    RamTstAllBlockInfo[NowBackTestBlockIndex].IsBlockTestedFlag = BLOCK_HAS_TESTED;
                    /*  report the production error RAMTST_MAIN_RAM_FAILURE to the DEM */
#if RAMTST_DEM_MAIN_RAM_FAILURE_ENABLE
                    Dem_ReportErrorStatus(RAMTST_MAIN_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif /* RAMTST_DEM_MAIN_RAM_FAILURE_ENABLE */
#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
                    RamTst_ErrorNotification();
#endif /* RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON */
                }

#if (RAMTST_TEST_TIMEOUT == STD_ON)
                RamTstTimeGap_Bgnd = Frt_CalculateElapsedMS(RamTstTimeStart_Bgnd);
                if (RamTstTimeGap_Bgnd >= RAMTST_TIMEOUTVALUE_BGND)
                {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
                    (void)Det_ReportError(
                        RAMTST_MODULE_ID,
                        RAMTST_INSTANCE_ID,
                        RAMTST_GET_MAIN_FUNCTION_ID,
                        RAMTST_E_TIMEOUT);
#endif /* STD_ON == RAMTST_DEV_ERROR_DETECT */
                    RamTstAllBlockInfo[RamTstBlockParamsPtr->RamTstBlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
                }
#endif /* RAMTST_TEST_TIMEOUT == STD_ON */

                /* check if test over */
                if ((RAMTST_RESULT_NOT_OK != RamTstAllBlockInfo[NowBackTestBlockIndex].BlockTestResult)
                    && (RamTstAllBlockInfo[NowBackTestBlockIndex].FinalEndAddr
                        == RamTstBlockParamsPtr->RamTstEndAddress))
                {
                    /* now block has test over */
                    RamTstAllBlockInfo[NowBackTestBlockIndex].BlockTestResult = RAMTST_RESULT_OK;
                    RamTstAllBlockInfo[NowBackTestBlockIndex].IsBlockTestedFlag = BLOCK_HAS_TESTED;
                    if ((uint16)(NowAlgParamTestedBlockNumber + 1u) <= RamTstAlgParamsPtr->RamTstNumberOfBlocks)
                    {
                        NowAlgParamTestedBlockNumber++;
                        NowBackTestBlockIndex++;
                    }
                    else
                    {
                        AllTestDone = TRUE;
                    }
                }
                if (BLOCK_HAS_TESTED != RamTstAllBlockInfo[NowBackTestBlockIndex].IsBlockTestedFlag)
                {
                    /* calc next test start addr */
                    RamTstAllBlockInfo[NowBackTestBlockIndex].NowTestedAddr += RamTstNumberOfTestedCellsUse;
                    RamTstAllBlockInfo[NowBackTestBlockIndex].FinalEndAddr += (uint32)(1);
                }
                RamTst_UpdateOverallTestResult();
            }
            else
            {
                /* this block not match the test conditions  or this block has tested, test next block */
                if ((uint16)(NowAlgParamTestedBlockNumber + 1u) <= RamTstAlgParamsPtr->RamTstNumberOfBlocks)
                {
                    NowAlgParamTestedBlockNumber++;
                    NowBackTestBlockIndex++;
                }
                else
                {
                    AllTestDone = TRUE;
                }
            }
        }
        /* check if all test has finished */
        if (TRUE == AllTestDone)
        {
            /* Once you've tested over, start over from the first block */
            NowBackTestBlockIndex = RamTstAlgParamsPtr->RamTstBlockParams[0].RamTstBlockId;
            NowAlgParamTestedBlockNumber = 0;
#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
            RamTst_TestCompletedNotification();
#endif /* RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON */
            RamTst_InitAllBlockStatus();
        }
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
/*************************************************************************/
/* RamTst
 * Brief               The function RamTst_TestCompleted shall be called every time when all RAM
                        blocks of the current test configuration have been tested in the background
 * Sync/Async          --
 * Reentrancy          Dont care
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_TestCompletedNotification(void)
{
    if (NULL_PTR != RamTstConfig->RamTstConfigParams.RamTst_TestCompletedNotificationPtr)
    {
        RamTstConfig->RamTstConfigParams.RamTst_TestCompletedNotificationPtr();
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON */

#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
/*************************************************************************/
/* RamTst
 * Brief               The function RamTst_Error shall be called every time when a RAM failure has
                        been detected by the selected test algorithm in the background.
 * Sync/Async          --
 * Reentrancy          Dont care
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
FUNC(void, RAMTST_CODE)
RamTst_ErrorNotification(void)
{
    if (NULL_PTR != RamTstConfig->RamTstConfigParams.RamTst_ErrorNotificationPtr)
    {
        RamTstConfig->RamTstConfigParams.RamTst_ErrorNotificationPtr();
    }
}
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
#endif /* RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON */
