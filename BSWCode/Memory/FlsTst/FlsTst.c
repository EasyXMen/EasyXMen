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
**  FILENAME    : FlsTst.c                                                    **
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
    ModeName:FlsTst<br>
  RuleSorce:puhua-rule.rcf 2.3.5

   \li PRQA S 0306 MISRA Rule 11.4 .<br>Reason:Cast between a pointer to object and an integral type.

    Reason: autosar require.
 */

/*==============================[I N C L U D E S]=============================*/
#include "FlsTst.h"
#include "SchM_FlsTst.h"
#include "Dem.h"
#include "Crc.h"
#if (FLSTST_TIMEOUT == STD_ON)
#include "FreeRTimer.h"
#endif /* FLSTST_TIMEOUT == STD_ON */
/*============================================================================*/

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#define FLSTST_AR_RELEASE_MAJOR_VERSION 4U
#define FLSTST_AR_RELEASE_MINOR_VERSION 5U
#define FLSTST_AR_RELEASE_PATCH_VERSION 0U
#define FLSTST_SW_MAJOR_VERSION         2U
#define FLSTST_SW_MINOR_VERSION         0U
#define FLSTST_SW_PATCH_VERSION         1U
/*============================================================================*/

/*==========================[V E R S I O N  C H E C K]========================*/
#ifndef FLSTST_AR_RELEASE_MAJOR_VERSION
#error " FLSTST version miss"
#endif
#ifndef FLSTST_AR_RELEASE_MINOR_VERSION
#error " FLSTST version miss"
#endif
#ifndef FLSTST_AR_RELEASE_PATCH_VERSION
#error " FLSTST version miss"
#endif
#if ((2 != FLSTST_CFG_SW_MAJOR_VERSION) || (0 != FLSTST_CFG_SW_MINOR_VERSION) || (1 != FLSTST_CFG_SW_PATCH_VERSION))
#error " cfg version mismatching"
#endif

/*============================================================================*/

/*======================[F U N C  D E C L A R A T I O N]======================*/
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"

#if (0 != FLSTST_BGND_BLOCK_NUM)
/* Calculation with a length of one Flash Cell */
static FUNC(void, FLSTST_CODE) FlsTst_CalcAtomic(void);
/* calculating Flash cells in background mode */
static FUNC(void, FLSTST_CODE) FlsTst_InterTest(void);
/* called when a memmory block test finished */
static FUNC(void, FLSTST_CODE) FlsTst_BlockTestFinish(void);
/* Copy information from FlsTst_BlockBgndInfo to FlsTst_CurRunning */
static FUNC(void, FLSTST_CODE) FlsTst_CopyBlockInfo(FlsTst_BlockIdType FlsTstBlockID);
#endif /* 0 != FLSTST_BGND_BLOCK_NUM */

#if (0 != FLSTST_FGND_BLOCK_NUM)
/* called by FlsTst_StartFgnd */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_StartFgndInterTest(FlsTst_BlockInfoType* BlockInfoPtr);
/* CRC32 calculation of the input block parameters in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_CRC32Fgnd(FlsTst_BlockInfoType* BlockInfoPtr);
/* CRC16 calculation of the input block parameters in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_CRC16Fgnd(FlsTst_BlockInfoType* BlockInfoPtr);
/* CRC8 calculation of the input block parameters in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_CRC8Fgnd(FlsTst_BlockInfoType* BlockInfoPtr);
/* Checksum calculation of the input block parameters in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_ChecksumFgnd(FlsTst_BlockInfoType* BlockInfoPtr);
/* ECC calculation of the input block parameters in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_ECCTestFgnd(FlsTst_BlockInfoType* BlockInfoPtr);
/* DuplicatedMemory calculation of the input block parameters in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_DuplicatedMemoryFgnd(FlsTst_BlockInfoType* BlockInfoPtr);
#endif /* 0 != FLSTST_FGND_BLOCK_NUM */

#if ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM))
/* Services to data comparison in foreground */
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_MemCompare(
    P2CONST(uint8, AUTOMATIC, FLSTST_APPL_CONST) SourceDataPtr,
    P2CONST(uint8, AUTOMATIC, FLSTST_APPL_CONST) DstDataPtr,
    VAR(uint32, AUTOMATIC) Length);
/* Services to CheckSum */
static FUNC(uint32, FLSTST_CODE) FlsTst_CheckSum(
    P2CONST(uint8, AUTOMATIC, FLSTST_APPL_CONST) Checksum_DataPtr,
    VAR(uint32, AUTOMATIC) Checksum_Length,
    VAR(uint32, AUTOMATIC) Checksum_StartValue,
    VAR(boolean, AUTOMATIC) Checksum_IsFirstCall);
#endif /* 0U != FLSTST_BGND_BLOCK_NUM || 0U != FLSTST_FGND_BLOCK_NUM */

#if (STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT)
/* Called every time when a complete test cycle had been tested */
static FUNC(void, FLSTST_CODE) FlsTst_TestCompletedNotification(void);
#endif /* STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT */

#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
/*============================================================================*/

/*=============================[V A R I A N T S]==============================*/
#define FLSTST_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "FlsTst_MemMap.h"
#if (0 == FLSTST_BGND_BLOCK_NUM)
static P2VAR(FlsTst_BlockInfoType, AUTOMATIC, FLSTST_APPL_DATA) FlsTst_BlockBgndInfo;
#else
static VAR(FlsTst_BlockInfoType, FLSTST_APPL_DATA) FlsTst_BlockBgndInfo[FLSTST_BGND_BLOCK_NUM];
static VAR(FlsTst_CurRunningType, FLSTST_APPL_DATA) FlsTst_CurRunning;
#endif /* 0 == FLSTST_BGND_BLOCK_NUM */

#if (0 == FLSTST_FGND_BLOCK_NUM)
static P2VAR(FlsTst_BlockInfoType, AUTOMATIC, FLSTST_APPL_DATA) FlsTst_BlockFgndInfo;
#else
static VAR(FlsTst_BlockInfoType, FLSTST_APPL_DATA) FlsTst_BlockFgndInfo[FLSTST_FGND_BLOCK_NUM];
#endif /* 0 == FLSTST_FGND_BLOCK_NUM */

static VAR(FlsTst_ModuleTypes, FLSTST_APPL_DATA) FlsTst_Module;

#if (STD_ON == FLSTST_INIT_ECC)
static VAR(FlsTst_ECCInitType, FLSTST_APPL_DATA) FlsTst_ECCInitInfo;
#endif /* STD_ON == FLSTST_INIT_ECC */

