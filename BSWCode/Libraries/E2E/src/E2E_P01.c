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
#include "E2E.h"
#include "E2E_P01.h"

/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P01_MAX_COUNTER_VALUE 0xEu
#define E2E_P01_CRC_INITVALUE     0xFFu
#define E2E_P01_CRC_XORVALUE      0xFFu
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/**
 * @brief        check the legitimacy of DataID mode
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @return       Std_ReturnType
 * @retval       E_OK     : DataID mode is legal
 * @retval       E_NOT_OK : DataID mode is  not legal
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL Std_ReturnType E2E_P01CheckDataIDMode(const E2E_P01ConfigType* ConfigPtr);
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
E2E_LOCAL Std_ReturnType E2E_P01ProtectVerifyInputs(
    const E2E_P01ConfigType*       ConfigPtr,
    const E2E_P01ProtectStateType* StatePtr,
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
E2E_LOCAL Std_ReturnType E2E_P01CheckVerifyInputs(
    const E2E_P01ConfigType*     ConfigPtr,
    const E2E_P01CheckStateType* StatePtr,
    const uint8*                 DataPtr);
/**
 * @brief        verify inputs of protect()
 * @param[in]    ConfigPtr : Pointer to static configuration.
 * @param[in]    Counter   : Counter to be used for protecting the next Data.
 * @param[in]    DataPtr   : Pointer to Data to be transmitted.
 * @return       uint8
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL uint8 E2E_P01CalculateCRC8(const E2E_P01ConfigType* ConfigPtr, uint8 Counter, const uint8* DataPtr);
/**
 * @brief        check the status depending on Counter.
 * @param[in]    Config          : Pointer to static configuration.
 * @param[inout] State           : Pointer to port/data communication state.
 * @param[in]    ReceivedCounter : the counter of current data.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL void
    E2E_P01Check_Seqence(const E2E_P01ConfigType* Config, E2E_P01CheckStateType* State, uint8 ReceivedCounter);
/**
 * @brief        Caculate the status to OK, OKSOMELOST, SYNC
 * @param[in]    ConfigPtr       : Pointer to static configuration.
 * @param[in]    status          : the next status.
 * @param[inout] StatePtr        : Pointer to port/data communication state.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL void E2E_P01AddStatus(
    E2E_P01CheckStateType*   StatePtr,
    const E2E_P01ConfigType* ConfigPtr,
    E2E_P01CheckStatusType   status);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 1.
 */
