/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : E2E.c                                                       **
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
 * V2.0.1 [2020/11/18] [YangBo] E2E_SM adjust to new type.
 * V2.1.0  2023-07-18   Jian.jiang      CP2.1 Release Version.
 * V2.1.1  2024-04-25   zhiqiang.huang      Modify E2EP11bug to not rely on DataIDNibbleOffset for
 * counter calculation in non E2E_P11_DATAID_NIBBLE mode.
 *         2024-05-22   qinmei.chen     TMS320 support
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:E2E<br>
  RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 4491 MISRA Rule 10.6 .<br>
    Reason:The data type of CRCoffse in the AUTOSAR document is uint16,
    and the input length of the Crc_CalculateCRCx is uint32, which is different

   \li PRQA S 4391 MISRA Rule 10.8 .<br>
    Reason:The data type of CRCoffse in the AUTOSAR document is uint16,
    and the input length of the Crc_CalculateCRCx is uint32, which is different

   \li PRQA S 0488 MISRA Rule 18.4 .<br>
    Reason:The offending items are array pointer operations

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "E2E.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
/*
      VERSION CHECK FOR E2E MODULE INCLUSION
*/

#ifndef E2E_AR_RELEASE_MAJOR_VERSION
#error "E2E_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef E2E_AR_RELEASE_MINOR_VERSION
#error "E2E_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#ifndef E2E_AR_RELEASE_REVISION_VERSION
#error "E2E_AR_RELEASE_REVISION_VERSION is not defined. "
#endif

#if (E2E_AR_RELEASE_MAJOR_VERSION != 4U)
#error "E2E_AR_RELEASE_MAJOR_VERSION does not match. "
#endif
#if (E2E_AR_RELEASE_MINOR_VERSION != 5U)
#error "MCU_AR_RELEASE_MINOR_VERSION does not match. "
#endif

#ifndef E2E_SW_MAJOR_VERSION
#error "E2E_SW_MAJOR_VERSION is not defined. "
#endif

#ifndef E2E_SW_MINOR_VERSION
#error "E2E_SW_MINOR_VERSION is not defined. "
#endif

#if (E2E_SW_MAJOR_VERSION != 2U)
#error "E2E_SW_MAJOR_VERSION does not match. "
#endif
#if (E2E_SW_MINOR_VERSION != 1U)
#error "E2E_SW_MINOR_VERSION does not match. "
#endif
#if (E2E_SW_PATCH_VERSION != 0U)
#error "E2E_SW_PATCH_VERSION does not match. "
#endif

/*
      VERSION CHECK FOR CRC MODULE INCLUSION
*/
#ifndef CRC_AR_RELEASE_MAJOR_VERSION
#error "CRC_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef CRC_AR_RELEASE_MINOR_VERSION
#error "CRC_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#if (CRC_AR_RELEASE_MAJOR_VERSION != 4U)
#error "CRC_AR_RELEASE_MAJOR_VERSION does not match. "
#endif
#if (CRC_AR_RELEASE_MINOR_VERSION != 5U)
#error "CRC_AR_RELEASE_MINOR_VERSION does not match. "
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/* PRQA S 1503 ++ */ /* MISRA Rule 2.1 */
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
static FUNC(void, E2E_CODE) E2E_SMAddStatus(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) Config,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) State);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
static FUNC(void, E2E_CODE) E2E_SMClearStatus(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) Config);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"

#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
static FUNC(void, E2E_CODE) E2E_SMClearRemainingStatus(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) Config,
    VAR(E2E_SMStateType, E2E_APPL_DATA) NextState);
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
 * Checks the communication channel. It determines if the data can be used for safety-related application,
 * based on history of checks performed by a corresponding E2E_P0XCheck() function.
 * Service ID: 0x30
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): ProfileStatus,Profile-independent status of the reception on one single Data in one cycle.
 *                 ConfigPtr,Pointer to static configuration.
 * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_INPUTERR_WRONG E2E_E_INTERR E2E_E_OK E2E_E_WRONGSTATE.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE)
