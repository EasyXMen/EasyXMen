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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Eep_62.c                                                    **
**                                                                            **
**  Created on  : 2022/02/15                                                  **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Eep_62<br>
  RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 3408 MISRA Rule 8.4 .<br>
    Reason:These two functions are deliberately designed to be empty, there is a unified processing of the main
  function, the function is not affected

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 1531 MISRA Rule 8.7 .<br>
    Reason:Configuration file design implementation needs.
 */

/*============================================================================*/
#include "Eep_62.h"
#include "Eep_62_Types.h"
#include "Eep_62_Cbk.h"
#include "SchM_Eep_62.h"

/*******************************************************************************
**                       Version  information                                 **
*******************************************************************************/

/*==============================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#if (EEP_62_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /* EEP_62_DEV_ERROR_DETECT == STD_ON */
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#if (                                                                                              \
    (EEP_62_DEM_E_COMPARE_FAILED_DETECT == STD_ON) || (EEP_62_DEM_E_WRITE_FAILED_DETECT == STD_ON) \
    || (EEP_62_DEM_E_READ_FAILED_DETECT == STD_ON) || (EEP_62_DEM_E_ERASE_FAILED_DETECT == STD_ON))
#include "Dem.h"
#endif

#include "Eep_62_Cfg.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef EEP_62_AR_RELEASE_MAJOR_VERSION
#error "EEP_62.c : Miss Specification Major Version"
#endif
#ifndef EEP_62_AR_RELEASE_MINOR_VERSION
#error "EEP_62.c : Miss Specification MINOR Version"
#endif
#ifndef EEP_62_AR_RELEASE_PATCH_VERSION
#error "EEP_62.c : Miss Specification PATCH Version"
#endif

#if (1u != EEP_62_CFG_H_SW_MAJOR_VERSION)
#error "EEP_62.c : Mismatch in Software PATCH Version"
#endif
#if (0u != EEP_62_CFG_H_SW_MINOR_VERSION)
#error "EEP_62.c : Mismatch in Software PATCH Version"
#endif
#if (0u != EEP_62_CFG_H_SW_PATCH_VERSION)
#error "EEP_62.c : Mismatch in Software PATCH Version"
#endif

/*==============================================================================
*                                       LOCAL MACROS
==============================================================================*/

/*******************************************************************************
**                      Global Variables                                      **
*******************************************************************************/

/*******************************************************************************
**                      Local Variables                                       **
*******************************************************************************/
#define EEP_62_START_SEC_VAR_INIT_8BIT
#include "Eep_62_MemMap.h"
static VAR(MemIf_StatusType, EEP_APPL_DATA) Eep_62_Status[EEP_62_NUMBER_OF_EEP_MODULE] = {MEMIF_UNINIT};
static VAR(MemIf_JobResultType, EEP_APPL_DATA) Eep_62_JobResult[EEP_62_NUMBER_OF_EEP_MODULE] = {MEMIF_JOB_OK};
#define EEP_62_STOP_SEC_VAR_INIT_8BIT
#include "Eep_62_MemMap.h"

#define EEP_62_START_SEC_VAR_NOINIT_8BIT
#include "Eep_62_MemMap.h"
static VAR(uint8, NVM_VAR_NOINIT) Eep_62_CompareBuffer[EEP_62_NUMBER_OF_EEP_MODULE][EEP_COMPARE_BUF_SIZE];
#define EEP_62_STOP_SEC_VAR_NOINIT_8BIT
#include "Eep_62_MemMap.h"

#define EEP_62_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Eep_62_MemMap.h"
static VAR(Eep_62_RuntimeType, EEP_APPL_DATA) Eep_62_Runtime[EEP_62_NUMBER_OF_EEP_MODULE];
#define EEP_62_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Eep_62_MemMap.h"

#define EEP_62_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Eep_62_MemMap.h"
static P2CONST(Eep_62_ConfigType, EEP_CONST, EEP_CONST_PBCFG) Eep_62_ConfigPtr;
#define EEP_62_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Eep_62_MemMap.h"

