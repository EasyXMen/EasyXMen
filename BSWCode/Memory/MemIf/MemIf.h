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
**  FILENAME    : MemIf.h                                                     **
**                                                                            **
**  Created on  : 2022/09/26                                                  **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : MemIf header description                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/* <VERSION>   <DATE>      <AUTHOR>       <REVISION LOG>
 *  V1.0.0    20200826    Haibin.Shao     Initial version
 *  V1.0.1    20211206    Haibin.Shao     Modify version(update version information)
 *  V2.0.0    20220407    tao.yu          ReWork mode and update to R19-11
 *  V2.0.1    20230607    peng.wu         Fix CPT-555:Modify the comment header info
 *  V2.0.2    20230918    tao.yu          add type description
 */

#ifndef MEMIF_H_
#define MEMIF_H_

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define MEMIF_VENDOR_ID                62U
#define MEMIF_MODULE_ID                22U
#define MEMIF_AR_RELEASE_MAJOR_VERSION (4U)
#define MEMIF_AR_RELEASE_MINOR_VERSION (5U)
#define MEMIF_AR_RELEASE_PATCH_VERSION (0U)
#define MEMIF_SW_MAJOR_VERSION         (2U)
#define MEMIF_SW_MINOR_VERSION         (0U)
#define MEMIF_SW_PATCH_VERSION         (2U)

/*==============================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "MemIf_Cfg.h"

/*******************************************************************************
**               Global Functions Declarations                                **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Invokes the "GetStatus" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_StatusType:
 */
/******************************************************************************/
extern FUNC(MemIf_StatusType, MEMIF_CODE) MemIf_GetStatus(VAR(uint8, AUTOMATIC) DeviceIndex);

/* Following functios will only be provided, if Development mode is activated */
#if (STD_ON == MEMIF_CFG_USE_FCT_API)
/******************************************************************************/
/*
 * Brief               Invokes the "Read" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 *                     BlockOffset:
 *                     Length:
 * Param-Name[out]     DataBufferPtr:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Read(
    VAR(uint8, AUTOMATIC) DeviceIndex,
    VAR(uint16, AUTOMATIC) BlockNumber,
    VAR(uint16, AUTOMATIC) BlockOffset,
    P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr,
    VAR(uint16, AUTOMATIC) Length);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
/******************************************************************************/
/*
 * Brief               Invokes the "Write" function of the underlying memory abstraction module
 *                     selected by the parameter DeviceIndex.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 *                     DataBufferPtr:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Write(
    VAR(uint8, AUTOMATIC) DeviceIndex,
    VAR(uint16, AUTOMATIC) BlockNumber,
    P2CONST(uint8, AUTOMATIC, MEMIF_APPL_CONST) DataBufferPtr);
/******************************************************************************/
/*
 * Brief               Invokes the "EraseImmediateBlock" function of the underlying memory
 *                     abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, MEMIF_CODE)
    MemIf_EraseImmediateBlock(VAR(uint8, AUTOMATIC) DeviceIndex, VAR(uint16, AUTOMATIC) BlockNumber);
/******************************************************************************/
/*
 * Brief               Invokes the "InvalidateBlock" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, MEMIF_CODE)
    MemIf_InvalidateBlock(VAR(uint8, AUTOMATIC) DeviceIndex, VAR(uint16, AUTOMATIC) BlockNumber);
/******************************************************************************/
/*
 * Brief               Invokes the "Cancel" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
extern FUNC(void, MEMIF_CODE) MemIf_Cancel(VAR(uint8, AUTOMATIC) DeviceIndex);

/******************************************************************************/
/*
 * Brief               Invokes the "SetMode" functions of all underlying memory abstraction modules.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Mode: work mode of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
extern FUNC(void, MEMIF_CODE) MemIf_SetMode(VAR(MemIf_ModeType, AUTOMATIC) Mode);

/******************************************************************************/
/*
 * Brief               Invokes the "GetJobResult" function of the underlying memory abstraction
 *                     module selected by the parameter DeviceIndex.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_JobResultType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return MEMIF_JOB_FAILED else
 *                     it shall return the value of the called function of the underlying module.
 */
/******************************************************************************/
extern FUNC(MemIf_JobResultType, MEMIF_CODE) MemIf_GetJobResult(VAR(uint8, AUTOMATIC) DeviceIndex);
#endif

#if (STD_ON == MEMIF_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Service to get the version information of this module.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     VersionInfo: Pointer to standard version information structure
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, MEMIF_CODE)
    MemIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, MEMIF_APPL_DATA) VersionInfoPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#endif
