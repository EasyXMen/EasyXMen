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
**  @file               : E2E.c
**  @author             : Yb,QinmeiChen
**  @date               : 2024/09/09
**  @vendor             : isoft
**  @description        : End-to-End Communication Protection Library
**
***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "E2E.h"
/* ================================================== version check ================================================= */
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
#if (E2E_AR_RELEASE_MINOR_VERSION != 9U)
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
#if (CRC_AR_RELEASE_MINOR_VERSION != 9U)
#error "CRC_AR_RELEASE_MINOR_VERSION does not match. "
#endif
/* ===================================================== macros ===================================================== */
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/**
 * @brief        count the number of OK and ERROR for next status determine
 * @param[in]    ProfileStatus : Profile-independent status of the reception on one single Data in one cycle
 * @param[in]    Config        : Pointer to static configuration.
 * @param[inout] State         : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void
    E2E_SMAddStatus(E2E_PCheckStatusType ProfileStatus, const E2E_SMConfigType* Config, E2E_SMCheckStateType* State);
/**
 * @brief        initialize status when the next status is INVALID
 * @param[in]    Config : Pointer to static configuration.
 * @param[inout] State  : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void E2E_SMClearStatus(E2E_SMCheckStateType* State, const E2E_SMConfigType* Config);
/**
 * @brief        initialize status depending on the next and current status
 * @param[in]    NextState : the next communication state.
 * @param[in]    Config    : Pointer to static configuration.
 * @param[inout] State     : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void
    E2E_SMClearRemainingStatus(E2E_SMCheckStateType* State, const E2E_SMConfigType* Config, E2E_SMStateType NextState);
/**
 * @brief        verify inputs of E2E_SMCheck()
 * @param[in]     ProfileStatus : Profile-independent status of the reception on one single Data in one cycle.
 * @param[in]     ConfigPtr     : Pointer to static configuration.
 * @param[in]     StatePtr      : Pointer to port/data communication state.
 * @return        Std_ReturnType
 * @retval        E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval        E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval        E2E_E_INTERR         : An internal library error has occurred
 * @retval        E2E_E_OK             : Function completed successfully
 * @retval        E2E_E_WRONGSTATE     : Function executed in wrong state
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace         -
 */
E2E_LOCAL Std_ReturnType E2E_SMCheckVerifyInputs(
    E2E_PCheckStatusType        ProfileStatus,
    const E2E_SMConfigType*     ConfigPtr,
    const E2E_SMCheckStateType* StatePtr);
/**
 * @brief        modify status when the next status is INVALID
 * @param[in]    ConfigPtr    : Pointer to static configuration.
 * @param[inout] StatePtr     : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void E2E_SMCheckStatusToInvalid(const E2E_SMConfigType* ConfigPtr, E2E_SMCheckStateType* StatePtr);
/**
 * @brief         the handle of SMcheck when the status is NODATA.
 * @param[in]     ProfileStatus : Profile-independent status of the reception on one single Data in one cycle.
 * @param[in]     ConfigPtr     : Pointer to static configuration.
 * @param[inout]  StatePtr      : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void handleofNodata(
    E2E_PCheckStatusType    ProfileStatus,
    const E2E_SMConfigType* ConfigPtr,
    E2E_SMCheckStateType*   StatePtr);
/**
 * @brief         the handle of SMcheck when the status is INIT.
 * @param[in]     ConfigPtr     : Pointer to static configuration.
 * @param[inout]  StatePtr      : Pointer to port/data communication state.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         -
 */
E2E_LOCAL void handleofInit(const E2E_SMConfigType* ConfigPtr, E2E_SMCheckStateType* StatePtr);
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h"
/**
 * retrive version info from macros
 */
void E2E_GetVersionInfo(Std_VersionInfoType* versionInfo) /*PRQA S 1503*/ /*VL_QAC_NoUsedApi*/
{
    if (versionInfo != NULL_PTR)
    {
        versionInfo->vendorID         = E2E_VENDOR_ID;
        versionInfo->moduleID         = E2E_MODULE_ID;
        versionInfo->sw_major_version = E2E_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = E2E_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = E2E_SW_PATCH_VERSION;
    }
}
/**
 * This service Checks the communication channel.
 */