#define FLSTST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "FlsTst_MemMap.h"

/*============================================================================*/

/*================[F U N C T I O N   I M P L E M E N T A T I O N S]===========*/
/*************************************************************************/
/*
 * Brief               Service for Flash Test initialization.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to configuration set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* req FlsTst_00017  */
/* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* FLSTST_CAST_OPERATORS_001 */
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_Init(P2CONST(FlsTst_ConfigType, AUTOMATIC, FLSTST_APPL_CONST) ConfigPtr)
{
    uint8 MemBlockIndex;
    uint32 TempSignature;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_INIT_ID, FLSTST_E_INIT_FAILED);
    }
    else if (FLSTST_UNINIT != FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_INIT_ID, FLSTST_E_ALREADY_INITIALIZED);
    }
    else
#endif STD_ON == FLSTST_DEV_ERROR_DETECT* /
    {
        SchM_Enter_FlsTst_Queue();
#if (0 != FLSTST_BGND_BLOCK_NUM)
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (MemBlockIndex = 0; MemBlockIndex < FLSTST_BGND_BLOCK_NUM; MemBlockIndex++)
#else
        MemBlockIndex = 0;
#endif /* 1u < FLSTST_BGND_BLOCK_NUM */
        {
            FlsTst_BlockBgndInfo[MemBlockIndex].MemoryBlockID =
                ConfigPtr->FlsTst_BgndBlockPtr[MemBlockIndex].BgndBlockIndex;
            FlsTst_BlockBgndInfo[MemBlockIndex].TestAlgorithm =
                ConfigPtr->FlsTst_BgndBlockPtr[MemBlockIndex].TestAlgorithm;
            FlsTst_BlockBgndInfo[MemBlockIndex].MemBlcokStartAddr =
                ConfigPtr->FlsTst_BgndBlockPtr[MemBlockIndex].BlockBaseAddress;
            FlsTst_BlockBgndInfo[MemBlockIndex].MemBlcokLength =
                ConfigPtr->FlsTst_BgndBlockPtr[MemBlockIndex].BlcokLength;
            /*Duplicated memory start address*/
            FlsTst_BlockBgndInfo[MemBlockIndex].DuplicateAddr =
                ConfigPtr->FlsTst_BgndBlockPtr[MemBlockIndex].DuplicateAddr;
            TempSignature = ConfigPtr->FlsTst_BgndBlockPtr[MemBlockIndex].SignatureAddress;
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatureStored = ((uint32)(*(uint8*)(TempSignature)) << 24u);
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatureStored += ((uint32)(*(uint8*)(TempSignature + 1U)) << 16u);
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatureStored += ((uint32)(*(uint8*)(TempSignature + 2U)) << 8u);
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatureStored += (uint32)(*(uint8*)(TempSignature + 3U));
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatueCalculated = 0u;
            FlsTst_BlockBgndInfo[MemBlockIndex].TestResult = FLSTST_RESULT_NOT_TESTED;
        }
#endif /* 0 != FLSTST_BGND_BLOCK_NUM */

#if (0 != FLSTST_FGND_BLOCK_NUM)
#if (1u < FLSTST_FGND_BLOCK_NUM)
        for (MemBlockIndex = 0; MemBlockIndex < FLSTST_FGND_BLOCK_NUM; MemBlockIndex++)
#else
        MemBlockIndex = 0;
#endif /* 1u < FLSTST_FGND_BLOCK_NUM */
        {
            FlsTst_BlockFgndInfo[MemBlockIndex].MemoryBlockID =
                ConfigPtr->FlsTst_FgndBlockPtr[MemBlockIndex].BgndBlockIndex;
            FlsTst_BlockFgndInfo[MemBlockIndex].TestAlgorithm =
                ConfigPtr->FlsTst_FgndBlockPtr[MemBlockIndex].TestAlgorithm;
            FlsTst_BlockFgndInfo[MemBlockIndex].MemBlcokStartAddr =
                ConfigPtr->FlsTst_FgndBlockPtr[MemBlockIndex].BlockBaseAddress;
            FlsTst_BlockFgndInfo[MemBlockIndex].MemBlcokLength =
                ConfigPtr->FlsTst_FgndBlockPtr[MemBlockIndex].BlcokLength;
            /*Duplicated memory start address*/
            FlsTst_BlockFgndInfo[MemBlockIndex].DuplicateAddr =
                ConfigPtr->FlsTst_FgndBlockPtr[MemBlockIndex].DuplicateAddr;
            TempSignature = ConfigPtr->FlsTst_FgndBlockPtr[MemBlockIndex].SignatureAddress;
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatureStored = ((uint32)(*(uint8*)(TempSignature)) << 24u);
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatureStored += ((uint32)(*(uint8*)(TempSignature + 1U)) << 16u);
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatureStored += ((uint32)(*(uint8*)(TempSignature + 2U)) << 8u);
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatureStored += (uint32)(*(uint8*)(TempSignature + 3U));
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatueCalculated = 0u;
            FlsTst_BlockFgndInfo[MemBlockIndex].TestResult = FLSTST_RESULT_NOT_TESTED;
        }
#endif /* 0 != FLSTST_FGND_BLOCK_NUM */

#if (STD_ON == FLSTST_INIT_ECC)
        FlsTst_ECCInitInfo.FlsTst_ECCInitPtr = ConfigPtr->FlsTst_ECCInitFunc;
        FlsTst_ECCInitInfo.FlsTst_ECCDeinitPtr = ConfigPtr->FlsTst_ECCDeInitFunc;
        FlsTst_ECCInitInfo.FlsTst_ECCInitPtr();
#endif /* STD_ON == FLSTST_INIT_ECC */
        FlsTst_Module.State = FLSTST_INIT;
        FlsTst_Module.ErrorInfo.ErrorBlockID = 0xFFu;
        FlsTst_Module.ErrorInfo.Algorithm = FLSTST_16BIT_CRC;
        FlsTst_Module.ErrorInfo.SignatureResult = 0U;
        FlsTst_Module.UserCallBack = ConfigPtr->FlsTstUserCallBack;
        FlsTst_Module.FlsTstECCTestFunction = ConfigPtr->FlsTst_ECCTestFunc;
        FlsTst_Module.IntervalID = 0U;
        FlsTst_Module.BgndResult = FLSTST_RESULT_NOT_TESTED;
        FlsTst_Module.FgndResult = FLSTST_NOT_TESTED;
        SchM_Exit_FlsTst_Queue();
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
/*************************************************************************/
/*
 * Brief               Service for Flash Test De-Initialization.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_DeInit(void)
{
    uint8 MemBlockIndex;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_DEINIT_ID, FLSTST_E_STATE_FAILURE);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        SchM_Enter_FlsTst_Queue();
#if (0 != FLSTST_BGND_BLOCK_NUM)
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (MemBlockIndex = 0; MemBlockIndex < FLSTST_BGND_BLOCK_NUM; MemBlockIndex++)
#else
        MemBlockIndex = 0;
#endif /* 1u < FLSTST_BGND_BLOCK_NUM */
        {
            FlsTst_BlockBgndInfo[MemBlockIndex].MemoryBlockID = 0u;
            FlsTst_BlockBgndInfo[MemBlockIndex].TestAlgorithm = FLSTST_16BIT_CRC;
            FlsTst_BlockBgndInfo[MemBlockIndex].MemBlcokStartAddr = 0u;
            FlsTst_BlockBgndInfo[MemBlockIndex].MemBlcokLength = 0u;
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatureStored = 0u;
            FlsTst_BlockBgndInfo[MemBlockIndex].SignatueCalculated = 0u;
            FlsTst_BlockBgndInfo[MemBlockIndex].TestResult = FLSTST_RESULT_NOT_TESTED;
        }
#endif /* 0 != FLSTST_BGND_BLOCK_NUM */

#if (0 != FLSTST_FGND_BLOCK_NUM)
#if (1u < FLSTST_FGND_BLOCK_NUM)
        for (MemBlockIndex = 0; MemBlockIndex < FLSTST_FGND_BLOCK_NUM; MemBlockIndex++)
#else
        MemBlockIndex = 0;
#endif /* 1u < FLSTST_FGND_BLOCK_NUM */
        {
            FlsTst_BlockFgndInfo[MemBlockIndex].MemoryBlockID = 0u;
            FlsTst_BlockFgndInfo[MemBlockIndex].TestAlgorithm = FLSTST_16BIT_CRC;
            FlsTst_BlockFgndInfo[MemBlockIndex].MemBlcokStartAddr = 0u;
            FlsTst_BlockFgndInfo[MemBlockIndex].MemBlcokLength = 0u;
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatureStored = 0u;
            FlsTst_BlockFgndInfo[MemBlockIndex].SignatueCalculated = 0u;
            FlsTst_BlockFgndInfo[MemBlockIndex].TestResult = FLSTST_RESULT_NOT_TESTED;
        }
#endif /* 0 != FLSTST_FGND_BLOCK_NUM */

#if (STD_ON == FLSTST_INIT_ECC)
        FlsTst_ECCInitInfo.FlsTst_ECCDeinitPtr();
#endif /* STD_ON == FLSTST_INIT_ECC */
        FlsTst_Module.State = FLSTST_UNINIT;
        SchM_Exit_FlsTst_Queue();
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service for executing foreground Flash Test.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FgndBlockId: Number of the foreground test to be
 *                     executed. This is dependent on configuration.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                       E_OK: Foreground test processed
 *                       E_NOT_OK: Foreground test not accepte
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_START_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(Std_ReturnType, FLSTST_CODE)
FlsTst_StartFgnd(FlsTst_BlockIdFgndType FgndBlockId)
{
    Std_ReturnType testResult;

    testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_UNINIT);
    }
    else if (FgndBlockId >= FLSTST_FGND_BLOCK_NUM)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        testResult = FlsTst_StartFgndInterTest(&FlsTst_BlockFgndInfo[FgndBlockId]);
    }
    return testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_START_FGND_API && 0 != FLSTST_FGND_BLOCK_NUM */

