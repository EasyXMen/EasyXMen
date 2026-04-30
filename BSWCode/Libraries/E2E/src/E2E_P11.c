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
**  @file               : E2E_P01.c
**  @author             : Yb,QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E_P11.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P11_CRC_INITVALUE     0xFFu
#define E2E_P11_CRC_XORVALUE      0xFFu
#define E2E_P11_MAX_COUNTER_VALUE 0xEu
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/**
 * @brief        check the status depending on Counter.
 * @param[in]    ConfigPtr       : Pointer to static configuration.
 * @param[in]    ReceivedCounter : the counter of current data.
 * @param[inout] StatePtr        : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void
    E2E_P11Check_Seqence(const E2E_P11ConfigType* ConfigPtr, E2E_P11CheckStateType* StatePtr, uint8 ReceivedCounter);
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
E2E_LOCAL Std_ReturnType E2E_P11CheckVerifyInputs(
    const E2E_P11ConfigType*     ConfigPtr,
    const E2E_P11CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint16                       Length);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/ /*PRQA S 1503 ++ */ /*VL_QAC_NoUsedApi*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 11.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType
    E2E_P11Protect(const E2E_P11ConfigType* ConfigPtr, E2E_P11ProtectStateType* StatePtr, uint8* DataPtr, uint16 Length)
{
    Std_ReturnType Ret;
    uint16         Offset;
    uint8          ComputedCRC = 0u;
    uint8          DataID_Hbyte;
    uint8          DataID_Lbyte;
    uint8          CRC_Data = 0u;

    /*Verify inputs of the protect function return*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if ((Length == (ConfigPtr->DataLength / E2E_BIT_LENGTH)) && (StatePtr->Counter <= E2E_P11_MAX_COUNTER_VALUE))
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
        /*Write DataIDNibble*/
        if (E2E_P11_DATAID_NIBBLE == ConfigPtr->DataIDMode)
        {
            DataPtr[ConfigPtr->DataIDNibbleOffset >> 3u] =
                (DataPtr[ConfigPtr->DataIDNibbleOffset >> 3u]
                 & (uint8)(0xFu << ((uint8)ConfigPtr->CounterOffset & 0x7u))) /* Reserve lower 4 bit */
                + (uint8)(((ConfigPtr->DataID & 0x0F00u) >> E2E_BIT_LENGTH) << (ConfigPtr->DataIDNibbleOffset & 0x7u));
            /*Write Counter*/
            DataPtr[ConfigPtr->CounterOffset >> 3u] =
                ((uint8)(DataPtr[ConfigPtr->CounterOffset >> 3u])
                 & (uint8)(0xFu << (ConfigPtr->DataIDNibbleOffset & 0x7u))) /* Reserve higher 4 bit */
                + (uint8)(StatePtr->Counter << (ConfigPtr->CounterOffset & 0x7u));
        }
        else
        {
            /*Write Counter*/
            DataPtr[ConfigPtr->CounterOffset >> 3u] =
                (uint8)(DataPtr[ConfigPtr->CounterOffset >> 3u]
                        & (uint8)(0xF0u << (ConfigPtr->CounterOffset & 0x7u))) /* Reserve higher 4 bit */
                + (uint8)(StatePtr->Counter << (ConfigPtr->CounterOffset & 0x7u));
        }
        /*Compute CRC*/
        Offset = ConfigPtr->CRCOffset / (uint16)E2E_BIT_LENGTH;
        if (E2E_P11_DATAID_BOTH == ConfigPtr->DataIDMode)
        {
            ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P11_CRC_INITVALUE, FALSE);
            ComputedCRC = Crc_CalculateCRC8(&DataID_Hbyte, 1u, ComputedCRC, FALSE);
        }
        /*E2E_P11_DATAID_NIBBLE*/
        else
        {
            ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P11_CRC_INITVALUE, FALSE);
            ComputedCRC = Crc_CalculateCRC8(&CRC_Data, 1u, ComputedCRC, FALSE);
        }
        if (Offset > 0u)
        {
            ComputedCRC = Crc_CalculateCRC8(&DataPtr[0], (uint32)Offset, ComputedCRC, FALSE);
            if (Length > (Offset + 1u))
            {
                ComputedCRC = Crc_CalculateCRC8(
                    &DataPtr[Offset + 1u],
                    (uint32)((uint32)Length - (uint32)Offset - 1uL),
                    ComputedCRC,
                    FALSE);
            }
        }
        else
        {
            ComputedCRC = Crc_CalculateCRC8(&DataPtr[1], (uint32)((uint32)Length - 1uL), ComputedCRC, FALSE);
        }
        /*Write CRC*/
        DataPtr[ConfigPtr->CRCOffset / E2E_BIT_LENGTH] = ComputedCRC ^ E2E_P11_CRC_XORVALUE;
        /*Increment Counter*/
        StatePtr->Counter++;
        StatePtr->Counter %= 15u;
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P11ProtectInit(E2E_P11ProtectStateType* StatePtr)
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
 * Checks the Data received using the E2E profile 11.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/ /*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_E2E_STM19*/
