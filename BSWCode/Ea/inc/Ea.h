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
 **  @file               : Ea.h
 **  @author             : peng.wu
 **  @date               : 2024/12/11
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of Ea
 **
 ***********************************************************************************************************************/

/** ====================================================================================================================
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author          Description
 *  --------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2022-04-15  tao.yu          initial version
 *  V02.00.00 2022-05-13  tao.yu          update version info
 *  V02.00.01 2023-05-09  tao.yu          fix some bugs and update version info
 *  V02.00.02 2024-01-22  tao.yu          QAC
 *  V02.00.03 2024-06-15  tao.yu          update memmap
 *  V03.00.00 2024-12-11  peng.wu         Update to R23-11
 *  V03.00.01 2025-03-03  peng.wu         CPT-12915, Fix runtime error report
 *  V03.00.02 2025-11-25  peng.wu         CPT-16556, report fail if lower layer driver is not initialized
 *                                        CPT-15419, Update the result before notify upper layer
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Ea<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Ea_2669
      Reason: Need to access a certain piece of data in the array through the pointer.
      Risk: The offset value of the array buffer is calculated before it is used, no risk.
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_Ea_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_2743
      Reason: The macro defines format requirements.
      Risk: Encapsulate the det error interface and invoke the error interface only once, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_MTR_Ea_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different EaBlockConfigurations and other configuration conditions, as well as some precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_Ea_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_2983
      Reason: the issue shall be avoided in actual running environment and configs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_2906
      Reason: Definite: Positive integer value truncated by cast to a smaller unsigned type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Ea_1531
      Reason: The object '%1s' is referenced in only one translation unit - but not the one in which it is defined.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

*/

#ifndef EA_H
#define EA_H

/* =================================================== inclusions =================================================== */
#include "Ea_Types.h"
#include "MemIf_Types.h"
#if (STD_ON == EA_DEV_ERROR_DETECT)
#include "Det.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name EA_VERSION_INFO
 * @{
 * @brief Identification and version information
 */
#define EA_VENDOR_ID                   62U /**< Value of vendor */
#define EA_MODULE_ID                   40U /**< Value of module */
#define EA_INSTANCE_ID                 0U  /**< Value of instance */
#define EA_AR_RELEASE_MAJOR_VERSION    4U  /**< Value of Autosar major version */
#define EA_AR_RELEASE_MINOR_VERSION    9U  /**< Value of Autosar minor version */
#define EA_AR_RELEASE_REVISION_VERSION 0U  /**< Value of Autosar patch version */
#define EA_SW_MAJOR_VERSION            3U  /**< Value of module major version */
#define EA_SW_MINOR_VERSION            0U  /**< Value of module minor version */
#define EA_SW_PATCH_VERSION            2U  /**< Value of module patch version */

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++*/ /* VL_Ea_3414 */
#if !defined(EA_LOCAL)
#define EA_LOCAL static /**< Defines used as the local keyword for variables or functions */
#endif
/* PRQA S 3414 -- */

#if !defined(EA_LOCAL_INLINE)
#define EA_LOCAL_INLINE static inline /**< Defines used as keyword for the local inline functions */
#endif

#if !defined(EA_PARAM_UNUSED)
#define EA_PARAM_UNUSED(x) (void)(x) /**< Defines used as keyword for the unused variables */
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#if (EA_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief       Report Develop Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75783
 */
EA_LOCAL_INLINE void Ea_Det_ReportError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, ApiId, ErrorId);
}

/**
 * @brief       Report Running Time Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75784
 */
EA_LOCAL_INLINE void Ea_Det_ReportRunTimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(EA_MODULE_ID, EA_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif

/* =========================================== external data declarations =========================================== */
/** Configuration information of this module @range NA*/ /* PRQA S 1512 ++ */ /* VL_Ea_1512 */
extern const Ea_ConfigType Ea_Config;
/* PRQA S 1512 -- */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Initializes the EEPROM abstraction module.
 * @param[in]   ConfigPtr: Pointer to the selected configuration set
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77901
 */
extern void Ea_Init(const Ea_ConfigType* ConfigPtr);

/**
 * @brief       Reads Length bytes of block BlockNumber at offset BlockOffset into the buffer DataBufferPtr.
 * @param[in]   BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 * @param[in]   BlockOffset: Read address offset inside the block(read offset within block)
 * @param[in]   Length: Number of bytes to read(Length of read job)
 * @param[out]  DataBufferPtr: Pointer to data buffer
 * @return      Std_ReturnType
 * @retval      E_OK: The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77937
 */
extern Std_ReturnType Ea_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length);

/**
 * @brief       Writes the contents of the DataBufferPtr to the block BlockNumber.
 * @param[in]   BlockNumber   : Number of logical block, also denoting start address of that block in EEPROM
 * @param[in]   DataBufferPtr : Pointer to data buffer
 * @return      Std_ReturnType
 * @retval      E_OK: The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77941
 */
extern Std_ReturnType Ea_Write(uint16 BlockNumber, const uint8* DataBufferPtr);

/**
 * @brief       Cancels the ongoing asynchronous operation.
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77938
 */
extern void Ea_Cancel(void);

/**
 * @brief       Service to return the status
 * @return      MemIf_StatusType
 * @retval      MEMIF_UNINIT: The EA module has not been initialized
 * @retval      MEMIF_IDLE: The EA module is currently idle
 * @retval      MEMIF_BUSY: The EA module is currently busy
 * @retval      MEMIF_BUSY_INTERNAL: The EA module is busy with internal management operations.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77939
 */
extern MemIf_StatusType Ea_GetStatus(void);

/**
 * @brief       Service to return the job result.
 * @return      MemIf_JobResultType
 * @retval      MEMIF_JOB_OK: The last job has been finished successfully
 * @retval      MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed
 * @retval      MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed)
 * @retval      MEMIF_JOB_FAILED: The last job has not been finished successfully (it failed)
 * @retval      MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data
 * @retval      MEMIF_BLOCK_INVALID: The requested block has been invalidated, the requested read operation can not be
 * performed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77940
 */
extern MemIf_JobResultType Ea_GetJobResult(void);

/**
 * @brief       Invalidates the block BlockNumber.
 * @param[in]   BlockNumber : Number of logical block, also denoting start address of that block in EEPROM
 * @return      Std_ReturnType
 * @retval      E_OK: The requested job has been accepted by the module
 * @retval      E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77942
 */
extern Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber);

#if (EA_VERSION_INFO_API == STD_ON)
/**
 * @brief       Service to get the version information of EA module.
 * @param[out]  VersionInfoPtr   : Pointer to standard version information structure
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-77943
 */
extern void Ea_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif

/**
 * @brief       Erases the block BlockNumber.
 * @param[in]   BlockNumber: Number of logical block, also denoting start address of that block in EEPROM.
 * @return      Std_ReturnType
 * @retval      E_OK: The requested job has been accepted by the module
 * @retval      E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77944
 */
extern Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber);

/**
 * @brief       Service to report to this module the successful end of an asynchronous operation.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77936
 */
extern void Ea_JobEndNotification(void);

#ifdef __cplusplus
}
#endif
#endif
