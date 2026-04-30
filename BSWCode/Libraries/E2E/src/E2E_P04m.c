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
**  @file               : E2E_P04m.c
**  @author             : QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E_P04m.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
#define E2E_P04M_CRC_INITVALUE     0xFFFFFFFFu
#define E2E_P04M_MAX_COUNTER_VALUE 0xFFFFu
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
/**
 * @brief        check the status depending on Counter.
 * @param[in]    ConfigPtr       : Pointer to static configuration.
 * @param[in]    ReceivedCounter : the counter of current data.
 * @param[inout] StatePtr        : Pointer to port/data communication state.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
E2E_LOCAL void E2E_P04mCheck_Seqence(
    const E2E_P04mConfigType* ConfigPtr,
    E2E_P04mCheckStateType*   StatePtr,
    uint16                    ReceivedCounter);
/**
 * @brief        verify inputs of check()
 * @param[in]    ConfigPtr     : Pointer to static configuration.
 * @param[in]    SourceID      : A system-unique identifier of the Data Source.
 * @param[in]    MessageType   : Type of the message (request/response)
 * @param[in]    MessageResult : Result of the message (OK/ERROR)
 * @param[in]    Length        : Length of the data in bytes.
 * @param[in]    DataPtr       : Pointer to Data to be transmitted.
 * @param[inout] StatePtr      : Pointer to received data.
 * @return       Std_ReturnType
 * @retval       E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval       E2E_E_OK             : Function completed successfully
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
/*PRQA S 6040 ++ */ /*VL_MTR_E2E_STPAR*/
E2E_LOCAL Std_ReturnType E2E_P04mCheckVerifyInputs(
    /*PRQA S 6040 -- */
    const E2E_P04mConfigType*     ConfigPtr,
    const E2E_P04mCheckStateType* StatePtr,
    Std_MessageTypeType           MessageType,
    Std_MessageResultType         MessageResult,
    const uint8*                  DataPtr,
    uint16                        Length);
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/ /*PRQA S 1503 ++ */ /*VL_QAC_NoUsedApi*/
/**
 * Protects the array/buffer to be transmitted using the E2E profile 4m.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/ /*PRQA S 6040 ++ */ /*VL_MTR_E2E_STPAR*/
