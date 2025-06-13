/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
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
            (versionInfo)->vendorID = LINIF_VENDOR_ID;                                                           \
            (versionInfo)->moduleID = LINIF_MODULE_ID;                                                           \
            (versionInfo)->sw_major_version = LINTP_H_SW_MAJOR_VERSION;                                          \
            (versionInfo)->sw_minor_version = LINTP_H_SW_MINOR_VERSION;                                          \
            (versionInfo)->sw_patch_version = LINTP_H_SW_PATCH_VERSION;                                          \
        }                                                                                                        \
    } while (0)
#else
#define LinTp_GetVersionInfo(versionInfo)                           \
    do                                                              \
    {                                                               \
        (versionInfo)->vendorID = LINIF_VENDOR_ID;                  \
        (versionInfo)->moduleID = LINIF_MODULE_ID;                  \
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