Std_ReturnType E2E_P01Protect(const E2E_P01ConfigType* ConfigPtr, E2E_P01ProtectStateType* StatePtr, uint8* DataPtr)
{
    uint8 CRC;
    uint8 Ret = E2E_P01ProtectVerifyInputs(ConfigPtr, StatePtr, DataPtr);

    if (E2E_E_OK == Ret)
    {
        /*Write the counter in the Data, at the configured CounterOffset. The counter goes either into low
        nibble (left branch) or high nibble (right branch) of Data. Note that the nibble next to Counter
        may be used by application.*/
        if (0uL == (ConfigPtr->CounterOffset % E2E_BIT_LENGTH))
        {
            /*PRQA S 2985 ++ */ /*VL_E2E_Redundant*/
            DataPtr[ConfigPtr->CounterOffset / (uint16)E2E_BIT_LENGTH] =
                (DataPtr[ConfigPtr->CounterOffset / E2E_BIT_LENGTH] & E2E_MASK_HIGH_NIBBLE)
                | (StatePtr->Counter & (uint8)E2E_MASK_LOW_NIBBLE);
            /*PRQA S 2985 -- */
        }
        else
        {
            /*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
            DataPtr[ConfigPtr->CounterOffset / (uint16)E2E_BIT_LENGTH] =
                (DataPtr[ConfigPtr->CounterOffset / E2E_BIT_LENGTH] & (uint8)E2E_MASK_LOW_NIBBLE)
                | ((uint8)(StatePtr->Counter << 4u));
            /*PRQA S 3120 -- */
        }
        /*Write the low nibble of high byte of Data ID - only for E2E_P01_DATAID_NIBBLE configuration.*/
        if (E2E_P01_DATAID_NIBBLE == ConfigPtr->DataIDMode)
        {
            if (0uL == ((ConfigPtr->DataIDNibbleOffset) % E2E_BIT_LENGTH))
            {
                DataPtr[ConfigPtr->DataIDNibbleOffset / E2E_BIT_LENGTH] =
                    (DataPtr[ConfigPtr->DataIDNibbleOffset / (uint16)E2E_BIT_LENGTH] & (uint8)E2E_MASK_HIGH_NIBBLE)
                    | ((uint8)(ConfigPtr->DataID >> E2E_BIT_LENGTH) & (uint8)E2E_MASK_LOW_NIBBLE);
            }
            else
            {
                /*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
                DataPtr[ConfigPtr->DataIDNibbleOffset / (uint16)E2E_BIT_LENGTH] =
                    (DataPtr[ConfigPtr->DataIDNibbleOffset / E2E_BIT_LENGTH] & (uint8)E2E_MASK_LOW_NIBBLE)
                    | ((uint8)(ConfigPtr->DataID >> 4u) & (uint8)E2E_MASK_HIGH_NIBBLE);
                /*PRQA S 3120 -- */
            }
        }

        CRC = E2E_P01CalculateCRC8(ConfigPtr, StatePtr->Counter, DataPtr);
        /*CRC is written to the Data at configured location.*/
        DataPtr[ConfigPtr->CRCOffset / E2E_BIT_LENGTH] = CRC;
        StatePtr->Counter                              = (StatePtr->Counter + 1u) % (E2E_P01_MAX_COUNTER_VALUE + 1u);
    }
    return Ret;
}
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P01ProtectInit(E2E_P01ProtectStateType* StatePtr)
{
    Std_ReturnType Ret;
    Ret = E2E_E_OK;

    /*@SWS_E2E_00386*/
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
 * Checks the Data received using the E2E profile 1.
 */
/*PRQA S 2889,6060 ++ */ /*VL_QAC_MultiReturn,VL_MTR_E2E_STM19*/
Std_ReturnType E2E_P01Check(
    /*PRQA S 2889,6060 -- */
    const E2E_P01ConfigType* Config,
    E2E_P01CheckStateType*   State,
    const uint8*             Data)
{
    uint8          ReceivedCounter;
    uint8          ReceivedCRC;
    uint8          ReceivedDataIDNibble;
    uint8          CalculatedCRC;
    Std_ReturnType Ret = E2E_P01CheckVerifyInputs(Config, State, Data);

    if (E2E_E_OK != Ret)
    {
        return Ret;
    }

    State->MaxDeltaCounter = E2E_P01_Min(((State->MaxDeltaCounter) + 1u), (E2E_P01_MAX_COUNTER_VALUE));

    if (State->NewDataAvailable)
    {
        /*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
        ReceivedCounter = (0uL == (Config->CounterOffset % E2E_BIT_LENGTH))
                              ? (Data[Config->CounterOffset / E2E_BIT_LENGTH] & (uint8)E2E_MASK_LOW_NIBBLE)
                              : (Data[Config->CounterOffset / E2E_BIT_LENGTH] >> 4u);
        /*PRQA S 3120 -- */

        if (ReceivedCounter >= (E2E_P01_MAX_COUNTER_VALUE + 1u))
        {
            Ret = E2E_E_INPUTERR_WRONG;
            return Ret;
        }
    }
    else
    {
        if (State->NoNewOrRepeatedDataCounter < E2E_P01_MAX_COUNTER_VALUE)
        {
            State->NoNewOrRepeatedDataCounter++;
        }
        State->Status = E2E_P01STATUS_NONEWDATA;
        return Ret;
    }

    ReceivedCRC = Data[Config->CRCOffset / E2E_BIT_LENGTH];

    if (E2E_P01_DATAID_NIBBLE == Config->DataIDMode)
    {
        /*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/
        ReceivedDataIDNibble = (0uL == (Config->DataIDNibbleOffset % E2E_BIT_LENGTH))
                                   ? (Data[Config->DataIDNibbleOffset / E2E_BIT_LENGTH] & (uint8)E2E_MASK_LOW_NIBBLE)
                                   : (Data[Config->DataIDNibbleOffset / E2E_BIT_LENGTH] >> 4u);
        /*PRQA S 3120 -- */
    }
    CalculatedCRC = E2E_P01CalculateCRC8(Config, ReceivedCounter, Data);

    if (ReceivedCRC == CalculatedCRC)
    {
        if ((E2E_P01_DATAID_NIBBLE == Config->DataIDMode)
            && (ReceivedDataIDNibble != (uint8)((Config->DataID >> E2E_BIT_LENGTH) & (uint16)E2E_MASK_LOW_NIBBLE)))
        {
            State->Status = E2E_P01STATUS_WRONGCRC;
        }
        else
        {
            /*Check if any correct data has already be received.*/
            if (State->WaitForFirstData)
            {
                State->WaitForFirstData = FALSE;
                State->MaxDeltaCounter  = Config->MaxDeltaCounterInit;
                State->LastValidCounter = ReceivedCounter;
                State->Status           = E2E_P01STATUS_INITIAL;
            }
            else
            {
                E2E_P01Check_Seqence(Config, State, ReceivedCounter);
            }
        }
    }
    else
    {
        State->Status = E2E_P01STATUS_WRONGCRC;
    }
    return Ret;
}
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P01CheckInit(E2E_P01CheckStateType* State)
{
    Std_ReturnType Ret;

    Ret = E2E_E_OK;
    /*@SWS_E2E_00389*/
    if (NULL_PTR == State)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        State->LastValidCounter           = 0u;
        State->MaxDeltaCounter            = 0u;
        State->WaitForFirstData           = TRUE;
        State->NewDataAvailable           = TRUE;
        State->LostData                   = 0u;
        State->Status                     = E2E_P01STATUS_NONEWDATA;
        State->NoNewOrRepeatedDataCounter = 0u;
        State->SyncCounter                = 0u;
    }
    return Ret;
}
/**
 * The function maps the check status of Profile 1 to a generic check status.
 */
