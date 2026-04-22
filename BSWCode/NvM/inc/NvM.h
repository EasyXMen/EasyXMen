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
 **  @file               : NvM.h
 **  @author             : peng.wu
 **  @date               : 2024/08/27
 **  @vendor             : isoft
 **  @description        : NVRAM manager code declarations
 **
 ***********************************************************************************************************************/
/* PRQA S 2053 ++ */ /* VL_NvM_2053 */
/** ====================================================================================================================
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------
 *  *
 *  Version   Date        Author       Description
 *  --------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2020-5-9    tao.yu        Create
 *  V02.00.00 2021-4-8    tao.yu        release version
 *  V02.00.01 2023-8-10   xue.han       QAC Change
 *  V02.00.02 2023-10-17  tao.yu        Optimize code, add comments
 *  V02.00.03 2023-11-7   peng.wu       CPT-7503, Fix the ReadReq problem when NVM_WRITE_BLOCK_ONCE is enabled
 *                                      CPT-7477, Verifies the NvMDatasetSelectionBits in the configuration
 *  V02.00.04 2024-06-24  tao.yu        CPT-9295, fix NVM_CANCEL_WRITE_ALL notification
 *  V03.00.00 2024-08-28  peng.wu       Update to R23-11
 *  V03.00.01 2024-10-22  peng.wu       CPT-10763, fix sharing buffer between redundant blocks
 *                                      CPT-10775, Fixed single-block notification callbacks in read cases
 *                                      Add QAC
 *  V03.00.02 2025-2-20  peng.wu        CPT-12258,CPT-12259,CPT-12261,CPT-12264,CPT-12362,CPT-12441,CPT-12446,
 *                                      CPT-12479,CPT-12483,CPT-12507,CPT-12519,CPT-12552,CPT-12554,CPT-12566,
 *                                      CPT-12586,CPT-12616,CPT-12655,CPT-12673,CPT-12687,CPT-12692,CPT-12715,
 *                                      CPT-12728,CPT-12770,CPT-12789,CPT-12794,Fix coverage
 *                                      CPT-13053,Fix preemption problem and header including
 *                                      CPT-13353,Added judgment for PRAM service in callback notification
 *                                      CPT-13500,Fix invalid/inconsistent result in error notification
 *                                      CPT-13643,Fix the logic for reading the first block during power-on
 *  V03.00.03 2025-4-23  peng.wu        CPT-13888,Fix problems under non-polling mode
 *  V03.00.04 2025-5-07  peng.wu        CPP-16958,Fix invalidate process during reading or writing
 *  V03.00.05 2025-6-10  peng.wu        CPT-14478,Fix the falg bit setting for writing once
 *                                      CPT-12770,Add the judgment status of pengding
 *            2025-6-24  peng.wu        CPT-14947,Fix the result after restoring the rom block
 *                                      CPT-14743,Fix the header file of the cfg file containing
 *                                      CPT-14957,Fix the result updating if called the error notification
 *                                      CPT-14979,Fix the CRC comparison anomaly issue
 *                                      CPT-15530,Fix the result to E_NOT_OK if initialization callback returns E_NOT_OK
 *                                      CPT-15531,Fix NvMSelectBlockForWriteAll block checked
 * V03.00.06 2025-10-24  haibin.shao    CPT-16144,Remove the allocation of redundant RAM
 *           2025-10-28  peng.wu        CPT-16159, Fix the mirror buffer used by the alignment function
 *                                      CPT-16038, Fix the issue of obtaining the encrypted length
 * V03.00.07 2025-11-24  haibin.shao    CPD-85532,For the functional points, add corresponding macro switch restrictions
 *                                      CPD-85533,Improve the processing logic and optimize the algorithm for the
 *                                                functional points
 *                                      CPD-85534,Optimize the algorithm for functional points to improve execution
 *                                                efficiency
 * V03.00.08 2025-12-05  peng.wu        CPT-16407, Fixed the data disorder when compress was configured
 *                                      CPD-16384, Fix the null buffer
 *                                      CPD-17183, Delete partition protect in get status api
 * V03.00.09 2026-02-09  peng.wu        CPT-18018, Fix the error in the pre-compilation switch for getting Ram Address
 *
 ==================================================================================================================== */