/*==============================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==============================================================================*/
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Erase_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData);
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Write_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData);
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Read_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData);
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Compare_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"

/*===============================================================================
                                       GLOBAL FUNCTIONS
===============================================================================*/
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"

/*************************************************************************/
/* Brief           CDD_M95320    Service for EEPROM initialization.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to configuration set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer(EcuM)
 */
/*************************************************************************/
FUNC(void, EEP_PUBLIC_CODE) Eep_62_Init(P2CONST(Eep_62_ConfigType, AUTOMATIC, EEP_APPL_CONST) ConfigPtr)
{
    uint8 iloop;
    uint16 jloop;
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    boolean detNoError = TRUE;
    for (iloop = 0; iloop < EEP_62_NUMBER_OF_EEP_MODULE; iloop++)
    {
        if (MEMIF_UNINIT != Eep_62_Status[iloop])
        {
            (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_INIT, EEP_62_E_REINIT);
            detNoError = (boolean)FALSE;
            break;
        }
    }
    if ((boolean)TRUE == detNoError)
    {
        if (NULL_PTR == ConfigPtr)
        {
            (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_INIT, EEP_62_E_PARAM_POINTER);
            detNoError = (boolean)FALSE;
        }
    }
    if ((boolean)TRUE == detNoError)
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_ConfigPtr = ConfigPtr;
        iloop = 0;
#if (EEP_62_NUMBER_OF_EEP_MODULE > 1u)
        for (; iloop < EEP_62_NUMBER_OF_EEP_MODULE; iloop++)
#endif /* EEP_62_NUMBER_OF_EEP_MODULE > 1u */
        {
            Eep_62_Status[iloop] = MEMIF_IDLE;
            Eep_62_JobResult[iloop] = MEMIF_JOB_OK;
            /* parameter init */
            Eep_62_Runtime[iloop].currentJob = EEP_62_JOB_NONE;
            Eep_62_Runtime[iloop].operateAddr = 0U;
            Eep_62_Runtime[iloop].dataBufferPtr = NULL_PTR;
            Eep_62_Runtime[iloop].WriteBuffer = NULL_PTR;
            Eep_62_Runtime[iloop].length = 0U;
            Eep_62_Runtime[iloop].CompareLength = 0u;
            Eep_62_Runtime[iloop].StateForRunMainFunction = FALSE;
            for (jloop = 0; jloop < EEP_COMPARE_BUF_SIZE; jloop++)
            {
                Eep_62_CompareBuffer[iloop][jloop] = 0U;
            }
            if (NULL_PTR != Eep_62_DevHwApis_at[iloop].DevInitApi)
            {
                Eep_62_DevHwApis_at[iloop].DevInitApi();
            }
            /* according to the default mode to set the current mode, read and write size. */
            Eep_62_SetMode(iloop, Eep_62_ConfigPtr[iloop].defaultMode);
        }
    }
    return;
}

/*************************************************************************/
/* Brief               Sets the mode.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Mode:
 *                     MEMIF_MODE_SLOW:
 *                          Slow read access/normal SPI access.
 *                     MEMIF_MODE_FAST:
 *                          Fast read access/SPI burst access.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        Eep_Init
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, EEP_PUBLIC_CODE) Eep_62_SetMode(VAR(uint8, AUTOMATIC) deviceIndex, VAR(MemIf_ModeType, AUTOMATIC) Mode)
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_SETMODE, EEP_62_E_UNINT);
    }
    else if (MEMIF_BUSY == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_SETMODE, EEP_62_E_BUSY);
    }
    else
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_Runtime[deviceIndex].currentMode = Mode;
        if (MEMIF_MODE_SLOW == Eep_62_Runtime[deviceIndex].currentMode)
        {
            /* Check if MEMIF_MODE_SLOW mode is set */
            Eep_62_Runtime[deviceIndex].maxReadSize =
                Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->normalReadBlockSize;
            Eep_62_Runtime[deviceIndex].maxWriteSize =
                Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->normalWriteBlockSize;
        }
        else if (MEMIF_MODE_FAST == Eep_62_Runtime[deviceIndex].currentMode)
        {
            /* Check if MEMIF_MODE_FAST mode is set*/
            Eep_62_Runtime[deviceIndex].maxReadSize = Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->fastReadBlockSize;
            Eep_62_Runtime[deviceIndex].maxWriteSize =
                Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->fastWriteBlockSize;
        }
        else
        {
            /* do nothing  */
        }
    }
}

