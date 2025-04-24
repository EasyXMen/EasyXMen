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
**  FILENAME    : CanTrcv_Driver.c                                          **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation by CanTrcv_Driver module  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTrcv.h"
#include "CanTrcv_Driver.h"
#include "Det.h"
#if (Timer_1us16bit == CANTRCV_TIMER_TYPE)
#include "Tm.h"
#endif /* Timer_1us16bit == CANTRCV_TIMER_TYPE */

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CANTRCV_PN(Transceiver)      CanTrcv_ConfigPtr->CanTrcvChannel[Transceiver].CanTrcvPartialNetwork
#define CANTRCV_CHANNEL(Transceiver) CanTrcv_ConfigPtr->CanTrcvChannel[Transceiver]

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef enum CanTrcv_Driver_WorkModeType_E
{
    CANTRCV_DRIVER_SLEEPMODE,
    CANTRCV_DRIVER_STANDBYMODE,
    CANTRCV_DRIVER_NORMALMODE
} CanTrcv_Driver_WorkModeType;
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Driver_ModeChangeProcess(uint8 Transceiver, CanTrcv_Driver_WorkModeType workMode);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ClearAllStatus(uint8 Transceiver);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
********************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/**
 * This function enable the CanTrcv HW wakeup functionality
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_EnableWakeup(uint8 Transceiver)
{
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    Std_ReturnType result = E_OK;
    if ((PN_ENABLED == CanTrcv_PNActivation) && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed))
    {
        result = CanTrcv_Driver_PNWKSetup(Transceiver);
    }
    result += CanTrcv_Driver_Init(Transceiver);
#else
    Std_ReturnType result;
    result = CanTrcv_Driver_Init(Transceiver);
#endif

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function initiliazes the CanTrcv HW.
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_Init(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
        /* todo: write to control register to enable Bus wakeup */
    }
    else
    {
        /* todo: write to control register to disable Bus wakeup */
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function disables wakeup functionality
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_DisableWakeup(uint8 Transceiver)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    result = CanTrcv_Driver_DisablePNWK(Transceiver);
#endif

    /* todo: write to control register to disable Bus wakeup */

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function reads from status registers
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
                   StatusType: request type
 * Parameters(INOUT): NA
 * Parameters(OUT): FlagState: the requested flag state
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Driver_ReadStatus(
    uint8 Transceiver,
    CanTrcv_Driver_StatusType StatusType,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState)
{
    Std_ReturnType result = E_OK;

    if (CANTRCV_DRIVER_WK != StatusType)
    {
        /* todo: read from specific status register */
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears specific status register
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
                   StatusType: request type
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ClearStatus(uint8 Transceiver, CanTrcv_Driver_StatusType StatusType)
{
    Std_ReturnType result = E_OK;

    if ((CANTRCV_DRIVER_CANSIL != StatusType) && (CANTRCV_DRIVER_SYSERR != StatusType))
    {
        /* todo: clear specific status register */
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function changes HW to Sleep Mode
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetSleepMode(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    if (PN_ENABLED == CanTrcv_PNActivation)
    {
        result = CanTrcv_Driver_PNWKSetup(Transceiver);
    }
#endif

    if (E_OK == result)
    {
        result = CanTrcv_Driver_ClearStatus(Transceiver, CANTRCV_DRIVER_WK);
    }

    if (E_OK == result)
    {
        result = CanTrcv_Driver_ModeChangeProcess(Transceiver, CANTRCV_DRIVER_SLEEPMODE);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function changes HW to Standby Mode
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetStandbyMode(uint8 Transceiver)
{
    Std_ReturnType result;

    result = CanTrcv_Driver_ClearStatus(Transceiver, CANTRCV_DRIVER_WK);

    if (E_OK == result)
    {
        CanTrcv_Driver_ModeChangeProcess(Transceiver, CANTRCV_DRIVER_STANDBYMODE);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function changes HW to Normal Mode
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetNormalMode(uint8 Transceiver)
{
    Std_ReturnType result;

    result = CanTrcv_Driver_ModeChangeProcess(Transceiver, CANTRCV_DRIVER_NORMALMODE);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets the current operation mode of HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): OpMode: current aquired opperation mode
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Driver_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode)
{
    Std_ReturnType result = E_OK;
    CanTrcv_Driver_WorkModeType workMode = CANTRCV_DRIVER_SLEEPMODE;

    /* todo: read from mode ctrl register to get current OpMode */

    if (CANTRCV_DRIVER_SLEEPMODE == workMode)
    {
        *OpMode = CANTRCV_TRCVMODE_SLEEP;
    }
    else if (CANTRCV_DRIVER_STANDBYMODE == workMode)
    {
        *OpMode = CANTRCV_TRCVMODE_STANDBY;
    }
    else if (CANTRCV_DRIVER_NORMALMODE == workMode)
    {
        *OpMode = CANTRCV_TRCVMODE_NORMAL;
    }
    else
    {
        /* do nothing */
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function deinitialize the HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_DeInit(uint8 Transceiver)
{
    Std_ReturnType result;

    result = CanTrcv_Driver_ClearAllStatus(Transceiver);
    result += CanTrcv_Driver_SetStandbyMode(Transceiver);
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    result += CanTrcv_Driver_DisablePNWK(Transceiver);
#endif
    /* todo: disable Bus wakeup functionality of HW */

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function disables Partial network wakeup
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_DisablePNWK(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    /* todo: write to registers to disbale PN wakeup functionalities */

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets transceiver system data from HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvSysData: the required system data
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Driver_GetTrcvSystemData(uint8 Transceiver, P2VAR(uint32, AUTOMATIC, CANTRCV_APPL_DATA) TrcvSysData)
{
    Std_ReturnType result = E_OK;

    /* todo: read from registers of HW */

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function setup PN wakeup functionalities
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_PNWKSetup(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    CanTrcv_TrcvFlagStateType isSyserrSet;

    if (NULL_PTR != CANTRCV_PN(Transceiver))
    {
/* todo: write to register to enable and setup PN wakeup functionalities */
#if (STD_ON == CANTRCV_BUS_ERR_FLAG)
        /* check SYSERR */
        if ((E_OK == result) && (TRUE == CANTRCV_PN(Transceiver)->CanTrcvBusErrFlag))
        {
            result += CanTrcv_Driver_ReadStatus(Transceiver, CANTRCV_DRIVER_SYSERR, &isSyserrSet);
            if ((E_OK == result) && (TRUE == isSyserrSet))
            {
                (void)CanTrcv_Driver_DisablePNWK(Transceiver);
                result = E_NOT_OK;
            }
        }
#endif /* (STD_ON == CANTRCV_BUS_ERR_FLAG) */
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /*  (STD_ON == CANTRCV_HWPN_SUPPORT) */

#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode)
{
    Std_ReturnType result = E_NOT_OK;

    switch (OpMode)
    {
    case CANTRCV_TRCVMODE_NORMAL:
        result = CanTrcv_Driver_SetNormalMode(Transceiver);
        break;
    case CANTRCV_TRCVMODE_SLEEP:
        result = CanTrcv_Driver_SetSleepMode(Transceiver);
        break;
    case CANTRCV_TRCVMODE_STANDBY:
        result = CanTrcv_Driver_SetStandbyMode(Transceiver);
        break;
    default:
        /* do nothing */
        break;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/**
 * This function process mode change for HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 *                 workMode: the requested workMode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Driver_ModeChangeProcess(uint8 Transceiver, CanTrcv_Driver_WorkModeType workMode)
{
#if (None == CANTRCV_TIMER_TYPE)
    uint16 waitTime;
#endif
    Std_ReturnType result = E_OK;
    CanTrcv_Driver_WorkModeType currentWorkMode = CANTRCV_DRIVER_SLEEPMODE;

/* todo: write to register to request mode change */
#if (Timer_1us16bit == CANTRCV_TIMER_TYPE)
    result += Tm_BusyWait1us16bit(CANTRCV_WAIT_TIME);
#else
/* todo: wait for specific period of time not using Tm Module */
#endif
    /* todo: read from register to obtain current opmode */

    if (currentWorkMode != workMode)
    {
        result = E_NOT_OK;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears all status in HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ClearAllStatus(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    /* write to status registers to clear them */

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