/* PRQA S 2053 -- */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:NvM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_NvM_2824
      Reason: Operations are performed through blocks in the configured array.
      Risk: The passed array index value is obtained only within the configured block range, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_2842
      Reason: The block id is used to record data. The block id is generated by the configuration.
      Risk: The passed array index value is obtained only within the configured block range, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_2844
      Reason: The block id is used to record data. The block id is generated by the configuration.
      Risk: The passed array index value is obtained only within the configured block range, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_2903
      Reason: The last bit of the queue records the number of current queues.
      Risk: This value is calculated on this interface only when it is in the queue, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_2985
      Reason: Block information needs to be recorded in the current queue to sort multiple jobs.
      Risk: This value is calculated on this interface only when it is in the queue, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_0311
      Reason: When a read request job is enqueued, the address cannot be const.
      Risk: The source address is not assigned when queued write request, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_0314
      Reason: void is specified in the autosar prototype interface requirements.
      Risk: In the subsequent use, operations are performed according to the specific type, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_0316
      Reason: void is specified in the autosar prototype interface requirements.
      Risk: In the subsequent use, operations are performed according to the specific type, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_0317
      Reason: void is specified in the autosar prototype interface requirements.
      Risk: In the subsequent use, operations are performed according to the specific type, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_0489
      Reason: ConfigId needs to be stored in the next uint address.
      Risk: The address of the operation is within the allowed range, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_3432
      Reason: Simple macro parameter expressions are only used for conditional judgments and do not require parentheses.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_0306
      Reason: Design and configure to ensure the security of pointer type conversion for different objects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_1334
      Reason: Only parameter name is different.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_NvM_3218
      Reason: To reduce the number of lines of code in a specific interface.
      Risk: The scope is within this file, but will not be accessed by other files, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_1514
      Reason: This variable needs to be used in the user's callback interface.
      Risk: It depends on the user configuration, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_5004
      Reason: Function pointer design requirements.
      Risk: The configured function interfaces need to define the same parameters, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_NvM_1512
      Reason: multiple declaration is necessary for RTE
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_0624
      Reason: function parameter can be considered as the same via typedef
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_2991
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_2995
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_3673
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_NvM_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different NvMBlockCipherings, NvMBlockDescriptors and other configuration conditions, as well as some precompiled
  macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_NvM_1501
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_4397
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_4399
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_4391
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_3678
      Reason: non-const type declaration is necessary depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_NvM_STCAL
      Reason: The software architecture is defined according to the AUTOSAR standard.
      Risk: Due to the fan distribution to many functions, understandability and testability can become overly complex.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_NvM_2880
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_3209
      Reason: '%1s()' returns a value which is always ignored.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_NvM_STPTH
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_NvM_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_1881
      Reason: The operands of this equality operator are expressions of different 'essential type' categories (%1s and
  %2s). Risk: No risk. Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_NvM_STCYC
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_NvM_2981
      Reason: This assignment is redundant. The value of this object is never used before being modified.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_2982
      Reason: The assignment is reserved for better readibility.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

     \li VL_NvM_2741
      Reason: This 'if' controlling expression is a constant expression and its value is 'true'.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

     \li VL_NvM_1531
      Reason: The object '%1s' is referenced in only one translation unit - but not the one in which it is defined.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

     \li VL_MTR_NvM_STLIN
      Reason: Splitting them into smaller parts will lead to performance degradation.
      Risk: No risk.
      Prevention: Design and code review + clear structure and well-commented code.

     \li VL_MTR_NvM_STST3
      Reason: Splitting them into smaller parts will lead to performance degradation.
      Risk: No risk.
      Prevention: Design and code review + clear structure and well-commented code.

    \li VL_NvM_2053
      Reason: Special code design of the NvM.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_NvM_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
*/

