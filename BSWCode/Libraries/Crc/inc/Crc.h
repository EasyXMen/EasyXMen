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
 **  @file               : Crc.h
 **  @author             : ShenXu,Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : Crc Routlines
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 * V02.00.00 2020-06-15  YangBo          Add CRC 64 method
 * V02.00.01 2020-09-29  YangBo          QAC FIX
 * V02.00.02 2020-12-23  YangBo,LXJ      CRC64 FIX
 * V02.00.03 2020-12-23  YangBo Change   loop i to suitable size
 * V02.00.04 2021-05-18  YangBo Change   loop i to suitable size
 * V02.01.00 2023-07-18  Jian.jiang      CP2.1 Release Version.
 * V02.01.01 2024-04-22  zhiqiang.huang  Add callout function for CRC calculation performed by hardware.
 *           2024-05-22  qinmei.chen     TMS320 support
 * V02.01.02 2024-07-26  qinmei.chen     CPT-8730 fix corresponding macro error between definition and invoke.
 * V02.01.03 2024-08-13  qinmei.chen     change something to adapt to CP-rules
 * V02.02.01 2024-09-09  qinmei.chen     Add CRC16/ARC method,Refactor and update to R23-11
 *
===================================================================================================================== */
/* ================================================ misar justifications ============================================ */
/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:Crc<br>
 RuleSorce:puhua_rules-2024.12.rcf

  \li VL_Crc_Redudant
   Reason: (crc ^= CRC_XORVALUE16) In this equation, CRC_XORVALUE16 is 0,
   and this equation is always equal to the left value. Since there is
   this action in the flow chart, add this line of code.and uint8 & 0xff
   is valid when TMS320
   Risk: Code redundancy
   Prevention: None.

  \li VL_Crc_NullPointer
   Reason: The judgment of null pointers has been determined by the upper
   level module, and excessive redundant judgments will result in lower efficiency
   Risk: The upper module determines that there is a misuse situation
   Prevention: None.

  \li VL_Crc_LeftShift
   Reason: The design of CRC algorithm requires removing the highest bit.
   Risk: No risk.
   Prevention: None.

  \li VL_Crc_Scope
   Reason: The static var should be stored in a fixed area.The current
    variable[Cal_Crc32Tab,Cal_Crc16Tab,Cal_Crc8Tab,Cal_Crc8H2FTab] is only accessed by one function, but the design
    reserves the possibility of being accessed by other functions in the future. Use file-scoped static variables to
    avoid refactoring costs when making subsequent modifications.
    Risk: No risk.
    Prevention: None.

   \li VL_Crc_Name
   Reason: The static analysis tool has identified multiple identifiers with the same
    base pattern 'crc' across the project, specifically noting both 'crc' (lowercase)
    in module crc and and 'CRC' (uppercase) in module E2E variants.
    But local variables within functions in separate
    compilation units, which also have no visibility to each other.
   Risk: No risk.
   Prevention: None.

*/

#ifndef CRC_H_
#define CRC_H_

/* =================================================== inclusions =================================================== */
#include "Crc_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/*@ req<SWS_Crc_00048> */
#define CRC_MODULE_ID                   (201u)
#define CRC_VENDOR_ID                   (62u)
#define CRC_AR_RELEASE_MAJOR_VERSION    (4u)
#define CRC_AR_RELEASE_MINOR_VERSION    (9u)
#define CRC_AR_RELEASE_REVISION_VERSION (0u)
#define CRC_SW_MAJOR_VERSION            (2u)
#define CRC_SW_MINOR_VERSION            (2u)
#define CRC_SW_PATCH_VERSION            (1u)
/* ===================================================== macros ===================================================== */
#ifndef CRC_LOCAL
#define CRC_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#ifndef CRC_LOCAL_INLINE
#define CRC_LOCAL_INLINE static inline
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief         This service returns the version information of this module
 * @param[out]    Versioninfo : Pointer to where to store the version
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56165
 */
void Crc_GetVersionInfo(Std_VersionInfoType* versionInfo);

#if (STD_ON == CRC8_ALGORITHM)
/**
 * @brief         This service makes a CRC8 calculation on Crc_Length data bytes, with SAE J1850 parameters
 * @param[in]     Crc_DataPtr     : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length      : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue8 : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall : TRUE: First call in a sequence or individual CRC calculation;
 *                                  start from initial value, ignore Crc_StartValue8.
 *                                  FALSE:Subsequent call in a call sequence; Crc_StartValue8 is
 *                                  interpreted to be the return value of the previous function call.
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56362
 */
