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
**  @file               : E2E_P02.c
**  @author             : Yb,QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E.h"
#include "E2E_P02.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P02_MAX_COUNTER_VALUE 0x0Fu
#define E2E_P02_CRC_INITVALUE     0xFFu
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/**
 * @brief        verify inputs of protect()
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    StatePtr  : Pointer to port/data communication state.
 * @param[in]    DataPtr   : Pointer to Data to be transmitted.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL Std_ReturnType E2E_P02ProtectVerifyInputs(
    const E2E_P02ConfigType*       ConfigPtr,
    const E2E_P02ProtectStateType* StatePtr,
    const uint8*                   DataPtr);
/**
 * @brief        verify inputs of protect()
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    StatePtr  : Pointer to port/data communication state.
 * @param[in]    DataPtr   : Pointer to Data to be transmitted.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval       E2E_E_INTERR         : An internal library error has occurred
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL Std_ReturnType E2E_P02CheckVerifyInputs(
    const E2E_P02ConfigType*     ConfigPtr,
    const E2E_P02CheckStateType* StatePtr,
    const uint8*                 DataPtr);
/**
 * @brief        Caculate the status of received counter
 * @param[in]    ConfigPtr       : Pointer to static configuration.
 * @param[in]    DeltaCounter    : Counter difference between two consecutive received data.
 * @param[in]    ReceivedCounter : the counter of current data.
 * @param[inout] StatePtr        : Pointer to port/data communication state.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL void E2E_P02AnalysisDeltaCounter(
    uint8                    DeltaCounter,
    E2E_P02CheckStateType*   StatePtr,
    const E2E_P02ConfigType* ConfigPtr,
    uint8                    ReceivedCounter);
/**
 * @brief        Caculate the status to OK, OKSOMELOST, SYNC
 * @param[in]    ConfigPtr       : Pointer to static configuration.
 * @param[in]    status          : the next status.
 * @param[inout] StatePtr        : Pointer to port/data communication state.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL void E2E_P02AddStatus(
    E2E_P02CheckStateType*   StatePtr,
    const E2E_P02ConfigType* ConfigPtr,
    E2E_P02CheckStatusType   status);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/ /*PRQA S 1503 ++ */ /*VL_QAC_NoUsedApi*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 2.
 */
