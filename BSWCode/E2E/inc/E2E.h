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
 **  @file               : E2E.h
 **  @author             : Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : End-to-End Communication Protection Library
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 * V02.00.00 2020-08-17   YangBo         Initial Vertion.
 * V02.00.01 2020-09-01   YangBo         change CheckInit counter init to max, and E2E_P01CheckInit
 *                                       and E2E_P02CheckInit WaitForFirstData = TRUE
 * V02.00.02 2020-09-15   YangBo         QAC FIX
 * V02.00.03 2020-10-29   YangBo         fix the NULL_PTR input of E2E_P22.
 * V02.00.04 2020-11-18   YangBo         E2E_SM adjust to new type.
 * V02.00.05 2021-05-11   wanglili       In the nibble mode in E2E_P01Check, the logic change of the comparison
 *                                       between ReceivedDataIDNibble and the configured dataid
 * V02.00.06 2021-08-24   Wanglili       Functional safety profile01 compatible when dataId is not used.
 * V02.00.07 2023-02-08   Jian.Jiang     Code walkthrough, in the conditional statement (==),
 *                                       constants must be placed on the left.
 * V02.00.08 2023-05-15   Jian.Jiang     QAC rectification.
 * V02.01.00 2023-07-18   Jian.jiang     CP2.1 Release Version.
 * V02.01.01 2024-07-05   qinmei.chen    CPT-9501 fix counter is not increased
 * V02.01.02 2024-04-25   zhiqiang.huang Modify E2EP11bug to not rely on DataIDNibbleOffset for
 *                                       counter calculation in non E2E_P11_DATAID_NIBBLE mode.
 *           2024-05-22   qinmei.chen    TMS320 support
 * V02.01.03 2024-8-13    qinmei.chen    change the logic of DeltaCounter of E2EP05 and E2EP06
 *                                       and other lines to adapt to CP-rules
 * V02.02.01 2024-09-09   qinmei.chen    Add Pxxm,P44,P08,Refactor and update to R23-11
 *
===================================================================================================================== */
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:E2E<br>
  RuleSorce:puhua_rules-2024.12.rcf

   \li VL_E2E_NULLPointer
    Reason: Before an external function calls an internal function, the pointer has been verified
    Risk: The new function added later may be not validated before calling the internal function
    Prevention: Functional reliability guaranteed by design.

    \li VL_E2E_Redundant
    Reason: Although redundant, it helps code understanding
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

    \li VL_E2E_STPAR
    Reason: According to the specifications, the parameter should exceed 6
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_E2E_STM19
    Reason: The presence of multiple exit points in the function reduces the cyclomatic complexity.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_E2E_STPAR
    Reason: The function parameters are close to the interfaces of the Autosar standard
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

    \li VL_E2E_Ifelse
    Reason: There is no need for an else statement here, as it does not meet the requirements
    and there is nothing to do.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

*/
#ifndef E2E_H_
#define E2E_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Crc.h"
#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
/* @req<SWS_E2E_00327> */
#define E2E_MODULE_ID                   (207u)
#define E2E_VENDOR_ID                   (62u)
#define E2E_AR_RELEASE_MAJOR_VERSION    (4u)
#define E2E_AR_RELEASE_MINOR_VERSION    (9u)
#define E2E_AR_RELEASE_REVISION_VERSION (0u)
#define E2E_SW_MAJOR_VERSION            (2u)
#define E2E_SW_MINOR_VERSION            (1u)
#define E2E_SW_PATCH_VERSION            (0u)
/* ===================================================== macros ===================================================== */
#define E2E_SOURCEID_MAX     (0xFFFFFFFuL)
#define E2E_BIT_LENGTH       (0x8uL)
#define E2E_MASK_HIGH_NIBBLE (0xF0u)
#define E2E_MASK_LOW_NIBBLE  (0x0FuL)
/* ===================================================== macros ===================================================== */

typedef uint8 E2E_PCheckStatusType;
/*OK: the checks of the Data in this cycle were successful (including counter check).*/
#define E2E_P_OK (E2E_PCheckStatusType)0x00u
/*Data has a repeated counter.*/
#define E2E_P_REPEATED (E2E_PCheckStatusType)0x01u
/*The checks of the Data in this cycle were successful, with the exception of counter
jump, which changed more than the allowed delta.*/
#define E2E_P_WRONGSEQUENCE (E2E_PCheckStatusType)0x02u
/*Error not related to counters occurred (e.g. wrong crc, wrong length, wrong Data ID)
or the return of the check function was not OK.*/
#define E2E_P_ERROR (E2E_PCheckStatusType)0x03u
/*No value has been received yet (e.g. during initialization). This is used as the initialization
value for the buffer, it is not returned by any E2E profile.*/
#define E2E_P_NOTAVAILABLE (E2E_PCheckStatusType)0x04u
/*No new data is available.*/
#define E2E_P_NONEWDATA (E2E_PCheckStatusType)0x05u

typedef uint8 E2E_SMStateType;
/*Communication functioning properly according to E2E, data can be used.*/
#define E2E_SM_VALID (E2E_SMStateType)0x00u
/*State before E2E_SMCheckInit() is invoked, data cannot be used.*/
#define E2E_SM_DEINIT (E2E_SMStateType)0x01u
/*No data from the sender is available since the initialization, data cannot be used.*/
#define E2E_SM_NODATA (E2E_SMStateType)0x02u
/*There has been some data received since startup, but it is not yet possible use it,
data cannot be used.*/
#define E2E_SM_INIT (E2E_SMStateType)0x03u
/*Communication not functioning properly, data cannot be used.*/
#define E2E_SM_INVALID (E2E_SMStateType)0x04u