Std_ReturnType E2E_P11Check(
    /*PRQA S 2889,6060 -- */
    const E2E_P11ConfigType* ConfigPtr,
    E2E_P11CheckStateType*   StatePtr,
    const uint8*             DataPtr,
    uint16                   Length)
{
    Std_ReturnType Ret;
    uint8          ReceivedNibble;
    uint8          ReceivedCounter;
    uint8          ReceivedCRC;
    uint16         Offset;
    uint8          ComputedCRC = 0u;
    uint8          DataID_Lbyte;
    uint8          DataID_Hbyte;
    uint8          CRC_Data = 0u;

    /*Verify inputs of the check function*/
    Ret = E2E_P11CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length);

    if (E2E_E_OK == Ret)
    {
        if (NULL_PTR == DataPtr)
        {
            StatePtr->Status = E2E_P11STATUS_NONEWDATA;
        }
        else
        {
            DataID_Lbyte = (uint8)(ConfigPtr->DataID);
            DataID_Hbyte = (uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH);
            /*Read DataIDNibble*/
            ReceivedNibble =
                (uint8)(DataPtr[(ConfigPtr->DataIDNibbleOffset >> 3u)] >> (ConfigPtr->DataIDNibbleOffset & 0x7u))
                & 0xFu;
            /*Read Counter*/
            ReceivedCounter =
                (uint8)(DataPtr[(ConfigPtr->CounterOffset >> 3u)] >> (ConfigPtr->CounterOffset & 0x7u)) & 0xFu;
            if (ReceivedCounter >= 15u)
            {
                Ret = E2E_E_INPUTERR_WRONG;
                return Ret;
            }
            /*Read CRC*/
            ReceivedCRC = DataPtr[(ConfigPtr->CRCOffset) / E2E_BIT_LENGTH];
            /*Compute CRC*/
            Offset = ConfigPtr->CRCOffset / (uint16)E2E_BIT_LENGTH;
            if (E2E_P11_DATAID_BOTH == ConfigPtr->DataIDMode)
            {
                ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P11_CRC_INITVALUE, FALSE);
                ComputedCRC = Crc_CalculateCRC8(&DataID_Hbyte, 1u, ComputedCRC, FALSE);
            }
            /*E2E_P11_DATAID_NIBBLE*/
            else
            {
                ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P11_CRC_INITVALUE, FALSE);
                ComputedCRC = Crc_CalculateCRC8(&CRC_Data, 1u, ComputedCRC, FALSE);
            }
            if (Offset > 0u)
            {
                ComputedCRC = Crc_CalculateCRC8(&DataPtr[0], (uint32)Offset, ComputedCRC, FALSE);
                if (Length > (Offset + 1u))
                {
                    ComputedCRC = Crc_CalculateCRC8(
                        &DataPtr[Offset + 1u],
                        (uint32)((uint32)Length - (uint32)Offset - 1uL),
                        ComputedCRC,
                        FALSE);
                }
            }
            else
            {
                ComputedCRC = Crc_CalculateCRC8(&DataPtr[1], (uint32)((uint32)Length - 1uL), ComputedCRC, FALSE);
            }
            ComputedCRC = ComputedCRC ^ E2E_P11_CRC_XORVALUE;
            /*Do Checks*/
            if (ReceivedCRC == ComputedCRC)
            {
                if (((E2E_P11_DATAID_NIBBLE == ConfigPtr->DataIDMode)
                     && (ReceivedNibble == (uint8)((uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH) & E2E_MASK_LOW_NIBBLE)))
                    || (E2E_P11_DATAID_BOTH == ConfigPtr->DataIDMode))
                {
                    E2E_P11Check_Seqence(ConfigPtr, StatePtr, ReceivedCounter);
                }
                else
                {
                    StatePtr->Status = E2E_P11STATUS_ERROR;
                }
            }
            else
            {
                StatePtr->Status = E2E_P11STATUS_ERROR;
            }
        }
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P11CheckInit(E2E_P11CheckStateType* StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = E2E_P11_MAX_COUNTER_VALUE;
        StatePtr->Status  = E2E_P11STATUS_ERROR;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * The function maps the check status of profile 11 to a generic check status.
 */
E2E_PCheckStatusType E2E_P11MapStatusToSM(Std_ReturnType CheckReturn, E2E_P11CheckStatusType Status)
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
        case E2E_P11STATUS_OK:
        case E2E_P11STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P11STATUS_ERROR:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P11STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P11STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P11STATUS_WRONGSEQUENCE:
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
E2E_LOCAL void
    E2E_P11Check_Seqence(const E2E_P11ConfigType* ConfigPtr, E2E_P11CheckStateType* StatePtr, uint8 ReceivedCounter)
{
    uint8 DeltaCounter;

    /*taking into wrap around 0xE*/
    if (ReceivedCounter >= StatePtr->Counter) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        DeltaCounter = ReceivedCounter - StatePtr->Counter;
    }
    else
    {
        DeltaCounter = E2E_P11_MAX_COUNTER_VALUE + ReceivedCounter - StatePtr->Counter + 1u;
    }
    if (DeltaCounter <= ConfigPtr->MaxDeltaCounter) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        if (DeltaCounter > 0u)
        {
            if (1u == DeltaCounter)
            {
                StatePtr->Status = E2E_P11STATUS_OK;
            }
            else
            {
                StatePtr->Status = E2E_P11STATUS_OKSOMELOST;
            }
        }
        if (0u == DeltaCounter)
        {
            StatePtr->Status = E2E_P11STATUS_REPEATED;
        }
    }
    else
    {
        StatePtr->Status = E2E_P11STATUS_WRONGSEQUENCE;
    }
    StatePtr->Counter = ReceivedCounter;
}

E2E_LOCAL Std_ReturnType E2E_P11CheckVerifyInputs(
    const E2E_P11ConfigType*     ConfigPtr,
    const E2E_P11CheckStateType* StatePtr,
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
            if (Length == 0uL)
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
