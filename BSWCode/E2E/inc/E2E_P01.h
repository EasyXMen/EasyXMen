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
 **  @file               : E2E_P01.h
 **  @author             : Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : End-to-End Communication Protection Library
 **
 ***********************************************************************************************************************/
#ifndef E2E_P01_H_
#define E2E_P01_H_

#ifdef __cplusplus

#endif

/* =================================================== inclusions =================================================== */
#include "E2E.h"
#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */
E2E_LOCAL_INLINE uint8 E2E_P01_Min(uint8 a, uint8 b)
{
    return (((a) <= (b)) ? (a) : (b));
}
/* ================================================ type definitions ================================================ */
/*E2E P01 DataID mode*/
typedef enum
{
    /*Two bytes are included in the CRC (double ID configuration) This is used in
    E2E variant 1A.*/
    E2E_P01_DATAID_BOTH,
    /*One of the two bytes byte is included, alternating high and low byte,
    depending on parity of the counter (alternating ID configuration).*/
    E2E_P01_DATAID_ALT,
    /*Only the low byte is included, the high byte is never used.*/
    E2E_P01_DATAID_LOW,
    /*The low byte is included in the implicit CRC calculation, the low nibble of the
    high byte is transmitted along with the data (i.e. it is explicitly included), the
    high nibble of the high byte is not used. This is applicable for the IDs up to 12 bits.
    This is used in E2E variant 1C.*/
    E2E_P01_DATAID_NIBBLE,
    /*used for functional safety without DataId*/
    E2E_P01_DATAID_NOUSED
} E2E_P01DataIDMode;

/*Configuration of transmitted Data (Data Element or I-PDU),for E2E Profile 1.
For each transmitted Data, there is an instance of this typedef.*/
typedef struct
{
    /*Bit offset of Counter in MSB first order. In variants 1A and 1B,
    CounterOffset is 8. The offset shall be a multiple of 4.*/
    uint16 CounterOffset;
    /*Bit offset of CRC (i.e. since *Data) in MSB first order. In variants 1A and
    1B, CRCOffset is 0. The offset shall be a multiple of 8.*/
    uint16 CRCOffset;
    /*A unique identifier, for protection against masquerading.*/
    uint16 DataID;
    /*Bit offset of the low nibble of the high byte of Data ID. This parameter is
    used by E2E Library only if DataIDMode = E2E_P01_DATAID_NIBBLE (otherwise it is
    ignored by E2ELibrary).For DataIDMode different than E2E_P01_DATAID_NIBBLE,
    DataIDNibbleOffset shall be initialized to 0 (even if it is ignored by E2E Library).*/
    uint16 DataIDNibbleOffset;
    /*Inclusion mode of ID in CRC computation (both bytes,alternating, or low byte only of
    ID included).*/
    E2E_P01DataIDMode DataIDMode;
    /*Length of data, in bits.The value shall be a multiple of 8 and shall be ' 240.*/
    uint16 DataLength;
    /*Initial maximum allowed gap between two counter values of two consecutively received
    valid Data.*/
    uint8 MaxDeltaCounterInit;
    /*The maximum amount of missing or repeated Data which the receiver does not expect to
    exceed under normal communication conditions.*/
    uint8 MaxNoNewOrRepeatedData;
    /*Number of Data required for validating the consistency of the counter that must be received
    with a valid counter (i.e. counter within the allowed lock-in range) after the detection of an
    unexpected behavior of a received counter.*/
    uint8 SyncCounterInit;
} E2E_P01ConfigType;

typedef struct
{
    /*Counter to be used for protecting the next Data.*/
    uint8 Counter;
} E2E_P01ProtectStateType;

