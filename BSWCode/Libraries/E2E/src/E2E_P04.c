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
**  @file               : E2E_P04.c
**  @author             : Yb,QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E_P04.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P04_CRC_INITVALUE     0xFFFFFFFFu
#define E2E_P04_MAX_COUNTER_VALUE 0xFFFFu
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/**
 * @brief        check the status depending on Counter.
 * @param[in]    ConfigPtr       : Pointer to static configuration.
 * @param[in]    ReceivedCounter : the counter of current data.
 * @param[inout] StatePtr        : Pointer to port/data communication state.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */

E2E_LOCAL void
    E2E_P04Check_Seqence(const E2E_P04ConfigType* ConfigPtr, E2E_P04CheckStateType* StatePtr, uint16 ReceivedCounter);
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
E2E_LOCAL Std_ReturnType E2E_P04CheckVerifyInputs(
    const E2E_P04ConfigType*     ConfigPtr,
    const E2E_P04CheckStateType* StatePtr,
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
 * Protects the array/buffer to be transmitted using the E2E profile 4.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType
    E2E_P04Protect(const E2E_P04ConfigType* ConfigPtr, E2E_P04ProtectStateType* StatePtr, uint8* DataPtr, uint16 Length)
{
    Std_ReturnType Ret = E2E_E_OK;
    uint16         Offset;
    uint32         ComputedCRC;

    /*Verify inputs of the protect function*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if ((Length >= (ConfigPtr->MinDataLength / E2E_BIT_LENGTH))
            && (Length <= (ConfigPtr->MaxDataLength / E2E_BIT_LENGTH)))
        {
            /*compute local variable uint16 Offset, which is in [byte]*/
            Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
            /*Copy 2-byte Length on bytes Data[Offset...Offset+1] in Big Endian order*/
            DataPtr[Offset]      = (uint8)(Length >> E2E_BIT_LENGTH);
            DataPtr[Offset + 1u] = (uint8)Length;
            /*Copy 2-byte State->Counter on bytes Data[Offset+2...Offset+3] in Big Endian order*/
            DataPtr[Offset + 2u] = (uint8)(StatePtr->Counter >> E2E_BIT_LENGTH);
            DataPtr[Offset + 3u] = (uint8)(StatePtr->Counter);
            /*Copy 4-byte Config->DataID to bytes Data[Offset+4...Offset+7] in Big Endian order*/
            DataPtr[Offset + 4u] = (uint8)(ConfigPtr->DataID >> 24u);
            DataPtr[Offset + 5u] = (uint8)(ConfigPtr->DataID >> 16u);
            DataPtr[Offset + 6u] = (uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH);
            DataPtr[Offset + 7u] = (uint8)(ConfigPtr->DataID);
            /*compute CRC*/
            ComputedCRC =
                Crc_CalculateCRC32P4(&DataPtr[0], ((uint32)Offset + E2E_BIT_LENGTH), E2E_P04_CRC_INITVALUE, TRUE);
            if ((Offset + 12u) < Length)
            {
                ComputedCRC = Crc_CalculateCRC32P4(
                    &DataPtr[Offset + 12u],
                    ((uint32)Length - (uint32)Offset - 12uL),
                    ComputedCRC,
                    FALSE);
            }
            /*Copy 4-byte local variable CRC on bytes Data[Offset+8...Offset+11] using big Endian order*/
            DataPtr[Offset + 8u]  = (uint8)(ComputedCRC >> 24u);
            DataPtr[Offset + 9u]  = (uint8)(ComputedCRC >> 16u);
            DataPtr[Offset + 10u] = (uint8)(ComputedCRC >> 8u);
            DataPtr[Offset + 11u] = (uint8)(ComputedCRC);
            /*Increment Counter*/
            StatePtr->Counter++;
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

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P04ProtectInit(E2E_P04ProtectStateType* StatePtr)
{
    Std_ReturnType Ret;

    Ret = E2E_E_OK;
    /*@SWS_E2E_00377*/
    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = 0u;
    }
    return Ret;
}
/**
 * Checks the Data received using the E2E profile 4.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
Std_ReturnType E2E_P04Check(
    const E2E_P04ConfigType* ConfigPtr,
    E2E_P04CheckStateType*   StatePtr,
    const uint8*             DataPtr,
    uint16                   Length)
{
    Std_ReturnType Ret;
    uint16         Offset;
    uint16         ReceivedLength;
    uint16         ReceivedCounter;
    uint32         ReceivedDataID;
    uint32         ReceivedCRC;
    uint32         ComputedCRC;

    Ret = E2E_P04CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length);
    if (E2E_E_OK == Ret)
    {
        if ((NULL_PTR != DataPtr))
        {
            /*compute local variable uint16 Offset, which is in [byte]*/
            Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
            /*Copy bytes Data[Offset...Offset+1] in Big Endian order to uint16 local variable ReceivedLength*/
            ReceivedLength = (uint16)((uint16)DataPtr[Offset] << E2E_BIT_LENGTH) + (uint16)(DataPtr[Offset + 1u]);
            /*Copy bytes Data[Offset+2...Offset+3] in Big Endian order on uint16 local variable ReceivedCounter*/
            ReceivedCounter = (uint16)((uint16)DataPtr[Offset + 2u] << E2E_BIT_LENGTH) + (uint16)(DataPtr[Offset + 3u]);
            /*Copy bytes Data[Offset+4...Offset+7] in Big Endian order on uint32 local variable ReceivedDataID*/
            ReceivedDataID =
                (uint32)((uint32)DataPtr[Offset + 4u] << 24u) + (uint32)((uint32)DataPtr[Offset + 5u] << 16u)
                + (uint32)((uint32)DataPtr[Offset + 6u] << E2E_BIT_LENGTH) + (uint32)(DataPtr[Offset + 7u]);
            /*Copy bytes Data[Offset+8...Offset+11] using big Endian order on 4-byte local variable ReceivedCRC*/
            ReceivedCRC = (uint32)((uint32)DataPtr[Offset + 8u] << 24u) + (uint32)((uint32)DataPtr[Offset + 9u] << 16u)
                          + (uint32)((uint32)DataPtr[Offset + 10u] << 8u) + ((uint32)DataPtr[Offset + 11u]);
            /*compute CRC*/
            ComputedCRC =
                Crc_CalculateCRC32P4(&DataPtr[0], ((uint32)Offset + E2E_BIT_LENGTH), E2E_P04_CRC_INITVALUE, TRUE);
            if ((Offset + 12u) < Length)
            {
                ComputedCRC = Crc_CalculateCRC32P4(
                    &DataPtr[Offset + 12u],
                    ((uint32)Length - (uint32)Offset - 12uL),
                    ComputedCRC,
                    FALSE);
            }
            /*Do Checks*/
            if ((ReceivedCRC == ComputedCRC) && (ReceivedDataID == ConfigPtr->DataID))
            {
                if (ReceivedLength == Length)
                {
                    E2E_P04Check_Seqence(ConfigPtr, StatePtr, ReceivedCounter);
                }
                else
                {
                    StatePtr->Status = E2E_P04STATUS_ERROR;
                }
            }
            else
            {
                StatePtr->Status = E2E_P04STATUS_ERROR;
            }
        }
        else
        {
            /*Do Checks*/
            StatePtr->Status = E2E_P04STATUS_NONEWDATA;
        }
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P04CheckInit(E2E_P04CheckStateType* StatePtr)
{
    uint8 Ret;

    /*@SWS_E2E_00378*/
    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = E2E_P04_MAX_COUNTER_VALUE;
        StatePtr->Status  = E2E_P04STATUS_ERROR;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * The function maps the check status of profile 4 to a generic check status.
 */
E2E_PCheckStatusType E2E_P04MapStatusToSM(Std_ReturnType CheckReturn, E2E_P04CheckStatusType Status)
{
    E2E_PCheckStatusType Ret;

    /*@SWS_E2E_00352*/
    if (E2E_E_OK != CheckReturn)
    {
        Ret = E2E_P_ERROR;
    }
    /*@SWS_E2E_00351*/
    else
    {
        switch (Status)
        {
        case E2E_P04STATUS_OK:
        case E2E_P04STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P04STATUS_ERROR:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P04STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P04STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P04STATUS_WRONGSEQUENCE:
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
    E2E_P04Check_Seqence(const E2E_P04ConfigType* ConfigPtr, E2E_P04CheckStateType* StatePtr, uint16 ReceivedCounter)
{
    uint16 DeltaCounter;
    /*Compute local variable DeltaCounter: ReceivedCounter - State->Counter
     * (taking into wrap around 0xFFFF)*/
    /*PRQA S 2814 ++ */ /*VL_E2E_NULLPointer*/
    DeltaCounter = (ReceivedCounter >= StatePtr->Counter)
                       ? (ReceivedCounter - StatePtr->Counter)
                       : (uint16)(E2E_P04_MAX_COUNTER_VALUE - StatePtr->Counter + ReceivedCounter + 1U);
    /*PRQA S 2814 -- */

    if ((DeltaCounter <= ConfigPtr->MaxDeltaCounter)) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        if (DeltaCounter > 0u)
        {
            if (1u == DeltaCounter)
            {
                StatePtr->Status = E2E_P04STATUS_OK;
            }
            else
            {
                StatePtr->Status = E2E_P04STATUS_OKSOMELOST;
            }
        }
        else
        {
            StatePtr->Status = E2E_P04STATUS_REPEATED;
        }
    }
    else
    {
        StatePtr->Status = E2E_P04STATUS_WRONGSEQUENCE;
    }
    StatePtr->Counter = ReceivedCounter;
}

E2E_LOCAL Std_ReturnType E2E_P04CheckVerifyInputs(
    const E2E_P04ConfigType*     ConfigPtr,
    const E2E_P04CheckStateType* StatePtr,
    const uint8*                 DataPtr,
    uint16                       Length)
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
