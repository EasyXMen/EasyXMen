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
**  FILENAME    : E2E.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef E2E_H_
#define E2E_H_
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION> <DATE> <AUTHOR> <REVISION LOG>
 * V2.0.0 [2020/8/17] [YangBo] Initial Vertion.
 * V2.0.1 [2021/8/24] [Wanglili] Functional safety profile01 compatible when dataId
 * is not used.
 * V2.0.2 [2023/2/08] [Jian.Jiang] Code walkthrough, in the conditional statement (==),
 * constants must be placed on the left.
 * V2.0.3 [2023/5/15] [Jian.Jiang] QAC rectification.
 * V2.0.4[2024/7/5] [qinmei.chen] CPT-9501 fix counter is not increased
 * V2.0.5 [2024/8/13] [qinmei.chen] change the logic of DeltaCounter of E2EP05
 * and E2EP06 and other lines to adapt to CP-rules
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "Crc.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
/* @req<SWS_E2E_00327> */
#define E2E_VENDOR_ID                   (62u)
#define E2E_MODULE_ID                   (207u)
#define E2E_AR_RELEASE_MAJOR_VERSION    (4u)
#define E2E_AR_RELEASE_MINOR_VERSION    (5u)
#define E2E_AR_RELEASE_REVISION_VERSION (0u)
#define E2E_SW_MAJOR_VERSION            (2u)
#define E2E_SW_MINOR_VERSION            (1u)
#define E2E_SW_PATCH_VERSION            (0u)

/**
 * Returns the version information of this module.
 * Service ID: 0x14
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): VersionInfo,Pointer to where to store the version information of this module.
 * Return value: NA
 *
 */
#define E2E_GetVersionInfo(VersionInfo)                         \
    do                                                          \
    {                                                           \
        (VersionInfo)->vendorID = E2E_VENDOR_ID;                \
        (VersionInfo)->moduleID = E2E_MODULE_ID;                \
        (VersionInfo)->sw_major_version = E2E_SW_MAJOR_VERSION; \
        (VersionInfo)->sw_minor_version = E2E_SW_MINOR_VERSION; \
        (VersionInfo)->sw_patch_version = E2E_SW_PATCH_VERSION; \
    } while (0)

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
#define E2E_E_INPUTERR_NULL (E2E_SMStateType)0x13u
/*At least one input parameter is erroneous, e.g. out of range*/
#define E2E_E_INPUTERR_WRONG (E2E_SMStateType)0x17u
/*An internal library error has occurred (e.g. error detected by program
flow monitoring, violated invariant or postcondition)*/
#define E2E_E_INTERR (E2E_SMStateType)0x19u
/*Function completed successfully*/
#define E2E_E_OK (E2E_SMStateType)0x00u
/*Function executed in wrong state*/
#define E2E_E_WRONGSTATE (E2E_SMStateType)0x1Au
/******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
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
} E2E_SMConfigType;

typedef struct
{
    /*Pointer to an array, in which the ProfileStatus-es of the last E2Echecks are stored.
    The array size shall be WindowSize*/
    uint8 ProfileStatusWindow[16]; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
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

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
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

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_SMCheck(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

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
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_SMCheckInit(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_DATA) ConfigPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#endif