E2E_SMCheck(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType Ret;
    Ret = E2E_E_OK;

    /*@SWS_E2E_00371*/
    if ((NULL_PTR == ConfigPtr) || (NULL_PTR == StatePtr))
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else if (ProfileStatus > 0x05u)
    {
        Ret = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        switch (StatePtr->SMState)
        {
        case E2E_SM_DEINIT:
            Ret = E2E_E_WRONGSTATE;
            break;
        case E2E_SM_NODATA:
            if ((ProfileStatus != E2E_P_ERROR) && (ProfileStatus != E2E_P_NONEWDATA))
            {
                StatePtr->SMState = E2E_SM_INIT;
            }
            break;
        case E2E_SM_INIT:
            E2E_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
            if ((StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateInit)
                && (StatePtr->OkCount >= ConfigPtr->MinOkStateInit))
            {
                E2E_SMClearRemainingStatus(StatePtr, ConfigPtr, (E2E_SMStateType)E2E_SM_VALID);
                StatePtr->SMState = E2E_SM_VALID;
            }
            else if (StatePtr->ErrorCount > ConfigPtr->MaxErrorStateInit)
            {
                if (TRUE == ConfigPtr->ClearToInvalid)
                {
                    E2E_SMClearStatus(StatePtr, ConfigPtr);
                }
                else
                {
                    E2E_SMClearRemainingStatus(StatePtr, ConfigPtr, (E2E_SMStateType)E2E_SM_INVALID);
                }

                StatePtr->SMState = E2E_SM_INVALID;
            }
            else
            {
                /*nothing to do*/
            }
            break;
        case E2E_SM_VALID:
            E2E_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
            if ((StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateValid)
                && (StatePtr->OkCount >= ConfigPtr->MinOkStateValid))
            {
                /*nothing to do*/
            }
            else
            {
                if (TRUE == ConfigPtr->ClearToInvalid)
                {
                    E2E_SMClearStatus(StatePtr, ConfigPtr);
                }
                StatePtr->SMState = E2E_SM_INVALID;
            }
            break;
        case E2E_SM_INVALID:
            E2E_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
            if ((StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateInvalid)
                && (StatePtr->OkCount >= ConfigPtr->MinOkStateInvalid))
            {
                E2E_SMClearRemainingStatus(StatePtr, ConfigPtr, E2E_SM_VALID);
                StatePtr->SMState = E2E_SM_VALID;
            }
            else
            {
                /*nothing to do*/
            }
            break;
        default:
            Ret = E2E_E_WRONGSTATE;
            break;
        }
    }

    return Ret;
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/**
 * Initializes the state machine.
 * Service ID: 0x31
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): ConfigPtr,Pointer to configuration of the state machine.
 * Parameters(INOUT): StatePtr,Pointer to port/data communication state.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E2E_E_INPUTERR_NULL E2E_E_OK.
 *
 */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
FUNC(Std_ReturnType, E2E_CODE)
E2E_SMCheckInit(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr)
{
    Std_ReturnType Ret;
    uint8 index;
    uint8 CurrentWindowSize;
    Ret = E2E_E_OK;

    if ((NULL_PTR == StatePtr) || (NULL_PTR == ConfigPtr))
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else
    {
        if (E2E_SM_VALID == StatePtr->SMState)
        {
            CurrentWindowSize = ConfigPtr->WindowSizeValid;
        }
        else if (E2E_SM_INIT == StatePtr->SMState)
        {
            CurrentWindowSize = ConfigPtr->WindowSizeInit;
        }
        else
        {
            CurrentWindowSize = ConfigPtr->WindowSizeInvalid;
        }
        for (index = 0; index < CurrentWindowSize; index++)
        {
            StatePtr->ProfileStatusWindow[index] = E2E_P_NOTAVAILABLE;
        }
        StatePtr->WindowTopIndex = 0;
        StatePtr->OkCount = 0;
        StatePtr->ErrorCount = 0;
        StatePtr->SMState = E2E_SM_NODATA;
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
static FUNC(void, E2E_CODE) E2E_SMAddStatus(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) Config,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) State)
{
    uint8 index;
    uint8 Count;
    uint8 CurrentWindowSize;

    State->ProfileStatusWindow[State->WindowTopIndex] = ProfileStatus;
    if (E2E_SM_VALID == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeValid;
    }
    else if (E2E_SM_INIT == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeInit;
    }
    else
    {
        CurrentWindowSize = Config->WindowSizeInvalid;
    }
    Count = 0;
    for (index = 0; index < CurrentWindowSize; index++)
    {
        if ((uint8)E2E_P_OK == State->ProfileStatusWindow[index])
        {
            Count++;
        }
    }
    State->OkCount = Count;

    Count = 0;
    for (index = 0; index < CurrentWindowSize; index++)
    {
        if ((uint8)E2E_P_ERROR == State->ProfileStatusWindow[index])
        {
            Count++;
        }
    }
    State->ErrorCount = Count;

    if ((State->WindowTopIndex) >= (CurrentWindowSize - (uint8)1))
    {
        State->WindowTopIndex = 0u;
    }
    else
    {
        State->WindowTopIndex++;
    }
}

static FUNC(void, E2E_CODE) E2E_SMClearStatus(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) Config)
{
    uint8 index;
    uint8 CurrentWindowSize;

    if (E2E_SM_VALID == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeValid;
    }
    else if (E2E_SM_INIT == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeInit;
    }
    else
    {
        CurrentWindowSize = Config->WindowSizeInvalid;
    }
    for (index = 0; index < CurrentWindowSize; index++)
    {
        State->ProfileStatusWindow[index] = E2E_P_NOTAVAILABLE;
    }
    State->OkCount = 0u;
    State->ErrorCount = 0u;
    State->WindowTopIndex = 0u;
}

static FUNC(void, E2E_CODE) E2E_SMClearRemainingStatus(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) Config,
    VAR(E2E_SMStateType, E2E_APPL_DATA) NextState)
{
    uint8 CurrentWindowSize;
    uint8 NextWindowSize;
    uint8 index;

    if (E2E_SM_VALID == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeValid;
    }
    else if (E2E_SM_INIT == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeInit;
    }
    else
    {
        CurrentWindowSize = Config->WindowSizeInvalid;
    }
    if (E2E_SM_VALID == NextState)
    {
        NextWindowSize = Config->WindowSizeValid;
    }
    else if (E2E_SM_INVALID == NextState)
    {
        NextWindowSize = Config->WindowSizeInvalid;
    }
    else
    {
        NextWindowSize = 0u;
    }
    if (CurrentWindowSize < NextWindowSize)
    {
        for (index = 0; index < (CurrentWindowSize - 1u); index++)
        {
            State->ProfileStatusWindow[index] = E2E_P_NOTAVAILABLE;
        }
    }
}
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/* PRQA S 1503 -- */ /* MISRA Rule 2.1 */
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