/*PRQA S 2889,6060,1532 ++ */ /*VL_QAC_MultiReturn,VL_MTR_E2E_STM19,VL_QAC_OneFunRef*/
Std_ReturnType E2E_SMCheck(
    /*PRQA S 2889,6060,1532 -- */
    E2E_PCheckStatusType    ProfileStatus,
    const E2E_SMConfigType* ConfigPtr,
    E2E_SMCheckStateType*   StatePtr)
{
    /*@SWS_E2E_00371*/
    Std_ReturnType Ret = E2E_SMCheckVerifyInputs(ProfileStatus, ConfigPtr, StatePtr);
    if (Ret != E2E_E_OK)
    {
        return Ret;
    }

    switch (StatePtr->SMState)
    {
    case E2E_SM_NODATA:
        handleofNodata(ProfileStatus, ConfigPtr, StatePtr);
        break;
    case E2E_SM_INIT:
        E2E_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
        handleofInit(ConfigPtr, StatePtr);
        break;
    case E2E_SM_VALID:
        E2E_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
        if ((StatePtr->ErrorCount > ConfigPtr->MaxErrorStateValid) || (StatePtr->OkCount < ConfigPtr->MinOkStateValid))
        {
            if (ConfigPtr->ClearToInvalid)
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
        break;
    default:
        Ret = E2E_E_WRONGSTATE;
        break;
    }

    return Ret;
}
/**
 * Initializes the state machine.
 */
/*PRQA S 1532 ++ */ /*VL_QAC_OneFunRef*/
Std_ReturnType E2E_SMCheckInit(E2E_SMCheckStateType* StatePtr, const E2E_SMConfigType* ConfigPtr)
/*PRQA S 1532 -- */
{
    Std_ReturnType Ret;
    uint8          index;
    uint8          CurrentWindowSize;
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
        for (index = 0u; index < CurrentWindowSize; index++)
        {
            StatePtr->ProfileStatusWindow[index] = E2E_P_NOTAVAILABLE; /*PRQA S 2844*/ /*VL_E2E_NULLPointer*/
        }
        StatePtr->WindowTopIndex = 0u;
        StatePtr->OkCount        = 0u;
        StatePtr->ErrorCount     = 0u;
        StatePtr->SMState        = E2E_SM_NODATA;
    }
    return Ret;
}
/* ========================================== internal function definitions ========================================= */
E2E_LOCAL void
    E2E_SMAddStatus(E2E_PCheckStatusType ProfileStatus, const E2E_SMConfigType* Config, E2E_SMCheckStateType* State)
{
    uint8 index;
    uint8 Count;
    uint8 CurrentWindowSize = Config->WindowSizeValid;

    State->ProfileStatusWindow[State->WindowTopIndex] = ProfileStatus; /*PRQA S 2814, 2844*/ /*VL_E2E_NULLPointer*/
    if (E2E_SM_NODATA != State->SMState)
    {
        if (E2E_SM_VALID == State->SMState)
        {
            CurrentWindowSize = Config->WindowSizeValid; /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
        }
        else if (E2E_SM_INIT == State->SMState)
        {
            CurrentWindowSize = Config->WindowSizeInit;
        }
        else
        {
            CurrentWindowSize = Config->WindowSizeInvalid;
        }
        Count = 0u;
        for (index = 0u; index < CurrentWindowSize; index++)
        {
            if ((uint8)E2E_P_OK == State->ProfileStatusWindow[index])
            {
                Count++;
            }
        }
        State->OkCount = Count;

        Count = 0u;
        for (index = 0u; index < CurrentWindowSize; index++)
        {
            if ((uint8)E2E_P_ERROR == State->ProfileStatusWindow[index])
            {
                Count++;
            }
        }
        State->ErrorCount = Count;
    }

    if ((State->WindowTopIndex) >= (CurrentWindowSize - (uint8)1))
    {
        State->WindowTopIndex = 0u;
    }
    else
    {
        State->WindowTopIndex++;
    }
}

E2E_LOCAL void E2E_SMClearStatus(E2E_SMCheckStateType* State, const E2E_SMConfigType* Config)
{
    uint8 index;
    uint8 CurrentWindowSize;

    if (E2E_SM_VALID == State->SMState) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        CurrentWindowSize = Config->WindowSizeValid; /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    }
    else if (E2E_SM_INIT == State->SMState)
    {
        CurrentWindowSize = Config->WindowSizeInit;
    }
    else
    {
        CurrentWindowSize = Config->WindowSizeInvalid;
    }
    for (index = 0u; index < CurrentWindowSize; index++)
    {
        State->ProfileStatusWindow[index] = E2E_P_NOTAVAILABLE; /*PRQA S 2844*/ /*VL_E2E_NULLPointer*/
    }
    State->OkCount        = 0u;
    State->ErrorCount     = 0u;
    State->WindowTopIndex = 0u;
}