/*************************************************************************/
/*
 * Brief               Service for aborting the Flash Test.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (0U != FLSTST_BGND_BLOCK_NUM)
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_Abort(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_ABORT_ID, FLSTST_E_UNINIT);
    }
    else if (FLSTST_ABORTED == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_ABORT_ID, FLSTST_E_STATE_FAILURE);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        FlsTst_Module.State = FLSTST_ABORTED;
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* 0U != FLSTST_BGND_BLOCK_NUM */
/*************************************************************************/
/*
 * Brief               Service for suspending current operation of the
 *                     Flash Test, until FlsTst_Resume is called.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0U != FLSTST_BGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_Suspend(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_SUSPEND_ID, FLSTST_E_UNINIT);
    }
    else if (FLSTST_ABORTED == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_SUSPEND_ID, FLSTST_E_STATE_FAILURE);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        FlsTst_Module.State = FLSTST_SUSPENDED;
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_SUSPEND_RESUME_API && 0U != FLSTST_BGND_BLOCK_NUM */

/*************************************************************************/
/*
 * Brief               Service for continuing the Flash Test at the point
 *                     it was suspended.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0U != FLSTST_BGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_Resume(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_RESUME_ID, FLSTST_E_UNINIT);
    }
    else if (FLSTST_SUSPENDED != FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_RESUME_ID, FLSTST_E_STATE_FAILURE);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        FlsTst_Module.State = FLSTST_RUNNING;
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_SUSPEND_RESUME_API && 0U != FLSTST_BGND_BLOCK_NUM */
/*************************************************************************/
/*
 * Brief               Service for continuing the Flash Test at the point
 *                     it was suspended.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_StateType
 *                  FLSTST_UNINIT:
 *                     The Flash Test is not initialized or not usable.
 *                  FLSTST_INIT:
 *                     The Flash Test is initialized and ready to be started.
 *                  FLSTST_RUNNING:
 *                     The Flash Test is currently running.
 *                  FLSTST_ABORTED锛�
 *                     The Flash Test is aborted.
 *                  FLSTST_SUSPENDED:
 *                     The Flash Test is waiting to be resumed or is waiting
 *                     to start forground mode test.
 * PreCondition        None
 */
