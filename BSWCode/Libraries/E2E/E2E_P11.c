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
**  FILENAME    : E2E_P11.c                                                   **
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
 * V2.0.1 [2020/9/15] [YangBo] QAC FIX.
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "E2E_P11.h"
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
/* PRQA S 3432,4491,1503 ++ */ /* MISRA Rule 20.7, Rule 10.6 ,Rule 2.1*/
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
static void E2E_P11Check_Seqence(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    uint8 ReceivedCounter);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
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

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 7.
 * This includes checksum calculation, handling of counter and Data ID.
 * Service ID: 0x3b
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
E2E_P11Protect(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length)
{
    Std_ReturnType Ret;
    uint16 Offset;
    uint8 ComputedCRC = 0;
    uint8 DataID_Hbyte;
    uint8 DataID_Lbyte;
    uint8 CRC_Data = 0;

    /*Verify inputs of the protect function return*/
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if ((Length == (ConfigPtr->DataLength / 8u)))
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
        DataID_Hbyte = (uint8)(ConfigPtr->DataID >> 8u);
        /*Write DataIDNibble*/
        if (E2E_P11_DATAID_NIBBLE == ConfigPtr->DataIDMode)
        {
            DataPtr[ConfigPtr->DataIDNibbleOffset >> 3u] =
                (DataPtr[ConfigPtr->DataIDNibbleOffset >> 3u]
                 & (0xFu << (ConfigPtr->CounterOffset & 0x7u))) /* Reserve lower 4 bit */
                + (uint8)(((ConfigPtr->DataID & 0x0F00u) >> 8u) << (ConfigPtr->DataIDNibbleOffset & 0x7u));
            /*Write Counter*/
            DataPtr[ConfigPtr->CounterOffset >> 3u] =
                ((DataPtr[ConfigPtr->CounterOffset >> 3u])
                 & (0xFu << (ConfigPtr->DataIDNibbleOffset & 0x7u))) /* Reserve higher 4 bit */
                + ((StatePtr->Counter & 0xFu) << (ConfigPtr->CounterOffset & 0x7u));
        }
        else
        {
            /*Write Counter*/
            DataPtr[ConfigPtr->CounterOffset >> 3u] =
                (DataPtr[ConfigPtr->CounterOffset >> 3u]
                 & (0xF0u << (ConfigPtr->CounterOffset & 0x7u))) /* Reserve higher 4 bit */
                + ((StatePtr->Counter & 0xFu) << (ConfigPtr->CounterOffset & 0x7u));
        }
        /*Compute CRC*/
        Offset = ConfigPtr->CRCOffset / 8u;
        if (E2E_P11_DATAID_BOTH == ConfigPtr->DataIDMode)
        {
            ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1, 0xFFu, FALSE);
            ComputedCRC = Crc_CalculateCRC8(&DataID_Hbyte, 1, ComputedCRC, FALSE);
        }
        else if (E2E_P11_DATAID_NIBBLE == ConfigPtr->DataIDMode)
        {
            ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1, 0xFFu, FALSE);
            ComputedCRC = Crc_CalculateCRC8(&CRC_Data, 1, ComputedCRC, FALSE);
        }
        else
        {
            /* do nothing */
        }
        if (Offset > 0u)
        {
            ComputedCRC = Crc_CalculateCRC8(&DataPtr[0], Offset, ComputedCRC, FALSE);
            if (Length > (Offset + 1u))
            {
                ComputedCRC = Crc_CalculateCRC8(&DataPtr[Offset + 1u], Length - Offset - 1u, ComputedCRC, FALSE);
            }
        }
        else
        {
            ComputedCRC = Crc_CalculateCRC8(&DataPtr[1], Length - 1u, ComputedCRC, FALSE);
        }
        /*Write CRC*/
        DataPtr[ConfigPtr->CRCOffset / 8u] = ComputedCRC;
        /*Increment Counter*/
        StatePtr->Counter++;
        StatePtr->Counter %= 15u;
    }

    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