typedef enum
{
    /*OK: The new data has been received according to communication medium,the CRC is
    correct, the Counter is incremented by 1 with respect to the most recent Data
    received with Status _INITIAL, _OK, or _OKSOMELOST. This means that no Data has been
    lost since the last correct data reception.*/
    E2E_P01STATUS_OK = 0,
    /*Error: the Check function has been invoked but no new Data is not available since
    the last call, according to communication medium (e.g. RTE, COM). As a result, no E2E
    checks of Data have been consequently executed.*/
    E2E_P01STATUS_NONEWDATA = 1,
    /*Error: The data has been received according to communication medium, but 1. the CRC
    is incorrect (applicable for all E2E Profile 1 configurations) or 2. the low nibble of
    the high byte of Data ID is incorrect (applicable only for E2E Profile 1 with
    E2E_P01DataIDMode = E2E_P01_DATAID_NIBBLE).*/
    E2E_P01STATUS_WRONGCRC = 2,
    /*NOT VALID: The new data has been received after detection of an unexpected behavior of
    counter. The data has a correct CRC and a counter within the expected range with respect
    to the most recent Data received, but the determined continuity check for the counter is
    not finalized yet.*/
    E2E_P01STATUS_SYNC = 3,
    /*Initial: The new data has been received according to communication medium, the CRC is
    correct, but this is the first Data since the receiver's initialization or reinitialization,
    so the Counter cannot be verified yet.*/
    E2E_P01STATUS_INITIAL = 4,
    /*Error: The new data has been received according to communication medium, the CRC is correct,
    but the Counter is identical to the most recent Data received with Status _INITIAL, _OK, or
    _OKSOMELOST.*/
    E2E_P01STATUS_REPEATED = 8,
    /*OK: The new data has been received according to communication medium, the CRC is correct,
    the Counter is incremented by DeltaCounter (1 < DeltaCounter = MaxDeltaCounter) with respect
    to the most recent Data received with Status _INITIAL, _OK, or _OKSOMELOST.*/
    E2E_P01STATUS_OKSOMELOST = 32,
    /*Error: The new data has been received according to communication medium, the CRC is correct
    , but the Counter Delta is too big (DeltaCounter > MaxDeltaCounter) with respect to the most
    recent Data received with Status _INITIAL, _OK, or _OKSOMELOST.*/
    E2E_P01STATUS_WRONGSEQUENCE = 64
} E2E_P01CheckStatusType;

typedef struct
{
    /*Counter value most recently received.*/
    uint8 LastValidCounter;
    /*MaxDeltaCounter specifies the maximum allowed difference between two
    counter values of consecutively received valid messages.*/
    uint8 MaxDeltaCounter;
    /*If true means that no correct data (with correct Data ID and CRC) has been yet received after
    the receiver initialization or reinitialization.*/
    boolean WaitForFirstData;
    /*Indicates to E2E Library that a new data is available for Library to be checked. This attribute
    is set by the E2E Library caller, and not by the E2E Library.*/
    boolean NewDataAvailable;
    /*Number of data (messages) lost since reception of last valid one. This attribute is set only if
    Status equals E2E_P01STATUS_OK or E2E_P01STATUS_OKSOMELOST.*/
    uint8 LostData;
    /*Result of the verification of the Data, determined by the Check function.*/
    E2E_P01CheckStatusType Status;
    /*Number of Data required for validating the consistency of the counter that must be received with
    a valid counter (i.e. counter within the allowed lock-in range) after the detection of an unexpected
    behavior of a received counter.*/
    uint8 SyncCounter;
    /*Amount of consecutive reception cycles in which either (1) there was no new data, or (2) when the
    data was repeated.*/
    uint8 NoNewOrRepeatedDataCounter;
} E2E_P01CheckStateType;
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief        Protects the array/buffer to be transmitted using the E2E profile 1.
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
 * @trace        CPD-46885
 */

Std_ReturnType E2E_P01Protect(const E2E_P01ConfigType* ConfigPtr, E2E_P01ProtectStateType* StatePtr, uint8* DataPtr);

/**
 * @brief        Initializes the protection state.
 * @param[out] StatePtr : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46886
 */

Std_ReturnType E2E_P01ProtectInit(E2E_P01ProtectStateType* StatePtr);

/**
 * @brief        Checks the Data received using the E2E profile 1.
 *               This includes CRC calculation, handling of Counter and Data ID.
 * @param[in]    Config : Pointer to static configuration.
 * @param[in]    Data   : Pointer to received data.
 * @param[inout] State  : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46889
 */

Std_ReturnType E2E_P01Check(const E2E_P01ConfigType* Config, E2E_P01CheckStateType* State, const uint8* Data);

/**
 * @brief        Initializes the check state.
 * @param[out] State : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46890
 */

Std_ReturnType E2E_P01CheckInit(E2E_P01CheckStateType* State);

/**
 * @brief       The function maps the check status of Profile 1 to a generic check status, which can be
 *              used by E2E state machine check function. The E2E Profile 1 delivers a more fine-granular
 *              status, but this is not relevant for the E2E state machine.
 * @param[in]   CheckReturn     : Return value of the E2E_P01Check function.
 * @param[in]   Status          : Status determined by E2E_P01Check function.
 * @param[in]   profileBehavior : FALSE : check has the legacy behavior, before R4.2
 *                                TRUE  : check behaves like new P4/P5/P6 profiles introduced in R4.2
 * @return      E2E_PCheckStatusType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-46892
 */
E2E_PCheckStatusType
    E2E_P01MapStatusToSM(Std_ReturnType CheckReturn, E2E_P01CheckStatusType Status, boolean ProfileBehavior);
#ifdef __cplusplus

#endif
#ifdef __cplusplus
}
#endif
#endif