/*At least one pointer parameter is a NULL pointer*/
#define E2E_E_INPUTERR_NULL (Std_ReturnType)0x13u
/*At least one input parameter is erroneous, e.g. out of range*/
#define E2E_E_INPUTERR_WRONG (Std_ReturnType)0x17u
/*An internal library error has occurred (e.g. error detected by program
flow monitoring, violated invariant or postcondition)*/
#define E2E_E_INTERR (Std_ReturnType)0x19u
/*Function completed successfully*/
#define E2E_E_OK (Std_ReturnType)0x00u
/*Function executed in wrong state*/
#define E2E_E_WRONGSTATE (Std_ReturnType)0x1Au

#ifndef E2E_LOCAL
#define E2E_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#ifndef E2E_LOCAL_INLINE
#define E2E_LOCAL_INLINE static inline
#endif
/* ================================================ type definitions ================================================ */
typedef struct
{
    /*Size of the monitoring window for the state machine.*/
    uint8 WindowSizeValid;
    /*Minimal number of checks in which ProfileStatus equal to E2E_P_OK was determined
    within the last WindowSize checks (for the state E2E_SM_INIT) required to change to
    state E2E_SM_VALID.*/
    uint8 MinOkStateInit;
    /*Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR was determined,
    within the last WindowSize checks (for the state E2E_SM_INIT).*/
    uint8 MaxErrorStateInit;
    /*Minimal number of checks in which ProfileStatus equal to E2E_P_OK was determined within
    the last WindowSize checks (for the state E2E_SM_VALID) required to keep in state E2E_SM_VALID.*/
    uint8 MinOkStateValid;
    /*Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR was determined, within the last
    WindowSize checks (for the state E2E_SM_VALID).*/
    uint8 MaxErrorStateValid;
    /*Minimum number of checks in which ProfileStatus equal to E2E_P_OK was determined within the last
    WindowSize checks (for the state E2E_SM_INVALID) required to change to state E2E_SM_VALID.*/
    uint8 MinOkStateInvalid;
    /*Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR was determined, within the last
    WindowSize checks (for the state E2E_SM_INVALID).*/
    uint8 MaxErrorStateInvalid;
    /*Size of the monitoring windows for the state machine during state INIT.*/
    uint8 WindowSizeInit;
    /*Size of the monitoring window for the state machine during state INVALID*/
    uint8 WindowSizeInvalid;
    /*Clear monitoring window data on transition to state INVALID.*/
    boolean ClearToInvalid;
    /*Restrict/allow tranistion from states INIT/NODATA to INVALID state.*/
    boolean TransitToInvalidExtended;
} E2E_SMConfigType;

typedef struct
{
    /*Pointer to an array, in which the ProfileStatus-es of the last E2Echecks are stored.
    The array size shall be WindowSize*/
    uint8 ProfileStatusWindow[16]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    /*index in the array, at which the next ProfileStatus is to be written.*/
    uint8 WindowTopIndex;
    /*Count of checks in which ProfileStatus equal to E2E_P_OK was determined, within the last
    WindowSize checks.*/
    uint8 OkCount;
    /*Count of checks in which ProfileStatus equal to E2E_P_ERROR was determined,within the
    last WindowSize checks.*/
    uint8 ErrorCount;
    /*The current state in the state machine.*/
    E2E_SMStateType SMState;
} E2E_SMCheckStateType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief         This service returns the version information of this module
 * @param[out]    Versioninfo : Pointer to where to store the version
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-47050
 */
void E2E_GetVersionInfo(Std_VersionInfoType* versionInfo);

/**
 * @brief         Checks the communication channel. It determines if the data can be used for safety-related
 *                application, based on history of checks performed by a corresponding E2E_P0XCheck() function.
 * @param[in]     ProfileStatus : Profile-independent status of the reception on one single Data in one cycle.
 * @param[in]     ConfigPtr     : Pointer to static configuration.
 * @param[inout]  StatePtr      : Pointer to port/data communication state.
 * @return        Std_ReturnType
 * @retval        E2E_E_INPUTERR_NULL  : At least one pointer parameter is a NULL pointer
 * @retval        E2E_E_INPUTERR_WRONG : At least one input parameter is erroneous, e.g. out of range
 * @retval        E2E_E_INTERR         : An internal library error has occurred
 * @retval        E2E_E_OK             : Function completed successfully
 * @retval        E2E_E_WRONGSTATE     : Function executed in wrong state
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-47046
 */
Std_ReturnType
    E2E_SMCheck(E2E_PCheckStatusType ProfileStatus, const E2E_SMConfigType* ConfigPtr, E2E_SMCheckStateType* StatePtr);

/**
 * @brief         Initializes the state machine.
 * @param[in]     ConfigPtr : Pointer to static configuration.
 * @param[out]    StatePtr  : Pointer to port/data communication state.
 * @return        Std_ReturnType
 * @retval        E2E_E_INPUTERR_NULL : At least one pointer parameter is a NULL pointer
 * @retval        E2E_E_OK            : Function completed successfully
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-47048
 */

Std_ReturnType E2E_SMCheckInit(E2E_SMCheckStateType* StatePtr, const E2E_SMConfigType* ConfigPtr);

#ifdef __cplusplus
}
#endif
#endif