Std_ReturnType E2E_P04mProtect(
    /*PRQA S 6040 -- */
    const E2E_P04mConfigType* ConfigPtr,
    E2E_P04mProtectStateType* StatePtr,
    uint32                    SourceID,
    Std_MessageTypeType       MessageType,
    Std_MessageResultType     MessageResult,
    uint8*                    DataPtr,
    uint16                    Length)
{
    Std_ReturnType Ret;
    uint16         Offset;
    uint32         ComputedCRC;

    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
    {
        if ((Length >= (ConfigPtr->MinDataLength / 8u)) && (Length <= (ConfigPtr->MaxDataLength / 8u))
            && (SourceID <= E2E_SOURCEID_MAX)
            && ((MessageType == STD_MESSAGETYPE_RESPONSE) || (MessageType == STD_MESSAGETYPE_REQUEST))
            && ((MessageResult == STD_MESSAGERESULT_ERROR) || (MessageResult == STD_MESSAGERESULT_OK)))
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

    /*Verify inputs of the protect function*/
    if (E2E_E_OK == Ret)
    {
        /*compute local variable uint16 Offset, which is in [byte]*/
        Offset = ConfigPtr->Offset / 8u;
        /*Copy 2-byte Length on bytes Data[Offset...Offset+1] in Big Endian order*/
        DataPtr[Offset]      = (uint8)(Length >> 8u);
        DataPtr[Offset + 1u] = (uint8)Length;
        /*Copy 2-byte State->Counter on bytes Data[Offset+2...Offset+3] in Big Endian order*/
        DataPtr[Offset + 2u] = (uint8)(StatePtr->Counter >> 8u);
        DataPtr[Offset + 3u] = (uint8)(StatePtr->Counter);
        /*Copy 4-byte Config->DataID to bytes Data[Offset+4...Offset+7] in Big Endian order*/
        DataPtr[Offset + 4u] = (uint8)(ConfigPtr->DataID >> 24u);
        DataPtr[Offset + 5u] = (uint8)(ConfigPtr->DataID >> 16u);
        DataPtr[Offset + 6u] = (uint8)(ConfigPtr->DataID >> 8u);
        DataPtr[Offset + 7u] = (uint8)(ConfigPtr->DataID);
        /*Copy 28-bit SourceID to bytes Data[offset+12...offset+15]*/
        DataPtr[Offset + 12u] = (uint8)(SourceID >> 24u);
        DataPtr[Offset + 13u] = (uint8)(SourceID >> 16u);
        DataPtr[Offset + 14u] = (uint8)(SourceID >> 8u);
        DataPtr[Offset + 15u] = (uint8)(SourceID);
        /*Copy MessageType and MessageResult to Data[offset+12]*/
        DataPtr[Offset + 12u] = (DataPtr[Offset + 12u] & (uint8)E2E_MASK_LOW_NIBBLE) | (uint8)((uint8)MessageType << 6u)
                                | (uint8)((uint8)MessageResult << 4u);
        /*compute CRC*/
        ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[0], ((uint32)Offset + 8uL), E2E_P04M_CRC_INITVALUE, TRUE);
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

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the protection state.
 */
Std_ReturnType E2E_P04mProtectInit(E2E_P04mProtectStateType* StatePtr)
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
 * Checks the Data received using the E2E profile 4m for client.
 */
/*PRQA S 3120 ++ */ /*VL_QAC_MagicNum*/ /*PRQA S 6040 ++ */ /*VL_MTR_E2E_STPAR*/
Std_ReturnType E2E_P04mSourceCheck(
    /*PRQA S 6040 -- */
    const E2E_P04mConfigType* ConfigPtr,
    E2E_P04mCheckStateType*   StatePtr,
    uint32                    SourceID,
    Std_MessageTypeType       MessageType,
    Std_MessageResultType     MessageResult,
    const uint8*              DataPtr,
    uint16                    Length)
{
    Std_ReturnType        Ret;
    uint16                Offset;
    uint16                ReceivedLength;
    uint16                ReceivedCounter;
    uint32                ReceivedDataID;
    uint32                ReceivedSourceID;
    Std_MessageTypeType   ReceivedMsgType;
    Std_MessageResultType ReceivedMsgResult;
    uint32                ReceivedCRC;
    uint32                ComputedCRC;

    Ret = E2E_P04mCheckVerifyInputs(ConfigPtr, StatePtr, MessageType, MessageResult, DataPtr, Length);

    if (E2E_E_OK == Ret)
    {
        if ((NULL_PTR != DataPtr))
        {
            /*compute local variable uint16 Offset, which is in [byte]*/
            Offset = ConfigPtr->Offset / (uint16)E2E_BIT_LENGTH;
            /*Copy bytes Data[Offset...Offset+1] in Big Endian order to uint16 local variable ReceivedLength*/
            ReceivedLength = (uint16)((uint16)DataPtr[Offset] << 8u) + (uint16)(DataPtr[Offset + 1u]);
            /*Copy bytes Data[Offset+2...Offset+3] in Big Endian order on uint16 local variable ReceivedCounter*/
            ReceivedCounter = (uint16)((uint16)DataPtr[Offset + 2u] << 8u) + (uint16)(DataPtr[Offset + 3u]);
            /*Copy bytes Data[Offset+4...Offset+7] in Big Endian order on uint32 local variable ReceivedDataID*/
            ReceivedDataID = (uint32)((uint32)DataPtr[Offset + 4u] << 24u)
                             + (uint32)((uint32)DataPtr[Offset + 5u] << 16u)
                             + (uint32)((uint32)DataPtr[Offset + 6u] << 8u) + (uint32)(DataPtr[Offset + 7u]);
            /*Copy Data[offset+12...offset+15] in Big Endian order to SourceID, MessageResult, MessageType*/
            ReceivedSourceID = (((uint32)DataPtr[Offset + 12u] & E2E_MASK_LOW_NIBBLE) << 24u)
                               + ((uint32)DataPtr[Offset + 13u] << 16u) + ((uint32)DataPtr[Offset + 14u] << 8u)
                               + (uint32)(DataPtr[Offset + 15u]);
            ReceivedMsgType =
                ((DataPtr[Offset + 12u] >> 6u) == 0u) ? STD_MESSAGETYPE_REQUEST : STD_MESSAGETYPE_RESPONSE;
            ReceivedMsgResult =
                (((DataPtr[Offset + 12u] >> 4u) & 0x03u) == 0u) ? STD_MESSAGERESULT_OK : STD_MESSAGERESULT_ERROR;
            /*Copy bytes Data[Offset+8...Offset+11] using big Endian order on 4-byte local variable ReceivedCRC*/
            ReceivedCRC = ((uint32)DataPtr[Offset + 8u] << 24u) + ((uint32)DataPtr[Offset + 9u] << 16u)
                          + ((uint32)DataPtr[Offset + 10u] << 8u) + ((uint32)DataPtr[Offset + 11u]);
            /*compute CRC*/
            ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[0], ((uint32)Offset + 8uL), E2E_P04M_CRC_INITVALUE, TRUE);
            if ((Offset + 12u) < Length)
            {
                ComputedCRC = Crc_CalculateCRC32P4(
                    &DataPtr[Offset + 12u],
                    ((uint32)Length - (uint32)Offset - 12uL),
                    ComputedCRC,
                    FALSE);
            }
            /*Do Checks*/
            if ((ReceivedCRC == ComputedCRC) && (ReceivedDataID == ConfigPtr->DataID) && (ReceivedLength == Length))
            {
                if ((ReceivedSourceID == SourceID) && (ReceivedMsgResult == MessageResult)
                    && (ReceivedMsgType == MessageType))
                {
                    E2E_P04mCheck_Seqence(ConfigPtr, StatePtr, ReceivedCounter);
                }
                else
                {
                    StatePtr->Status = E2E_P04MSTATUS_ERROR;
                }
            }
            else
            {
                StatePtr->Status = E2E_P04MSTATUS_ERROR;
            }
        }
        else
        {
            /*Do Checks*/
            StatePtr->Status = E2E_P04MSTATUS_NONEWDATA;
        }
    }

    return Ret;
}
/**
 * Checks the Data received using the E2E profile 4m for server.
 */
/*PRQA S 6040 ++ */ /*VL_MTR_E2E_STPAR*/
Std_ReturnType E2E_P04mSinkCheck(
    /*PRQA S 6040 -- */
    const E2E_P04mConfigType* ConfigPtr,
    E2E_P04mCheckStateType*   StatePtr,
    uint32*                   SourceID,
    Std_MessageTypeType       MessageType,
    Std_MessageResultType     MessageResult,
    const uint8*              DataPtr,
    uint16                    Length)
{
    Std_ReturnType        Ret;
    uint16                Offset;
    uint16                ReceivedLength;
    uint16                ReceivedCounter;
    uint32                ReceivedDataID;
    uint32                ReceivedCRC;
    Std_MessageTypeType   ReceivedMsgType;
    Std_MessageResultType ReceivedMsgResult;
    uint32                ComputedCRC;

    Ret = E2E_P04mCheckVerifyInputs(ConfigPtr, StatePtr, MessageType, MessageResult, DataPtr, Length);
    if (NULL_PTR == SourceID)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }

    if (E2E_E_OK == Ret)
    {
        if (NULL_PTR != DataPtr)
        {
            /*compute local variable uint16 Offset, which is in [byte]*/
            Offset = ConfigPtr->Offset / 8u;
            /*Copy bytes Data[Offset...Offset+1] in Big Endian order to uint16 local variable ReceivedLength*/
            ReceivedLength = (uint16)((uint16)DataPtr[Offset] << 8u) + (uint16)(DataPtr[Offset + 1u]);
            /*Copy bytes Data[Offset+2...Offset+3] in Big Endian order on uint16 local variable ReceivedCounter*/
            ReceivedCounter = (uint16)((uint16)DataPtr[Offset + 2u] << 8u) + (uint16)(DataPtr[Offset + 3u]);
            /*Copy bytes Data[Offset+4...Offset+7] in Big Endian order on uint32 local variable ReceivedDataID*/
            ReceivedDataID = (uint32)((uint32)DataPtr[Offset + 4u] << 24u)
                             + (uint32)((uint32)DataPtr[Offset + 5u] << 16u)
                             + (uint32)((uint32)DataPtr[Offset + 6u] << 8u) + (uint32)(DataPtr[Offset + 7u]);
            /*Copy Data[offset+12...offset+15] in Big Endian order to SourceID, MessageResult, MessageType*/
            *SourceID = (uint32)(((uint32)DataPtr[Offset + 12u] & E2E_MASK_LOW_NIBBLE) << 24u)
                        + (uint32)((uint32)DataPtr[Offset + 13u] << 16u) + (uint32)((uint32)DataPtr[Offset + 14u] << 8u)
                        + (uint32)(DataPtr[Offset + 15u]);
            ReceivedMsgType =
                ((DataPtr[Offset + 12u] >> 6u) == 0u) ? STD_MESSAGETYPE_REQUEST : STD_MESSAGETYPE_RESPONSE;
            ReceivedMsgResult =
                (((DataPtr[Offset + 12u] >> 4u) & 0x03u) == 0u) ? STD_MESSAGERESULT_OK : STD_MESSAGERESULT_ERROR;
            /*Copy bytes Data[Offset+8...Offset+11] using big Endian order on 4-byte local variable ReceivedCRC*/
            ReceivedCRC = (uint32)((uint32)DataPtr[Offset + 8u] << 24u) + (uint32)((uint32)DataPtr[Offset + 9u] << 16u)
                          + (uint32)((uint32)DataPtr[Offset + 10u] << 8u) + ((uint32)DataPtr[Offset + 11u]);
            /*compute CRC*/
            ComputedCRC = Crc_CalculateCRC32P4(&DataPtr[0], ((uint32)Offset + 8uL), E2E_P04M_CRC_INITVALUE, TRUE);
            if ((Offset + 12u) < Length)
            {
                ComputedCRC = Crc_CalculateCRC32P4(
                    &DataPtr[Offset + 12u],
                    ((uint32)Length - (uint32)Offset - 12uL),
                    ComputedCRC,
                    FALSE);
            }
            /*Do Checks*/
            if ((ReceivedCRC == ComputedCRC) && (ReceivedDataID == ConfigPtr->DataID) && (ReceivedLength == Length))
            {
                if ((ReceivedMsgResult == MessageResult) && (ReceivedMsgType == MessageType))
                {
                    E2E_P04mCheck_Seqence(ConfigPtr, StatePtr, ReceivedCounter);
                }
                else
                {
                    StatePtr->Status = E2E_P04MSTATUS_ERROR;
                }
            }
            else
            {
                StatePtr->Status = E2E_P04MSTATUS_ERROR;
            }
        }
        else
        {
            /*Do Checks*/
            StatePtr->Status = E2E_P04MSTATUS_NONEWDATA;
        }
    }

    return Ret;
}
/*PRQA S 3120 -- */
/**
 * Initializes the check state.
 */