/*************************************************************************/
/* Brief               Reads from EEPROM..
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Eep_62_AddressType:
 *                          Address offset in EEPROM (will be added to the EEPROM base address).
 *                          range: [0 .. (EEP_SIZE - 1)]
 *                     Length:
 *                          Number of bytes to read.
 *                          range: [1 .. (EEP_SIZE - EepromAddress)]
 * Param-Name[out]     DataBufferPtr:
 *                          Pointer to destination data buffer in RAM.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType:
 *                          (Std_ReturnType)E_OK: read command has been accepted.
 *                          (Std_ReturnType)E_NOT_OK: read command has not been accepted.
 * PreCondition        Eep_Init
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532,3432,3673++ */ /* MISRA Rule 8.7,Rule 20.7,Rule 8.13 */
FUNC(Std_ReturnType, EEP_PUBLIC_CODE)
Eep_62_Read(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    P2VAR(uint8, AUTOMATIC, EEP_APPL_DATA) DataBufferPtr,
    VAR(Eep_62_LengthType, AUTOMATIC) Length)
/* PRQA S 1532,3432,3673-- */ /* MISRA Rule 8.7,Rule 20.7,Rule 8.13 */
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    Std_ReturnType retVal = (Std_ReturnType)E_NOT_OK;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_READ, EEP_62_E_UNINT);
    }
    else if (MEMIF_IDLE != Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_READ, EEP_62_E_BUSY);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_READ, EEP_62_E_PARAM_DATA);
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_READ, EEP_62_E_PARAM_DATA);
    }
    else if ((Length < 1U) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_READ, EEP_62_E_PARAM_LENGTH);
    }
    else if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_READ, EEP_62_E_BUSY);
    }
    else
#else
    Std_ReturnType retVal;
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_Status[deviceIndex] = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob = EEP_62_JOB_READ;
        Eep_62_Runtime[deviceIndex].length = Length;
        Eep_62_Runtime[deviceIndex].operateAddr = EepromAddress;
        Eep_62_Runtime[deviceIndex].dataBufferPtr = (uint8*)DataBufferPtr;
        retVal = (Std_ReturnType)E_OK;
    }
    return retVal;
}

