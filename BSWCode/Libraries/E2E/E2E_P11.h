/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : E2E_P11.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef E2E_P11_H_
#define E2E_P11_H_

#ifdef __cplusplus

extern "C"
{
#endif
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "E2E.h"
    /*******************************************************************************
    **                      Global Symbols                                        **
    *******************************************************************************/
    /*******************************************************************************
    **                      Global Data Types                                     **
    *******************************************************************************/
    typedef enum
    {
        /*Two bytes are included in the CRC (double ID configuration) This is used in
    E2E variant 1A.*/
        E2E_P11_DATAID_BOTH = 0,
        /*The low byte is included in the implicit CRC calculation, the low nibble of the
    high byte is transmitted along with the data (i.e. it is explicitly included), the
    high nibble of the high byte is not used. This is applicable for the IDs up to 12 bits.
    This is used in E2E variant 1C.*/
        E2E_P11_DATAID_NIBBLE = 3
    } E2E_P11DataIDMode;

    typedef struct
    {
        /*Length of data, in bits.*/
        uint16 DataLength;
        /*A unique identifier, for protection against masquerading*/
        uint16 DataID;
        /*Maximum allowed gap between two counter values of two consecutively
    received valid Data*/
        uint8 MaxDeltaCounter;
        E2E_P11DataIDMode DataIDMode;
        /*Bit offset of CRC (i.e. since *Data) in MSB first order*/
        uint16 CRCOffset;
        /*Bit offset of Counter in MSB first order. In variants 1A and 1B, Counter
    Offset is 8. The offset shall be a multiple of 4.*/
        uint16 CounterOffset;
        /*Bit offset of the low nibble of the high byte of Data ID.*/
        uint16 DataIDNibbleOffset;
    } E2E_P11ConfigType;

    typedef struct
    {
        /*Counter to be used for protecting the next Data.*/
        uint8 Counter;
    } E2E_P11ProtectStateType;

    typedef enum
    {
        /*OK: the checks of the Data in this cycle were successful (including counter check,
    which was incremented by 1).*/
        E2E_P11STATUS_OK = 0x00,
        /*Error: the Check function has been invoked but no new Data is not available since
    the last call, according to communication medium (e.g. RTE, COM). As a result, no E2E
    checks of Data have been consequently executed.*/
        E2E_P11STATUS_NONEWDATA = 0x01,
        /*Error: error not related to counters occurred (e.g. wrong crc, wrong length).*/
        E2E_P11STATUS_ERROR = 0x07,
        /*Error: the checks of the Data in this cycle were successful, with the exception of the
    repetition.*/
        E2E_P11STATUS_REPEATED = 0x08,
        /*OK: the checks of the Data in this cycle were successful (including counter check,
    which was incremented within the allowed configured delta).*/
        E2E_P11STATUS_OKSOMELOST = 0x20,
        /*Error: the checks of the Data in this cycle were successful, with the exception of
    counter jump, which changed more than the allowed delta*/
        E2E_P11STATUS_WRONGSEQUENCE = 0x40
    } E2E_P11CheckStatusType;

    typedef struct
    {
        /*Result of the verification of the Data in this cycle, determined by the
    Check function.*/
        E2E_P11CheckStatusType Status;
        /*Counter of the data in previous cycle.*/
        uint8 Counter;
    } E2E_P11CheckStateType;

    /*******************************************************************************
    **                      Global Data                                           **
    *******************************************************************************/
    /*******************************************************************************
    **                      Global Functions                                      **
    *******************************************************************************/

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE) E2E_P11Protect(
        P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
        P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
        P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
        uint16 Length);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE)
        E2E_P11ProtectInit(P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE) E2E_P11Check(
        P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
        P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
        P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
        uint16 Length);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE)
        E2E_P11CheckInit(P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    extern FUNC(E2E_PCheckStatusType, E2E_CODE)
        E2E_P11MapStatusToSM(Std_ReturnType CheckReturn, E2E_P11CheckStatusType Status);

#ifdef __cplusplus
}

#endif /* end of __cplusplus */
#endif /*E2E_P11_H_ */
