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
********************************************************************************
**                                                                            **
**  FILENAME    : LinTp.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LinIf<br>
  RuleSorce:puhua_rules-2024.12.rcf

  \li  PRQA S 2991,2995 VL_LinTp_Logical .<br>
    Reason: The condition will be modified within the loop.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li  PRQA S 2983 VL_LinTp_PointerCalu .<br>
    Reason:Need to traverse the structure array using pointer increment.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 3408 VL_LinTp_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

  \li PRQA S 3218 VL_LinTp_OnlyAccess .<br>
    Reason: file scoped variables only used in one function are necessary for functional usage
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 4342 VL_LinTp_4342 .<br>
    Reason: enum conversion is necessary for functional usage
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 1502 VL_LinTp_1502
    Reason: The configuration struct is used by other modules.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li PRQA S 6030 VL_MTR_LinTp_STMIF .<br>
    Reason: There is a lot of non-generic code, allowing for higher nesting levels to be accepted.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6050 VL_MTR_LinTp_STST3 .<br>
    Reason: Splitting their implementation will result in a decrease in performance.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6070 VL_MTR_LinTp_STCAL .<br>
    Reason: Reducing the number of function calls will increase the nesting depth and cyclomatic complexity.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6010 VL_MTR_LinTp_STCYC .<br>
    Reason: The function needs to ensure the integrity of its functionality.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 6060 VL_MTR_LinTp_STM19 .<br>
    Reason: Multiple exit points can improve function execution efficiency.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6020 VL_MTR_LinTp_STLIN .<br>
    Reason: Splitting them into smaller parts will lead to performance degradation.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6080 VL_MTR_LinTp_STPTH .<br>
    Reason: Splitting the function into sub-functions will lead to an increase in the stack.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 2982 VL_LinTp_2982 .<br>
    Reason: The assignment is reserved for better readibility.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.
*/

#ifndef LINTP_H
#define LINTP_H

#include "LinIf_Cfg.h"

#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Types.h"
#include "Lin_GeneralTypes.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define LINTP_H_SW_MAJOR_VERSION 2U
#define LINTP_H_SW_MINOR_VERSION 0U
#define LINTP_H_SW_PATCH_VERSION 14U

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
#define LINTP_INIT_ID            (uint8)0x40
#define LINTP_TRANSMIT_ID        (uint8)0x41
#define LINTP_GETVERSIONINFO_ID  (uint8)0x42
#define LINTP_SHUTDOWN_ID        (uint8)0x43
#define LINTP_CHANGEPARAMETER_ID (uint8)0x44
#endif

#define LINTP_NOUSED(paramter) (void)(paramter)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(LinTp_ConfigType, LINIF_CONST) LinTp_PCConfig;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Initializes the LIN Transport Layer.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the LIN Transport Protocol
 *                                configuration
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Init(P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr);

/******************************************************************************/
/*
 * Brief               Requests the transfer of segmented data.
 * ServiceId           0x53
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      LinTpTxSduId: This parameter contains the unique
 *                                   identifier of the N-SDU to be transmitted
 *                     LinTpTxInfoPtr: A pointer to a structure with N-SDU
 *                                     related data
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK: The request can be started successfully
 *                     E_NOT_OK: The request can not be started
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinTp_Transmit(PduIdType LinTpTxSduId, P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr);

/******************************************************************************/
/*
 * Brief               Returns the version information of this module
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     versionInfo: Pointer to where to store the version
 *                                  information of this module
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == LINTP_VERSION_INFO_API)

#if (STD_ON == LINIF_DEV_ERROR_DETECT)
#define LinTp_GetVersionInfo(versionInfo)                                                                        \
    do                                                                                                           \
    {                                                                                                            \
        if (NULL_PTR == (versionInfo))                                                                           \
        {                                                                                                        \
            Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, LINTP_GETVERSIONINFO_ID, LINIF_E_PARAM_POINTER); \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            (versionInfo)->vendorID         = LINIF_VENDOR_ID;                                                   \
            (versionInfo)->moduleID         = LINIF_MODULE_ID;                                                   \
            (versionInfo)->sw_major_version = LINTP_H_SW_MAJOR_VERSION;                                          \
            (versionInfo)->sw_minor_version = LINTP_H_SW_MINOR_VERSION;                                          \
            (versionInfo)->sw_patch_version = LINTP_H_SW_PATCH_VERSION;                                          \
        }                                                                                                        \
    } while (0)
#else
#define LinTp_GetVersionInfo(versionInfo)                           \
    do                                                              \
    {                                                               \
        (versionInfo)->vendorID         = LINIF_VENDOR_ID;          \
        (versionInfo)->moduleID         = LINIF_MODULE_ID;          \
        (versionInfo)->sw_major_version = LINTP_H_SW_MAJOR_VERSION; \
        (versionInfo)->sw_minor_version = LINTP_H_SW_MINOR_VERSION; \
        (versionInfo)->sw_patch_version = LINTP_H_SW_PATCH_VERSION; \
    } while (0)
#endif

#endif

/******************************************************************************/
/*
 * Brief               Shutdowns the LIN TP
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Shutdown(void);

/******************************************************************************/
/*
 * Brief               A dummy method introduced for interface compatibility
 * ServiceId           0x4b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id:        Identifier of the received N-SDU on which the
 *                                reception parameter has to be changed.
 *                     parameter: The selected parameter that the request shall
 *                                change (STmin).
 *                     value:     The new value of the parameter.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_NOT_OK:  request is not accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);

#endif

#endif /* LINTP_H */