#ifndef NVM_H
#define NVM_H

/* =================================================== inclusions =================================================== */
#include "NvM_Lcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/** Vendor and module identification */
#define NVM_VENDOR_ID   (62u) /**< Value of vendor */
#define NVM_MODULE_ID   (13u) /**< Value of module */
#define NVM_INSTANCE_ID (0u)  /**< Value of instance */

/** AUTOSAR Software specification version information */
#define NVM_AR_RELEASE_MAJOR_VERSION    (4u) /**< Value of Autosar major version */
#define NVM_AR_RELEASE_MINOR_VERSION    (9u) /**< Value of Autosar minor version */
#define NVM_AR_RELEASE_REVISION_VERSION (0u) /**< Value of Autosar patch version */

/** Software version information */
#define NVM_SW_MAJOR_VERSION (3u) /**< Value of module major version */
#define NVM_SW_MINOR_VERSION (0u) /**< Value of module minor version */
#define NVM_SW_PATCH_VERSION (9u) /**< Value of module patch version */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/**
 * @brief Container for a management structure to configure the composition of a given NVRAM Block Management Type
 * @range NA
 */
extern const NvM_BlockDescriptorType NvM_BlockDescriptor[NVM_BLOCK_NUM_ALL];

/**
 * @brief Container for NvmMultiBlockCallback @range NA
 */
extern const NvM_MultiBlockCallbackType NvmMultiBlockCallback;

#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
/**
 * @brief Mirror buffer for synchronization mechanism @range NA
 */
extern uint8 NVM_RamMirror[NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR];
#endif

/* ========================================= external function declarations ========================================= */
/* PRQA S 1512 EOF */          /* VL_NvM_1512 */
/* PRQA S 3449,3451,3209 ++ */ /* VL_QAC_MultiDeclaration,VL_QAC_MultiDeclaration,VL_NvM_3209 */
/**
 * @brief       Service for resetting all internal variables
 * @param[in]   ConfigPtr: Pointer to the selected configuration set
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76597
 */
extern void NvM_Init(const NvM_ConfigType* ConfigPtr);

#if (STD_ON == NVM_VERSION_INFO_API)
/**
 * @brief       Service to get the version information of the NvM module
 * @param[out]  VersionInfo: Pointer to where to store the version information of this module
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76598
 */
extern void NvM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif

/**
 * @brief       Service to read the block dependent error/status information
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[out]  RequestResultPtr: Pointer to where to store the request result
 * @return      Std_ReturnType
 * @retval      E_OK: The block dependent error/status information was read successfully
 * @retval      E_NOT_OK: An error occured
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76599
 */
/* PRQA S 1513 ++ */ /* VL_QAC_MultiDeclaration */
extern Std_ReturnType NvM_GetErrorStatus(NvM_BlockIdType BlockId, NvM_RequestResultType* RequestResultPtr);

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/**
 * @brief       Service for setting the RAM block status of a permanent RAM block or the status of
 *              the explicit synchronization of a NVRAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   BlockChanged: TRUE : Validate the RAM block and mark block as changed
 * @param[in]                 FALSE: Invalidate the RAM block and mark block as unchanged
 * @return      Std_ReturnType
 * @retval      E_OK: The status of the permanent RAM block or the explicit
 *                        synchronization was changed as requested
 * @retval      E_NOT_OK: An error occurred
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76600
 */
/* PRQA S 0624 ++ */ /* VL_NvM_0624 */
extern Std_ReturnType NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged);
/* PRQA S 0624 -- */
#endif
/**
 * @brief       Service to cancel a running NvM_WriteAll request
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76601
 */
extern void NvM_CancelWriteAll(void);

/**
 * @brief       Initiates a multi block read request
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76602
 */
extern void NvM_ReadAll(void);

/**
 * @brief       Initiates a multi block write request
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76603
 */
extern void NvM_WriteAll(void);

#if ((NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS) && (STD_ON == NVM_SELECT_BLOCK_FOR_FIRST_INIT_ALL))
/**
 * @brief       The function initiates a multi block first initialization request
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76604
 */
