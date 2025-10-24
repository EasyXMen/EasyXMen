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
 **  @file               : LinTp.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : Implementation for LinTp
 **
 ***********************************************************************************************************************/

/** ====================================================================================================================
 * REVISION HISTORY
 * -------------------------------------------------------------------------------------------------------------------
 * Version      Date        Author       Description
 * -------------------------------------------------------------------------------------------------------------------
 * V02.00.00    2020-08-04  HuRongbo     R19_11 LinTp initial version.
 * V02.00.01    2021-11-24  HuRongbo     Modify LinTp_SlaveAbortTxRxAndNotifyFailToUpper Change Dcm API to PduR
 *              2022-07-11  HuRongbo     QAC check issue fix
 * V02.00.02    2022-11-14  HuRongbo     1.Resolve UNINIT state config data access in
                                           LinTp_GetTxNSdu()(JIRA:PRD0032020-233)
 * V02.00.03    2022-11-14  HuRongbo     1.Change LinTp_MasterRTData[] from static to macro LINTP_LOCAL 2.Change private
                                           variable keyword from static to macro LINTP_LOCAL
 * V02.00.04    2022-12-15   ZhaoTong    fix PH-PRD-QC-004-2022PRD0042022-357
 * V02.00.05    2023-03-22   ZhaoTong    1.Change the define of type LinTp_ConfigType and LinTp_ChannelConfigType
 * V02.00.06    2023-09-15   ZhaoTong    1.Bug fix CPT-6932.It's necessary to add a macro switch When referencing the
                                           Master/Slave header file
 * V02.00.07    2023-09-20   ZhaoTong    Bug fix CPT-6946 1.Add two macro for Master/Slave of TP separately.
 * V02.00.08    2024-01-05   ZhaoTong    Muliticore adapt
 * V02.00.09    2024-07-05   ZhaoTong    Add macro condition for the call to API BswM_LinTp_RequestMode.
 * V02.01.00    2024-12-05   ZouZhiJia   R23-11 development first release.
 * V02.02.00    2025-4-29    ZouZhiJia   Optimize Code.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LinIf<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li  PRQA S 2991,2995 VL_LinTp_Logical .<br>
    Reason: The condition will be modified within the loop.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

    \li PRQA S 6030 VL_MTR_LinTp_STMIF .<br>
    Reason: There is a lot of non-generic code, allowing for higher nesting levels to be accepted.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

    \li PRQA S 6070 VL_MTR_LinTp_STCAL .<br>
    Reason: Reducing the number of function calls will increase the nesting depth and cyclomatic complexity.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.
*/

#ifndef LINTP_H_
#define LINTP_H_

#include "LinIf_Cfg.h"
#include "LinIf.h"

#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Types.h"
#include "Lin_GeneralTypes.h"
#include "LinTp_Cfg.h"
#include "LinTp_PBcfg.h"
#include "LinTp_Internal.h"

/* ===================================================== macros ===================================================== */
#define LINTP_SW_MAJOR_VERSION 2U
#define LINTP_SW_MINOR_VERSION 2U
#define LINTP_SW_PATCH_VERSION 0U

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
#define LINTP_INIT_ID            (uint8)0x40u
#define LINTP_TRANSMIT_ID        (uint8)0x41u
#define LINTP_GETVERSIONINFO_ID  (uint8)0x42u
#define LINTP_SHUTDOWN_ID        (uint8)0x43u
#define LINTP_CHANGEPARAMETER_ID (uint8)0x44u
#endif

#define LINTP_NOUSED(paramter) (void)(paramter)

/* ================================================ type definitions ================================================ */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief               Initializes the LIN Transport Layer.
 * @param[in]           ConfigPtr: Pointer to the LIN Transport Protocol
 *                                 configuration
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56893
 */
void LinTp_Init(const LinTp_ConfigType* ConfigPtr);

/**
 * @brief               Requests the transfer of segmented data.
 * @param[in]           LinTpTxSduId: This parameter contains the unique
 *                                    identifier of the N-SDU to be transmitted
 * @param[in]           LinTpTxInfoPtr: A pointer to a structure with N-SDU
 *                                      related data
 * @return              E_OK: The request can be started successfully
 * @return              E_NOT_OK: The request can not be started
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56894
 */
Std_ReturnType LinTp_Transmit(PduIdType LinTpTxSduId, const PduInfoType* LinTpTxInfoPtr);

/**
 * @brief              Returns the version information of this module.
 * @param[out]         versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-56895
 */
#if (STD_ON == LINTP_VERSION_INFO_API)
LINTP_LOCAL_INLINE void LinTp_GetVersionInfo(Std_VersionInfoType* versionInfo)
{
#if (STD_ON == LINIF_DEV_ERROR_DETECT)
    if (NULL_PTR == (versionInfo))
    {
        Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, LINTP_GETVERSIONINFO_ID, LINIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        versionInfo->vendorID         = LINIF_VENDOR_ID;
        versionInfo->moduleID         = LINIF_MODULE_ID;
        versionInfo->sw_major_version = LINTP_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = LINTP_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = LINTP_SW_PATCH_VERSION;
    }
}
#endif

/**
 * @brief               Shutdowns the LIN TP
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56897
 */
void LinTp_Shutdown(void);

/**
 * @brief               A dummy method introduced for interface compatibility
 * @param[in]           id:        Identifier of the received N-SDU on which the
 *                                 reception parameter has to be changed.
 * @param[in]           parameter: The selected parameter that the request shall
 *                                 change (STmin).
 * @param[in]           value:     The new value of the parameter.
 * @return              E_NOT_OK:  request is not accepted.
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-56898
 */
Std_ReturnType LinTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);

#endif

#endif