/*************************************************************************/
/* Brief               Writes to EEPROM.
 * ServiceId           0x3
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Eep_62_AddressType: address offset
 *                     DataBufferPtr: Pointer to source data buffer
 *                     Length       : Number of bytes to write
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        Eep_Init
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, EEP_PUBLIC_CODE)
Eep_62_Write(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    VAR(Eep_62_LengthType, AUTOMATIC) Length)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    Std_ReturnType retVal = (Std_ReturnType)E_NOT_OK;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_WRITE, EEP_62_E_UNINT);
    }
    else if (MEMIF_IDLE != Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_WRITE, EEP_62_E_BUSY);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_WRITE, EEP_62_E_PARAM_DATA);
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_WRITE, EEP_62_E_PARAM_ADDRESS);
    }
    else if ((Length < 1u) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_WRITE, EEP_62_E_PARAM_LENGTH);
    }
    else if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_WRITE, EEP_62_E_BUSY);
    }
    else
#else
    Std_ReturnType retVal;
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_Status[deviceIndex] = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob = EEP_62_JOB_WRITE;
        Eep_62_Runtime[deviceIndex].length = Length;
        Eep_62_Runtime[deviceIndex].operateAddr = EepromAddress;
        Eep_62_Runtime[deviceIndex].WriteBuffer = DataBufferPtr;
        retVal = (Std_ReturnType)E_OK;
    }
    return retVal;
}

/*************************************************************************/
/* Brief               Service for erasing EEPROM sections.
 * ServiceId           0x4
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EepromAddress: address offset
 *                     Length       : Number of bytes to write
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, EEP_PUBLIC_CODE)
Eep_62_Erase(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    VAR(Eep_62_LengthType, AUTOMATIC) Length)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    Std_ReturnType retVal = (Std_ReturnType)E_NOT_OK;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_ERASE, EEP_62_E_UNINT);
    }
    else if (MEMIF_IDLE != Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_ERASE, EEP_62_E_BUSY);
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_ERASE, EEP_62_E_PARAM_ADDRESS);
    }
    else if ((Length < 1U) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_ERASE, EEP_62_E_PARAM_LENGTH);
    }
    else if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_ERASE, EEP_62_E_BUSY);
    }
    else
#else
    Std_ReturnType retVal;
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_Status[deviceIndex] = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob = EEP_62_JOB_ERASE;
        Eep_62_Runtime[deviceIndex].length = Length;
        Eep_62_Runtime[deviceIndex].operateAddr = EepromAddress;
        retVal = (Std_ReturnType)E_OK;
    }
    return retVal;
}

/*************************************************************************/
/* Brief               Compares a data block in EEPROM with an
 *                     EEPROM block in the memory.
 * ServiceId           0x5
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EepromAddress: address offset
 *                     DataBufferPtr: Pointer to data buffer
 *                     Length       : Number of bytes to write
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, EEP_PUBLIC_CODE)
Eep_62_Compare(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    VAR(Eep_62_LengthType, AUTOMATIC) Length)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    Std_ReturnType retVal = (Std_ReturnType)E_NOT_OK;
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_COMPARE, EEP_62_E_UNINT);
    }
    else if (MEMIF_IDLE != Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_COMPARE, EEP_62_E_BUSY);
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_COMPARE, EEP_62_E_PARAM_DATA);
    }
    else if (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - 1U))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_COMPARE, EEP_62_E_PARAM_ADDRESS);
    }
    else if ((Length < 1u) || (Length > (Eep_62_ConfigPtr[deviceIndex].DeviceConfigPtr->usedSize - EepromAddress)))
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_COMPARE, EEP_62_E_PARAM_LENGTH);
    }
    else if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_COMPARE, EEP_62_E_BUSY);
    }
    else
#else
    Std_ReturnType retVal;
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_Status[deviceIndex] = MEMIF_BUSY;
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_PENDING;
        Eep_62_Runtime[deviceIndex].currentJob = EEP_62_JOB_COMPARE;
        Eep_62_Runtime[deviceIndex].length = Length;
        Eep_62_Runtime[deviceIndex].CompareLength = Length;
        Eep_62_Runtime[deviceIndex].operateAddr = EepromAddress;
        Eep_62_Runtime[deviceIndex].dataBufferPtr = &Eep_62_CompareBuffer[deviceIndex][0];
        Eep_62_Runtime[deviceIndex].WriteBuffer = DataBufferPtr;
        retVal = (Std_ReturnType)E_OK;
    }
    return retVal;
}

/*************************************************************************/
/* Brief               Cancels a running job.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, EEP_PUBLIC_CODE) Eep_62_Cancel(VAR(uint8, AUTOMATIC) deviceIndex)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (MEMIF_UNINIT == Eep_62_Status[deviceIndex])
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_CANCEL, EEP_62_E_UNINT);
    }
    else if ((boolean)TRUE == Eep_62_Runtime[deviceIndex].StateForRunMainFunction)
    {
        Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_CANCEL, EEP_62_E_IN_MAINF);
    }
    else
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_62_Runtime[deviceIndex].currentJob = EEP_62_JOB_NONE;
        Eep_62_Runtime[deviceIndex].length = 0u;
        Eep_62_Runtime[deviceIndex].CompareLength = 0u;
        Eep_62_Runtime[deviceIndex].operateAddr = 0u;
        Eep_62_Runtime[deviceIndex].dataBufferPtr = NULL_PTR;
        Eep_62_Runtime[deviceIndex].WriteBuffer = NULL_PTR;
        if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
        {
            Eep_62_JobResult[deviceIndex] = MEMIF_JOB_CANCELED;
            /* call the job Error notification*/
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing */
        }
    }
}

