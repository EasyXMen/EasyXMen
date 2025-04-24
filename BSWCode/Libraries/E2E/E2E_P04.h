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
**  FILENAME    : E2E_P04.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef E2E_P04_H_
#define E2E_P04_H_

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
    typedef struct
    {
        /*A system-unique identifier of the Data.*/
        uint32 DataID;
        /*Bit offset of the first bit of the E2E header from the beginning of the Data Array
        (bit numbering: bit 0 is the least important). The offset shall be a multiple of 8 and
        0 ' Offset ' MaxDataLength-(12*8).*/
        uint16 Offset;
        /*Minimal length of Data array, in bits. E2E checks that Length is ' MinDataLength.
        The value shall be = 4096*8 (4kB) and shall be ' 12*8*/
        uint16 MinDataLength;
        /*Maximal length of Data, in bits. E2E checks that DataLength is ' MinDataLength.
        The value shall be = 4096*8 (4kB) and it shall be ' MinDataLength*/
        uint16 MaxDataLength;
        /*Maximum allowed gap between two counter values of two consecutively received valid Data.*/
        uint16 MaxDeltaCounter;
    } E2E_P04ConfigType;

    typedef struct
    {
        /*Counter to be used for protecting the next Data.*/
        uint16 Counter;
    } E2E_P04ProtectStateType;

    typedef enum
    {
        /*OK: the checks of the Data in this cycle were successful (including counter
        check, which was incremented by 1).*/
        E2E_P04STATUS_OK = 0x00,
        /*Error: the Check function has been invoked but no new Data is not available
        since the last call, according to communication medium (e.g. RTE,COM). As a result,
        no E2E checks of Data have been consequently executed.*/
        E2E_P04STATUS_NONEWDATA = 0x01,
        /*Error: error not related to counters occurred (e.g. wrong crc, wrong length,
        wrong options, wrong Data ID).*/
        E2E_P04STATUS_ERROR = 0x07,
        /*Error: the checks of the Data in this cycle were successful, with the exception of
        the repetition.*/
        E2E_P04STATUS_REPEATED = 0x08,
        /*OK: the checks of the Data in this cycle were successful (including counter check,
        which was incremented within the allowed configured delta).*/
        E2E_P04STATUS_OKSOMELOST = 0x20,
        /*Error: the checks of the Data in this cycle were successful, with the exception of
        counter jump, which changed more than the allowed delta*/
        E2E_P04STATUS_WRONGSEQUENCE = 0x40
    } E2E_P04CheckStatusType;

    typedef struct
    {
        /*Result of the verification of the Data in this cycle, determined by the
        Check function.*/
        E2E_P04CheckStatusType Status;
        /*Counter of the data in previous cycle.It is initialized with 0.*/
        uint16 Counter;
    } E2E_P04CheckStateType;

    /*******************************************************************************
    **                      Global Data                                           **
    *******************************************************************************/
    /*******************************************************************************
    **                      Global Functions                                      **
    *******************************************************************************/
    /**
     * Protects the array/buffer to be transmitted using the E2E profile 4.
     * This includes checksum calculation, handling of counter and Data ID.
     * Service ID: 0x21
     * Sync/Async: synchronous
     * Reentrancy: Reentrant
     * Parameters(IN): ConfigPtr,Pointer to static configuration.
     *                 Length,Length of the data in bytes.
     * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
     *                    DataPtr,Pointer to Data to be transmitted.
     * Parameters(OUT): NA
     * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK.
     *
     */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE) E2E_P04Protect(
        P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
        P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
        P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
        uint16 Length);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * Initializes the protection state.
     * Service ID: 0x22
     * Sync/Async: synchronous
     * Reentrancy: Reentrant
     * Parameters(IN): NA
     * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
     * Parameters(OUT): NA
     * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_OK.
     *
     */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE)
        E2E_P04ProtectInit(P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * Checks the Data received using the E2E profile 4.
     * This includes CRC calculation, handling of Counter and Data ID.
     * Service ID: 0x23
     * Sync/Async: synchronous
     * Reentrancy: Reentrant
     * Parameters(IN): ConfigPtr,Pointer to static configuration.
     *                 DataPtr,Pointer to received data.
     *                 Length,Length of the data in bytes.
     * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
     * Parameters(OUT): NA
     * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK.
     *
     */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE) E2E_P04Check(
        P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
        P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
        P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
        uint16 Length);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * Initializes the check state
     * Service ID: 0x24
     * Sync/Async: synchronous
     * Reentrancy: Reentrant
     * Parameters(IN): NA
     * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
     * Parameters(OUT): NA
     * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_OK.
     *
     */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE)
        E2E_P04CheckInit(P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * The function maps the check status of Profile 4 to a generic check status, which can be used by E2E state machine
     * check function. The E2E Profile 4 delivers a more fine-granular status, but this is not relevant for the E2E
     * state machine. Service ID: 0x25 Sync/Async: synchronous Reentrancy: Reentrant Parameters(IN): CheckReturn,Return
     * value of the E2E_P04Check function Status,Status determined by E2E_P04Check function Parameters(INOUT): NA
     * Parameters(OUT): NA Return value: E2E_PCheckStatusType,Profile-independent status of the reception on one single
     * Data in one cycle.
     *
     */

    extern FUNC(E2E_PCheckStatusType, E2E_CODE)
        E2E_P04MapStatusToSM(Std_ReturnType CheckReturn, E2E_P04CheckStatusType Status);

#ifdef __cplusplus
}

#endif /* end of __cplusplus */
#endif /*E2E_P04_H_ */