/*************************************************************************/
#if (STD_ON == FLSTST_GET_CURRENT_STATE_API)
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(FlsTst_StateType, FLSTST_CODE)
FlsTst_GetCurrentState(void)
{
    return FlsTst_Module.State;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_GET_CURRENT_STATE_API */
/*************************************************************************/
/*
 * Brief               Service returns the Background Flash Test result.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestResultBgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_RESULT_BGND_API) && (0 != FLSTST_BGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(FlsTst_TestResultBgndType, FLSTST_CODE)
FlsTst_GetTestResultBgnd(void)
{
    FlsTst_TestResultBgndType TestResultBgnd = {0, FLSTST_RESULT_NOT_TESTED};

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_GETTESTRESULTBGND_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestResultBgnd.FlsTstIntervalID = FlsTst_Module.IntervalID;
        TestResultBgnd.result = FlsTst_Module.BgndResult;
    }

    return TestResultBgnd;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_GET_TEST_RESULT_BGND_API && 0 != FLSTST_BGND_BLOCK_NUM */

/*************************************************************************/
/*
 * Brief               Service returns the Foreground Flash Test result.
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestResultFgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_RESULT_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(FlsTst_TestResultFgndType, FLSTST_CODE)
FlsTst_GetTestResultFgnd(void)
{
    FlsTst_TestResultFgndType TestResultFgnd = FLSTST_NOT_TESTED;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_GETTESTRESULTFGND_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestResultFgnd = FlsTst_Module.FgndResult;
    }
    return TestResultFgnd;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_GET_TEST_RESULT_FGND_API && 0 != FLSTST_FGND_BLOCK_NUM */
/*************************************************************************/
/*
 * Brief               Service returns the version information of this module.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo:Pointer to where to store the version
 *                     information of this module.
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (STD_ON == FLSTST_VERSION_INFO_API)
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_GetVersionInfo(Std_VersionInfoType* versionInfo)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_GETVERSIONINFO_ID, FLSTST_E_UNINIT);
    }
    else if (NULL_PTR == versionInfo)
    {
        FLSTST_DET_REPORTERROR(FLSTST_GETVERSIONINFO_ID, FLSTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        (versionInfo)->vendorID = (uint16)FLSTST_VENDOR_ID;
        (versionInfo)->moduleID = (uint8)FLSTST_MODULE_ID;
        (versionInfo)->sw_major_version = (uint8)FLSTST_SW_MAJOR_VERSION;
        (versionInfo)->sw_minor_version = (uint8)FLSTST_SW_MINOR_VERSION;
        (versionInfo)->sw_patch_version = (uint8)FLSTST_SW_PATCH_VERSION;
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_VERSION_INFO_API */

/*************************************************************************/
/*
 * Brief               Service returns the Flash Test result in background
 *                     mode.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestSignatureBgndType
 * PreCondition        None
 */
/*************************************************************************/

#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_BGND_API) && (0 != FLSTST_BGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(FlsTst_TestSignatureBgndType, FLSTST_CODE)
FlsTst_GetTestSignatureBgnd(void)
{
    FlsTst_TestSignatureBgndType TestSignatureBgnd;
    FlsTst_BlockIdType MemBlockIndex;

    TestSignatureBgnd.BgndSignature = 0;
    TestSignatureBgnd.FlsTstIntervalID = 0;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        for (MemBlockIndex = 0; MemBlockIndex < FLSTST_BGND_BLOCK_NUM; MemBlockIndex++)
        {
            if (FLSTST_RESULT_OK == FlsTst_BlockBgndInfo[MemBlockIndex].TestResult)
            {
                /* sum of Signature of all blocks */
                TestSignatureBgnd.BgndSignature += FlsTst_BlockBgndInfo[MemBlockIndex].SignatueCalculated;
            }
            else
            {
                /*empty*/
            }
        }
        TestSignatureBgnd.FlsTstIntervalID = FlsTst_Module.IntervalID;
    }
    return TestSignatureBgnd;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_GET_TEST_SIGNATURE_BGND_API && 0 != FLSTST_BGND_BLOCK_NUM */

/*************************************************************************/
/*
 * Brief               Service returns the Flash Test result in background
 *                     mode.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_TestSignatureFgndType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_FGND_API) && (0 != FLSTST_FGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(FlsTst_TestSignatureFgndType, FLSTST_CODE)
FlsTst_GetTestSignatureFgnd(void)
{
    FlsTst_TestSignatureFgndType TestSignatureFgnd;
    FlsTst_BlockIdType MemBlockIndex;

    TestSignatureFgnd.FgndSignature = 0;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_GETTESTSIGNATUREFGND_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (MemBlockIndex = 0; MemBlockIndex < FLSTST_BGND_BLOCK_NUM; MemBlockIndex++)
#else
        MemBlockIndex = 0;
#endif /* 1u < FLSTST_BGND_BLOCK_NUM */
        {
            if (FLSTST_RESULT_OK == FlsTst_BlockFgndInfo[MemBlockIndex].TestResult)
            {
                /* sum of Signature of all blocks */
#if (1u < FLSTST_BGND_BLOCK_NUM)
                TestSignatureFgnd.FgndSignature += FlsTst_BlockFgndInfo[MemBlockIndex].SignatueCalculated;
#else
                TestSignatureFgnd.FgndSignature = FlsTst_BlockFgndInfo[MemBlockIndex].SignatueCalculated;
#endif /* 1u < FLSTST_BGND_BLOCK_NUM */
            }
            else
            {
                /* empty */
            }
        }
    }
    return TestSignatureFgnd;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_GET_TEST_SIGNATURE_FGND_API && 0 != FLSTST_FGND_BLOCK_NUM */

/*************************************************************************/
/*
 * Brief               Service returns error detais monitored from the
 *                     Flash module.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              FlsTst_ErrorDetailsType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_GET_ERRORD_ETAILS_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM)))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(FlsTst_ErrorDetailsType, FLSTST_CODE)
FlsTst_GetErrorDetails(void)
{
    FlsTst_ErrorDetailsType ErrDetails = {0, FLSTST_16BIT_CRC, 0};

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_GETERRORDETAILS_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        ErrDetails = FlsTst_Module.ErrorInfo;
    }
    return ErrDetails;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_GET_ERRORD_ETAILS_API && \
    0U != FLSTST_BGND_BLOCK_NUM || 0U != FLSTST_FGND_BLOCK_NUM */

