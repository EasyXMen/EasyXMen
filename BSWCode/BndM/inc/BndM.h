
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
 **  @file               : BndM.h
 **  @author             : haibin.shao
 **  @date               : 2024/09/27
 **  @vendor             : isoft
 **  @description        : Declaration for Bulk NvData Manager
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version     Date          Author          Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V3.0.0      2025-7-8      haibin.shao     First version
 *  V3.0.1      2026-5-27     haibin.shao     The notification callback function for the MemAcc component adaptation
 *  V3.0.2      2026-06-03    tao.yu          add arti
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanNm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_BndM_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_0306
      Reason: needs to convert integer Pointers into Pointers pointing to objects.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_0309
      Reason: The code needs to convert.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_0488
      Reason: Calculates the offset value of the array.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_0750
      Reason: Fast variable type conversion, Guaranteed data consistency, Low space consumption.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_0759
      Reason: Fast variable type conversion, Guaranteed data consistency, Low space consumption.
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_3218
      Reason: variable design requires
      Risk: No risk.
      Prevention: None.

    \li VL_BndM_0311
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: None.
 */

#ifndef BNDM_H_
#define BNDM_H_

/* =================================================== inclusions =================================================== */
#include "BndM_Cfg.h"
#include "BndM_Types.h"
#include "BndM_OptMacros.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/**
 * @name AR version
 * @{
 * @brief The AR version information for Bulk NvData Manager
 */
#define BNDM_AR_RELEASE_MAJOR_VERSION    (4u) /**< Indicate that the major version number of AR */
#define BNDM_AR_RELEASE_MINOR_VERSION    (9u) /**< Indicate that the minor version number of AR */
#define BNDM_AR_RELEASE_REVISION_VERSION (0u) /**< Indicate that the revision version number of AR */
/** @} */

/**
 * @name SW version
 * @{
 * @brief The software version information for Bulk NvData Manager
 */
#define BNDM_SW_MAJOR_VERSION (3u) /**< Indicate that the major version number of SW */
#define BNDM_SW_MINOR_VERSION (0u) /**< Indicate that the minor version number of SW */
#define BNDM_SW_PATCH_VERSION (2u) /**< Indicate that the patch version number of SW */
/** @} */

/**
 * @name BndM ID
 * @{
 * @brief The module label identifier used to describe the BndM component
 */
#define BNDM_VENDOR_ID   (62u)  /**< Indicate that the vendor ID of the BndM component */
#define BNDM_MODULE_ID   (251u) /**< Indicate that the module ID of the BndM component */
#define BNDM_INSTANCE_ID (0U)   /**< Indicate that the instance ID of the BndM component */
/** @} */

/* ===================================================== macros ===================================================== */

/**
 * @name Development error
 * @{
 * @brief used to describe the BndM module development error code
 */
#define BNDM_SERVICE_ID_INIT               (0x01u) /**< The service ID indicating the 'Init' function */
#define BNDM_SERVICE_ID_GETVERSIONINFO     (0x02u) /**< The service ID indicating the 'GetVersion' function */
#define BNDM_SERVICE_ID_GETBLOCKPTR        (0x03u) /**< The service ID indicating the 'GetBlockPtr' function */
#define BNDM_SERVICE_ID_WRITESTART         (0x04u) /**< The service ID indicating the 'WriteStart' function */
#define BNDM_SERVICE_ID_WRITEBLOCK         (0x05u) /**< The service ID indicating the 'WriteBlock' function */
#define BNDM_SERVICE_ID_WRITEFINALIZE      (0x06u) /**< The service ID indicating the 'WriteFinalize' function */
#define BNDM_SERVICE_ID_WRITECANCEL        (0x07u) /**< The service ID indicating the 'WriteCancel' function */
#define BNDM_SERVICE_ID_JOBENDNOTIFICATION (0x08u) /**< The service ID indicating the 'JobEndNotification' function */
#define BNDM_SERVICE_ID_MAINFUNCTION       (0x55u) /**< The service ID indicating the 'MainFunction' function */
/** @} */

/**
 * @name Service ID
 * @{
 * @brief The service interface identifier used to describe the BndM module
 */
#define BNDM_E_NO_ERROR       (0x00u) /**< Indicate that no development error was detected */
#define BNDM_E_PARAM          (0x01u) /**< The development error indicating an null pointer */
#define BNDM_E_WRONG_SEQUENCE (0x02u) /**< The development error indicating an wrong sequence */

/** @} */

#define BNDM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "BndM_MemMap.h"
/**
 * @brief The global PB configuration structure of the BndM module
 * @range N/A
 */
extern const BndM_ConfigType BndM_Config;
#define BNDM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to initializes the bulk non volatile Data Manager.
 * @param[in]    ConfigPtr: Pointer to the selected configuration structure.
 * @return       void
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern void BndM_Init(const BndM_ConfigType* ConfigPtr);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#if (BNDM_VERSION_INFO_API == STD_ON)
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to get the version information of BndM module.
 * @param[out]   versioninfo: Pointer to where to store the version information of BndM module
 * @return       void
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern void BndM_GetVersionInfo(Std_VersionInfoType* versioninfo);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#endif
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to will trigger the start of writing phase.
 * @return       Std_ReturnType
 * @retval       E_OK: The preparation request is accepted.
 * @retval       E_NOT_OK: The preparation request is rejected.
 * @reentrant    FALSE
 * @synchronous  FALSE
 * @trace        CPD-PLACEHOLDE
 */
extern Std_ReturnType BndM_WriteStart(void);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to will trigger the finalization of writing phase.
 * @return       Std_ReturnType
 * @retval       E_OK: The finalization request is accepted.
 * @retval       E_NOT_OK: The finalization request is rejected.
 * @reentrant    Reentrant Finalize the writing.
 * @synchronous  FALSE
 * @trace        CPD-PLACEHOLDE
 */
extern Std_ReturnType BndM_WriteFinalize(void);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to will trigger the cancel of writing phase.
 * @return       void
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-PLACEHOLDE
 */
extern void BndM_WriteCancel(void);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        This callback function is called when a MemAcc job has been finished with positive result.
 * @return       void
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern void BndM_JobEndNotification(MemAcc_AddressAreaIdType addressAreaId, MemAcc_JobResultType jobResult);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Schedule function for the background processing.
 * @return       void
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern void BndM_MainFunction(void);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif /* BNDM_H_ */
