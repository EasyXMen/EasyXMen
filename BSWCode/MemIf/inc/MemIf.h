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
/*
 ************************************************************************************************************************
 **
 **  @file               : MemIf.h
 **  @author             : tao.yu
 **  @date               : 2024/09/19
 **  @vendor             : isoft
 **  @description        : API declaration of MemIf
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:MemIf<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_MemIf_1531
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MemIf_5004
      Reason: pointer type definition is essential
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MemIf_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MemIf_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_MemIf_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  MemIfNumberOfDevices and other configuration conditions, as well as some precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

 */

#ifndef MEMIF_H_
#define MEMIF_H_

/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2020-08-26  Haibin.Shao   Initial version
 *  V01.00.01 2021-12-06  Haibin.Shao   Modify version(update version information)
 *  V02.00.00 2022-04-07  tao.yu        ReWork mode and update to R19-11
 *  V02.00.01 2023-06-07  peng.wu       Fix CPT-555:Modify the comment header info
 *  V02.00.02 2023-09-18  tao.yu        add type description
 *  V03.00.00 2024-09-19  tao.yu        ReWork and update to R23-11
 *  V03.00.01 2025-05-06  peng.wu       CPT-14238, Fix incorrect state when an invalid DeviceIndex is passed in
 *
 ==================================================================================================================== */

/* =================================================== inclusions =================================================== */
#include "MemIf_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
#define MEMIF_VENDOR_ID                   (62u) /** Value of vendor */
#define MEMIF_MODULE_ID                   (22u) /** Value of module */
#define MEMIF_INSTANCE_ID                 (0u)  /** Value of instance */
#define MEMIF_AR_RELEASE_MAJOR_VERSION    (4u)  /** Value of Autosar major version */
#define MEMIF_AR_RELEASE_MINOR_VERSION    (9u)  /** Value of Autosar minor version */
#define MEMIF_AR_RELEASE_REVISION_VERSION (0u)  /** Value of Autosar patch version */
#define MEMIF_SW_MAJOR_VERSION            (3u)  /** Value of module major version */
#define MEMIF_SW_MINOR_VERSION            (0u)  /** Value of module minor version */
#define MEMIF_SW_PATCH_VERSION            (1u)  /** Value of module patch version */

/* ===================================================== macros ===================================================== */
#define MEMIF_BROADCAST_ID (0xFFu) /**< Value of broadcast in multiple device */

/* ========================================= external function declarations ========================================= */

#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief       Invokes the "Read" function of the underlying memory abstraction module selected by the parameter
 *              DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @param[in]   BlockNumber: number of logic block
 * @param[in]   BlockOffset: Read address offset inside the block
 * @param[in]   Length: Number of bytes to read
 * @param[out]  DataBufferPtr: Pointer to data buffer
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75681
 */
extern Std_ReturnType
    MemIf_Read(uint16 DeviceIndex, uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length);

/**
 * @brief       Invokes the "Write" function of the underlying memory abstraction module selected by the parameter
 *              DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @param[in]   BlockNumber: number of logic block
 * @param[in]   DataBufferPtr: Pointer to data buffer
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75682
 */
extern Std_ReturnType MemIf_Write(uint16 DeviceIndex, uint16 BlockNumber, const uint8* DataBufferPtr);

/**
 * @brief       Invokes the "Cancel" function of the underlying memory abstraction module selected by the parameter
 *              DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75683
 */
extern void MemIf_Cancel(uint16 DeviceIndex);

/**
 * @brief       Invokes the "GetJobResult" function of the underlying memory abstraction module selected by the
 *              parameter DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @return      MemIf_StatusType
 * @retval      MEMIF_JOB_OK: The job has been finished successfully
 * @retval      MEMIF_JOB_FAILED: The job has not been finished successfully
 * @retval      MEMIF_JOB_PENDING: The job has not yet been finished.
 * @retval      MEMIF_JOB_CANCELED: The job has been canceled.
 * @retval      MEMIF_BLOCK_INCONSISTENT: 1. The requested block is inconsistent, it may contain corrupted data.
 *                                        2. Block is NOT found.
 * @retval      MEMIF_BLOCK_INVALID: The requested block has been marked as invalid, the requested operation can not be
 *                                   performed.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75684
 */
extern MemIf_JobResultType MemIf_GetJobResult(uint16 DeviceIndex);

/**
 * @brief       Invokes the "InvalidateBlock" function of the underlying memory abstraction module selected by the
 *              parameter DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @param[in]   BlockNumber: number of logic block
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75685
 */
extern Std_ReturnType MemIf_InvalidateBlock(uint16 DeviceIndex, uint16 BlockNumber);

/**
 * @brief       Invokes the "EraseImmediateBlock" function of the underlying memory abstraction module selected by the
 *              parameter DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @param[in]   BlockNumber: number of logic block
 * @return      Std_ReturnType
 * @retval      E_OK    : The requested job has been accepted by the module
 * @retval      E_NOT_OK: The requested job has not been accepted by the module.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75686
 */
extern Std_ReturnType MemIf_EraseImmediateBlock(uint16 DeviceIndex, uint16 BlockNumber);
#endif

#if (MEMIF_DEV_ERROR_DETECT == STD_ON) || (MEMIF_NUMBER_OF_DEVICES > 1U)
/**
 * @brief       Invokes the "GetStatus" function of the underlying memory abstraction module selected by the parameter
 *              DeviceIndex.
 * @param[in]   DeviceIndex: index number of device
 * @return      MemIf_StatusType
 * @retval      MEMIF_UNINIT: The underlying abstraction module or device driver has not been initialized (yet).
 * @retval      MEMIF_IDLE: The underlying abstraction module or device driver is currently idle.
 * @retval      MEMIF_BUSY: The underlying abstraction module or device driver is currently busy.
 * @retval      MEMIF_BUSY_INTERNAL: The underlying abstraction module is busy with internal management operations. The
 *                                   underlying device driver can be busy or idle.
 * @reentrant   Non Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75687
 */
extern MemIf_StatusType MemIf_GetStatus(uint16 DeviceIndex);
#endif

#if (MEMIF_VERSION_INFO_API == STD_ON)
/**
 * @brief       Returns version information.
 * @param[out]  VersionInfoPtr: Pointer to standard version information structure.
 * @reentrant   Reentrant
 * @synchronous Synchronous
 * @trace       CPD-75688
 */
extern void MemIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif

#ifdef __cplusplus
}
#endif

#endif

/* ----------------------------------------------------- end of file ------------------------------------------------ */