E2E_PCheckStatusType
    E2E_P01MapStatusToSM(Std_ReturnType CheckReturn, E2E_P01CheckStatusType Status, boolean ProfileBehavior)
{
    E2E_PCheckStatusType Ret;

    /*@SWS_E2E_00384*/
    if (E2E_E_OK != CheckReturn)
    {
        Ret = E2E_P_ERROR;
    }
    /*@SWS_E2E_00383*/
    else
    {
        switch (Status)
        {
        case E2E_P01STATUS_OK:
        case E2E_P01STATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P01STATUS_SYNC: /*@SWS_E2E_00476*/
            Ret = ProfileBehavior ? E2E_P_OK : E2E_P_WRONGSEQUENCE;
            break;
        case E2E_P01STATUS_WRONGCRC:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P01STATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P01STATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P01STATUS_WRONGSEQUENCE:
            Ret = E2E_P_WRONGSEQUENCE;
            break;
        case E2E_P01STATUS_INITIAL: /*@SWS_E2E_00476*/
            Ret = ProfileBehavior ? E2E_P_WRONGSEQUENCE : E2E_P_OK;
            break;
        default:
            Ret = E2E_P_ERROR;
            break;
        }
    }
    return Ret;
}
/*PRQA S 1532 -- */
/* ========================================== internal function definitions ========================================= */
E2E_LOCAL Std_ReturnType E2E_P01CheckDataIDMode(const E2E_P01ConfigType* ConfigPtr)
{
    Std_ReturnType ret;

    if ((E2E_P01_DATAID_ALT == ConfigPtr->DataIDMode) || (E2E_P01_DATAID_BOTH == ConfigPtr->DataIDMode)
        || (E2E_P01_DATAID_LOW == ConfigPtr->DataIDMode) || (E2E_P01_DATAID_NIBBLE == ConfigPtr->DataIDMode)
        || (E2E_P01_DATAID_NOUSED == ConfigPtr->DataIDMode)) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        ret = E_OK;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}
