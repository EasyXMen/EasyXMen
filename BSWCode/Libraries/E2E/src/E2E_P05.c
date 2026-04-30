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
**  @file               : E2E_P05.c
**  @author             : QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E_P05.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P05_CRC_INITVALUE     0xFFFFu
#define E2E_P05_MAX_COUNTER_VALUE 0xFFU
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
E2E_LOCAL Std_ReturnType E2E_P05CheckVerifyInputs(
    const E2E_P05ConfigType*     ConfigPtr,
    const E2E_P05CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint16                       Length);
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/ /*PRQA S 1503 ++ */ /*VL_QAC_NoUsedApi*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 5.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType
    E2E_P05Protect(const E2E_P05ConfigType* ConfigPtr, E2E_P05ProtectStateType* StatePtr, uint8* DataPtr, uint16 Length)
{
    Std_ReturnType Ret;
    uint16         Offset;
    uint16         ComputedCRC;
    uint8          DataID_Hbyte;
    uint8          DataID_Lbyte;

    /*Verify inputs of the protect function*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if (Length == (ConfigPtr->DataLength / E2E_BIT_LENGTH))
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
        DataID_Lbyte = (uint8)(ConfigPtr->DataID);
        DataID_Hbyte = (uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH);
        /*Compute Offset*/
        Offset               = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
        DataPtr[Offset + 2u] = StatePtr->Counter;
        /*Compute CRC*/
        if ((ConfigPtr->Offset) > 0u)
        {
            ComputedCRC = Crc_CalculateCRC16(&DataPtr[0], (uint32)Offset, E2E_P05_CRC_INITVALUE, TRUE);
            ComputedCRC = Crc_CalculateCRC16(
                &DataPtr[Offset + 2u],
                (uint32)((uint32)Length - (uint32)Offset - 2uL),
                ComputedCRC,
                FALSE);
        }
        else
        {
            ComputedCRC =
                Crc_CalculateCRC16(&DataPtr[Offset + 2u], (uint32)((uint32)Length - 2uL), E2E_P05_CRC_INITVALUE, TRUE);
        }
        ComputedCRC = Crc_CalculateCRC16(&DataID_Lbyte, 1uL, ComputedCRC, FALSE);
        ComputedCRC = Crc_CalculateCRC16(&DataID_Hbyte, 1uL, ComputedCRC, FALSE);
        /*Copy 2-byte local variable CRC on bytes Data[Offset+0...Offset+1] using Little Endian order*/
        DataPtr[Offset]      = (uint8)ComputedCRC;
        DataPtr[Offset + 1u] = (uint8)(ComputedCRC >> E2E_BIT_LENGTH);
        StatePtr->Counter++;
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P05ProtectInit(E2E_P05ProtectStateType* StatePtr)
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
 * Checks the Data received using the E2E profile 5.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType E2E_P05Check(
    const E2E_P05ConfigType* ConfigPtr,
    E2E_P05CheckStateType*   StatePtr,
    const uint8*             DataPtr,
    uint16                   Length)
{
    Std_ReturnType Ret;
    uint16         Offset;
    uint8          ReceivedCounter;
    uint16         ReceivedCRC;
    uint16         ComputedCRC;
    uint8          DeltaCounter;
    uint8          DataID_Hbyte;
    uint8          DataID_Lbyte;

    /*CRCVerify inputs of the check function*/
    Ret = E2E_P05CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length);

    if (E2E_E_OK == Ret)
    {
        DataID_Lbyte = (uint8)(ConfigPtr->DataID);
        DataID_Hbyte = (uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH);
        if (NULL_PTR != DataPtr)
        {
            /*Compute Offset*/
            Offset          = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
            ReceivedCounter = DataPtr[Offset + 2u];
            /*Copy bytes Data[Offset+0...Offset+1] using Li ttle Endian order on 2-byte local variable ReceivedCRC*/
            ReceivedCRC = (uint16)DataPtr[Offset] + (uint16)(((uint16)DataPtr[Offset + 1u]) << E2E_BIT_LENGTH);
            /*Compute CRC*/
            if ((ConfigPtr->Offset) > 0u)
            {
                ComputedCRC = Crc_CalculateCRC16(&DataPtr[0], (uint32)Offset, E2E_P05_CRC_INITVALUE, TRUE);
                ComputedCRC = Crc_CalculateCRC16(
                    &DataPtr[Offset + 2u],
                    (uint32)((uint32)Length - (uint32)Offset - 2uL),
                    ComputedCRC,
                    FALSE);
            }
            else
            {
                ComputedCRC = Crc_CalculateCRC16(
                    &DataPtr[Offset + 2u],
                    (uint32)((uint32)Length - 2uL),
                    E2E_P05_CRC_INITVALUE,
                    TRUE);
            }
            ComputedCRC = Crc_CalculateCRC16(&DataID_Lbyte, 1u, ComputedCRC, FALSE);
            ComputedCRC = Crc_CalculateCRC16(&DataID_Hbyte, 1u, ComputedCRC, FALSE);
            /*Do Checks*/
            if (ReceivedCRC == ComputedCRC)
            {
                /*taking into wrap around 0xFF*/
                DeltaCounter =
                    (ReceivedCounter >= StatePtr->Counter)
                        ? (uint8)(ReceivedCounter - StatePtr->Counter)
                        : (uint8)((uint8)(E2E_P05_MAX_COUNTER_VALUE - StatePtr->Counter) + ReceivedCounter + 1U);
                if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
                {
                    StatePtr->Status = (DeltaCounter > 0u)
                                           ? ((1u == DeltaCounter) ? E2E_P05STATUS_OK : E2E_P05STATUS_OKSOMELOST)
                                           : E2E_P05STATUS_REPEATED;
                }
                else
                {
                    StatePtr->Status = E2E_P05STATUS_WRONGSEQUENCE;
                }
                StatePtr->Counter = ReceivedCounter;
            }
            else
            {
                StatePtr->Status = E2E_P05STATUS_ERROR;
            }
        }
        else
        {
            StatePtr->Status = E2E_P05STATUS_NONEWDATA;
        }
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P05CheckInit(E2E_P05CheckStateType* StatePtr)
{
    Std_ReturnType Ret;

    /*@SWS_E2E_00451*/
    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = E2E_P05_MAX_COUNTER_VALUE;
        StatePtr->Status  = E2E_P05STATUS_ERROR;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * The function maps the check status of profile 5 to a generic check status.
 */
E2E_PCheckStatusType E2E_P05MapStatusToSM(Std_ReturnType CheckReturn, E2E_P05CheckStatusType Status)
{
    E2E_PCheckStatusType Ret;

    /*@SWS_E2E_00454*/
    if (E2E_E_OK != CheckReturn)
    {
        Ret = E2E_P_ERROR;
    }
    /*@SWS_E2E_00453*/
    else
    {
        switch (Status)
        {
        case E2E_P05STATUS_OK:
        case E2E_P05STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P05STATUS_ERROR:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P05STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P05STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P05STATUS_WRONGSEQUENCE:
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
E2E_LOCAL Std_ReturnType E2E_P05CheckVerifyInputs(
    const E2E_P05ConfigType*     ConfigPtr,
    const E2E_P05CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint16                       Length)
{
    Std_ReturnType Ret;
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        if (NULL_PTR != DataPtr)
        {
            if (Length == (ConfigPtr->DataLength / E2E_BIT_LENGTH))
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