/**
 * Initializes the protection state.
 * Service ID: 0x3c
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG
 * E2E_E_INTERR E2E_E_OK.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE) E2E_P11ProtectInit(P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
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
 * Checks the Data received using the E2E profile 11.
 * This includes CRC calculation, handling of Counter.
 * Service ID: 0x38
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
E2E_P11Check(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length)
{
    Std_ReturnType Ret;
    boolean NewDataAvailable;
    uint8 ReceivedNibble;
    uint8 ReceivedCounter;
    uint8 ReceivedCRC;
    uint16 Offset;
    uint8 ComputedCRC = 0;
    uint8 DataID_Lbyte;
    uint8 DataID_Hbyte;
    uint8 CRC_Data = 0;

    /*Verify inputs of the check function*/
    NewDataAvailable = FALSE;
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        if (((DataPtr != NULL_PTR) && (Length != 0u)) || ((NULL_PTR == DataPtr) && (0u == Length)))
        {
            if (DataPtr != NULL_PTR)
            {
                if (Length == ((ConfigPtr->DataLength) / 8u))
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
            StatePtr->Status = E2E_P11STATUS_NONEWDATA;
        }
        else
        {
            DataID_Lbyte = (uint8)(ConfigPtr->DataID);
            DataID_Hbyte = (uint8)(ConfigPtr->DataID >> 8u);
            /*Read DataIDNibble*/
            ReceivedNibble =
                (DataPtr[(ConfigPtr->DataIDNibbleOffset >> 3u)] >> (ConfigPtr->DataIDNibbleOffset & 0x7u)) & 0xFu;
            /*Read Counter*/
            ReceivedCounter = (DataPtr[(ConfigPtr->CounterOffset >> 3u)] >> (ConfigPtr->CounterOffset & 0x7u)) & 0xFu;
            if (ReceivedCounter >= 15u)
            {
                Ret = E2E_E_INPUTERR_WRONG;
                return Ret;
            }
            /*Read CRC*/
            ReceivedCRC = DataPtr[(ConfigPtr->CRCOffset) / 8u];
            /*Compute CRC*/
            Offset = ConfigPtr->CRCOffset / 8u;
            if (E2E_P11_DATAID_BOTH == ConfigPtr->DataIDMode)
            {
                ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1, 0xFFu, FALSE);
                ComputedCRC = Crc_CalculateCRC8(&DataID_Hbyte, 1, ComputedCRC, FALSE);
            }
            else if (E2E_P11_DATAID_NIBBLE == ConfigPtr->DataIDMode)
            {
                ComputedCRC = Crc_CalculateCRC8(&DataID_Lbyte, 1, 0xFFu, FALSE);
                ComputedCRC = Crc_CalculateCRC8(&CRC_Data, 1, ComputedCRC, FALSE);
            }
            else
            {
                /* do nothing */
            }
            if (Offset > 0u)
            {
                ComputedCRC = Crc_CalculateCRC8(&DataPtr[0], Offset, ComputedCRC, FALSE);
                if (Length > (Offset + 1u))
                {
                    ComputedCRC = Crc_CalculateCRC8(&DataPtr[Offset + 1u], Length - Offset - 1u, ComputedCRC, FALSE);
                }
            }
            else
            {
                ComputedCRC = Crc_CalculateCRC8(&DataPtr[1], Length - 1u, ComputedCRC, FALSE);
            }
        }
        /*Do Checks*/
        if (TRUE == NewDataAvailable)
        {
            if (ReceivedCRC == ComputedCRC)
            {
                if (((E2E_P11_DATAID_NIBBLE == ConfigPtr->DataIDMode)
                     && (ReceivedNibble == ((uint8)(ConfigPtr->DataID >> 8u) & 0x0Fu)))
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
        else
        {
            StatePtr->Status = E2E_P11STATUS_NONEWDATA;
        }
    }

    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/**
 * Initializes the check state
 * Service ID: 0x24
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P11CheckInit(P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType Ret;

    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = 0xEu;
        StatePtr->Status = E2E_P11STATUS_ERROR;
        Ret = E2E_E_OK;
    }
    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/**
 * The function maps the check status of Profile 11 to a generic check status, which can be used by E2E state machine
 * check function. The E2E Profile 11 delivers a more fine-granular status, but this is not relevant for the E2E state
 * machine. Service ID: 0x3a Sync/Async: synchronous Reentrancy: Reentrant Parameters(IN): CheckReturn,Return value of
 * the E2E_P11Check function Status,Status determined by E2E_P11Check function Parameters(INOUT): NA Parameters(OUT): NA
 * Return value: E2E_PCheckStatusType,Profile-independent status of the
 * reception on one single Data in one cycle.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P11MapStatusToSM(Std_ReturnType CheckReturn, E2E_P11CheckStatusType Status)
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
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
static void E2E_P11Check_Seqence(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    uint8 ReceivedCounter)
{
    uint8 DeltaCounter;

    /*taking into wrap around 0xE*/
    if (ReceivedCounter >= StatePtr->Counter)
    {
        DeltaCounter = ReceivedCounter - StatePtr->Counter;
    }
    else
    {
        DeltaCounter = 0xEu + ReceivedCounter - StatePtr->Counter + 1u;
    }
    if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
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
/* PRQA S 3432,4491,1503 -- */ /* MISRA Rule 20.7, Rule 10.6 ,Rule 2.1*/
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
