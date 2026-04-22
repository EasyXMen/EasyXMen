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
 **  @file               : Fee.h
 **  @author             : peng.wu
 **  @date               : 2024/09/15
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of FEE
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author          Description
 *  --------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2024-09-15  zhengfei.li     1. Initial
 *  V02.00.00 2021-08-27  zhengfei.li     Release version
 *  V02.00.01 2023-10-24  peng.wu         CPT-7312, Added the declaration of configuration parameters
 *  V02.00.02 2023-12-25  tao.yu          remove SchM
 *  V02.00.03 2024-05-14  haibin.shao     adj Det restriction and macro definition location
 *  V03.00.00 2024-09-15  peng.wu         Update to R23-11
 *  V03.00.01 2024-10-22  peng.wu         CPT-10925, Fix coverage issues with some code
 *                                        Add QAC
 *                                        CPT-13146, Delete CALLBACK_CODE
 *  V03.01.00 2025-09-18  haibin.shao      Add the Cluster group feature function for the Fee
 *  V03.01.01 2025-11-21  peng.wu          Fix the swap trap issue
 *  V03.01.02 2025-01-08  peng.wu         CPT-17546, Fix page switching, the issue of inconsistent data read back.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanNm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Fee_2669
      Reason: Need to access a certain piece of data in the array through the pointer.
      Risk: The offset value of the array buffer is calculated before it is used, no risk.
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_Fee_2814
      Reason: The configuration array used is a null pointer.
      Risk: This array is configuration data and will always be generated, there is no null pointer case, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2824
      Reason: The array index value passed may be out of range, causing a null pointer to be obtained.
      Risk: The passed array index value is obtained only within the configured block range, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2844
      Reason: When using arrays, index values can be out of range, resulting in access errors.
      Risk: The passed array index value is obtained only within the configured block range, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2908
      Reason: To calculate each byte of data, it is necessary to split the large integer data.
      Risk: The splitting of data is performed on a byte scale, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2991
      Reason: The macro defines format requirements.
      Risk: Check whether errorId is valid before using it, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2995
      Reason: The status or result needs to be checked for validity.
      Risk: This value is defined or modified before the status or result is checked, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_0488
      Reason: Calculates the offset value of the array.
      Risk: The offset value is calculated with a fixed length, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_0490
      Reason: Need to compare whether the start address is within the range of the end address.
      Risk: The offset calculation of the pointer is fixed and only increases by 1, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_1290
      Reason: The data type needs to be converted when filling the data of the array.
      Risk: The parameters passed in are defined as unsigned types, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2743
      Reason: The macro defines format requirements.
      Risk: Encapsulate the det error interface and invoke the error interface only once, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_2880
      Reason: The macro defines format requirements.
      Risk: Encapsulate the det error interface and invoke the error interface only once, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_3103
      Reason: The middle value should be taken when the dichotomy is calculated.
      Risk: A data type conversion is performed when the value is assigned, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_3120
      Reason: In the generated code, there are a lot of devil numbers, don't have to define a macro.
      Risk: The reader can derive the meaning of the number based on the annotation or structure type, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_3408
      Reason: Internal variables have no external declaration.
      Risk: Internal variables are provided for use in this file only, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_3412
      Reason: Macro definitions such as error reporting and offset calculation are multi-line definitions.
      Risk: The macro defines format requirements, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_5004
      Reason: Function pointer design requirements.
      Risk: The configured function interfaces need to define the same parameters, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Fee_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Fee_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Fee_1253
      Reason: Converted to different integer type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Fee_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different FeeClusters, FeeBlockConfigurations and other configuration conditions, as well as some precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_Fee_1531
      Reason: The object '%1s' is referenced in only one translation unit - but not the one in which it is defined.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Fee_3223
      Reason: Bitmap initialization usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Fee_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Fee_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
location.
      Risk: No risk.
      Prevention: None.

 */

#ifndef FEE_H
#define FEE_H

/* =================================================== inclusions =================================================== */
#include "Fee_Lcfg.h"
#if (STD_ON == FEE_DEV_ERROR_DETECT)
#include "Det.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/** Vendor and module identification */
#define FEE_VENDOR_ID   62u
#define FEE_MODULE_ID   21u
#define FEE_INSTANCE_ID 0u

/** AUTOSAR Software specification version information */
#define FEE_AR_RELEASE_MAJOR_VERSION    (4u)
#define FEE_AR_RELEASE_MINOR_VERSION    (9u)
#define FEE_AR_RELEASE_REVISION_VERSION (0u)

