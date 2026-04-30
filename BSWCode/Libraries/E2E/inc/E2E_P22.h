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
 **  @file               : E2E_P22.h
 **  @author             : Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : End-to-End Communication Protection Library
 **
 ***********************************************************************************************************************/
#ifndef E2E_P22_H_
#define E2E_P22_H_

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
    /*An array of appropriately chosen Data IDs for protection against
masquerading.*/
    uint8 DataIDList[16]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    /*Initial maximum allowed gap between two counter values of two
consecutively received valid Data.*/
    uint8 MaxDeltaCounter;
    /*Offset of the E2E header in the Data[] array in bits.*/
    uint16 Offset;
} E2E_P22ConfigType;

typedef struct
{
    /*Counter to be used for protecting the next Data.*/
    uint8 Counter;
} E2E_P22ProtectStateType;

typedef enum
{
    /*OK: the checks of the Data in this cycle were successful (including counter check,
which was incremented by 1).*/
    E2E_P22STATUS_OK = 0,
    /*Error: the Check function has been invoked but no new Data is not available since
the last call, according to communication medium (e.g. RTE, COM). As a result, no E2E
checks of Data have been consequently executed.*/
    E2E_P22STATUS_NONEWDATA = 1,
    /*Error: error not related to counters occurred (e.g. wrong crc, wrong length).*/
    E2E_P22STATUS_ERROR = 7,
    /*Error: the checks of the Data in this cycle were successful, with the exception of the
repetition.*/
    E2E_P22STATUS_REPEATED = 8,
    /*OK: the checks of the Data in this cycle were successful (including counter check,
which was incremented within the allowed configured delta).*/
    E2E_P22STATUS_OKSOMELOST = 32,
    /*Error: the checks of the Data in this cycle were successful, with the exception of
counter jump, which changed more than the allowed delta*/
    E2E_P22STATUS_WRONGSEQUENCE = 64
} E2E_P22CheckStatusType;

typedef struct
{
    /*Result of the verification of the Data in this cycle, determined by the
Check function.*/
    E2E_P22CheckStatusType Status;
    /*Counter of the data in previous cycle.*/
    uint8 Counter;
} E2E_P22CheckStateType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief        Protects the array/buffer to be transmitted using the E2E profile 22.
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
 * @trace        CPD-47013
 */

Std_ReturnType E2E_P22Protect(
    const E2E_P22ConfigType* ConfigPtr,
    E2E_P22ProtectStateType* StatePtr,
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
 * @trace        CPD-47014
 */

Std_ReturnType E2E_P22ProtectInit(E2E_P22ProtectStateType* StatePtr);

/**
 * @brief        Checks the Data received using the E2E profile 22.
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
 * @trace        CPD-47017
 */

Std_ReturnType E2E_P22Check(
    const E2E_P22ConfigType* ConfigPtr,
    E2E_P22CheckStateType*   StatePtr,
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
 * @trace        CPD-47018
 */

Std_ReturnType E2E_P22CheckInit(E2E_P22CheckStateType* StatePtr);

/**
 * @brief       The function maps the check status of profile 22 to a generic check status, which can be
 *              used by E2E state machine check function. The E2E profile 22 delivers a more fine-granular
 *              status, but this is not relevant for the E2E state machine.
 * @param[in]   CheckReturn     : Return value of the E2E_P01Check function.
 * @param[in]   Status          : Status determined by E2E_P01Check function.
 * @return      E2E_PCheckStatusType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-47020
 */
E2E_PCheckStatusType E2E_P22MapStatusToSM(Std_ReturnType CheckReturn, E2E_P22CheckStatusType Status);

#ifdef __cplusplus

#endif
#ifdef __cplusplus
}
#endif
#endif
