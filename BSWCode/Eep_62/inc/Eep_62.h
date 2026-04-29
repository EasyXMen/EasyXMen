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
 **  @file               : Eep_62.h
 **  @author             : peng.wu
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : API declaration of Eep
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2021-01-23  Haibin.Shao     1. Initial Version
 *  V01.00.01 2023-09-18  tao.yu          remove warning
 *  V02.00.00 2024-02-02  xue.han         QAC
 *  V02.00.01 2024-08-13  haibin.shao     Append QAC remarks and remove main function declarations from header files
 *  V03.00.00 2024-12-23  peng.wu         Update to R23-11
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Eep_62<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Eep_62_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_1504
      Reason: Multi-partition variable design requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_1531
      Reason: The object '%1s' is referenced in only one translation unit - but not the one in which it is defined.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Eep_62_2963
      Reason: Strong rotations between different pointer types have no effect.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */

#ifndef EEP_62_H
#define EEP_62_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Eep_62_Cfg.h"
#include "Eep_62_Types.h"
#include "Eep_62_PBcfg.h"
#if (EEP_62_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name EEP_62_VERSION_INFO
 * @{
 * @brief Identification and version information
 */
#define EEP_62_VENDOR_ID                   62U  /**< Value of vendor */
#define EEP_62_MODULE_ID                   250U /**< Value of module */
#define EEP_62_INSTANCE_ID                 0U   /**< Value of instance */
#define EEP_62_AR_RELEASE_MAJOR_VERSION    4U   /**< Value of Autosar major version */
#define EEP_62_AR_RELEASE_MINOR_VERSION    9U   /**< Value of Autosar minor version */
#define EEP_62_AR_RELEASE_REVISION_VERSION 0U   /**< Value of Autosar patch version */
#define EEP_62_SW_MAJOR_VERSION            3U   /**< Value of module major version */
#define EEP_62_SW_MINOR_VERSION            0U   /**< Value of module minor version */
#define EEP_62_SW_PATCH_VERSION            0U   /**< Value of module patch version */

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++ */ /* VL_Eep_62_3414 */
#if !defined(EEP_62_LOCAL)
#define EEP_62_LOCAL static /**< Defines used as the local keyword for variables or functions */
#endif
/* PRQA S 3414 -- */

#if !defined(EEP_62_LOCAL_INLINE)
#define EEP_62_LOCAL_INLINE static inline /**< Defines used as keyword for the local inline functions */
#endif

/**
 * @name EEP_62_SERVICE_ID
 * @{
 * @brief SERVICE ID Definition
 */
#define EEP_62_ID_INIT         (0x00U) /**< Eep_62_Init */
#define EEP_62_ID_SETMODE      (0x01U) /**< Eep_62_SetMode */
#define EEP_62_ID_READ         (0x02U) /**< Eep_62_Read */
#define EEP_62_ID_WRITE        (0x03U) /**< Eep_62_Write */
#define EEP_62_ID_ERASE        (0x04U) /**< Eep_62_Erase */
#define EEP_62_ID_COMPARE      (0x05U) /**< Eep_62_Compare */
#define EEP_62_ID_CANCEL       (0x06U) /**< Eep_62_Cancel */
#define EEP_62_ID_GETSTATUS    (0x07U) /**< Eep_62_GetStatus */
#define EEP_62_ID_GETJOBRESULT (0x08U) /**< Eep_62_GetJobResult */
#define EEP_62_ID_MFUNC        (0x09U) /**< Eep_62_MainFunction */
#define EEP_62_ID_GETVInfo     (0x0AU) /**< Eep_62_GetVersionInfo */

/**
 * @name EEP_62_DEVELOP_ERROR_ID
 * @{
 * @brief Develop Error Definition
 */
#define EEP_62_E_NO_ERROR        (0x00u) /**< API function called with no det error */
#define EEP_62_E_INIT_FAILED     (0x10U) /**< Invalid  configuration set selection */
#define EEP_62_E_PARAM_ADDRESS   (0x11U) /**< Invalid  configuration set selection */
#define EEP_62_E_PARAM_DATA      (0x12U) /**< Invalid  configuration set selection */
#define EEP_62_E_PARAM_LENGTH    (0x13U) /**< Invalid  configuration set selection */
#define EEP_62_E_UNINT           (0x20U) /**< API service called without module initialization */
#define EEP_62_E_PARAM_POINTER   (0x23U) /**< API service called with a NULL pointer */
#define EEP_62_E_IN_MAINF        (0x25U) /**< RUN IN MAINFCTION when Call EEP_Cancel */
#define EEP_62_E_WRONG_CONDITION (0x26U) /**< Partition info error  */

/**
 * @name EEP_62_RUNTIME_ERROR_ID
 * @{
 * @brief Runtime Error Definition
 */
#define EEP_62_E_BUSY    (0x21U) /**< API service called while driver still busy */
#define EEP_62_E_TIMEOUT (0x22U) /**< Timeout exceeded */

/**
 * @name EEP_62_DRIVER_INFO
 * @{
 * @brief Define the type according to the driver
 */
#define Eep_AddressType Eep_62_AddressType /**< Address type */
#define Eep_LengthType  Eep_62_LengthType  /**< Length type */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == EEP_62_DEV_ERROR_DETECT)
/**
 * @brief       Report Develop Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76923
 */
EEP_62_LOCAL_INLINE void Eep_Det_ReportError(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, ApiId, ErrorId));
}

/**
 * @brief       Report Running Time Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76924
 */
EEP_62_LOCAL_INLINE void Eep_Det_ReportRunTimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(EEP_62_MODULE_ID, EEP_62_INSTANCE_ID, ApiId, ErrorId);
}
#endif

