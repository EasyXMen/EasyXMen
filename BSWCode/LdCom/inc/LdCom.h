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
 **  @file               : LdCom.h
 **  @author             : zhengfei.li
 **  @date               : 2020/07/03
 **  @vendor             : isoft
 **  @description        : Large Data Communication header
 **
 ***********************************************************************************************************************/

/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 2020-07-03  zhengfei.li  Initial version (Upgrade according to the R19_11 standards)
 *  V02.00.01 2021-01-08  zhengfei.li  Optimize macro names
 *  V02.00.02 2023-04-25  tong.zhao    add NULL pointer check for API parameter((JIRA: CPT-267))
 *  V02.00.03 2023-11-23  tong.zhao    Code Execution Optimization.(JIRA: CPD-33590)
 *  V02.00.04 2024-02-29  tong.zhao    QAC check issue fix
 *  V02.00.05 2024-04-19  tong.zhao    Modify the logic error that occurs when PduIdType is uint8.
 *  V02.00.06 2024-06-14  tong.zhao    Modify callback funcation logic.
 *  V02.01.00 2024-10-14  fupeng.yu    Support R23-11.
 *  V02.02.00 2025-04-29  fupeng.yu    Optimized code.
 *  V02.02.01 2025-06-05  fupeng.yu    Resolve the conditional judgment on PduId in function LdCom_DetCheckPduId.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LdCom<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li PRQA S 1532 VL_LdCom_CallBack .<br>
    Reason:In order to make the module code structure clear, the functions are classified.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

    \li VL_LdCom_0793
      Reason: Dependent on configuration.
      Risk: No risk
      Prevention: Configure shorter names as much as possible
 */

#ifndef LDCOM_H_
#define LDCOM_H_

/* =================================================== inclusions =================================================== */
#include "LdCom_Types.h"
#include "LdCom_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/*Published information elements*/
#define LDCOM_AR_RELEASE_MAJOR_VERSION 4u /**< LdCom autosar major version*/
#define LDCOM_AR_RELEASE_MINOR_VERSION 9u /**< LdCom autosar minor version*/
#define LDCOM_AR_RELEASE_PATCH_VERSION 0u /**< LdCom autosar patch version*/

#define LDCOM_SW_MAJOR_VERSION 2u
#define LDCOM_SW_MINOR_VERSION 2u
#define LDCOM_SW_PATCH_VERSION 0u

/* ===================================================== macros ===================================================== */
#define LDCOM_MODULE_ID   49u
#define LDCOM_VENDOR_ID   62u
#define LDCOM_INSTANCE_ID 0u

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
/** Error classification */
#define LDCOM_E_PARAM              ((uint8)0x01u)
#define LDCOM_E_UNINIT             ((uint8)0x02u)
#define LDCOM_E_PARAM_POINTER      ((uint8)0x03u)
#define LDCOM_E_INVALID_PDU_SDU_ID ((uint8)0x04u)
#define LDCOM_E_INVALID_SIGNAL_ID  ((uint8)0x05u)
#define LDCOM_E_INIT_FAILED        ((uint8)0x06u)
#define LDCOM_E_INVALID_PARTITION  ((uint8)0x80u)

/** Service ID[hex]. */
#define LDCOM_SERVICE_ID_INIT             ((uint8)0x01u)
#define LDCOM_SERVICE_ID_DEINIT           ((uint8)0x02u)
#define LDCOM_SERVICE_ID_GETVERSIONINFO   ((uint8)0x03u)
#define LDCOM_SERVICE_ID_TRANSMIT         ((uint8)0x49u)
#define LDCOM_SERVICE_ID_COPYTXDATA       ((uint8)0x43u)
#define LDCOM_SERVICE_ID_TPTXCONFIRMATION ((uint8)0x48u)
#define LDCOM_SERVICE_ID_STARTOFRECEPTION ((uint8)0x46u)
#define LDCOM_SERVICE_ID_COPYRXDATA       ((uint8)0x44u)
#define LDCOM_SERVICE_ID_TPRXINDICATION   ((uint8)0x45u)
#define LDCOM_SERVICE_ID_RXINDICATION     ((uint8)0x42u)
#define LDCOM_SERVICE_ID_TXCONFIRMATION   ((uint8)0x40u)
#define LDCOM_SERVICE_ID_TRIGGERTRANSMIT  ((uint8)0x41u)
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief         Initializes internal and external interfaces and variables of the AUTOSAR LdCom module
 *                for the further processing.
 * @param[in]     config : Pointer to the AUTOSAR LdCom module's configuration data.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-56832
 */
void LdCom_Init(const LdCom_ConfigType* config);

/**
 * @brief         With a call to LdCom_DeInit the AUTOSAR LdCom module is put into an not initialized state.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-56834
 */
void LdCom_DeInit(void);

#if (STD_ON == LDCOM_VERSION_INFO_API)
/**
 * @brief         Returns the version information of ldCom module.
 * @param[out]    versioninfo : Pointer to where to store the version information of ldCom module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-56835
 */
void LdCom_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief         Requests transmission of a signal.
 * @param[in]     Id          : Identifier of the signal to be transmitted.
 * @param[in]     PduInfoPtr  : Length of and pointer to the signal data and pointer to MetaData.
 * @return        Std_ReturnType
 * @retval        E_OK     : Transmit request has been accepted.
 * @retval        E_NOT_OK : Transmit request has not been accepted.
 * @reentrant     Reentrant for different Ids. Non reentrant for the same Id.
 * @synchronous   TRUE
 * @trace         CPD-56836
 */
Std_ReturnType LdCom_Transmit(PduIdType Id, const PduInfoType* PduInfoPtr);

#ifdef __cplusplus
}
#endif

#endif
