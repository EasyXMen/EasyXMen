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
 **  @file               : E2E_P06.h
 **  @author             : Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : End-to-End Communication Protection Library
 **
 ***********************************************************************************************************************/
#ifndef E2E_P06_H_
#define E2E_P06_H_

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
    /*Bit offset of the first bit of the E2E header from the beginning of the Data
    Array (bit numbering: bit 0 is the least important). The offset shall be a multiple
    of 8 and 0 ' Offset ' MaxDataLength- (5*8).*/
    uint16 Offset;
    /*Minimal length of Data array, in bytes (i.e. the length of the E2E-protected
    message, without counting the field Length itself). E2E checks that Length is
    ' MinDataLength.*/
    uint16 MinDataLength;
    /*Maximal length of Data, in bits. E2E checks that DataLength is ' MaxDataLength.*/
    uint16 MaxDataLength;
    /*A system-unique identifier of the Data*/
    uint16 DataID;
    /*Maximum allowed gap between two counter values of two consecutively received valid
    Data.*/
    uint8 MaxDeltaCounter;
} E2E_P06ConfigType;

typedef struct
{
    /*Counter to be used for protecting the next Data.*/
    uint8 Counter;
} E2E_P06ProtectStateType;

typedef enum
{
    /*OK: the checks of the Data in this cycle were successful (including counter check,
    which was incremented by 1).*/
    E2E_P06STATUS_OK = 0,
    /*Error: the Check function has been invoked but no new Data is not available since
    the last call, according to communication medium (e.g. RTE, COM). As a result, no E2E
    checks of Data have been consequently executed.*/
    E2E_P06STATUS_NONEWDATA = 1,
    /*Error: error not related to counters occurred (e.g. wrong crc, wrong length).*/
    E2E_P06STATUS_ERROR = 7,
    /*Error: the checks of the Data in this cycle were successful, with the exception of the
    repetition.*/
    E2E_P06STATUS_REPEATED = 8,
    /*OK: the checks of the Data in this cycle were successful (including counter check,
    which was incremented within the allowed configured delta).*/
    E2E_P06STATUS_OKSOMELOST = 32,
    /*Error: the checks of the Data in this cycle were successful, with the exception of
    counter jump, which changed more than the allowed delta*/
    E2E_P06STATUS_WRONGSEQUENCE = 64
} E2E_P06CheckStatusType;

typedef struct
{
    /*Result of the verification of the Data in this cycle, determined by the Check function.*/
    E2E_P06CheckStatusType Status;
    /*Counter of the data in previous cycle. It is initialized with 0.*/
    uint8 Counter;
} E2E_P06CheckStateType;
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief        Protects the array/buffer to be transmitted using the E2E profile 6.
 *               This includes checksum calculation, handling of counter and Data ID.
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    Length    : Length of the data in bytes.
 * @param[inout] StatePtr  : Pointer to port/data communication state.
 * @param[inout] DataPtr   : Pointer to Data to be transmitted.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46945
 */

Std_ReturnType E2E_P06Protect(
    const E2E_P06ConfigType* ConfigPtr,
    E2E_P06ProtectStateType* StatePtr,
    uint8*                   DataPtr,
    uint16                   Length);

/**
 * @brief        Initializes the protection state.
 * @param[out]   StatePtr : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46946
 */

Std_ReturnType E2E_P06ProtectInit(E2E_P06ProtectStateType* StatePtr);

/**
 * @brief        Checks the Data received using the E2E profile 6.
 *               This includes CRC calculation, handling of Counter and Data ID.
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    DataPtr   : Pointer to received data.
 * @param[in]    Length    : Length of the data in bytes.
 * @param[inout] StatePtr  : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46949
 */

Std_ReturnType E2E_P06Check(
    const E2E_P06ConfigType* ConfigPtr,
    E2E_P06CheckStateType*   StatePtr,
    const uint8*             DataPtr,
    uint16                   Length);

/**
 * @brief        Initializes the check state.
 * @param[out]   StatePtr : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-46950
 */

Std_ReturnType E2E_P06CheckInit(E2E_P06CheckStateType* StatePtr);

/**
 * @brief       The function maps the check status of Profile 1 to a generic check status, which can be
 *              used by E2E state machine check function. The E2E Profile 1 delivers a more fine-granular
 *              status, but this is not relevant for the E2E state machine.
 * @param[in]   CheckReturn     : Return value of the E2E_P01Check function.
 * @param[in]   Status          : Status determined by E2E_P01Check function.
 * @return      E2E_PCheckStatusType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-46952
 */
E2E_PCheckStatusType E2E_P06MapStatusToSM(Std_ReturnType CheckReturn, E2E_P06CheckStatusType Status);

#ifdef __cplusplus

#endif
#ifdef __cplusplus
}
#endif
#endif