/*************************************************************************/
/*
 * Brief               Service executes a test of ECC hardware. This is
 *                     only applicable in case the hardware provices
 *                     such functionality.
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 */
/*************************************************************************/
#if ((STD_ON == FLSTST_TEST_ECC_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM)))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(Std_ReturnType, FLSTST_CODE)
FlsTst_TestEcc(void)
{
    Std_ReturnType Result;
    FlsTst_LengthType l_EccLength;
    FlsTst_AddressType TestStartAddr;
    FlsTst_AddressType TestEndAddr;

    TestStartAddr = FlsTst_CurRunning.TestAddr;
    Result = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_TESTECC_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        if (FLSTST_TESTCELL_LENGTH_ATOMIC < FlsTst_CurRunning.TestLength)
        {
            l_EccLength = FLSTST_TESTCELL_LENGTH_ATOMIC;
        }
        else
        {
            l_EccLength = FlsTst_CurRunning.TestLength;
        }

        TestEndAddr = TestStartAddr + l_EccLength;

        if (NULL_PTR != FlsTst_Module.FlsTstECCTestFunction)
        {
            Result = FlsTst_Module.FlsTstECCTestFunction((const uint32*)TestStartAddr, (const uint32*)TestEndAddr);
        }
        else
        {
            /* Do not support Flash ECC test function */
        }
    }
    return Result;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_TEST_ECC_API && \
    0U != FLSTST_BGND_BLOCK_NUM || 0U != FLSTST_FGND_BLOCK_NUM */
/*************************************************************************/
/*
 * Brief               Service for executing the Flash Test in background
 *                     mode.
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (0U != FLSTST_BGND_BLOCK_NUM)
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
FUNC(void, FLSTST_CODE)
FlsTst_MainFunction(void)
{
    FlsTst_GetResource();
    switch (FlsTst_Module.State)
    {
    case FLSTST_UNINIT:
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
        FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_UNINIT);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
        break;

    case FLSTST_INIT:
        /*Auto start*/
        FlsTst_Module.State = FLSTST_RUNNING;
        if (FLSTST_RESULT_NOT_TESTED != FlsTst_Module.BgndResult)
        {
            FlsTst_Module.IntervalID++;
        }
        if (FlsTst_Module.IntervalID > FLSTST_TEST_INTERVALID_END_VALUE)
        {
            FlsTst_Module.IntervalID = 0u;
        }
        FlsTst_CopyBlockInfo(0u);
        FlsTst_InterTest();
        break;

    case FLSTST_RUNNING:
        /*Continue Calc*/
        FlsTst_InterTest();
        break;

    case FLSTST_ABORTED:
        /*do nothing*/
        break;

    case FLSTST_SUSPENDED:
        break;

    default:
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
        FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_PARAM_INVALID);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
        break;
    }
    FlsTst_FreeResource();
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* 0U != FLSTST_BGND_BLOCK_NUM */
/*************************************************************************/
/*
 * Brief               The function FlsTst_TestCompletedNotification shall be called
 *                     every time when a complete test cycle had been
 *                     tested.
 * ServiceId           0x0e
 * Sync/Async          Synchronous
 * Reentrancy          Don't care
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (0 != FLSTST_BGND_BLOCK_NUM)
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
#if (STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT)
static FUNC(void, FLSTST_CODE) FlsTst_TestCompletedNotification(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FLSTST_DET_REPORTERROR(FLSTST_TESTCOMPLETENOTIFY_ID, FLSTST_E_UNINIT);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        if (NULL_PTR != FlsTst_Module.UserCallBack)
        {
            FlsTst_Module.UserCallBack();
        }
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT */

/*************************************************************************/
/*
 * Brief               Internel function to Copy information from
 *                     FlsTst_BlockBgndInfo to FlsTst_CurRunning.
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FlsTstBlockID: Block ID in background test.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(void, FLSTST_CODE) FlsTst_CopyBlockInfo(FlsTst_BlockIdType FlsTstBlockID)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FlsTstBlockID >= FLSTST_BGND_BLOCK_NUM)
    {
        FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        SchM_Enter_FlsTst_Queue();
        FlsTst_CurRunning.FirstFlag = FALSE;
        FlsTst_CurRunning.CalcResult = 0;
        FlsTst_CurRunning.CurBlockID = FlsTst_BlockBgndInfo[FlsTstBlockID].MemoryBlockID;
        FlsTst_CurRunning.TestAddr = FlsTst_BlockBgndInfo[FlsTstBlockID].MemBlcokStartAddr;
        FlsTst_CurRunning.TestLength = FlsTst_BlockBgndInfo[FlsTstBlockID].MemBlcokLength;
        FlsTst_CurRunning.TestAlgorithm = FlsTst_BlockBgndInfo[FlsTstBlockID].TestAlgorithm;
        /*Duplicated memory start address*/
        FlsTst_CurRunning.DupTestAddr = FlsTst_BlockBgndInfo[FlsTstBlockID].DuplicateAddr;
        SchM_Exit_FlsTst_Queue();
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Internel function in bcakground test,be called when
 *                     a memmory block test finished.  another block wil
 *                     to be tested in current test intervel
 *
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(void, FLSTST_CODE) FlsTst_BlockTestFinish(void)
{
    FlsTst_BlockIdType BlockID;
    FlsTst_BlockIdType MemBlockIndex;
    boolean l_ErrorBlock;

    l_ErrorBlock = FALSE;
    BlockID = FlsTst_CurRunning.CurBlockID;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (BlockID > (FLSTST_BGND_BLOCK_NUM - 1U))
    {
        FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        FlsTst_BlockBgndInfo[BlockID].SignatueCalculated = FlsTst_CurRunning.CalcResult;
        if (FlsTst_BlockBgndInfo[BlockID].SignatueCalculated == FlsTst_BlockBgndInfo[BlockID].SignatureStored)
        {
            FlsTst_BlockBgndInfo[BlockID].TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            FlsTst_Module.BgndResult = FLSTST_RESULT_NOT_OK;
            FlsTst_BlockBgndInfo[BlockID].TestResult = FLSTST_RESULT_NOT_OK;
            /*at least one block test failed*/
#if (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE == STD_ON)
            Dem_ReportErrorStatus(FLSTST_E_FLSTST_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif /* FLSTST_DEM_FLSTST_E_FLSTST_FAILURE == STD_ON */
        }
    }

    /* Another block need to be tested*/
#if (FLSTST_BGND_BLOCK_NUM > 1)
    if (BlockID < ((FlsTst_BlockIdType)(FLSTST_BGND_BLOCK_NUM - 1U)))
    {
        BlockID++;
        FlsTst_CopyBlockInfo(BlockID);
    }
    else /*the last block test finished*/
#endif   /* FLSTST_BGND_BLOCK_NUM > 1 */
    {
        /*no more block need to be tested, current test interval finished.*/
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (MemBlockIndex = 0U; MemBlockIndex < FLSTST_BGND_BLOCK_NUM; MemBlockIndex++)
#else
        MemBlockIndex = 0;
#endif /*1u < FLSTST_BGND_BLOCK_NUM*/
        {
            if (FLSTST_RESULT_NOT_OK == FlsTst_BlockBgndInfo[MemBlockIndex].TestResult)
            {
                l_ErrorBlock = TRUE;
                FlsTst_Module.ErrorInfo.ErrorBlockID = MemBlockIndex;
                FlsTst_Module.ErrorInfo.Algorithm = FlsTst_BlockBgndInfo[MemBlockIndex].TestAlgorithm;
                FlsTst_Module.ErrorInfo.SignatureResult = FlsTst_BlockBgndInfo[MemBlockIndex].SignatueCalculated;
#if (1u < FLSTST_BGND_BLOCK_NUM)
                break;
#endif /*1u < FLSTST_BGND_BLOCK_NUM*/
            }
        }

        if (FALSE == l_ErrorBlock)
        {
            FlsTst_Module.BgndResult = FLSTST_RESULT_OK;
#if (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE == STD_ON)
            Dem_ReportErrorStatus(FLSTST_E_FLSTST_FAILURE, DEM_EVENT_STATUS_PASSED);
#endif /* FLSTST_DEM_FLSTST_E_FLSTST_FAILURE == STD_ON */
        }
        else
        {
            /* empty */
        }

        FlsTst_Module.State = FLSTST_INIT;
#if (STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT)
        FlsTst_TestCompletedNotification();
#endif /* STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT */
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Internel function in bcakground test, called in one
 *                     Flstst_MainFunctiuon to calculating Flash cells
 *                     in background mode *
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(void, FLSTST_CODE) FlsTst_InterTest(void)
{
    uint32 Cycletimes;
    uint32 l_Counter;
    Std_ReturnType ret = E_NOT_OK;
#if (FLSTST_TIMEOUT == STD_ON)
    uint32 FlsTstTimeStart_Bgnd;
    uint32 FlsTstTimeGap_Bgnd;
#endif /* FLSTST_TIMEOUT == STD_ON */

    if ((FlsTst_CurRunning.TestLength == 0U) && (FLSTST_RUNNING == FlsTst_Module.State))
    {
        /*Current Block Calc Finish*/
        FlsTst_BlockTestFinish();
    }
    else
    {
        /* First calculate CRC*/
        if ((FLSTST_TESTCELLS_NUM * FLSTST_TESTCELL_LENGTH_ATOMIC) < FlsTst_CurRunning.TestLength)
        {
            Cycletimes = FLSTST_TESTCELLS_NUM;
        }
        else /* Length smaller than FLSTST_NUMOFTESTEDCELLS,but not 0 */
        {
            Cycletimes = FlsTst_CurRunning.TestLength / FLSTST_TESTCELL_LENGTH_ATOMIC;
            Cycletimes = Cycletimes + 1U;
        }

#if (FLSTST_TIMEOUT == STD_ON)
        FlsTstTimeStart_Bgnd = Frt_ReadOutMS();
#endif /* FLSTST_TIMEOUT == STD_ON */

        for (l_Counter = 0U; ((l_Counter < Cycletimes) && (ret == E_OK)); l_Counter++)
        {
            if (FLSTST_RUNNING == FlsTst_Module.State)
            {
                FlsTst_CalcAtomic();
#if (FLSTST_TIMEOUT == STD_ON)
                FlsTstTimeGap_Bgnd = Frt_CalculateElapsedMS(FlsTstTimeStart_Bgnd);
                if (FlsTstTimeGap_Bgnd >= FLSTST_TIMEOUTVALUE_BGND)
                {
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
                    FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_TIMEOUT);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
                    FlsTst_Abort();
                    FlsTst_Module.BgndResult = FLSTST_RESULT_NOT_OK;
                    ret = E_NOT_OK;
                }
#endif /* FLSTST_TIMEOUT == STD_ON */
            }
            else if ((FLSTST_SUSPENDED == FlsTst_Module.State) || (FLSTST_ABORTED == FlsTst_Module.State))
            {
                ret = E_NOT_OK;
            }
            else
            {
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
                FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_STATE_FAILURE);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
            }

            if (0U == FlsTst_CurRunning.TestLength)
            {
                break;
            }
        }
    }
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

/*************************************************************************/
/*
 * Brief               Internel function in bcakground test, called to
 *                     calculating Flash cell once.
 *                     in background mode.
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(void, FLSTST_CODE) FlsTst_CalcAtomic(void)
{
    FlsTst_LengthType l_CrcLength;
    boolean l_FirstTimeCalc;

    SchM_Enter_FlsTst_Queue();
    /*if a block was First time to calculate CRC*/
    if (FALSE == FlsTst_CurRunning.FirstFlag)
    {
        FlsTst_CurRunning.FirstFlag = TRUE;
        l_FirstTimeCalc = TRUE;
    }
    else
    {
        l_FirstTimeCalc = FALSE;
    }

    if (FLSTST_TESTCELL_LENGTH_ATOMIC < FlsTst_CurRunning.TestLength)
    {
        l_CrcLength = FLSTST_TESTCELL_LENGTH_ATOMIC;
    }
    else
    {
        l_CrcLength = FlsTst_CurRunning.TestLength;
    }

    switch (FlsTst_CurRunning.TestAlgorithm)
    {
    case FLSTST_16BIT_CRC:
        FlsTst_CurRunning.CalcResult = (uint32)Crc_CalculateCRC16(
            (uint8*)FlsTst_CurRunning.TestAddr,
            l_CrcLength,
            (uint16)FlsTst_CurRunning.CalcResult,
            l_FirstTimeCalc);
        break;

    case FLSTST_32BIT_CRC:
        FlsTst_CurRunning.CalcResult = (uint32)Crc_CalculateCRC32(
            (uint8*)FlsTst_CurRunning.TestAddr,
            l_CrcLength,
            (uint32)FlsTst_CurRunning.CalcResult,
            l_FirstTimeCalc);
        break;

    case FLSTST_8BIT_CRC:
        FlsTst_CurRunning.CalcResult = (uint32)Crc_CalculateCRC8(
            (uint8*)FlsTst_CurRunning.TestAddr,
            l_CrcLength,
            (uint8)FlsTst_CurRunning.CalcResult,
            l_FirstTimeCalc);
        break;

    case FLSTST_CHECKSUM:
        FlsTst_CurRunning.CalcResult = (uint32)FlsTst_CheckSum(
            (uint8*)FlsTst_CurRunning.TestAddr,
            l_CrcLength,
            (uint32)FlsTst_CurRunning.CalcResult,
            l_FirstTimeCalc);
        break;

    case FLSTST_DUPLICATED_MEMORY:
        FlsTst_CurRunning.CalcResult = (uint32)
            FlsTst_MemCompare((uint8*)FlsTst_CurRunning.TestAddr, (uint8*)FlsTst_CurRunning.DupTestAddr, l_CrcLength);
        break;

    case FLSTST_ECC:
#if ((STD_ON == FLSTST_TEST_ECC_API) && ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM)))
        FlsTst_CurRunning.CalcResult = FlsTst_TestEcc();
#endif /* STD_ON == FLSTST_TEST_ECC_API && \
    0U != FLSTST_BGND_BLOCK_NUM || 0U != FLSTST_FGND_BLOCK_NUM */
        break;

    default:
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
        /* do not support other Algorithm. */
        FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_PARAM_INVALID);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
        break;
    }
    FlsTst_CurRunning.TestLength -= l_CrcLength;
    FlsTst_CurRunning.TestAddr += l_CrcLength;
    FlsTst_CurRunning.DupTestAddr += l_CrcLength;
    SchM_Exit_FlsTst_Queue();
    return;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* 0 != FLSTST_BGND_BLOCK_NUM */
/*************************************************************************/
/*
 * Brief               The function FlsTst_StartFgndInterTest shall be
 *                     called by FlsTst_StartFgnd.+
 *                     tested.
 * ServiceId           None, Interel Function
 * Sync/Async          Synchronous
 * Reentrancy          Don't care
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
#if (0 != FLSTST_FGND_BLOCK_NUM)
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_StartFgndInterTest(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType testResult;
    FlsTst_AlgorithmType Algorithm;
    boolean l_BlockError;
    FlsTst_StateType BgndState;

#if (FLSTST_TIMEOUT == STD_ON)
    uint32 FlsTstTimeStart_Fgnd;
    uint32 FlsTstTimeGap_Fgnd;
#endif /* FLSTST_TIMEOUT == STD_ON */

    testResult = E_NOT_OK;
    l_BlockError = FALSE;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        BgndState = FlsTst_Module.State;
        FlsTst_Module.State = FLSTST_RUNNING;
        Algorithm = BlockInfoPtr->TestAlgorithm;
#if (FLSTST_TIMEOUT == STD_ON)
        FlsTstTimeStart_Fgnd = Frt_ReadOutMS();
#endif /* FLSTST_TIMEOUT == STD_ON */
        switch (Algorithm)
        {
        case FLSTST_16BIT_CRC:
            testResult = FlsTst_CRC16Fgnd(BlockInfoPtr);
            break;

        case FLSTST_32BIT_CRC:
            testResult = FlsTst_CRC32Fgnd(BlockInfoPtr);
            break;

        case FLSTST_8BIT_CRC:
            testResult = FlsTst_CRC8Fgnd(BlockInfoPtr);
            break;

        case FLSTST_CHECKSUM:
            testResult = FlsTst_ChecksumFgnd(BlockInfoPtr);
            break;

        case FLSTST_DUPLICATED_MEMORY:
            testResult = FlsTst_DuplicatedMemoryFgnd(BlockInfoPtr);
            break;

        case FLSTST_ECC:
            testResult = FlsTst_ECCTestFgnd(BlockInfoPtr);
            break;

        default:
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
            FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
            testResult = E_NOT_OK;
            break;
        }
#if (FLSTST_TIMEOUT == STD_ON)
        FlsTstTimeGap_Fgnd = Frt_CalculateElapsedMS(FlsTstTimeStart_Fgnd);
        if (FlsTstTimeGap_Fgnd >= FLSTST_TIMEOUTVALUE_FGND)
        {
            FlsTstTimeGap_Fgnd = 0;
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
            FLSTST_DET_REPORTERROR(FLSTST_MAINFUNCTION_ID, FLSTST_E_TIMEOUT);
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
            FlsTst_Abort();
            FlsTst_Module.FgndResult = FLSTST_NOT_OK;
            l_BlockError = TRUE;
        }
#endif /* FLSTST_TIMEOUT == STD_ON */
        if ((FLSTST_RESULT_NOT_OK == BlockInfoPtr->TestResult)
#if (FLSTST_TIMEOUT == STD_ON)
            && (l_BlockError == FALSE)
#endif /* FLSTST_TIMEOUT == STD_ON */
        )
        {
            /*copy error details info*/
            FlsTst_Module.ErrorInfo.ErrorBlockID = BlockInfoPtr->MemoryBlockID;
            FlsTst_Module.ErrorInfo.Algorithm = BlockInfoPtr->TestAlgorithm;
            FlsTst_Module.ErrorInfo.SignatureResult = BlockInfoPtr->SignatueCalculated;
            FlsTst_Module.FgndResult = FLSTST_NOT_OK;
            l_BlockError = TRUE;
#if (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE == STD_ON)
            Dem_ReportErrorStatus(FLSTST_E_FLSTST_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif /* FLSTST_DEM_FLSTST_E_FLSTST_FAILURE == STD_ON */
        }

        if (FALSE == l_BlockError)
        {
            FlsTst_Module.FgndResult = FLSTST_OK;
        }
        if (FLSTST_RUNNING == FlsTst_Module.State)
        {
            FlsTst_Module.State = BgndState;
        }
    }
    return testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_CRC32Fgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{

    FlsTst_AddressType TestAddr;
    FlsTst_LengthType TestLength;
    FlsTst_SignatureType SignatureResult;
    Std_ReturnType l_testResult;

    l_testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else if (FLSTST_32BIT_CRC != BlockInfoPtr->TestAlgorithm)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestAddr = BlockInfoPtr->MemBlcokStartAddr;
        TestLength = BlockInfoPtr->MemBlcokLength;
        SignatureResult = 0u;

        /* if test method changes,Modify the code below */
        /* Use CRC32  */
        SignatureResult = (uint32)Crc_CalculateCRC32((uint8*)TestAddr, TestLength, (uint16)SignatureResult, TRUE);

        BlockInfoPtr->SignatueCalculated = SignatureResult;
        if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
        l_testResult = E_OK;
    }
    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_CRC16Fgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    FlsTst_AddressType TestAddr;
    FlsTst_LengthType TestLength;
    FlsTst_SignatureType SignatureResult;
    Std_ReturnType l_testResult;

    l_testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else if (FLSTST_16BIT_CRC != BlockInfoPtr->TestAlgorithm)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestAddr = BlockInfoPtr->MemBlcokStartAddr;
        TestLength = BlockInfoPtr->MemBlcokLength;
        SignatureResult = 0u;

        /* if test method changes,Modify the code below */
        /* Use CRC16  */
        SignatureResult = (uint32)Crc_CalculateCRC16((uint8*)TestAddr, TestLength, (uint16)SignatureResult, TRUE);

        BlockInfoPtr->SignatueCalculated = SignatureResult;
        if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
        l_testResult = E_OK;
    }

    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_CRC8Fgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    FlsTst_AddressType TestAddr;
    FlsTst_LengthType TestLength;
    FlsTst_SignatureType SignatureResult;
    Std_ReturnType l_testResult;

    l_testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else if (FLSTST_8BIT_CRC != BlockInfoPtr->TestAlgorithm)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestAddr = BlockInfoPtr->MemBlcokStartAddr;
        TestLength = BlockInfoPtr->MemBlcokLength;
        SignatureResult = 0u;

        /* if test method changes,Modify the code below */
        /* Use CRC8  */
        SignatureResult = Crc_CalculateCRC8((uint8*)TestAddr, TestLength, (uint8)SignatureResult, TRUE);
        BlockInfoPtr->SignatueCalculated = SignatureResult;
        if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
        l_testResult = E_OK;
    }
    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_DuplicatedMemoryFgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    FlsTst_AddressType TestAddr;
    FlsTst_LengthType TestLength;
    FlsTst_AddressType DuplicateMemoryAddr;
    Std_ReturnType l_testResult;

    l_testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else if (FLSTST_DUPLICATED_MEMORY != BlockInfoPtr->TestAlgorithm)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestAddr = BlockInfoPtr->MemBlcokStartAddr;
        TestLength = BlockInfoPtr->MemBlcokLength;
        /*duplicate memory address*/
        DuplicateMemoryAddr = (FlsTst_AddressType)BlockInfoPtr->DuplicateAddr;
        /* if test method changes,Modify the code below */
        /* FlsTst Duplicated Memory  Foreground  test */
        l_testResult = FlsTst_MemCompare((uint8*)TestAddr, (uint8*)DuplicateMemoryAddr, TestLength);

        if ((Std_ReturnType)E_OK == l_testResult)
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
    }
    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_ChecksumFgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    FlsTst_AddressType TestAddr;
    FlsTst_LengthType TestLength;
    FlsTst_SignatureType SignatureResult;
    Std_ReturnType l_testResult;

    l_testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else if (FLSTST_CHECKSUM != BlockInfoPtr->TestAlgorithm)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestAddr = BlockInfoPtr->MemBlcokStartAddr;
        TestLength = BlockInfoPtr->MemBlcokLength;
        SignatureResult = 0u;

        /* if test method changes,Modify the code below */
        /* Use CRC8  */
        SignatureResult = FlsTst_CheckSum((uint8*)TestAddr, TestLength, SignatureResult, TRUE);

        BlockInfoPtr->SignatueCalculated = SignatureResult;
        if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
        l_testResult = E_OK;
    }
    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_ECCTestFgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType l_testResult;
    FlsTst_AddressType TestStartAddr;
    FlsTst_AddressType TestEndAddr;

    l_testResult = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (NULL_PTR == BlockInfoPtr)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_POINTER);
    }
    else if (FLSTST_ECC != BlockInfoPtr->TestAlgorithm)
    {
        FLSTST_DET_REPORTERROR(FLSTST_STARTFGND_ID, FLSTST_E_PARAM_INVALID);
    }
    else