Std_ReturnType E2E_P02Protect(const E2E_P02ConfigType* ConfigPtr, E2E_P02ProtectStateType* StatePtr, uint8* DataPtr)
{
    uint8 Ret = E2E_P02ProtectVerifyInputs(ConfigPtr, StatePtr, DataPtr);
    if (E2E_E_OK == Ret)
    {
        uint8 DataID;
        uint8 CRC;

        if (StatePtr->Counter < E2E_P02_MAX_COUNTER_VALUE)
        {
            StatePtr->Counter++;
        }
        else
        {
            StatePtr->Counter = 0u;
        }
        DataPtr[1] = (DataPtr[1] & E2E_MASK_HIGH_NIBBLE) | (StatePtr->Counter);
        DataID     = ConfigPtr->DataIDList[StatePtr->Counter];
        CRC        = Crc_CalculateCRC8H2F(
            &DataPtr[1],
            (((uint32)ConfigPtr->DataLength / E2E_BIT_LENGTH) - 1uL),
            E2E_P02_CRC_INITVALUE,
            TRUE);
        CRC        = Crc_CalculateCRC8H2F(&DataID, 1u, CRC, FALSE);
        DataPtr[0] = CRC;
    }

    return Ret;
}
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P02ProtectInit(E2E_P02ProtectStateType* StatePtr)
{
    uint8 Ret;
    Ret = E2E_E_OK;

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
 * Checks the Data received using the E2E profile 2.
 */
Std_ReturnType E2E_P02Check(const E2E_P02ConfigType* ConfigPtr, E2E_P02CheckStateType* StatePtr, const uint8* DataPtr)
{
    Std_ReturnType Ret = E2E_P02CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr);
    if (E2E_E_OK == Ret)
    {
        uint8 ReceivedCounter;
        uint8 DataID;
        uint8 CalcualtedCRC;
        uint8 DeltaCounter;

        if (StatePtr->MaxDeltaCounter < E2E_P02_MAX_COUNTER_VALUE)
        {
            StatePtr->MaxDeltaCounter++;
        }

        if (StatePtr->NewDataAvailable)
        {
            ReceivedCounter = (DataPtr[1] & (uint8)E2E_MASK_LOW_NIBBLE);
            DataID          = ConfigPtr->DataIDList[ReceivedCounter];
            CalcualtedCRC   = Crc_CalculateCRC8H2F(
                &DataPtr[1],
                (((uint32)ConfigPtr->DataLength / E2E_BIT_LENGTH) - 1uL),
                E2E_P02_CRC_INITVALUE,
                TRUE);
            CalcualtedCRC = Crc_CalculateCRC8H2F(&DataID, 1u, CalcualtedCRC, FALSE);
            /*checksum ok*/
            if (CalcualtedCRC == DataPtr[0])
            {
                if (StatePtr->WaitForFirstData)
                {
                    StatePtr->WaitForFirstData = FALSE;
                    StatePtr->MaxDeltaCounter  = ConfigPtr->MaxDeltaCounterInit;
                    StatePtr->LastValidCounter = ReceivedCounter;
                    StatePtr->Status           = E2E_P02STATUS_INITIAL;
                }
                else
                {
                    /*caculate delta counter*/
                    DeltaCounter =
                        (ReceivedCounter >= StatePtr->LastValidCounter)
                            ? (ReceivedCounter - StatePtr->LastValidCounter)
                            : (uint8)(ReceivedCounter + E2E_P02_MAX_COUNTER_VALUE + 1u - StatePtr->LastValidCounter);
                    E2E_P02AnalysisDeltaCounter(DeltaCounter, StatePtr, ConfigPtr, ReceivedCounter);
                }
            }
            else
            {
                StatePtr->Status = E2E_P02STATUS_WRONGCRC;
            }
        }
        else
        {
            if (StatePtr->NoNewOrRepeatedDataCounter < E2E_P02_MAX_COUNTER_VALUE)
            {
                StatePtr->NoNewOrRepeatedDataCounter++;
            }
            StatePtr->Status = E2E_P02STATUS_NONEWDATA;
        }
    }

    return Ret;
}
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P02CheckInit(E2E_P02CheckStateType* StatePtr)
{
    uint8 Ret;

    Ret = E2E_E_OK;
    /*@SWS_E2E_00392*/
    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->LastValidCounter           = 0u;
        StatePtr->MaxDeltaCounter            = 0u;
        StatePtr->WaitForFirstData           = TRUE;
        StatePtr->NewDataAvailable           = TRUE;
        StatePtr->LostData                   = 0u;
        StatePtr->Status                     = E2E_P02STATUS_NONEWDATA;
        StatePtr->NoNewOrRepeatedDataCounter = 0u;
        StatePtr->SyncCounter                = 0u;
    }
    return Ret;
}
/**
 * The function maps the check status of Profile 1 to a generic check status.
 */
