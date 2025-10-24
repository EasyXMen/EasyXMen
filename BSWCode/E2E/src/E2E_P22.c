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
**  @file               : E2E_P22.c
**  @author             : Yb,QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E_P22.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P22_CRC_INITVALUE     0xFFu
#define E2E_P22_MAX_COUNTER_VALUE 0xFu
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
/**
 * @brief        verify inputs of check()
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    DataPtr   : Pointer to received data.
 * @param[in]    Length    : Length of the data in bytes.
 * @param[in]    StatePtr  : Pointer to port/data communication state.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL Std_ReturnType E2E_P22CheckVerifyInputs(
    const E2E_P22ConfigType*     ConfigPtr,
    const E2E_P22CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint16                       Length);
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/ /*PRQA S 1503 ++ */ /*VL_QAC_NoUsedApi*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 22.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType
    E2E_P22Protect(const E2E_P22ConfigType* ConfigPtr, E2E_P22ProtectStateType* StatePtr, uint8* DataPtr, uint16 Length)
{
    Std_ReturnType Ret;
    uint8          Counter;
    uint16         Offset;
    uint8          ComputedCRC;

    /*Verify inputs of the protect function return*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
        if ((Length == (ConfigPtr->DataLength / E2E_BIT_LENGTH)) && (Length >= (Offset + 2uL)))
        {
            Ret = E2E_E_OK;
        }
        else
        {
            Ret = E2E_E_INPUTERR_WRONG;
        }
    }
    else
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    if (E2E_E_OK == Ret)
    {
        /*Compute offset*/
        Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
        /*Increment Counter*/
        StatePtr->Counter++;
        StatePtr->Counter %= (E2E_P22_MAX_COUNTER_VALUE + 1u);
        /*Write Counter*/
        Counter = StatePtr->Counter;
        DataPtr[(ConfigPtr->Offset >> 3u) + 1u] =
            (DataPtr[(ConfigPtr->Offset >> 3u) + 1u] & (uint8)E2E_MASK_HIGH_NIBBLE) + ((StatePtr->Counter));
        /*Compute CRC*/
        if (ConfigPtr->Offset > 0u)
        {
            ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[0], (uint32)Offset, E2E_P22_CRC_INITVALUE, TRUE);
            ComputedCRC = Crc_CalculateCRC8H2F(
                &DataPtr[Offset + 1u],
                (uint32)((uint32)Length - (uint32)Offset - 1uL),
                ComputedCRC,
                FALSE);
        }
        else
        {
            ComputedCRC =
                Crc_CalculateCRC8H2F(&DataPtr[1], (uint32)((uint32)Length - 1uL), E2E_P22_CRC_INITVALUE, TRUE);
        }
        ComputedCRC = Crc_CalculateCRC8H2F(&ConfigPtr->DataIDList[Counter], 1uL, ComputedCRC, FALSE);
        /*Write CRC*/
        DataPtr[ConfigPtr->Offset / E2E_BIT_LENGTH] = ComputedCRC;
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P22ProtectInit(E2E_P22ProtectStateType* StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = 0u;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * Checks the Data received using the E2E profile 22.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType E2E_P22Check(
    const E2E_P22ConfigType* ConfigPtr,
    E2E_P22CheckStateType*   StatePtr,
    const uint8*             DataPtr,
    uint16                   Length)
{
    Std_ReturnType Ret;
    uint16         Offset = 0u;
    uint8          ReceivedCRC;
    uint8          ReceivedCounter;
    uint8          DeltaCounter;
    uint8          ComputedCRC;

    /*Verify inputs of the check function*/
    Ret = E2E_P22CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length);
    if (E2E_E_OK == Ret)
    {
        if (NULL_PTR == DataPtr)
        {
            StatePtr->Status = E2E_P22STATUS_NONEWDATA;
        }
        else
        {
            Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
            /*Read Counter*/
            ReceivedCounter = DataPtr[(ConfigPtr->Offset >> 3u) + 1u] & (uint8)E2E_MASK_LOW_NIBBLE;
            /*Read CRC*/
            ReceivedCRC = DataPtr[ConfigPtr->Offset / E2E_BIT_LENGTH];
            /*Compute CRC*/
            if (ConfigPtr->Offset > 0u)
            {
                ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[0], (uint32)Offset, E2E_P22_CRC_INITVALUE, TRUE);
                ComputedCRC = Crc_CalculateCRC8H2F(
                    &DataPtr[Offset + 1u],
                    (uint32)((uint32)Length - (uint32)Offset - 1uL),
                    ComputedCRC,
                    FALSE);
            }
            else
            {
                ComputedCRC =
                    Crc_CalculateCRC8H2F(&DataPtr[1], (uint32)((uint32)Length - 1uL), E2E_P22_CRC_INITVALUE, TRUE);
            }
            ComputedCRC = Crc_CalculateCRC8H2F(&ConfigPtr->DataIDList[ReceivedCounter], 1uL, ComputedCRC, FALSE);
        }

        /*Do Checks*/
        if (NULL_PTR != DataPtr)
        {
            if (ReceivedCRC == ComputedCRC)
            {
                /*taking into wrap around 0xF*/
                if (ReceivedCounter >= StatePtr->Counter)
                {
                    DeltaCounter = ReceivedCounter - StatePtr->Counter;
                }
                else
                {
                    DeltaCounter = E2E_P22_MAX_COUNTER_VALUE + ReceivedCounter - StatePtr->Counter + 1u;
                }
                if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
                {
                    StatePtr->Status = (DeltaCounter > 0u)
                                           ? ((1u == DeltaCounter) ? E2E_P22STATUS_OK : E2E_P22STATUS_OKSOMELOST)
                                           : E2E_P22STATUS_REPEATED;
                }
                else
                {
                    StatePtr->Status = E2E_P22STATUS_WRONGSEQUENCE;
                }
                StatePtr->Counter = ReceivedCounter;
            }
            else
            {
                StatePtr->Status = E2E_P22STATUS_ERROR;
            }
        }
        else
        {
            StatePtr->Status = E2E_P22STATUS_NONEWDATA;
        }
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P22CheckInit(E2E_P22CheckStateType* StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = E2E_P22_MAX_COUNTER_VALUE;
        StatePtr->Status  = E2E_P22STATUS_ERROR;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * The function maps the check status of profile 22 to a generic check status.
 */
E2E_PCheckStatusType E2E_P22MapStatusToSM(Std_ReturnType CheckReturn, E2E_P22CheckStatusType Status)
{
    E2E_PCheckStatusType Ret;

    if (CheckReturn != E2E_E_OK)
    {
        Ret = E2E_P_ERROR;
    }
    else
    {
        switch (Status)
        {
        case E2E_P22STATUS_OK:
        case E2E_P22STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P22STATUS_ERROR:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P22STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P22STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P22STATUS_WRONGSEQUENCE:
            Ret = E2E_P_WRONGSEQUENCE;
            break;
        default:
            Ret = E2E_P_ERROR;
            break;
        }
    }
    return Ret;
}
/*PRQA S 1532 -- */ /*PRQA S 1503 -- */
/* ========================================== internal function definitions ========================================= */
E2E_LOCAL Std_ReturnType E2E_P22CheckVerifyInputs(
    const E2E_P22ConfigType*     ConfigPtr,
    const E2E_P22CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint16                       Length)
{
    Std_ReturnType Ret;
    uint16         Offset = 0u;

    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        if (NULL_PTR != DataPtr)
        {
            Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
            if ((Length == (ConfigPtr->DataLength / E2E_BIT_LENGTH))
                && (Length >= (Offset + 2u))) /*PRQA S 3120*/ /*VL_QAC_MagicNum*/
            {
                Ret = E2E_E_OK;
            }
            else
            {
                Ret = E2E_E_INPUTERR_WRONG;
            }
        }
        else
        {
            if (Length == 0u)
            {
                Ret = E2E_E_OK;
            }
            else
            {
                Ret = E2E_E_INPUTERR_WRONG;
            }
        }
    }
    else
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