#if (EEP_62_ECUC_PARTITION_ENABLE == STD_ON)
/**
 * @brief       Check satellite partition current id
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76925
 */
EEP_62_LOCAL_INLINE boolean Eep_62_CheckPartition(void)
{
    return (boolean)(GetApplicationID() == EEP_62_PARTITION_ID);
}
#endif

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Service for EEPROM initialization.
 * @param[in]   ConfigPtr: Pointer to configuration set
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76981
 */
extern void Eep_62_Init(const Eep_62_ConfigType* ConfigPtr);

/**
 * @brief       Sets the mode.
 * @param[in]   deviceIndex: The used device index
 * @param[in]   Mode: MEMIF_MODE_SLOW: Slow read access / normal SPI access. MEMIF_MODE_FAST: Fast read access / SPI
 * burst access.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76982
 */
extern void Eep_62_SetMode(uint8 deviceIndex, MemIf_ModeType Mode);

/**
 * @brief       Reads from EEPROM.
 * @param[in]   deviceIndex: The used device index
 * @param[in]   EepromAddress: Address offset in EEPROM (will be added to the EEPROM base address). Min.: 0 Max.:
 * EEP_SIZE - 1
 * @param[in]   Length: Number of bytes to read Min.: 1 Max.: EEP_SIZE - Eeprom Address
 * @param[out]  DataBufferPtr: Pointer to destination data buffer in RAM
 * @return      Std_ReturnType
 * @retval      E_OK: read command has been accepted
 * @retval      E_NOT_OK: read command has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76936
 */
extern Std_ReturnType
    Eep_62_Read(uint8 deviceIndex, Eep_62_AddressType EepromAddress, uint8* DataBufferPtr, Eep_62_LengthType Length);

/**
 * @brief       Writes from EEPROM.
 * @param[in]   deviceIndex: The used device index
 * @param[in]   EepromAddress: Address offset in EEPROM (will be added to the EEPROM base address). Min.: 0 Max.:
 * EEP_SIZE - 1
 * @param[in]   DataBufferPtr: Pointer to source data
 * @param[in]   Length: Number of bytes to read Min.: 1 Max.: EEP_SIZE - Eeprom Address
 * @return      Std_ReturnType
 * @retval      E_OK: write command has been accepted
 * @retval      E_NOT_OK: write command has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76935
 */
extern Std_ReturnType Eep_62_Write(
    uint8              deviceIndex,
    Eep_62_AddressType EepromAddress,
    const uint8*       DataBufferPtr,
    Eep_62_LengthType  Length);

/**
 * @brief       Service for erasing EEPROM sections
 * @param[in]   deviceIndex: The used device index
 * @param[in]   EepromAddress: Address offset in EEPROM. Min.: 0 Max.: EEP_SIZE - 1
 * @param[in]   Length: Number of bytes to read Min.: 1 Max.: EEP_SIZE - Eeprom Address
 * @return      Std_ReturnType
 * @retval      E_OK: erase command has been accepted
 * @retval      E_NOT_OK: erase command has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76934
 */
extern Std_ReturnType Eep_62_Erase(uint8 deviceIndex, Eep_62_AddressType EepromAddress, Eep_62_LengthType Length);

/**
 * @brief       Compares a data block in EEPROM with an EEPROM block in the memory
 * @param[in]   deviceIndex: The used device index
 * @param[in]   EepromAddress: Address offset in EEPROM (will be added to the EEPROM base address). Min.: 0 Max.:
 * EEP_SIZE - 1
 * @param[in]   DataBufferPtr: Pointer to data buffer (compare data)
 * @param[in]   Length: Number of bytes to read Min.: 1 Max.: EEP_SIZE - Eeprom Address
 * @return      Std_ReturnType
 * @retval      E_OK: compare command has been accepted
 * @retval      E_NOT_OK: compare command has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76937
 */
extern Std_ReturnType Eep_62_Compare(
    uint8              deviceIndex,
    Eep_62_AddressType EepromAddress,
    const uint8*       DataBufferPtr,
    Eep_62_LengthType  Length);

/**
 * @brief       Cancels a running job.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76983
 */
extern void Eep_62_Cancel(uint8 deviceIndex);

/**
 * @brief       Returns the EEPROM status.
 * @return      MemIf_StatusType
 * @retval      MEMIF_UNINIT: The module has not been initialized
 * @retval      MEMIF_IDLE: The module is currently idle
 * @retval      MEMIF_BUSY: The module is currently busy
 * @retval      MEMIF_BUSY_INTERNAL: The module is busy with internal management operations.
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76984
 */
extern MemIf_StatusType Eep_62_GetStatus(uint8 deviceIndex);

/**
 * @brief       This service returns the result of the last job.
 * @return      MemIf_JobResultType
 * @retval      MEMIF_JOB_OK    : The last job has been finished successfully
 * @retval      MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed
 * @retval      MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed)
 * @retval      MEMIF_JOB_FAILED: The last job has not been finished successfully (it failed)
 * @retval      MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data
 * @retval      MEMIF_BLOCK_INVALID: The requested block has been invalidated, the requested read operation can not be
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76985
 */
extern MemIf_JobResultType Eep_62_GetJobResult(uint8 deviceIndex);

#if (EEP_62_VERSION_INFO_API == STD_ON)
/**
 * @brief       Service to get the version information of this module.
 * @param[out]  versioninfo: Pointer to where to store the version information of this module.
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76986
 */
extern void Eep_62_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#ifdef __cplusplus
}
#endif
#endif