/*************************************************************************/
/* Brief               Returns the EEPROM status.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              MemIf_StatusType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(MemIf_StatusType, EEP_PUBLIC_CODE) Eep_62_GetStatus(VAR(uint8, AUTOMATIC) deviceIndex)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    return Eep_62_Status[deviceIndex];
}

/*************************************************************************/
/* Brief               This service returns the result of the last job.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              MemIf_JobResultType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(MemIf_JobResultType, EEP_PUBLIC_CODE) Eep_62_GetJobResult(VAR(uint8, AUTOMATIC) deviceIndex)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    return Eep_62_JobResult[deviceIndex];
}

/*************************************************************************/
/* Brief               Service to perform the processing of the EEPROM
 *                     jobs (read/write/erase/compare) .
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, EEP_PUBLIC_CODE) Eep_62_MainFunction(void)
{
    uint8 iloop = 0;
#if (EEP_62_NUMBER_OF_EEP_MODULE > 1u)
    for (; iloop < EEP_62_NUMBER_OF_EEP_MODULE; iloop++)
#endif /* EEP_62_NUMBER_OF_EEP_MODULE > 1u */
    {
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
        if (MEMIF_UNINIT == Eep_62_Status[iloop])
        {
            Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_MFUNC, EEP_62_E_UNINT);
        }
        else
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
        {
            Eep_62_Runtime[iloop].StateForRunMainFunction = (boolean)TRUE;
            if (MEMIF_BUSY == Eep_62_Status[iloop])
            {
                switch (Eep_62_Runtime[iloop].currentJob)
                {
                case EEP_62_JOB_ERASE: {
                    Eep_62_Erase_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                }
                case EEP_62_JOB_WRITE: {
                    Eep_62_Write_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                }
                case EEP_62_JOB_READ: {
                    Eep_62_Read_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                }
                case EEP_62_JOB_COMPARE: {
                    Eep_62_Compare_Handle(iloop, &Eep_62_Runtime[iloop]);
                    break;
                }
                default: {
                    /* do nothing */
                    break;
                }
                }
            }
            /* Eep_73_Status not in Busy state. IDLE */
            else
            {
                /* do nothing */
            }
            Eep_62_Runtime[iloop].StateForRunMainFunction = (boolean)FALSE;
        }
    }
    return;
}

#if (EEP_62_VERSION_INFO_API == STD_ON)
/*************************************************************************/
/* Brief               Service to get the version information of
 *                     this module.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None.
 * Param-Name[out]     versioninfo : Pointer to where to store the version
 *                                   information of this module.
 * Param-Name[in/out]  None
 * Return              MemIf_JobResultType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, EEP_PUBLIC_CODE) Eep_62_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, EEP_APPL_DATA) versioninfo)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        (void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, EEP_62_ID_GETVInfo, EEP_62_E_PARAM_DATA);
    }
    else
#endif /* STD_ON == EEP_62_DEV_ERROR_DETECT */
    {
        versioninfo->vendorID = EEP_62_VENDOR_ID;
        versioninfo->moduleID = EEP_62_MODULE_ID;
        versioninfo->sw_major_version = EEP_62_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = EEP_62_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = EEP_62_SW_PATCH_VERSION;
    }
}
#endif /* EEP_62_VERSION_INFO_API == STD_ON */

#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"