Std_ReturnType E2E_P04mCheckInit(E2E_P04mCheckStateType* StatePtr)
{
    uint8 Ret;

    /*@SWS_E2E_00378*/
    if (NULL_PTR == StatePtr)
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        StatePtr->Counter = E2E_P04M_MAX_COUNTER_VALUE;
        StatePtr->Status  = E2E_P04MSTATUS_ERROR;
        Ret               = E2E_E_OK;
    }
    return Ret;
}
/**
 * The function maps the check status of profile 4m to a generic check status.
 */
E2E_PCheckStatusType E2E_P04mMapStatusToSM(Std_ReturnType CheckReturn, E2E_P04mCheckStatusType Status)
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
        case E2E_P04MSTATUS_OK:
        case E2E_P04MSTATUS_OKSOMELOST:
            Ret = E2E_P_OK;
            break;
        case E2E_P04MSTATUS_ERROR:
            Ret = E2E_P_ERROR;
            break;
        case E2E_P04MSTATUS_REPEATED:
            Ret = E2E_P_REPEATED;
            break;
        case E2E_P04MSTATUS_NONEWDATA:
            Ret = E2E_P_NONEWDATA;
            break;
        case E2E_P04MSTATUS_WRONGSEQUENCE:
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
    E2E_P04mCheck_Seqence(const E2E_P04mConfigType* ConfigPtr, E2E_P04mCheckStateType* StatePtr, uint16 ReceivedCounter)
{
    uint16 DeltaCounter;
    /*Compute local variable DeltaCounter: ReceivedCounter - State->Counter
     * (taking into wrap around 0xFFFF)*/
    /*PRQA S 2814 ++ */ /*VL_E2E_NULLPointer*/
    DeltaCounter = (ReceivedCounter >= StatePtr->Counter)
                       ? (ReceivedCounter - StatePtr->Counter)
                       : (uint16)(E2E_P04M_MAX_COUNTER_VALUE - StatePtr->Counter + ReceivedCounter + 1U);
    /*PRQA S 2814 -- */

    if (DeltaCounter <= ConfigPtr->MaxDeltaCounter) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        if (DeltaCounter > 0u)
        {
            if (1u == DeltaCounter)
            {
                StatePtr->Status = E2E_P04MSTATUS_OK;
            }
            else
            {
                StatePtr->Status = E2E_P04MSTATUS_OKSOMELOST;
            }
        }
        else
        {
            StatePtr->Status = E2E_P04MSTATUS_REPEATED;
        }
    }
    else
    {
        StatePtr->Status = E2E_P04MSTATUS_WRONGSEQUENCE;
    }
    StatePtr->Counter = ReceivedCounter;
}
/*PRQA S 6040 ++ */ /*VL_MTR_E2E_STPAR*/
E2E_LOCAL Std_ReturnType E2E_P04mCheckVerifyInputs(
    /*PRQA S 6040 -- */
    const E2E_P04mConfigType*     ConfigPtr,
    const E2E_P04mCheckStateType* StatePtr,
    Std_MessageTypeType           MessageType,
    Std_MessageResultType         MessageResult,
    const uint8*                  DataPtr,
    uint16                        Length)
{
    Std_ReturnType Ret;
    if ((NULL_PTR == ConfigPtr) || (NULL_PTR == StatePtr))
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        if (NULL_PTR != DataPtr)
        {
            if ((Length >= (ConfigPtr->MinDataLength / E2E_BIT_LENGTH))
                && (Length <= (ConfigPtr->MaxDataLength / E2E_BIT_LENGTH))
                && ((MessageType == STD_MESSAGETYPE_RESPONSE) || (MessageType == STD_MESSAGETYPE_REQUEST))
                && ((MessageResult == STD_MESSAGERESULT_ERROR) || (MessageResult == STD_MESSAGERESULT_OK)))
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
    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