E2E_LOCAL Std_ReturnType E2E_P01ProtectVerifyInputs(
    const E2E_P01ConfigType*       ConfigPtr,
    const E2E_P01ProtectStateType* StatePtr,
    const uint8*                   DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* Check input pointer parameter for NULL_PTR */
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        /* Check if value of Counter is within valid range */
        if (StatePtr->Counter > E2E_P01_MAX_COUNTER_VALUE)
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else if (E_NOT_OK == E2E_P01CheckDataIDMode(ConfigPtr))
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
E2E_LOCAL Std_ReturnType E2E_P01CheckVerifyInputs(
    const E2E_P01ConfigType*     ConfigPtr,
    const E2E_P01CheckStateType* StatePtr,
    const uint8*                 DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* Check input parameter for NULL_PTR */
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        /* Check if counter values are within valid range */
        if ((StatePtr->LastValidCounter > E2E_P01_MAX_COUNTER_VALUE)
            || (StatePtr->MaxDeltaCounter > E2E_P01_MAX_COUNTER_VALUE)
            || (StatePtr->SyncCounter > ConfigPtr->SyncCounterInit)
            || (StatePtr->NoNewOrRepeatedDataCounter > E2E_P01_MAX_COUNTER_VALUE))
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else if (E_NOT_OK == E2E_P01CheckDataIDMode(ConfigPtr))
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
E2E_LOCAL uint8 E2E_P01CalculateCRC8(const E2E_P01ConfigType* ConfigPtr, uint8 Counter, const uint8* DataPtr)
{
    uint8 CRC = 0u;
    uint8 DataID_Hbyte;
    uint8 DataID_Lbyte;
    uint8 Data_0 = 0u;

    DataID_Hbyte = (uint8)((ConfigPtr->DataID) >> E2E_BIT_LENGTH); /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    DataID_Lbyte = (uint8)(ConfigPtr->DataID);
    /*Compute CRC over DataID, depending on DataIDMode setting.The first invocation of Crc_CalculateCRC8() is done with
      start value 0xFF.The CalculateCRC8() is XORing the start value provided by the caller (equal 0xFF) with 0xFF,
      resulting with actual internal start value equal to 0x00.*/
    switch (ConfigPtr->DataIDMode)
    {
    case E2E_P01_DATAID_BOTH:
        CRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P01_CRC_INITVALUE, FALSE);
        CRC = Crc_CalculateCRC8(&DataID_Hbyte, 1u, CRC, FALSE);
        break;
    case E2E_P01_DATAID_LOW:
        CRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P01_CRC_INITVALUE, FALSE);
        break;
    case E2E_P01_DATAID_ALT:
        if (0u == (Counter % 2u)) /*PRQA S 3120*/ /*VL_QAC_MagicNum*/
        {
            CRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P01_CRC_INITVALUE, FALSE);
        }
        else
        {
            CRC = Crc_CalculateCRC8(&DataID_Hbyte, 1u, E2E_P01_CRC_INITVALUE, FALSE);
        }
        break;
    case E2E_P01_DATAID_NIBBLE:
        CRC = Crc_CalculateCRC8(&DataID_Lbyte, 1u, E2E_P01_CRC_INITVALUE, FALSE);
        CRC = Crc_CalculateCRC8(&Data_0, 1u, CRC, FALSE);
        break;
    default:
        /*nothing to do*/
        break;
    }
    /*Compute CRC over the area before the CRC (if CRC is not the first byte)*/
    if ((ConfigPtr->CRCOffset >= E2E_BIT_LENGTH) && (E2E_P01_DATAID_NOUSED != ConfigPtr->DataIDMode))
    {
        CRC = Crc_CalculateCRC8(DataPtr, ((uint32)ConfigPtr->CRCOffset / E2E_BIT_LENGTH), CRC, FALSE);
    }
    else if ((0u == ConfigPtr->CRCOffset) && (E2E_P01_DATAID_NOUSED != ConfigPtr->DataIDMode))
    {
        /*nothing to do*/
    }
    else
    {
        CRC = Crc_CalculateCRC8(DataPtr, ((uint32)ConfigPtr->CRCOffset / E2E_BIT_LENGTH), E2E_P01_CRC_INITVALUE, FALSE);
    }
    /*Compute the area after CRC, if CRC is not the last byte. Start with the byte after CRC,
      finish with the last byte of Data.*/
    if ((ConfigPtr->CRCOffset / (uint16)E2E_BIT_LENGTH) < ((ConfigPtr->DataLength / (uint16)E2E_BIT_LENGTH) - 1u))
    {
        /*PRQA S 2824 ++ */ /*VL_E2E_NULLPointer*/
        CRC = Crc_CalculateCRC8(
            &DataPtr[((ConfigPtr->CRCOffset / (uint16)E2E_BIT_LENGTH) + 1u)],
            (((uint32)(ConfigPtr->DataLength) / E2E_BIT_LENGTH) - ((uint32)ConfigPtr->CRCOffset / E2E_BIT_LENGTH)
             - 1uL),
            CRC,
            FALSE);
        /*PRQA S 2824 -- */
    }
    CRC = CRC ^ E2E_P01_CRC_XORVALUE;

    return CRC;
}
E2E_LOCAL void
    E2E_P01Check_Seqence(const E2E_P01ConfigType* Config, E2E_P01CheckStateType* State, uint8 ReceivedCounter)
{
    uint8 DeltaCounter;

    /*Compute the delta, taking into account the overflow.*/
    if (ReceivedCounter >= State->LastValidCounter) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        DeltaCounter = ReceivedCounter - State->LastValidCounter;
    }
    else
    {
        DeltaCounter = E2E_P01_MAX_COUNTER_VALUE + 1U + ReceivedCounter - State->LastValidCounter;
    }
    /*The previous and the current data have correct CRC, verify the counter.*/
    if (0u == DeltaCounter)
    {
        if (State->NoNewOrRepeatedDataCounter < E2E_P01_MAX_COUNTER_VALUE)
        {
            State->NoNewOrRepeatedDataCounter++;
        }
        State->Status = E2E_P01STATUS_REPEATED;
    }
    else if (1u == DeltaCounter)
    {
        State->MaxDeltaCounter  = Config->MaxDeltaCounterInit; /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
        State->LastValidCounter = ReceivedCounter;
        State->LostData         = 0u;
        E2E_P01AddStatus(State, Config, E2E_P01STATUS_OK);
    }
    else if ((DeltaCounter <= State->MaxDeltaCounter))
    {
        State->MaxDeltaCounter  = Config->MaxDeltaCounterInit;
        State->LastValidCounter = ReceivedCounter;
        State->LostData         = (DeltaCounter - 1u);
        E2E_P01AddStatus(State, Config, E2E_P01STATUS_OKSOMELOST);
    }
    else
    {
        State->NoNewOrRepeatedDataCounter = 0u;
        State->SyncCounter                = Config->SyncCounterInit;
        if (State->SyncCounter > 0u)
        {
            State->MaxDeltaCounter  = Config->MaxDeltaCounterInit;
            State->LastValidCounter = ReceivedCounter;
        }
        State->Status = E2E_P01STATUS_WRONGSEQUENCE;
    }
}
E2E_LOCAL void
    E2E_P01AddStatus(E2E_P01CheckStateType* StatePtr, const E2E_P01ConfigType* ConfigPtr, E2E_P01CheckStatusType status)
{
    if (StatePtr->NoNewOrRepeatedDataCounter <= ConfigPtr->MaxNoNewOrRepeatedData)
    {
        StatePtr->NoNewOrRepeatedDataCounter = 0u;
        if (StatePtr->SyncCounter > 0u)
        {
            StatePtr->SyncCounter--;
            StatePtr->Status = E2E_P01STATUS_SYNC;
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
        StatePtr->Status                     = E2E_P01STATUS_SYNC;
    }
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
