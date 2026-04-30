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
**  @file               : E2E_P08.c
**  @author             : Yb,QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E_P08.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P08_CRC_INITVALUE     0xFFFFFFFFuL
#define E2E_P08_MAX_COUNTER_VALUE 0xFFFFFFFFuL
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
    E2E_P08Check_Seqence(const E2E_P08ConfigType* ConfigPtr, E2E_P08CheckStateType* StatePtr, uint32 ReceivedCounter);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/* ============================================ internal data definitions =========================================== */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
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
E2E_LOCAL Std_ReturnType E2E_P08CheckVerifyInputs(
    const E2E_P08ConfigType*     ConfigPtr,
    const E2E_P08CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint32                       Length);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/ /*PRQA S 1503 ++ */ /*VL_QAC_NoUsedApi*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 8.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType
    E2E_P08Protect(const E2E_P08ConfigType* ConfigPtr, E2E_P08ProtectStateType* StatePtr, uint8* DataPtr, uint32 Length)
{
    Std_ReturnType Ret;
    uint32         Offset;
    uint32         ComputedCRC;

    /*Verify inputs of the protect function return*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if ((Length >= (ConfigPtr->MinDataLength / E2E_BIT_LENGTH))
            && (Length <= (ConfigPtr->MaxDataLength / E2E_BIT_LENGTH)))
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
        /*Compute offset[*/
        Offset = ConfigPtr->Offset / E2E_BIT_LENGTH;
        /*Write Length*/
        DataPtr[Offset + 4uL] = (uint8)(Length >> 24u);
        DataPtr[Offset + 5uL] = (uint8)(Length >> 16u);
        DataPtr[Offset + 6uL] = (uint8)(Length >> E2E_BIT_LENGTH);
        DataPtr[Offset + 7uL] = (uint8)Length;
        /*Write Counter*/
        DataPtr[Offset + 8uL]  = (uint8)(StatePtr->Counter >> 24u);
        DataPtr[Offset + 9uL]  = (uint8)(StatePtr->Counter >> 16u);
        DataPtr[Offset + 10uL] = (uint8)(StatePtr->Counter >> E2E_BIT_LENGTH);
        DataPtr[Offset + 11uL] = (uint8)(StatePtr->Counter);
        /*Write DataID*/
        DataPtr[Offset + 12uL] = (uint8)(ConfigPtr->DataID >> 24u);
        DataPtr[Offset + 13uL] = (uint8)(ConfigPtr->DataID >> 16u);
        DataPtr[Offset + 14uL] = (uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH);
        DataPtr[Offset + 15uL] = (uint8)(ConfigPtr->DataID);
        /*Compute CRC*/
        if (ConfigPtr->Offset > 0uL)
        {
            ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[0], Offset, E2E_P08_CRC_INITVALUE, TRUE);
            ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[Offset + 4uL], Length - Offset - 4uL, ComputedCRC, FALSE);
        }
        else
        {
            ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[Offset + 4uL], Length - 4uL, E2E_P08_CRC_INITVALUE, TRUE);
        }
        /*Write CRC*/
        DataPtr[Offset]       = (uint8)(ComputedCRC >> 24u);
        DataPtr[Offset + 1uL] = (uint8)(ComputedCRC >> 16u);
        DataPtr[Offset + 2uL] = (uint8)(ComputedCRC >> E2E_BIT_LENGTH);
        DataPtr[Offset + 3uL] = (uint8)ComputedCRC;
        /*increment counter*/
        StatePtr->Counter++;
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P08ProtectInit(E2E_P08ProtectStateType* StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = 0uL;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * Checks the Data received using the E2E profile 8.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType E2E_P08Check(
    const E2E_P08ConfigType* ConfigPtr,
    E2E_P08CheckStateType*   StatePtr,
    const uint8*             DataPtr,
    uint32                   Length)
{
    Std_ReturnType Ret;
    uint32         Offset;
    uint32         ReceivedLength;
    uint32         ReceivedCounter;
    uint32         ReceivedDataID;
    uint32         ReceivedCRC;
    uint32         ComputedCRC;

    /*Verify inputs of the check function*/
    Ret = E2E_P08CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length);

    if (E2E_E_OK == Ret)
    {
        if (NULL_PTR != DataPtr)
        {
            /*Compute offset*/
            Offset = ConfigPtr->Offset / E2E_BIT_LENGTH;
            /*Read Length*/
            ReceivedLength =
                (uint32)(((uint32)DataPtr[Offset + 4uL]) << 24u) + (uint32)(((uint32)DataPtr[Offset + 5uL]) << 16u)
                + (uint32)(((uint32)DataPtr[Offset + 6uL]) << E2E_BIT_LENGTH) + (uint32)DataPtr[Offset + 7uL];
            /*Read Counter*/
            ReceivedCounter =
                (uint32)(((uint32)DataPtr[Offset + 8uL]) << 24u) + (uint32)(((uint32)DataPtr[Offset + 9uL]) << 16u)
                + (uint32)(((uint32)DataPtr[Offset + 10uL]) << E2E_BIT_LENGTH) + (uint32)DataPtr[Offset + 11uL];
            /*Read DataID*/
            ReceivedDataID =
                (uint32)(((uint32)DataPtr[Offset + 12uL]) << 24u) + (uint32)(((uint32)DataPtr[Offset + 13uL]) << 16u)
                + (uint32)(((uint32)DataPtr[Offset + 14uL]) << E2E_BIT_LENGTH) + (uint32)DataPtr[Offset + 15uL];
            /*Read CRC*/
            ReceivedCRC = (uint32)(((uint32)DataPtr[Offset]) << 24u) + (uint32)(((uint32)DataPtr[Offset + 1uL]) << 16u)
                          + (uint32)(((uint32)DataPtr[Offset + 2uL]) << E2E_BIT_LENGTH)
                          + (((uint32)DataPtr[Offset + 3uL]));
            /*Compute CRC*/
            if (ConfigPtr->Offset > 0uL)
            {
                ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[0], Offset, E2E_P08_CRC_INITVALUE, TRUE);
                ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[Offset + 4uL], Length - Offset - 4uL, ComputedCRC, FALSE);
            }
            else
            {
                ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[Offset + 4uL], Length - 4uL, E2E_P08_CRC_INITVALUE, TRUE);
            }
            /*Do Checks*/
            if ((ReceivedCRC == ComputedCRC) && (ReceivedDataID == ConfigPtr->DataID))
            {
                if (ReceivedLength == Length)
                {
                    E2E_P08Check_Seqence(ConfigPtr, StatePtr, ReceivedCounter);
                }
                else
                {
                    StatePtr->Status = E2E_P08STATUS_ERROR;
                }
            }
            else
            {
                StatePtr->Status = E2E_P08STATUS_ERROR;
            }
        }
        else
        {
            StatePtr->Status = E2E_P08STATUS_NONEWDATA;
        }
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P08CheckInit(E2E_P08CheckStateType* StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = E2E_P08_MAX_COUNTER_VALUE;
        StatePtr->Status  = E2E_P08STATUS_ERROR;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * The function maps the check status of profile 8 to a generic check status.
 */
E2E_PCheckStatusType E2E_P08MapStatusToSM(Std_ReturnType CheckReturn, E2E_P08CheckStatusType Status)
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
        case E2E_P08STATUS_OK:
        case E2E_P08STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P08STATUS_ERROR:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P08STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P08STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P08STATUS_WRONGSEQUENCE:
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
    E2E_P08Check_Seqence(const E2E_P08ConfigType* ConfigPtr, E2E_P08CheckStateType* StatePtr, uint32 ReceivedCounter)
{
    uint32 DeltaCounter;
    /*PRQA S 2814 ++ */ /*VL_E2E_NULLPointer*/
    DeltaCounter = (ReceivedCounter >= StatePtr->Counter)
                       ? (uint32)(ReceivedCounter - StatePtr->Counter)
                       : (uint32)((uint32)(E2E_P08_MAX_COUNTER_VALUE - StatePtr->Counter) + (ReceivedCounter + 1uL));
    if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
    /*PRQA S 2814 -- */
    {
        if (DeltaCounter > 0uL)
        {
            if (1uL == DeltaCounter)
            {
                StatePtr->Status = E2E_P08STATUS_OK;
            }
            else
            {
                StatePtr->Status = E2E_P08STATUS_OKSOMELOST;
            }
        }
        else
        {
            StatePtr->Status = E2E_P08STATUS_REPEATED;
        }
    }
    else
    {
        StatePtr->Status = E2E_P08STATUS_WRONGSEQUENCE;
    }
    StatePtr->Counter = ReceivedCounter;
}

E2E_LOCAL Std_ReturnType E2E_P08CheckVerifyInputs(
    const E2E_P08ConfigType*     ConfigPtr,
    const E2E_P08CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint32                       Length)
{
    Std_ReturnType Ret;
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        if (NULL_PTR != DataPtr)
        {
            if ((Length >= (ConfigPtr->MinDataLength / E2E_BIT_LENGTH))
                && (Length <= (ConfigPtr->MaxDataLength / E2E_BIT_LENGTH)))
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