E2E_PCheckStatusType
    E2E_P02MapStatusToSM(Std_ReturnType CheckReturn, E2E_P02CheckStatusType Status, boolean ProfileBehavior)
{
    E2E_PCheckStatusType Ret;

    /*@SWS_E2E_00381*/
    if (CheckReturn != E2E_E_OK)
    {
        Ret = E2E_P_ERROR;
    }
    /*@SWS_E2E_00380*/
    else
    {
        switch (Status)
        {
        case E2E_P02STATUS_OK:
        case E2E_P02STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P02STATUS_SYNC: /*@SWS_E2E_00477*/
            Ret = ProfileBehavior ? E2E_P_OK : E2E_P_WRONGSEQUENCE;
            break;
        case E2E_P02STATUS_WRONGCRC:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P02STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P02STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P02STATUS_WRONGSEQUENCE:
            Ret = E2E_P_WRONGSEQUENCE;
            break;
        case E2E_P02STATUS_INITIAL: /*@SWS_E2E_00477*/
            Ret = ProfileBehavior ? E2E_P_WRONGSEQUENCE : E2E_P_OK;
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
E2E_LOCAL Std_ReturnType E2E_P02ProtectVerifyInputs(
    const E2E_P02ConfigType*       ConfigPtr,
    const E2E_P02ProtectStateType* StatePtr,
    const uint8*                   DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* Check if input pointer parameters are not NULL */
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        /* Check if Counter is within valid range */
        if (StatePtr->Counter > E2E_P02_MAX_COUNTER_VALUE)
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else
        {
            ret = E2E_E_OK;
        }
    }
    return ret;
}
E2E_LOCAL Std_ReturnType E2E_P02CheckVerifyInputs(
    const E2E_P02ConfigType*     ConfigPtr,
    const E2E_P02CheckStateType* StatePtr,
    const uint8*                 DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* Check if input pointer parameters are valid and if state parameter are within range */
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if ((StatePtr->LastValidCounter > E2E_P02_MAX_COUNTER_VALUE)
            || (StatePtr->MaxDeltaCounter > E2E_P02_MAX_COUNTER_VALUE)
            || (StatePtr->SyncCounter > ConfigPtr->SyncCounterInit)
            || (StatePtr->NoNewOrRepeatedDataCounter > E2E_P02_MAX_COUNTER_VALUE))
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else
        {
            ret = E2E_E_OK;
        }
    }
    return ret;
}

E2E_LOCAL void E2E_P02AnalysisDeltaCounter(
    uint8                    DeltaCounter,
    E2E_P02CheckStateType*   StatePtr,
    const E2E_P02ConfigType* ConfigPtr,
    uint8                    ReceivedCounter)
{
    if (0u == DeltaCounter)
    {
        if (StatePtr->NoNewOrRepeatedDataCounter < E2E_P02_MAX_COUNTER_VALUE) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
        {
            StatePtr->NoNewOrRepeatedDataCounter++;
        }
        StatePtr->Status = E2E_P02STATUS_REPEATED;
    }
    else if (1u == DeltaCounter)
    {
        StatePtr->MaxDeltaCounter  = ConfigPtr->MaxDeltaCounterInit; /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
        StatePtr->LastValidCounter = ReceivedCounter;
        StatePtr->LostData         = 0u;
        E2E_P02AddStatus(StatePtr, ConfigPtr, E2E_P02STATUS_OK);
    }
    else if (DeltaCounter <= StatePtr->MaxDeltaCounter)
    {
        StatePtr->MaxDeltaCounter  = ConfigPtr->MaxDeltaCounterInit;
        StatePtr->LastValidCounter = ReceivedCounter;
        StatePtr->LostData         = (DeltaCounter - 1u);
        E2E_P02AddStatus(StatePtr, ConfigPtr, E2E_P02STATUS_OKSOMELOST);
    }
    else
    {
        StatePtr->NoNewOrRepeatedDataCounter = 0u;
        StatePtr->SyncCounter                = ConfigPtr->SyncCounterInit;
        if (StatePtr->SyncCounter > 0u)
        {
            StatePtr->MaxDeltaCounter  = ConfigPtr->MaxDeltaCounterInit;
            StatePtr->LastValidCounter = ReceivedCounter;
            StatePtr->Status           = E2E_P02STATUS_WRONGSEQUENCE;
        }
    }
}
E2E_LOCAL void
    E2E_P02AddStatus(E2E_P02CheckStateType* StatePtr, const E2E_P02ConfigType* ConfigPtr, E2E_P02CheckStatusType status)
{
    if (StatePtr->NoNewOrRepeatedDataCounter <= ConfigPtr->MaxNoNewOrRepeatedData)
    {
        StatePtr->NoNewOrRepeatedDataCounter = 0u;
        if (StatePtr->SyncCounter > 0u)
        {
            StatePtr->SyncCounter--;
            StatePtr->Status = E2E_P02STATUS_SYNC;
        }
        else
        {
            StatePtr->Status = status;
        }
    }
    else
    {
        StatePtr->NoNewOrRepeatedDataCounter = 0u;
        StatePtr->SyncCounter                = ConfigPtr->SyncCounterInit;
        StatePtr->Status                     = E2E_P02STATUS_SYNC;
    }
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
