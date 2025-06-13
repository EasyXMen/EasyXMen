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
**  FILENAME    : Crc.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : ShenXu,Yb                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef CRC_H_
#define CRC_H_
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION> <DATE> <AUTHOR> <REVISION LOG>
 * V2.0.0 [2020/6/15] [YangBo] Add CRC 64 method
 * V2.0.1 [2020/9/29] [YangBo] QAC FIX
 * V2.0.2 [2020/12/23] [YangBo,LXJ] CRC64 FIX
 * V2.0.3 [2020/12/23] [YangBo] Change loop i to suitable size
 * V2.0.4 [2021/05/18] [YangBo] Change loop i to suitable size
 * V2.1.0 2023-07-18   Jian.jiang      CP2.1 Release Version.
 * V2.1.1 2024-04-22   zhiqiang.huang      Add callout function for CRC calculation performed by hardware.
 *        2024-05-22   qinmei.chen     TMS320 support
 * v2.1.2 2024-07-26   qinmei.chen    CPT-8730 fix corresponding macro error between definition and invoke.
 * v2.1.3 2024-08-13   qinmei.chen    change something to adapt to CP-rules
 * v2.1.4 2024-10-29   qinmei.chen    CPT-10780 Use more precise macros in CRC for
 *                                   fixing corresponding macro error between definition and invoke.
 */

/*@ req<SWS_Crc_00048> */
#define CRC_VENDOR_ID                   (62u)
#define CRC_MODULE_ID                   (201u)
#define CRC_AR_RELEASE_MAJOR_VERSION    (4u)
#define CRC_AR_RELEASE_MINOR_VERSION    (5u)
#define CRC_AR_RELEASE_REVISION_VERSION (0u)
#define CRC_SW_MAJOR_VERSION            (2u)
#define CRC_SW_MINOR_VERSION            (1u)
#define CRC_SW_PATCH_VERSION            (3u)

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Crc_Cfg.h"

/*@ req<SWS_Crc_00017>,<SWS_Crc_00021>,<SWS_Crc_00011> */
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*
 * Brief               This service returns the version information of
 *                     this module
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Versioninfo: Pointer to where to store the version
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
#define Crc_GetVersionInfo(VersionInfo)                         \
    do                                                          \
    {                                                           \
        (VersionInfo)->vendorID = CRC_VENDOR_ID;                \
        (VersionInfo)->moduleID = CRC_MODULE_ID;                \
        (VersionInfo)->sw_major_version = CRC_SW_MAJOR_VERSION; \
        (VersionInfo)->sw_minor_version = CRC_SW_MINOR_VERSION; \
        (VersionInfo)->sw_patch_version = CRC_SW_PATCH_VERSION; \
        (VersionInfo)->sw_major_version = CRC_SW_MAJOR_VERSION; \
        (VersionInfo)->sw_minor_version = CRC_SW_MINOR_VERSION; \
        (VersionInfo)->sw_patch_version = CRC_SW_PATCH_VERSION; \
    } while (0)

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/******************************************************************************/
#if (STD_ON == CRC8_ALGORITHM)
/*************************************************************************/
/*
 * Brief               This service makes a CRC8 calculation on Crc_Length
 *                     data bytes, with SAE J1850 parameters
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Crc_DataPtr:Pointer to start address of data block
 *                     to be calculated.
 *                     Crc_Length:Length of data block to be calculated in bytes.
 *                     Crc_StartValue8:Start value when the algorithm starts.
 *                     Crc_IsFirstCall:
 *                     TRUE: First call in a sequence or individual CRC calculation;
 *                     start from initial value, ignore Crc_StartValue8.
 *                     FALSE:Subsequent call in a call sequence; Crc_StartValue8 is
 *                     interpreted to be the return value of the previous function call.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* req<SWS_Crc_00031>*/

extern FUNC(uint8, CRC_CODE) Crc_CalculateCRC8(
    P2CONST(uint8, AUTOMATIC, CRC_APPL_CONST) Crc_DataPtr,
    VAR(uint32, AUTOMATIC) Crc_Length,
    VAR(uint8, AUTOMATIC) Crc_StartValue8,
    VAR(boolean, AUTOMATIC) Crc_IsFirstCall);

#endif /* STD_ON == CRC8_ALGORITHM */

#if (STD_ON == CRC8H2F_ALGORITHM)
/*************************************************************************/
/*
 * Brief               This service makes a CRC8 calculation with the
 *                     Polynomial 0x2F on Crc_Length
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Crc_DataPtr:Pointer to start address of data block
 *                     to be calculated.
 *                     Crc_Length:Length of data block to be calculated in bytes.
 *                     Crc_StartValue8H2F:Start value when the algorithm starts.
 *                     Crc_IsFirstCall:
 *                     TRUE: First call in a sequence or individual CRC calculation;
 *                     start from initial value, ignore Crc_StartValue8.
 *                     FALSE:Subsequent call in a call sequence; Crc_StartValue8 is
 *                     interpreted to be the return value of the previous function call.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* req<SWS_Crc_00043>*/

