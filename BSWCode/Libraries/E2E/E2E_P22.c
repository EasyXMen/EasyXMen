/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : E2E_P22.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION> <DATE> <AUTHOR> <REVISION LOG>
 * V2.0.0 [2020/8/17] [YangBo] Initial Vertion.
 * V2.0.1 [2020/9/15] [YangBo] QAC TEST.
 * V2.0.2 [2020/10/29] [YangBo] fix the NULL_PTR input.
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "E2E_P22.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/* PRQA S 3432,4491,1503++ */ /* MISRA Rule 20.7, Rule 10.6,Rule 2.1 */
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 22.
 * This includes checksum calculation, handling of counter and Data ID.
 * Service ID: 0x21
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): ConfigPtr,Pointer to static configuration.
 *                 Length,Length of the data in bytes.
 * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
 *                    DataPtr,Pointer to Data to be transmitted.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE)
E2E_P22Protect(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length)
{
    Std_ReturnType Ret;
    uint8 Counter;
    uint16 Offset;
    uint8 ComputedCRC;

    /*Verify inputs of the protect function return*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        Offset = ConfigPtr->Offset / 8u;
        if ((Length == (ConfigPtr->DataLength / 8u)) && (Length >= (Offset + 2u)))
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
        Offset = ConfigPtr->Offset / 8u;
        /*Increment Counter*/
        StatePtr->Counter++;
        StatePtr->Counter %= 16u;
        /*Write Counter*/
        Counter = StatePtr->Counter;
        DataPtr[(ConfigPtr->Offset >> 3u) + 1u] =
            (DataPtr[(ConfigPtr->Offset >> 3u) + 1u] & 0xF0u) + ((StatePtr->Counter));
        /*Compute CRC*/
        if (ConfigPtr->Offset > 0u)
        {
            ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[0], Offset, 0xFFu, TRUE);
            ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[Offset + 1u], Length - Offset - 1u, ComputedCRC, FALSE);
        }
        else
        {
            ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[1], Length - 1u, 0xFFu, TRUE);
        }
        ComputedCRC = Crc_CalculateCRC8H2F(&ConfigPtr->DataIDList[Counter], (uint32)1, ComputedCRC, FALSE);
        /*Write CRC*/
        DataPtr[ConfigPtr->Offset / 8u] = ComputedCRC;
    }

    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

/**
 * Initializes the protection state.
 * Service ID: 0x41
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_OK.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE) E2E_P22ProtectInit(P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = 0;
        Ret = E2E_E_OK;
    }
    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

/**
 * Checks the Data received using the E2E profile 22.
 * This includes CRC calculation, handling of Counter.
 * Service ID: 0x3d
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): ConfigPtr,Pointer to static configuration.
 *                 DataPtr,Pointer to received data.
 *                 Length,Length of the data in bytes.
 * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE)
E2E_P22Check(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length)
{
    Std_ReturnType Ret;
    uint16 Offset = 0;
    uint8 ReceivedCRC;
    uint8 ReceivedCounter;
    uint8 DeltaCounter;
    uint8 ComputedCRC;
    boolean NewDataAvailable;

    /*Verify inputs of the check function*/
    NewDataAvailable = FALSE;
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        if (((DataPtr != NULL_PTR) && (Length != 0u)) || ((NULL_PTR == DataPtr) && (0u == Length)))
        {
            Offset = ConfigPtr->Offset / 8u;
            if (DataPtr != NULL_PTR)
            {
                if ((Length == (ConfigPtr->DataLength / 8u)) && (Length >= (Offset + 2u)))
                {
                    NewDataAvailable = TRUE;
                    Ret = E2E_E_OK;
                }
                else
                {
                    Ret = E2E_E_INPUTERR_WRONG;
                }
            }
            else
            {
                Ret = E2E_E_OK;
            }
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
        if (NULL_PTR == DataPtr)
        {
            StatePtr->Status = E2E_P22STATUS_NONEWDATA;
        }
        else
        {
            /*Read Counter*/
            ReceivedCounter = DataPtr[(ConfigPtr->Offset >> 3u) + 1u] & 0x0Fu;
            /*Read CRC*/
            ReceivedCRC = DataPtr[ConfigPtr->Offset / 8u];
            /*Compute CRC*/
            if (ConfigPtr->Offset > 0u)
            {
                ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[0], Offset, 0xFFu, TRUE);
                ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[Offset + 1u], Length - Offset - 1u, ComputedCRC, FALSE);
            }
            else
            {
                ComputedCRC = Crc_CalculateCRC8H2F(&DataPtr[1], Length - 1u, 0xFFu, TRUE);
            }
            ComputedCRC = Crc_CalculateCRC8H2F(&ConfigPtr->DataIDList[ReceivedCounter], 1, ComputedCRC, FALSE);
        }

        /*Do Checks*/
        if (TRUE == NewDataAvailable)
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
                    DeltaCounter = 0xFu + ReceivedCounter - StatePtr->Counter + 1u;
                }
                if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
                {
                    if (DeltaCounter > 0u)
                    {
                        if (1u == DeltaCounter)
                        {
                            StatePtr->Status = E2E_P22STATUS_OK;
                        }
                        else
                        {
                            StatePtr->Status = E2E_P22STATUS_OKSOMELOST;
                        }
                    }
                    if (0u == DeltaCounter)
                    {
                        StatePtr->Status = E2E_P22STATUS_REPEATED;
                    }
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
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

/**
 * Initializes the check state
 * Service ID: 0x3e
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): State,Pointer to port/data communication state.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_OK.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE) E2E_P22CheckInit(P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = 0xF;
        StatePtr->Status = E2E_P22STATUS_ERROR;
        Ret = E2E_E_OK;
    }
    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/**
 * The function maps the check status of Profile 22 to a generic check status, which can be used by E2E state machine
 * check function. The E2E Profile 22 delivers a more fine-granular status, but this is not relevant for the E2E state
 * machine. Service ID: 0x3a Sync/Async: synchronous Reentrancy: Reentrant Parameters(IN): CheckReturn,Return value of
 * the E2E_P22Check function Status,Status determined by E2E_P22Check function Parameters(INOUT): NA Parameters(OUT): NA
 * Return value: E2E_PCheckStatusType,Profile-independent status of the
 * reception on one single Data in one cycle.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P22MapStatusToSM(Std_ReturnType CheckReturn, E2E_P22CheckStatusType Status)
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
/* PRQA S 3432,4491,1503 -- */ /* MISRA Rule 20.7, Rule 10.6,Rule 2.1 */
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