#endif /* STD_ON == FLSTST_DEV_ERROR_DETECT */
    {
        TestStartAddr = BlockInfoPtr->MemBlcokStartAddr;
        TestEndAddr = TestStartAddr + BlockInfoPtr->MemBlcokLength;

        if (NULL_PTR != FlsTst_Module.FlsTstECCTestFunction)
        {
            l_testResult =
                FlsTst_Module.FlsTstECCTestFunction((const uint32*)TestStartAddr, (const uint32*)TestEndAddr);

            if (E_OK == l_testResult)
            {
                BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
            }
            else
            {
                BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
            }
        }
        else
        {
            /* Do not support Flash ECC test function */
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
    }
    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif               /* 0 != FLSTST_FGND_BLOCK_NUM */
/* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

#if ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(uint32, FLSTST_CODE) FlsTst_CheckSum(
    P2CONST(uint8, AUTOMATIC, FLSTST_APPL_CONST) Checksum_DataPtr,
    VAR(uint32, AUTOMATIC) Checksum_Length,
    VAR(uint32, AUTOMATIC) Checksum_StartValue,
    VAR(boolean, AUTOMATIC) Checksum_IsFirstCall)
{
    uint32 checksumResult;
    uint32 i;

    if ((boolean)TRUE == Checksum_IsFirstCall)
    {
        checksumResult = 0U;
    }
    else
    {
        checksumResult = Checksum_StartValue;
    }

    for (i = 0; i < Checksum_Length; i++)
    {
        checksumResult += Checksum_DataPtr[i];
    }
    return checksumResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* 0U != FLSTST_BGND_BLOCK_NUM || 0U != FLSTST_FGND_BLOCK_NUM */

#if ((0U != FLSTST_BGND_BLOCK_NUM) || (0U != FLSTST_FGND_BLOCK_NUM))
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
static FUNC(Std_ReturnType, FLSTST_CODE) FlsTst_MemCompare(
    P2CONST(uint8, AUTOMATIC, FLSTST_APPL_CONST) SourceDataPtr,
    P2CONST(uint8, AUTOMATIC, FLSTST_APPL_CONST) DstDataPtr,
    VAR(uint32, AUTOMATIC) Length)
{
    uint32 i;
    const uint8* pDest = (const uint8*)DstDataPtr;
    const uint8* pSrc = (const uint8*)SourceDataPtr;
    Std_ReturnType l_testResult;

    l_testResult = E_OK;

    for (i = 0U; i < Length; i++)
    {
        if (pDest[i] != pSrc[i])
        {
            l_testResult = E_NOT_OK;
            break;
        }
    }
    return l_testResult;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif /* 0U != FLSTST_BGND_BLOCK_NUM || 0U != FLSTST_FGND_BLOCK_NUM */