/*==============================================================================
*                                       LOCAL FUNCTIONS
==============================================================================*/
#define EEP_62_START_SEC_CODE
#include "Eep_62_MemMap.h"

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Erase_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType retVal;
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevEraseApi)
    {
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevEraseApi(
            Eep_RunData->operateAddr,
            EEP_62_ERASE_VALUE,
            Eep_RunData->length);
    }
    if ((Std_ReturnType)E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
#if (STD_ON == EEP_62_DEM_E_ERASE_FAILED_DETECT)
        Dem_ReportErrorStatus(EEP_62_E_ERASE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif /* STD_ON == EEP_62_DEM_E_ERASE_FAILED_DETECT */
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob = EEP_62_JOB_NONE;
        /* call the job Error notification*/
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if ((Std_ReturnType)E_OK == retVal)
    {
        if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
        {
            Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
        }
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob = EEP_62_JOB_NONE;
        /* call the job end notification*/
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
        }
    }
    else
    {
        /* do nothing */
    }
}

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Write_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType retVal;
    Eep_62_LengthType writeBytes;
    /* get the source address of the Buffer */
    Eep_62_LengthType bytesRemain = Eep_RunData->length;
    writeBytes = Eep_RunData->maxWriteSize;
    /* if the send is small then is alignment data */
    if (bytesRemain < writeBytes)
    {
        writeBytes = (Eep_62_LengthType)(bytesRemain);
    }
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevWriteApi)
    {
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevWriteApi(
            Eep_RunData->operateAddr,
            Eep_RunData->WriteBuffer,
            writeBytes);
    }
    if ((Std_ReturnType)E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob = EEP_62_JOB_NONE;
#if (STD_ON == EEP_62_DEM_E_WRITE_FAILED_DETECT)
        Dem_ReportErrorStatus(EEP_62_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif /* STD_ON == EEP_62_DEM_E_WRITE_FAILED_DETECT */
        /* call the job Error notification*/
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if ((Std_ReturnType)E_OK == retVal)
    {
        /* set the next read data address and the source buffer */
        Eep_RunData->operateAddr += (Eep_62_AddressType)writeBytes;
        Eep_RunData->WriteBuffer = &Eep_RunData->WriteBuffer[writeBytes];
        /* update the remain bytes to write */
        Eep_RunData->length -= writeBytes;
        if ((Eep_62_LengthType)0U == Eep_RunData->length)
        {
            if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
            {
                Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
            }
            Eep_62_Status[deviceIndex] = MEMIF_IDLE;
            Eep_RunData->currentJob = EEP_62_JOB_NONE;
            /* call the job end notification*/
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
            }
        }
    }
    else
    {
        /* do nothing */
    }
}

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Read_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType retVal;
    Eep_62_LengthType readBytes;
    /* get the source address of the Buffer */
    Eep_62_LengthType* bytesRemain = &Eep_RunData->length;
    readBytes = Eep_RunData->maxReadSize;
    /* if the read is small then is alignment data */
    if (*bytesRemain < readBytes)
    {
        readBytes = (Eep_62_LengthType)*bytesRemain;
    }
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevReadApi)
    {
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevReadApi(
            Eep_RunData->operateAddr,
            Eep_RunData->dataBufferPtr,
            readBytes);
    }
    if ((Std_ReturnType)E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
#if (STD_ON == EEP_62_DEM_E_READ_FAILED_DETECT)
        Dem_ReportErrorStatus(EEP_62_E_READ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif /* STD_ON == EEP_62_DEM_E_READ_FAILED_DETECT */
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob = EEP_62_JOB_NONE;
        /* call the job Error notification*/
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if ((Std_ReturnType)E_OK == retVal)
    {
        /* set the next read data address and the source buffer */
        Eep_RunData->operateAddr += (Eep_62_AddressType)readBytes;
        Eep_RunData->dataBufferPtr = &Eep_RunData->dataBufferPtr[readBytes];
        /* update the remain bytes to write */
        *bytesRemain -= readBytes;
        if (*bytesRemain == 0U)
        {
            if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
            {
                Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
            }
            Eep_62_Status[deviceIndex] = MEMIF_IDLE;
            Eep_RunData->currentJob = EEP_62_JOB_NONE;
            /* call the job end notification*/
            if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
            {
                Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
            }
        }
    }
    else
    {
        /* do nothing */
    }
}

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static FUNC(void, EEP_PRIVATE_CODE) Eep_62_Compare_Handle(
    VAR(uint8, AUTOMATIC) deviceIndex,
    P2VAR(Eep_62_RuntimeType, AUTOMATIC, EEP_APPL_DATA) Eep_RunData)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 index = 0u;
    Std_ReturnType retVal;
    Std_ReturnType compareResult;
    Eep_62_LengthType compareBytes;

    /* get the runtime data/address of the Buffer */
    Eep_62_LengthType* bytesRemain = &Eep_RunData->length;
    compareBytes = Eep_RunData->maxReadSize;

    /* get the comparison data/address of the Buffer */
    Eep_62_LengthType compareLength;
    const uint8* referenceData;

    /* if the read is small then is alignment data */
    if (*bytesRemain < compareBytes)
    {
        compareBytes = (Eep_62_LengthType)*bytesRemain;
    }
    if (NULL_PTR == Eep_62_DevHwApis_at[deviceIndex].DevReadApi)
    {
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = Eep_62_DevHwApis_at[deviceIndex].DevReadApi(
            Eep_RunData->operateAddr,
            Eep_RunData->dataBufferPtr,
            compareBytes);
    }
    if ((Std_ReturnType)E_NOT_OK == retVal)
    {
        Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
#if (STD_ON == EEP_62_DEM_E_COMPARE_FAILED_DETECT)
        Dem_ReportErrorStatus(EEP_62_E_COMPARE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif /* STD_ON == EEP_62_DEM_E_COMPARE_FAILED_DETECT */
        Eep_62_Status[deviceIndex] = MEMIF_IDLE;
        Eep_RunData->currentJob = EEP_62_JOB_NONE;
        /* call the job Error notification*/
        if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
        {
            Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
        }
    }
    else if ((Std_ReturnType)E_OK == retVal)
    {
        /* set the next read data address and the source buffer */
        Eep_RunData->operateAddr += (Eep_62_AddressType)compareBytes;
        Eep_RunData->dataBufferPtr = &Eep_RunData->dataBufferPtr[compareBytes];
        /* update the remain bytes to read */
        *bytesRemain -= compareBytes;
        if ((Eep_62_LengthType)0U == *bytesRemain)
        {
            compareLength = Eep_RunData->CompareLength;
            referenceData = Eep_RunData->WriteBuffer;
            for (; index < compareLength; index++)
            {
                if (referenceData[index] != Eep_62_CompareBuffer[deviceIndex][index])
                {
                    compareResult = (Std_ReturnType)E_NOT_OK;
                    break;
                }
                else
                {
                    compareResult = (Std_ReturnType)E_OK;
                }
            }
            if ((Std_ReturnType)E_NOT_OK == compareResult)
            {
                Eep_62_JobResult[deviceIndex] = MEMIF_JOB_FAILED;
#if (STD_ON == EEP_62_DEM_E_COMPARE_FAILED_DETECT)
                Dem_ReportErrorStatus(EEP_62_E_COMPARE_FAILED, DEM_EVENT_STATUS_FAILED);
#endif /* STD_ON == EEP_62_DEM_E_COMPARE_FAILED_DETECT */
                Eep_62_Status[deviceIndex] = MEMIF_IDLE;
                Eep_RunData->currentJob = EEP_62_JOB_NONE;
                /* call the job Error notification*/
                if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobErrorNotification)
                {
                    Eep_62_ConfigPtr[deviceIndex].jobErrorNotification();
                }
            }
            else if ((Std_ReturnType)E_OK == compareResult)
            {
                if (MEMIF_JOB_PENDING == Eep_62_JobResult[deviceIndex])
                {
                    Eep_62_JobResult[deviceIndex] = MEMIF_JOB_OK;
                }
                Eep_62_Status[deviceIndex] = MEMIF_IDLE;
                Eep_RunData->currentJob = EEP_62_JOB_NONE;
                /* call the job end notification*/
                if (NULL_PTR != Eep_62_ConfigPtr[deviceIndex].jobEndNotification)
                {
                    Eep_62_ConfigPtr[deviceIndex].jobEndNotification();
                }
            }
            else
            {
                /*idle*/
            }
        }
    }
    else
    {
        /*idle*/
    }
}

#define EEP_62_STOP_SEC_CODE
#include "Eep_62_MemMap.h"