/** Software version information */
#define FEE_SW_MAJOR_VERSION (3u)
#define FEE_SW_MINOR_VERSION (1u)
#define FEE_SW_PATCH_VERSION (2u)

/* ===================================================== macros ===================================================== */
/** Defines used as the local keyword for variables or functions */
#if !defined(FEE_LOCAL)
#define FEE_LOCAL static /* PRQA S 3414 */ /* VL_Fee_3414 */
#endif

/** Defines used as keyword for the local inline functions */
#if !defined(FEE_LOCAL_INLINE)
#define FEE_LOCAL_INLINE static inline
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == FEE_DEV_ERROR_DETECT)
/**
 * @brief Report Develop Error
 */
FEE_LOCAL_INLINE void Fee_Det_ReportError(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(FEE_MODULE_ID, FEE_INSTANCE_ID, ApiId, ErrorId));
}

/**
 * @brief Report Running Time Error
 */
FEE_LOCAL_INLINE void Fee_Det_ReportRunTimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(FEE_MODULE_ID, FEE_INSTANCE_ID, ApiId, ErrorId);
}
#endif

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/* PRQA S 1513,1512 EOF */ /* VL_Fee_1513,VL_Fee_1512 */
/**
 * @brief       Service to initialize the FEE module
 * @param[in]   ConfigPtr   : Pointer to the selected configuration set
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-61309
 */
extern void Fee_Init(const Fee_ConfigType* ConfigPtr);

#if (FEE_SET_MODE_SUPPORTED == STD_ON)
/**
 * @brief       Function to switch the mode of the underlying Flash Driver
 * @param[in]   Mode   : Desired mode for the underlying flash driver
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-62221
 */
extern void Fee_SetMode(MemIf_ModeType Mode);
#endif

/**
 * @brief       Service to initiate a read job
 * @param[in]   BlockNumber   : Number of logical block, also denoting start address of that block in flash memory
 * @param[in]   BlockOffset    : Read address offset inside the block
 * @param[in]   Length : Number of bytes to read
 * @param[out]  DataBufferPtr : Pointer to data buffer
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-61308
 */
extern Std_ReturnType Fee_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length);

/**
 * @brief       Service to initiate a write job
 * @param[in]   BlockNumber   : Number of logical block, also denoting start address of that block in Flash EEPROM
 * @param[in]   DataBufferPtr : Pointer to data buffer
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-61307
 */
extern Std_ReturnType Fee_Write(uint16 BlockNumber, const uint8* DataBufferPtr);

/**
 * @brief       Service to call the cancel function of the underlying flash driver
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-61306
 */
extern void Fee_Cancel(void);

/**
 * @brief       Service to return the status
 * @return      MemIf_StatusType
 * @retval      MEMIF_UNINIT    : The FEE module has not been initialized
 * @retval      MEMIF_IDLE    : The FEE module is currently idle
 * @retval      MEMIF_BUSY    : The FEE module is currently busy
 * @retval      MEMIF_BUSY_INTERNAL    : The FEE module is busy with internal management operations.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-61305
 */
extern MemIf_StatusType Fee_GetStatus(void);

/**
 * @brief       Service to query the result of the last accepted job issued by the upper layer software
 * @return      MemIf_JobResultType
 * @retval      MEMIF_JOB_OK    : The last job has been finished successfully
 * @retval      MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed
 * @retval      MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed)
 * @retval      MEMIF_JOB_FAILED: The last job has not been finished successfully (it failed)
 * @retval      MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data
 * @retval      MEMIF_BLOCK_INVALID: The requested block has been invalidated, the requested read operation can not be
 * performed
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-61304
 */
extern MemIf_JobResultType Fee_GetJobResult(void);

/**
 * @brief       Service to invalidate a logical block.
 * @param[in]   BlockNumber : Number of logical block, also denoting start address of that block in flash memory
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-61303
 */
extern Std_ReturnType Fee_InvalidateBlock(uint16 BlockNumber);

#if (FEE_VERSION_INFO_API == STD_ON)
/**
 * @brief       Service to return the version information of the FEE module
 * @param[out]  Versioninfo   : Pointer to standard version information structure
 * @reentrant   Reentrant
 * @synchronous Synchronous
 * @trace       CPD-61302
 */
extern void Fee_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif

/**
 * @brief       Service to erase a logical block.
 * @param[in]   BlockNumber    : Number of logical block, also denoting start address of that block in EEPROM.
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant   Non Reentrant
 * @synchronous Asynchronous
 * @trace       CPD-61301
 */
extern Std_ReturnType Fee_EraseImmediateBlock(uint16 BlockNumber);

#ifdef __cplusplus
}
#endif
#endif