uint8 Crc_CalculateCRC8(const uint8* Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, boolean Crc_IsFirstCall);

#endif

#if (STD_ON == CRC8H2F_ALGORITHM)
/**
 * @brief         This service makes a CRC8 calculation with the Polynomial 0x2F on Crc_Length
 * @param[in]     Crc_DataPtr        : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length         : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue8H2F : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall    : TRUE: First call in a sequence or individual CRC calculation;
 *                                     start from initial value, ignore Crc_StartValue8H2F.
 *                                     FALSE:Subsequent call in a call sequence; Crc_StartValue8H2F is
 *                                     interpreted to be the return value of the previous function call.
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56363
 */
uint8 Crc_CalculateCRC8H2F(
    const uint8* Crc_DataPtr,
    uint32       Crc_Length,
    uint8        Crc_StartValue8H2F,
    boolean      Crc_IsFirstCall);

#endif

#if (STD_ON == CRC16_ALGORITHM)
/**
 * @brief         This service makes a CRC16 calculation on Crc_Length data bytes.
 * @param[in]     Crc_DataPtr      : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length       : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue16 : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall  : TRUE: First call in a sequence or individual CRC calculation;
 *                                   start from initial value, ignore Crc_StartValue16.
 *                                   FALSE:Subsequent call in a call sequence; Crc_StartValue16 is
 *                                   interpreted to be the return value of the previous function call.
 * @return        uint16
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56364
 */
uint16
    Crc_CalculateCRC16(const uint8* Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16, boolean Crc_IsFirstCall);

#endif

#if (STD_ON == CRC16ARC_ALGORITHM)
/**
 * @brief         This service makes a CRC16 calculation on Crc_Length data bytes, using the polynomial 0x8005.
 * @param[in]     Crc_DataPtr      : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length       : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue16 : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall  : TRUE: First call in a sequence or individual CRC calculation;
 *                                   start from initial value, ignore Crc_StartValue16.
 *                                   FALSE:Subsequent call in a call sequence; Crc_StartValue16 is
 *                                   interpreted to be the return value of the previous function call.
 * @return        uint16
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56365
 */
uint16 Crc_CalculateCRC16ARC(
    const uint8* Crc_DataPtr,
    uint32       Crc_Length,
    uint16       Crc_StartValue16,
    boolean      Crc_IsFirstCall);

#endif

#if (STD_ON == CRC32_ALGORITHM)
/**
 * @brief         This service makes a CRC32 calculation on Crc_Length data bytes.
 * @param[in]     Crc_DataPtr      : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length       : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue32 : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall  : TRUE: First call in a sequence or individual CRC calculation;
 *                                   start from initial value, ignore Crc_StartValue32.
 *                                   FALSE:Subsequent call in a call sequence; Crc_StartValue32 is
 *                                   interpreted to be the return value of the previous function call.
 * @return        uint32
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56366
 */
uint32
    Crc_CalculateCRC32(const uint8* Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall);

#endif

#if (STD_ON == CRC32P4_ALGORITHM)
/**
 * @brief         This service makes a CRC32 calculation on Crc_Length data bytes.
 * @param[in]     Crc_DataPtr      : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length       : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue32 : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall  : TRUE: First call in a sequence or individual CRC calculation;
 *                                   start from initial value, ignore Crc_StartValue32.
 *                                   FALSE:Subsequent call in a call sequence; Crc_StartValue32 is
 *                                   interpreted to be the return value of the previous function call.
 * @return        uint32
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56367
 */
uint32
    Crc_CalculateCRC32P4(const uint8* Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall);

#endif

#if (STD_ON == CRC64_ALGORITHM)
/**
 * @brief         This service makes a CRC64 calculation on Crc_Length data bytes.
 * @param[in]     Crc_DataPtr      : Pointer to start address of data block to be calculated.
 * @param[in]     Crc_Length       : Length of data block to be calculated in bytes.
 * @param[in]     Crc_StartValue64 : Start value when the algorithm starts.
 * @param[in]     Crc_IsFirstCall  : TRUE: First call in a sequence or individual CRC calculation;
 *                                   start from initial value, ignore Crc_StartValue64.
 *                                   FALSE:Subsequent call in a call sequence; Crc_StartValue64 is
 *                                   interpreted to be the return value of the previous function call.
 * @return        uint64
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-56368
 */
uint64
    Crc_CalculateCRC64(const uint8* Crc_DataPtr, uint64 Crc_Length, uint64 Crc_StartValue64, boolean Crc_IsFirstCall);

#endif
#ifdef __cplusplus
}
#endif
#endif
