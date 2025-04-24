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
**  FILENAME    : E2E_P02.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef E2E_P02_H_
#define E2E_P02_H_

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
        /*Length of Data, in bits. The value shall be a multiple of 8.*/
        uint16 DataLength;
        /*An array of appropriately chosen Data IDs for protection against masquerading.*/
        uint8 DataIDList[16];
        /*Initial maximum allowed gap between two counter values of two consecutively received valid Data.*/
        uint8 MaxDeltaCounterInit;
        /*The maximum amount of missing or repeated Data which the receiver does not expect to exceed under
         *  normal communication conditions.*/
        uint8 MaxNoNewOrRepeatedData;
        /*Number of Data required for validating the consistency of the counter that must be received with a
         *  valid counter*/
        uint8 SyncCounterInit;
        /*Offset of the E2E header in the Data[] array in bits.*/
        uint16 Offset;
    } E2E_P02ConfigType;

    typedef struct
    {
        /*Counter to be used for protecting the Data.*/
        uint8 Counter;
    } E2E_P02ProtectStateType;

    typedef enum
    {
        /*OK: The new data has been received according to communication medium, the CRC is
        correct, the Counter is incremented by 1 with respect to the most recent Data received
        with Status _INITIAL, _OK, or _OKSOMELOST. This means that no Data has been lost since
        the last correct data reception.*/
        E2E_P02STATUS_OK = 0x00,
        /*Error: the Check function has been invoked but no new Data is not available since the
        last call, according to communication medium (e.g. RTE, COM). As a result, no E2E checks
        of Data have been consequently executed.*/
        E2E_P02STATUS_NONEWDATA = 0x01,
        /*Error: The data has been received according to communication medium, but the CRC is incorrect.*/
        E2E_P02STATUS_WRONGCRC = 0x02,
        /*NOT VALID: The new data has been received after detection of an unexpected behavior of counter.*/
        E2E_P02STATUS_SYNC = 0x03,
        /*Initial: The new data has been received according to communication medium, the CRC is correct,
        but this is the first Data since the receiver's initialization or reinitialization, so the
        Counter cannot be verified yet.*/
        E2E_P02STATUS_INITIAL = 0x04,
        /*Error: The new data has been received according to communication medium, the CRC is correct,
        but the Counter is identical to the most recent Data received with Status _INITIAL, _OK, or
        _OKSOMELOST.*/
        E2E_P02STATUS_REPEATED = 0x08,
        /*OK: The new data has been received according to communication medium, the CRC is correct, the
        Counter is incremented by DeltaCounter (1 < DeltaCounter=MaxDeltaCounter) with respect to the most
        recent Data received with Status _INITIAL,_OK, or _OKSOMELOST.*/
        E2E_P02STATUS_OKSOMELOST = 0x20,
        /*Error: The new data has been received according to communication medium, the CRC is correct, but
        the Counter Delta is too big (DeltaCounter > MaxDeltaCounter) with respect to the most recent Data
        received with Status _INITIAL, _OK, or _OKSOMELOST.*/
        E2E_P02STATUS_WRONGSEQUENCE = 0x40
    } E2E_P02CheckStatusType;

    typedef struct
    {
        /*Counter of last valid received message.*/
        uint8 LastValidCounter;
        uint8 MaxDeltaCounter;
        /*If true means that no correct data has been yet received after the receiver
         *  initialization or reinitialization.*/
        boolean WaitForFirstData;
        boolean NewDataAvailable;
        /*Number of data (messages) lost since reception of last valid one.*/
        uint8 LostData;
        /*Result of the verification of the Data, determined by the Check function.*/
        E2E_P02CheckStatusType Status;
        /*Number of Data required for validating the consistency of the counter that must
        be received with a valid counter (i.e. counter within the allowed lock-in range)
        after the detection of an unexpected behavior of a received counter.*/
        uint8 SyncCounter;
        /*Amount of consecutive reception cycles in which either (1) there was no new data,
        or (2) when the data was repeated.*/
        uint8 NoNewOrRepeatedDataCounter;
    } E2E_P02CheckStateType;
    /*******************************************************************************
    **                      Global Data                                           **
    *******************************************************************************/
    /*******************************************************************************
    **                      Global Functions                                      **
    *******************************************************************************/
    /**
     * Protects the array/buffer to be transmitted using the E2E profile 2.
     * This includes checksum calculation, handling of sequence counter and Data ID.
     * Service ID: 0x03
     * Sync/Async: synchronous
     * Reentrancy: Reentrant
     * Parameters(IN): ConfigPtr,Pointer to static configuration.
     * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
     *                    DataPtr,Pointer to Data to be transmitted.
     * Parameters(OUT): NA
     * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK.
     *
     */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE) E2E_P02Protect(
        P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
        P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
        P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * Initializes the protection state.
     * Service ID: 0x1e
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
        E2E_P02ProtectInit(P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * Checks the Data received using the E2E profile 2.
     * This includes CRC calculation, handling of Counter and Data ID.
     * Service ID: 0x04
     * Sync/Async: synchronous
     * Reentrancy: Reentrant
     * Parameters(IN): ConfigPtr,Pointer to static configuration.
     *                 DataPtr,Pointer to received data.
     * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
     * Parameters(OUT): NA
     * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK.
     *
     */

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    extern FUNC(Std_ReturnType, E2E_CODE) E2E_P02Check(
        P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
        P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
        P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * Initializes the check state
     * Service ID: 0x1f
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
        E2E_P02CheckInit(P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /**
     * The function maps the check status of Profile 2 to a generic check status, which can be used by E2E state machine
     * check function. The E2E Profile 2 delivers a more fine-granular status, but this is not relevant for the E2E
     * state machine. Service ID: 0x20 Sync/Async: synchronous Reentrancy: Reentrant Parameters(IN): CheckReturn,Return
     * value of the E2E_P01Check function Status,Status determined by E2E_P01Check function profileBehavior,FALSE: check
     * has the legacy behavior, before R4.2 TRUE: check behaves like new P4/P5/P6 profiles introduced in R4.2
     * Parameters(INOUT): NA Parameters(OUT): NA Return value: E2E_PCheckStatusType,Profile-independent status of the
     * reception on one single Data in one cycle.
     *
     */

    extern FUNC(E2E_PCheckStatusType, E2E_CODE)
        E2E_P02MapStatusToSM(Std_ReturnType CheckReturn, E2E_P02CheckStatusType Status, boolean ProfileBehavior);

#ifdef __cplusplus
}

#endif /* end of __cplusplus */

#endif /* E2E_P02_H_ */
