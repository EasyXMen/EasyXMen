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
 **  @file               : E2E_P02.h
 **  @author             : Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : End-to-End Communication Protection Library
 **
 ***********************************************************************************************************************/
#ifndef E2E_P02_H_
#define E2E_P02_H_

#ifdef __cplusplus

#endif
/* =================================================== inclusions =================================================== */
#include "E2E.h"
#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */
/* ================================================ type definitions ================================================ */
typedef struct
{
    /*Length of Data, in bits. The value shall be a multiple of 8.*/
    uint16 DataLength;
    /*An array of appropriately chosen Data IDs for protection against masquerading.*/
    uint8 DataIDList[16]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
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
    E2E_P02STATUS_OK = 0,
    /*Error: the Check function has been invoked but no new Data is not available since the
    last call, according to communication medium (e.g. RTE, COM). As a result, no E2E checks
    of Data have been consequently executed.*/
    E2E_P02STATUS_NONEWDATA = 1,
    /*Error: The data has been received according to communication medium, but the CRC is incorrect.*/
    E2E_P02STATUS_WRONGCRC = 2,
    /*NOT VALID: The new data has been received after detection of an unexpected behavior of counter.*/
    E2E_P02STATUS_SYNC = 3,
    /*Initial: The new data has been received according to communication medium, the CRC is correct,
    but this is the first Data since the receiver's initialization or reinitialization, so the
    Counter cannot be verified yet.*/
    E2E_P02STATUS_INITIAL = 4,
    /*Error: The new data has been received according to communication medium, the CRC is correct,
    but the Counter is identical to the most recent Data received with Status _INITIAL, _OK, or
    _OKSOMELOST.*/
    E2E_P02STATUS_REPEATED = 8,
    /*OK: The new data has been received according to communication medium, the CRC is correct, the
    Counter is incremented by DeltaCounter (1 < DeltaCounter=MaxDeltaCounter) with respect to the most
    recent Data received with Status _INITIAL,_OK, or _OKSOMELOST.*/
    E2E_P02STATUS_OKSOMELOST = 32,
    /*Error: The new data has been received according to communication medium, the CRC is correct, but
    the Counter Delta is too big (DeltaCounter > MaxDeltaCounter) with respect to the most recent Data
    received with Status _INITIAL, _OK, or _OKSOMELOST.*/
    E2E_P02STATUS_WRONGSEQUENCE = 64
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
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief        Protects the array/buffer to be transmitted using the E2E profile 2.
 *               This includes checksum calculation, handling of counter and Data ID.
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[inout] StatePtr  : Pointer to port/data communication state.
 * @param[inout] DataPtr   : Pointer to Data to be transmitted.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46898
 */

Std_ReturnType E2E_P02Protect(const E2E_P02ConfigType* ConfigPtr, E2E_P02ProtectStateType* StatePtr, uint8* DataPtr);

/**
 * @brief        Initializes the protection state.
 * @param[out]   StatePtr : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46899
 */

Std_ReturnType E2E_P02ProtectInit(E2E_P02ProtectStateType* StatePtr);

/**
 * @brief        Checks the Data received using the E2E profile 2.
 *               This includes CRC calculation, handling of Counter and Data ID.
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    DataPtr   : Pointer to received data.
 * @param[inout] StatePtr  : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46902
 */

Std_ReturnType E2E_P02Check(const E2E_P02ConfigType* ConfigPtr, E2E_P02CheckStateType* StatePtr, const uint8* DataPtr);

/**
 * @brief        Initializes the check state.
 * @param[out]   StatePtr : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46903
 */

Std_ReturnType E2E_P02CheckInit(E2E_P02CheckStateType* StatePtr);

/**
 * @brief       The function maps the check status of Profile 2 to a generic check status, which can be
 *              used by E2E state machine check function. The E2E Profile 2 delivers a more fine-granular
 *              status, but this is not relevant for the E2E state machine.
 * @param[in]   CheckReturn     : Return value of the E2E_P02Check function.
 * @param[in]   Status          : Status determined by E2E_P02Check function.
 * @param[in]   profileBehavior : FALSE : check has the legacy behavior, before R4.2
 *                                TRUE  : check behaves like new P4/P5/P6 profiles introduced in R4.2
 * @return      E2E_PCheckStatusType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-46905
 */
E2E_PCheckStatusType
    E2E_P02MapStatusToSM(Std_ReturnType CheckReturn, E2E_P02CheckStatusType Status, boolean ProfileBehavior);

#ifdef __cplusplus
#endif
#ifdef __cplusplus
}
#endif
#endif