extern void NvM_FirstInitAll(void);
#endif

/**
 * @brief       Service to copy the data of the permanent RAM block to its corresponding NV block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous FALSE
 * @trace       CPD-76605
 */
extern Std_ReturnType NvM_WritePRAMBlock(NvM_BlockIdType BlockId);

/**
 * @brief       Service to restore the default data to its corresponding permanent RAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76606
 */
extern Std_ReturnType NvM_RestorePRAMBlockDefaults(NvM_BlockIdType BlockId);

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief       Service for setting the DataIndex of a dataset NVRAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   DataIndex: Index of a dataset NVRAM block
 * @return      Std_ReturnType
 * @retval      E_OK: The index position was set successfully
 * @retval      E_NOT_OK: An error occurred
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76607
 */
extern Std_ReturnType NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex);

/**
 * @brief       Service for getting the currently set DataIndex of a dataset NVRAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[out]  DataIndexPtr: Pointer to where to store the current dataset index (0..255)
 * @return      Std_ReturnType
 * @retval      E_OK: The index position has been retrieved successfully
 * @retval      E_NOT_OK: An error occurred
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76608
 */
/* PRQA S 0624 ++ */ /* VL_NvM_0624 */
extern Std_ReturnType NvM_GetDataIndex(NvM_BlockIdType BlockId, uint8* DataIndexPtr);
/* PRQA S 0624 -- */

/**
 * @brief       Service to copy the data of the NV block to its corresponding permanent RAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous FALSE
 * @trace       CPD-76609
 */
extern Std_ReturnType NvM_ReadPRAMBlock(NvM_BlockIdType BlockId);

/**
 * @brief       Service to copy the data of the NV block to its corresponding RAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   NvM_DstPtr: Pointer to the RAM data block
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous FALSE
 * @trace       CPD-76610
 */
extern Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, void* NvM_DstPtr);

/**
 * @brief       Service to copy the data of the RAM block to its corresponding NV block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   NvM_SrcPtr: Pointer to the RAM data block
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous FALSE
 * @trace       CPD-76611
 */
extern Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId, const void* NvM_SrcPtr);

/**
 * @brief       Service to restore the default data to its corresponding RAM block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[out]  NvM_DestPtr: Pointer to the RAM data block
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76612
 */
extern Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, void* NvM_DestPtr);

/**
 * @brief       Service to cancel all jobs pending for a NV block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @return      Std_ReturnType
 * @retval      E_OK: The job was successfully removed from queue
 * @retval      E_NOT_OK: The job could not be found in the queue
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76613
 */
extern Std_ReturnType NvM_CancelJobs(NvM_BlockIdType BlockId);

/**
 * @brief       Initiates a multi block validation request
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-76614
 */
extern void NvM_ValidateAll(void);

#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief       Service for setting/resetting the write protection for a NV block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   ProtectionEnabled: TRUE : Write protection shall be enabled
 *                                 FALSE: Write protection shall be disabled
 * @return      Std_ReturnType
 * @retval      E_OK: The block was enabled/disabled as requested
 * @retval      E_NOT_OK: An error occurred
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76615
 */
/* PRQA S 0624 ++ */ /* VL_NvM_0624 */
extern Std_ReturnType NvM_SetBlockProtection(NvM_BlockIdType BlockId, boolean ProtectionEnabled);
/* PRQA S 0624 -- */

/**
 * @brief       Service to invalidate a NV block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous FALSE
 * @trace       CPD-76616
 */
extern Std_ReturnType NvM_InvalidateNvBlock(NvM_BlockIdType BlockId);

/**
 * @brief       Service to erase a NV block
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous FALSE
 * @trace       CPD-76617
 */
extern Std_ReturnType NvM_EraseNvBlock(NvM_BlockIdType BlockId);
#endif
/* PRQA S 3449,3451,3209 -- */
/* PRQA S 1513 -- */
#ifdef __cplusplus
}
#endif
#endif