extern FUNC(uint8, CRC_CODE) Crc_CalculateCRC8H2F(
    P2CONST(uint8, AUTOMATIC, CRC_APPL_CONST) Crc_DataPtr,
    VAR(uint32, AUTOMATIC) Crc_Length,
    VAR(uint8, AUTOMATIC) Crc_StartValue8H2F,
    VAR(boolean, AUTOMATIC) Crc_IsFirstCall);

#endif /* STD_ON == CRC8H2F_ALGORITHM */

#if (STD_ON == CRC16_ALGORITHM)
/*************************************************************************/
/*
 * Brief               This service makes a CRC16 calculation on Crc_Length
 *                     data bytes.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Crc_DataPtr:Pointer to start address of data block
 *                     to be calculated.
 *                     Crc_Length:Length of data block to be calculated in bytes.
 *                     Crc_StartValue16:Start value when the algorithm starts.
 *                     Crc_IsFirstCall:
 *                     TRUE: First call in a sequence or individual CRC calculation;
 *                     start from initial value, ignore Crc_StartValue16.
 *                     FALSE:Subsequent call in a call sequence; Crc_StartValue16 is
 *                     interpreted to be the return value of the previous function call.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint16
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* req<SWS_Crc_00019>*/

extern FUNC(uint16, CRC_CODE) Crc_CalculateCRC16(
    P2CONST(uint8, AUTOMATIC, CRC_APPL_CONST) Crc_DataPtr,
    VAR(uint32, AUTOMATIC) Crc_Length,
    VAR(uint16, AUTOMATIC) Crc_StartValue16,
    VAR(boolean, AUTOMATIC) Crc_IsFirstCall);

#endif /* STD_ON == CRC16_ALGORITHM */

#if (STD_ON == CRC32_ALGORITHM)
/*************************************************************************/
/*
 * Brief               This service makes a CRC32 calculation on Crc_Length
 *                     data bytes.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Crc_DataPtr: Pointer to start address of data block
 *                     to be calculated.
 *                     Crc_Length: Length of data block to be calculated in bytes.
 *                     Crc_StartValue32: Start value when the algorithm starts.
 *                     Crc_IsFirstCall:
 *                     TRUE: First call in a sequence or individual CRC calculation;
 *                     start from initial value, ignore Crc_StartValue32.
 *                     FALSE: Subsequent call in a call sequence; Crc_StartValue32 is
 *                     interpreted to be the return value of the previous function call.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint16
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* req<SWS_Crc_00020>*/

extern FUNC(uint32, CRC_CODE) Crc_CalculateCRC32(
    P2CONST(uint8, AUTOMATIC, CRC_APPL_CONST) Crc_DataPtr,
    VAR(uint32, AUTOMATIC) Crc_Length,
    VAR(uint32, AUTOMATIC) Crc_StartValue32,
    VAR(boolean, AUTOMATIC) Crc_IsFirstCall);

#endif /* STD_ON == CRC32_ALGORITHM */

#if (STD_ON == CRC32P4_ALGORITHM)
/*************************************************************************/
/*
 * Brief               This service makes a CRC32 calculation on Crc_Length
 *                     data bytes.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Crc_DataPtr: Pointer to start address of data block
 *                     to be calculated.
 *                     Crc_Length: Length of data block to be calculated in bytes.
 *                     Crc_StartValue32: Start value when the algorithm starts.
 *                     Crc_IsFirstCall:
 *                     TRUE: First call in a sequence or individual CRC calculation;
 *                     start from initial value, ignore Crc_StartValue32.
 *                     FALSE: Subsequent call in a call sequence; Crc_StartValue32 is
 *                     interpreted to be the return value of the previous function call.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint16
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
/* req<SWS_Crc_00058>*/

extern FUNC(uint32, CRC_CODE) Crc_CalculateCRC32P4(
    P2CONST(uint8, AUTOMATIC, CRC_APPL_CONST) Crc_DataPtr,
    VAR(uint32, AUTOMATIC) Crc_Length,
    VAR(uint32, AUTOMATIC) Crc_StartValue32,
    VAR(boolean, AUTOMATIC) Crc_IsFirstCall);

#endif /* STD_ON == CRC32P4_ALGORITHM */

#if (STD_ON == CRC64_ALGORITHM)

extern FUNC(uint64, CRC_CODE) Crc_CalculateCRC64(
    P2CONST(uint8, AUTOMATIC, CRC_APPL_CONST) Crc_DataPtr,
    VAR(uint64, AUTOMATIC) Crc_Length,
    VAR(uint64, AUTOMATIC) Crc_StartValue64,
    VAR(boolean, AUTOMATIC) Crc_IsFirstCall);

#endif /* STD_ON == CRC64_ALGORITHM */

#endif /* CRC_H_ */