E2E_LOCAL void
    E2E_SMClearRemainingStatus(E2E_SMCheckStateType* State, const E2E_SMConfigType* Config, E2E_SMStateType NextState)
{
    uint8 CurrentWindowSize;
    uint8 NextWindowSize;
    uint8 index;

    if ((E2E_SM_VALID == State->SMState) || (E2E_SM_NODATA == State->SMState)) /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
    {
        CurrentWindowSize = Config->WindowSizeValid; /*PRQA S 2814*/ /*VL_E2E_NULLPointer*/
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
        for (index = 0u; index < (CurrentWindowSize - 1u); index++)
        {
            State->ProfileStatusWindow[index] = E2E_P_NOTAVAILABLE; /*PRQA S 2844*/ /*VL_E2E_NULLPointer*/
        }
    }
}

E2E_LOCAL Std_ReturnType E2E_SMCheckVerifyInputs(
    E2E_PCheckStatusType        ProfileStatus,
    const E2E_SMConfigType*     ConfigPtr,
    const E2E_SMCheckStateType* StatePtr)
{
    Std_ReturnType Ret = E2E_E_OK;
    if ((NULL_PTR == ConfigPtr) || (NULL_PTR == StatePtr))
    {
        Ret = E2E_E_INPUTERR_NULL;
    }
    else if (ProfileStatus > E2E_P_NONEWDATA)
    {
        Ret = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        Ret = E2E_E_OK;
    }
    return Ret;
}
E2E_LOCAL void E2E_SMCheckStatusToInvalid(const E2E_SMConfigType* ConfigPtr, E2E_SMCheckStateType* StatePtr)
{
    if (ConfigPtr->ClearToInvalid)
    {
        E2E_SMClearStatus(StatePtr, ConfigPtr);
    }
    else
    {
        E2E_SMClearRemainingStatus(StatePtr, ConfigPtr, (E2E_SMStateType)E2E_SM_INVALID);
    }
    StatePtr->SMState = E2E_SM_INVALID;
}
E2E_LOCAL void handleofNodata(
    E2E_PCheckStatusType    ProfileStatus,
    const E2E_SMConfigType* ConfigPtr,
    E2E_SMCheckStateType*   StatePtr)
{
    if (ConfigPtr->TransitToInvalidExtended)
    {
        E2E_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
        if ((StatePtr->WindowTopIndex == 0u) || (StatePtr->WindowTopIndex >= ConfigPtr->WindowSizeInit))
        {
            if (ConfigPtr->ClearToInvalid)
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
            if ((ProfileStatus != E2E_P_ERROR) && (ProfileStatus != E2E_P_NONEWDATA))
            {
                StatePtr->SMState = E2E_SM_INIT;
            }
        }
    }
    else
    {
        if ((ProfileStatus != E2E_P_ERROR) && (ProfileStatus != E2E_P_NONEWDATA))
        {
            StatePtr->SMState = E2E_SM_INIT;
        }
    }
}
E2E_LOCAL void handleofInit(const E2E_SMConfigType* ConfigPtr, E2E_SMCheckStateType* StatePtr)
{
    if ((StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateInit) && (StatePtr->OkCount >= ConfigPtr->MinOkStateInit))
    {
        E2E_SMClearRemainingStatus(StatePtr, ConfigPtr, (E2E_SMStateType)E2E_SM_VALID);
        StatePtr->SMState = E2E_SM_VALID;
    }
    /*PRQA S 2004 ++ */ /*VL_E2E_Ifelse*/
    else if (
        /*PRQA S 2004 -- */
        (StatePtr->ErrorCount > ConfigPtr->MaxErrorStateInit)
        || ((ConfigPtr->TransitToInvalidExtended)
            && (((StatePtr->WindowTopIndex == 0u) || (StatePtr->WindowTopIndex >= ConfigPtr->WindowSizeInit))
                || ((StatePtr->OkCount + (ConfigPtr->WindowSizeInit - StatePtr->WindowTopIndex))
                    < ConfigPtr->MinOkStateInit))))
    {
        E2E_SMCheckStatusToInvalid(ConfigPtr, StatePtr);
    }
}
#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h"
