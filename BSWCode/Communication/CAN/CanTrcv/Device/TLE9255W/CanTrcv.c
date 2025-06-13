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
**  FILENAME    : CanTrcv.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation by CanTrcv module           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V2.0.0    20210401  Xinrun.Wang   Initial version
 *  V2.0.1    20230704  tong.zhao     Modify the conditions for CanTrcv_SetOpMode
 *                                    to call CanIf_ConfirmPnAvailability
 *
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Det.h"
#include "CanIf_Cbk.h"
#include "CanTrcv_TLE9255W.h"

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define CANTRCV_C_AR_MAJOR_VERSION 19U
#define CANTRCV_C_AR_MINOR_VERSION 11U
#define CANTRCV_C_AR_PATCH_VERSION 0U
#define CANTRCV_C_SW_MAJOR_VERSION 2U
#define CANTRCV_C_SW_MINOR_VERSION 0U
#define CANTRCV_C_SW_PATCH_VERSION 1U

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                             \
    (CANTRCV_C_AR_MAJOR_VERSION != CANTRCV_H_AR_MAJOR_VERSION)    \
    || (CANTRCV_C_AR_MINOR_VERSION != CANTRCV_H_AR_MINOR_VERSION) \
    || (CANTRCV_C_AR_PATCH_VERSION != CANTRCV_H_AR_PATCH_VERSION) \
    || (CANTRCV_C_SW_MAJOR_VERSION != CANTRCV_H_SW_MAJOR_VERSION) \
    || (CANTRCV_C_SW_MINOR_VERSION != CANTRCV_H_SW_MINOR_VERSION) \
    || (CANTRCV_C_SW_PATCH_VERSION != CANTRCV_H_SW_PATCH_VERSION))

#error "CanTrcv.c version mismatching with CanTrcv.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
#define CANTRCV_DET_REPORT(ApiId, ErrorId) \
    ((void)Det_ReportError(CANTRCV_MODULE_ID, CANTRCV_INSTANCE_ID, (ApiId), (ErrorId)))
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */

#define CANTRCV_DET_REPORT_RUNTIME(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(CANTRCV_MODULE_ID, CANTRCV_INSTANCE_ID, (ApiId), (ErrorId)))

#define CANTRCV_DEM_REPORT_PASSED(eventId) (void)Dem_SetEventStatus((eventId), DEM_EVENT_STATUS_PASSED)
#define CANTRCV_DEM_REPORT_FAILED(eventId) (void)Dem_SetEventStatus((eventId), DEM_EVENT_STATUS_FAILED)

#define CANTRCV_CHANNEL(Transceiver)       CanTrcv_ConfigPtr->CanTrcvChannel[(Transceiver)]

#define CANTRCV_E_BUS_ERROR(Transceiver) \
    (CanTrcv_ConfigPtr->CanTrcvChannel[(Transceiver)].CanTrcvDemEventParameterRefs->CanTrcvBusErrParam)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
/* perform det check for transceiver index and module state */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_DetCheck(uint8 Transceiver, uint8 serviceId);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */

/* handle wakeup events */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_HandleWakeupEvents(uint8 Transceiver);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
/* CanTrcv PN switch */
#define CANTRCV_START_SEC_VAR_INIT_8
#include "CanTrcv_MemMap.h"
VAR(CanTrcv_PNActivationType, CANTRCV_VAR_NOINIT) CanTrcv_PNActivation = PN_DISABLED;
#define CANTRCV_STOP_SEC_VAR_INIT_8
#include "CanTrcv_MemMap.h"
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

/* CanTrcv config data */
#define CANTRCV_START_SEC_VAR_INIT_PTR
#include "CanTrcv_MemMap.h"
P2CONST(CanTrcv_ConfigType, CANTRCV_CONST, CANTRCV_CONST_PBCFG) CanTrcv_ConfigPtr = NULL_PTR;
#define CANTRCV_STOP_SEC_VAR_INIT_PTR
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define CANTRCV_START_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"
static VAR(CanTrcv_TrcvWakeupReasonType, CANTRCV_VAR_NOINIT) CanTrcv_WakeupReason[CANTRCV_MAX_CHANNELS];
#define CANTRCV_STOP_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"

#define CANTRCV_START_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"
static VAR(CanTrcv_TrcvWakeupModeType, CANTRCV_VAR_NOINIT) CanTrcv_WakeupMode[CANTRCV_MAX_CHANNELS];
#define CANTRCV_STOP_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"

/* whether initialized */
#define CANTRCV_START_SEC_VAR_INIT_8
#include "CanTrcv_MemMap.h"
static VAR(CanTrcv_StateType, CANTRCV_VAR_POWER_ON_INIT) CanTrcv_State = CANTRCV_NOT_ACTIVE;
#define CANTRCV_STOP_SEC_VAR_INIT_8
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/**
 * This function initializes the CanTrcv module.
 * Service ID: 0x00
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): ConfigPtr: Pointer to selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANTRCV_CODE) CanTrcv_Init(P2CONST(CanTrcv_ConfigType, AUTOMATIC, CANTRCV_CONST_PBCFG) ConfigPtr)
{
    uint8 index;
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_POWER_ON_FLAG)
    CanTrcv_TrcvFlagStateType isPorSet;
#endif /* STD_ON == CANTRCV_POWER_ON_FLAG */
    CanTrcv_TrcvFlagStateType isSyserrSet;
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        CANTRCV_DET_REPORT(CANTRCV_INIT_ID, CANTRCV_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
    {
        CanTrcv_ConfigPtr = ConfigPtr;

        /* initialize every channel */
        for (index = 0; index < CANTRCV_MAX_CHANNELS; index++)
        {
            if (TRUE == CANTRCV_CHANNEL(index).CanTrcvChannelUsed)
            {
                /* initialize global variables */
                CanTrcv_WakeupReason[index] = CANTRCV_WU_RESET;
                CanTrcv_WakeupMode[index] = CANTRCV_WUMODE_DISABLE;
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
                if (TRUE == CANTRCV_CHANNEL(index).CanTrcvHwPnSupport)
                {
                    CanTrcv_PNActivation = PN_ENABLED;
                }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

                result = CanTrcv_TLE9255W_DisableWakeup(index);

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_POWER_ON_FLAG)
                if ((E_OK == result) && (TRUE == CANTRCV_CHANNEL(index).CanTrcvHwPnSupport)
                    && (NULL_PTR != CANTRCV_CHANNEL(index).CanTrcvPartialNetwork))
                {
                    if (TRUE == CANTRCV_CHANNEL(index).CanTrcvPartialNetwork->CanTrcvPowerOnFlag)
                    {
                        result = CanTrcv_TLE9255W_ReadStatus(index, CANTRCV_TLE9255W_POR, &isPorSet);

                        if ((E_OK == result) && (CANTRCV_FLAG_SET == isPorSet)
                            && (NULL_PTR != CANTRCV_CHANNEL(index).CanTrcvPorWakeupSourceRef))
                        {
                            CanTrcv_WakeupReason[index] = CANTRCV_WU_POWER_ON;
                            EcuM_SetWakeupEvent(*(CANTRCV_CHANNEL(index).CanTrcvPorWakeupSourceRef));
                            result = CanTrcv_TLE9255W_ClearStatus(index, CANTRCV_TLE9255W_POR);
                        }
                    }
                }
#endif /*  (STD_ON == CANTRCV_POWER_ON_FLAG) */

                if (E_OK == result)
                {
                    result = CanTrcv_TLE9255W_ReadStatus(index, CANTRCV_TLE9255W_SYSERR, &isSyserrSet);

                    if ((E_OK == result) && (CANTRCV_FLAG_SET == isSyserrSet)
                        && (NULL_PTR != CANTRCV_CHANNEL(index).CanTrcvSyserrWakeupSourceRef))
                    {
                        CanTrcv_WakeupReason[index] = CANTRCV_WU_BY_SYSERR;
                        EcuM_SetWakeupEvent(*(CANTRCV_CHANNEL(index).CanTrcvSyserrWakeupSourceRef));
                    }
                }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

                if (E_OK == result)
                {
                    result = CanTrcv_TLE9255W_SetOpMode(index, CANTRCV_CHANNEL(index).CanTrcvInitState);
                }
                else
                {
                    CANTRCV_DET_REPORT_RUNTIME(CANTRCV_INIT_ID, CANTRCV_E_NO_TRCV_CONTROL);
                    break;
                }
            }
        }

        if (E_OK == result)
        {
            /* set state to ACTIVE */
            CanTrcv_State = CANTRCV_ACTIVE;
        }
    }
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function sets the mode of the Transceiver to the value OpMode.
 * Service ID: 0x01
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant for different transceivers
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 OpMode: This parameter contains the desired operating mode.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request for transceiver mode change has been accepted.
 *               E_NOT_OK: the request for transceiver mode change has not been accepted
 *                         or any parameter is out of the allowed range.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode)
{
    CanTrcv_TrcvModeType curOpMode;
    Std_ReturnType result = E_OK;
    boolean isDet = FALSE;
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_POWER_ON_FLAG)
    CanTrcv_TrcvFlagStateType isPorSet;
#endif /* STD_ON == CANTRCV_POWER_ON_FLAG */
    CanTrcv_TrcvFlagStateType isSyserrSet;
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_SETOPMODE_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if (E_OK == result)
    {
        result += CanTrcv_GetOpMode(Transceiver, &curOpMode);

        if ((E_OK == result) && (curOpMode != OpMode))
        {
            switch (OpMode)
            {
            case CANTRCV_TRCVMODE_SLEEP:
                /* check whether in normal state */
                if (CANTRCV_TRCVMODE_NORMAL == curOpMode)
                {
/* in normal state, report error */
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
                    CANTRCV_DET_REPORT(CANTRCV_SETOPMODE_ID, CANTRCV_E_TRCV_NOT_STANDBY);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
                    isDet = TRUE;
                    result = E_NOT_OK;
                }
                else
                {
                    /* not in normal state, set state */
                    result = CanTrcv_TLE9255W_SetSleepMode(Transceiver);
                }
                break;
            case CANTRCV_TRCVMODE_STANDBY:
                /* check whether in sleep state */
                if (CANTRCV_TRCVMODE_SLEEP == curOpMode)
                {
/* in sleep state, report error */
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
                    CANTRCV_DET_REPORT(CANTRCV_SETOPMODE_ID, CANTRCV_E_TRCV_NOT_NORMAL);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
                    isDet = TRUE;
                    result = E_NOT_OK;
                }
                else
                {
                    /* not in sleep state, set state */
                    result = CanTrcv_TLE9255W_SetStandbyMode(Transceiver);
                    if (E_OK == result)
                    {
                        CanTrcv_WakeupReason[Transceiver] = CANTRCV_WU_ERROR;
                        if (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanTrcvIcuChannelRef)
                        {
                            Icu_17_TimerIp_EnableNotification(*(CANTRCV_CHANNEL(Transceiver).CanTrcvIcuChannelRef));
                        }
                    }
                }
                break;
            case CANTRCV_TRCVMODE_NORMAL:
                result = CanTrcv_TLE9255W_SetNormalMode(Transceiver);
                if (E_OK == result)
                {
                    CanTrcv_WakeupReason[Transceiver] = CANTRCV_WU_INTERNALLY;
                    if (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanTrcvIcuChannelRef)
                    {
                        Icu_17_TimerIp_DisableNotification(*(CANTRCV_CHANNEL(Transceiver).CanTrcvIcuChannelRef));
                    }
                }
                break;
            default:
/* input OpMode not valid */
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
                CANTRCV_DET_REPORT(CANTRCV_SETOPMODE_ID, CANTRCV_E_PARAM_TRCV_OPMODE);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
                isDet = TRUE;
                result = E_NOT_OK;
                break;
            }
        }

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_POWER_ON_FLAG)
        if ((TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
            && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork))
        {
            if ((E_OK == result) && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork->CanTrcvPowerOnFlag))
            {
                result = CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_POR, &isPorSet);
            }
            if ((E_OK == result) && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork->CanTrcvPowerOnFlag)
                && (CANTRCV_FLAG_SET == isPorSet))
            {
                result = CanTrcv_TLE9255W_PNWKSetup(Transceiver);
                if (E_OK == result)
                {
                    CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_POR);
                }
            }
        }
#endif /* (STD_ON == CANTRCV_POWER_ON_FLAG) */

        if (E_OK == result)
        {
            result += CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_SYSERR, &isSyserrSet);
        }
        if ((E_OK == result) && (CANTRCV_FLAG_CLEARED == isSyserrSet) && (CANTRCV_TRCVMODE_NORMAL == OpMode)
            && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
            && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef))
        {
            CanIf_ConfirmPnAvailability(*(CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef));
        }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

        if ((E_OK == result) && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef))
        {
            /* modify current opmode and notify CanIf */
            CanIf_TrcvModeIndication(*(CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef), OpMode);
        }
        else
        {
            if (FALSE == isDet)
            {
                CANTRCV_DET_REPORT_RUNTIME(CANTRCV_SETOPMODE_ID, CANTRCV_E_NO_TRCV_CONTROL);
            }
        }
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets the mode of the Transceiver and returns it in OpMode.
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): OpMode: Pointer to operation mode of the bus the API is applied to.
 * Return value: E_OK: the operation mode was detected.
 *               E_NOT_OK: the operation mode was not detected.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_GETOPMODE_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if (E_OK == result)
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        if (NULL_PTR == OpMode)
        {
            CANTRCV_DET_REPORT(CANTRCV_GETOPMODE_ID, CANTRCV_E_PARAM_POINTER);
            result = E_NOT_OK;
        }
        else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        {
            result = CanTrcv_TLE9255W_GetOpMode(Transceiver, OpMode);
            if (E_OK != result)
            {
                CANTRCV_DET_REPORT_RUNTIME(CANTRCV_GETOPMODE_ID, CANTRCV_E_NO_TRCV_CONTROL);
            }
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets the wakeup reason for the Transceiver and returns it in parameter Reason.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): reason: Pointer to wake up reason of the bus the API is applied to.
 * Return value: E_OK: the transceiver wakeup reason was provided.
 *               E_NOT_OK: no wake up reason is available or the service request failed due to development errors.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_GetBusWuReason(uint8 Transceiver, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_APPL_DATA) reason)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_GETBUSWUREASON_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if (E_OK == result)
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        if (NULL_PTR == reason)
        {
            CANTRCV_DET_REPORT(CANTRCV_GETBUSWUREASON_ID, CANTRCV_E_PARAM_POINTER);
            result = E_NOT_OK;
        }
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        if (E_OK == result)
        {
            if (CANTRCV_WU_ERROR == CanTrcv_WakeupReason[Transceiver])
            {
                result = E_NOT_OK;
            }
            else
            {
                *reason = CanTrcv_WakeupReason[Transceiver];
            }
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (STD_ON == CANTRCV_VERSIONINFO_API)
/**
 * This function gets the version of the module and returns it in VersionInfo.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo: Pointer to version information of this module.
 * Return value: NA
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANTRCV_CODE) CanTrcv_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTRCV_APPL_DATA) versioninfo)
{
    if (NULL_PTR == versioninfo)
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        CANTRCV_DET_REPORT(CANTRCV_GETVERSIONINFO_ID, CANTRCV_E_PARAM_POINTER);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
    }
    else
    {
        versioninfo->moduleID = CANTRCV_MODULE_ID;
        versioninfo->sw_major_version = CANTRCV_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANTRCV_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANTRCV_C_SW_PATCH_VERSION;
        versioninfo->vendorID = CANTRCV_VENDOR_ID;
    }
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* STD_ON == CANTRCV_VERSIONINFO_API */

/**
 * This function enables, disables or clears wake-up events of the Transceiver according to TrcvWakeupMode.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different transceivers
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 TrcvWakeupMode: Requested transceiver wakeup reason.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the wakeup state has been changed to the requested mode.
 *               E_NOT_OK: the wakeup state change has failed or the parameter is out of the allowed range. The previous
 * state has not been changed.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_SetWakeupMode(uint8 Transceiver, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
    boolean isDet = FALSE;
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_SETWAKEUPMODE_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if ((E_OK == result) && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed))
    {
        switch (TrcvWakeupMode)
        {
        case CANTRCV_WUMODE_ENABLE:
            result = CanTrcv_TLE9255W_EnableWakeup(Transceiver);
            CanTrcv_WakeupMode[Transceiver] = CANTRCV_WUMODE_ENABLE;
            if ((CANTRCV_WU_BY_BUS == CanTrcv_WakeupReason[Transceiver])
                && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanTrcvBusWakeupSourceRef))
            {
                EcuM_SetWakeupEvent(*(CANTRCV_CHANNEL(Transceiver).CanTrcvBusWakeupSourceRef));
            }
            break;
        case CANTRCV_WUMODE_DISABLE:
            result = CanTrcv_TLE9255W_DisableWakeup(Transceiver);
            CanTrcv_WakeupMode[Transceiver] = CANTRCV_WUMODE_DISABLE;
            break;
        case CANTRCV_WUMODE_CLEAR:
            result = CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_WK);
            if (E_OK == result)
            {
                CanTrcv_WakeupReason[Transceiver] = CANTRCV_WU_ERROR;
            }
            break;
        default:
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
            isDet = TRUE;
            CANTRCV_DET_REPORT(CANTRCV_SETWAKEUPMODE_ID, CANTRCV_E_PARAM_TRCV_WAKEUP_MODE);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
            result = E_NOT_OK;
            break;
        }

        if ((E_OK != result) && (FALSE == isDet))
        {
            CANTRCV_DET_REPORT_RUNTIME(CANTRCV_SETWAKEUPMODE_ID, CANTRCV_E_NO_TRCV_CONTROL);
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (CANTRCV_WAKEUP_BY_POLLING == CANTRCV_WAKEUP_SUPPORT)
/**
 * This is the main function of CanTrcv, which scans all busses for wake up events and perform these event.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANTRCV_CODE) CanTrcv_MainFunction(void)
{
    uint8 index;
    CanTrcv_TrcvModeType OpMode;
    Std_ReturnType result = E_OK;

    if (CANTRCV_NOT_ACTIVE == CanTrcv_State)
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        CANTRCV_DET_REPORT(CANTRCV_MAINFUNCTION_ID, CANTRCV_E_UNINIT);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */
    }
    else
    {
        for (index = 0; index < CANTRCV_MAX_CHANNELS; index++)
        {
            if ((E_OK == result) && (TRUE == CANTRCV_CHANNEL(index).CanTrcvWakeupByBusUsed)
                && (TRUE == CANTRCV_CHANNEL(index).CanTrcvChannelUsed))
            {
                result = CanTrcv_TLE9255W_GetOpMode(index, &OpMode);
                if ((E_OK == result) && (CANTRCV_TRCVMODE_NORMAL != OpMode))
                {
                    result = CanTrcv_HandleWakeupEvents(index);
                }

                if (E_OK != result)
                {
                    CANTRCV_DET_REPORT_RUNTIME(CANTRCV_MAINFUNCTION_ID, CANTRCV_E_NO_TRCV_CONTROL);
                    break;
                }
            }
        }
    }
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* CANTRCV_WAKEUP_SUPPORT == CANTRCV_WAKEUP_BY_POLLING */

/**
 * This function is called by underlying CANIF in case a wake up interrupt is detected.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: API call has been accepted
 *               E_NOT_OK: API call has not been accepted.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_CheckWakeup(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_CHECKWAKEUP_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if (E_OK == result)
    {
        result = CanTrcv_HandleWakeupEvents(Transceiver);
        if (E_OK != result)
        {
            CANTRCV_DET_REPORT_RUNTIME(CANTRCV_CHECKWAKEUP_ID, CANTRCV_E_NO_TRCV_CONTROL);
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_BUS_ERR_FLAG)
/**
 * This function reads the transceiver diagnostic status periodically and sets product/development accordingly.
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANTRCV_CODE) CanTrcv_MainFunctionDiagnostics(void)
{
    uint8 index;
    Std_ReturnType result = E_OK;
    CanTrcv_TrcvFlagStateType flagState;

    for (index = 0; index < CANTRCV_MAX_CHANNELS; index++)
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        result = CanTrcv_DetCheck(index, CANTRCV_MAINFUNCTIONDIAGNOSTICS_ID);
        if (E_OK == result)
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        {
            if ((TRUE == CANTRCV_CHANNEL(index).CanTrcvChannelUsed)
                && (TRUE == CANTRCV_CHANNEL(index).CanTrcvHwPnSupport)
                && (NULL_PTR != CANTRCV_CHANNEL(index).CanTrcvPartialNetwork)
                && (TRUE == CANTRCV_CHANNEL(index).CanTrcvPartialNetwork->CanTrcvBusErrFlag))
            {
                result = CanTrcv_TLE9255W_ReadStatus(index, CANTRCV_TLE9255W_TXD_TO, &flagState);
                if (E_OK == result)
                {
                    if (CANTRCV_FLAG_SET == flagState)
                    {
                        CANTRCV_DEM_REPORT_FAILED(CANTRCV_E_BUS_ERROR(index));
                        result = CanTrcv_TLE9255W_ClearStatus(index, CANTRCV_TLE9255W_TXD_TO);
                    }
                    else
                    {
                        CANTRCV_DEM_REPORT_PASSED(CANTRCV_E_BUS_ERROR(index));
                    }
                }
                if (E_NOT_OK == result)
                {
                    CANTRCV_DET_REPORT_RUNTIME(CANTRCV_MAINFUNCTIONDIAGNOSTICS_ID, CANTRCV_E_NO_TRCV_CONTROL);
                    break;
                }
            }
        }
    }
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* STD_ON == CANTRCV_BUS_ERR_FLAG */
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

/**
 * This function de-initializes the CanTrcv module.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANTRCV_CODE) CanTrcv_DeInit(void)
{
    uint8 index;
    Std_ReturnType result = E_OK;
    CanTrcv_TrcvModeType curOpMode;

    for (index = 0; index < CANTRCV_MAX_CHANNELS; index++)
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        result = CanTrcv_DetCheck(index, CANTRCV_DEINIT_ID);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        if ((E_OK == result) && (TRUE == CANTRCV_CHANNEL(index).CanTrcvChannelUsed))
        {
            result = CanTrcv_TLE9255W_GetOpMode(index, &curOpMode);

            if (E_OK == result)
            {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
                if (curOpMode != CANTRCV_TRCVMODE_NORMAL)
                {
                    CANTRCV_DET_REPORT(CANTRCV_DEINIT_ID, CANTRCV_E_TRCV_NOT_NORMAL);
                }
                else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
                {
                    result = CanTrcv_TLE9255W_DeInit(index);
                }
            }
            if (E_OK != result)
            {
                CANTRCV_DET_REPORT_RUNTIME(CANTRCV_DEINIT_ID, CANTRCV_E_NO_TRCV_CONTROL);
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (E_OK == result)
    {
        CanTrcv_State = CANTRCV_NOT_ACTIVE;
    }
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (STD_ON == CANTRCV_HWPN_SUPPORT)

/**
 * This function reads the transceiver configuration/status data and returns
 * it through parameter TrcvSysData. This API shall exist only if CanTrcvHwPnSupport = TRUE.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvSysData: Configuration/Status data of the transceiver.
 * Return value: E_OK: the transceiver status is successfully read.
 *               E_NOT_OK: the transceiver status data is not available or a development error occurs.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_GetTrcvSystemData(uint8 Transceiver, P2VAR(uint32, AUTOMATIC, CANTRCV_CONST) TrcvSysData)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_GETTRCVSYSTEMDATA_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if ((E_OK == result) && (PN_ENABLED == CanTrcv_PNActivation)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport))
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        if (NULL_PTR == TrcvSysData)
        {
            CANTRCV_DET_REPORT(CANTRCV_GETTRCVSYSTEMDATA_ID, CANTRCV_E_PARAM_POINTER);
            result = E_NOT_OK;
        }
        else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        {
            result = CanTrcv_TLE9255W_GetTrcvSystemData(Transceiver, TrcvSysData);
            if (E_OK != result)
            {
                CANTRCV_DET_REPORT_RUNTIME(CANTRCV_GETTRCVSYSTEMDATA_ID, CANTRCV_E_NO_TRCV_CONTROL);
            }
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears the WUF flag in the transceiver hardware.
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different transceivers
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the WUF flag has been cleared.
 *               E_NOT_OK: the WUF flag has not been cleared or a development error occurs.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_ClearTrcvWufFlag(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_CLEARTRCVWUFFLAG_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if ((E_OK == result) && (PN_ENABLED == CanTrcv_PNActivation)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport))
    {

        result = CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_WUF);
        if (E_OK != result)
        {
            CANTRCV_DET_REPORT_RUNTIME(CANTRCV_CLEARTRCVWUFFLAG_ID, CANTRCV_E_NO_TRCV_CONTROL);
        }
    }
    else
    {
        result = E_NOT_OK;
    }
    if ((E_OK == result) && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef))
    {
        CanIf_ClearTrcvWufFlagIndication(*(CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef));
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function reads the status of the timeout flag from the transceiver hardware.
 * Service ID: 0x0b
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): FlagState: State of the timeout flag.
 * Return value: E_OK: status of the timeout flag is success-fully read.
 *               E_NOT_OK: status of the timeout flag could not be read.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_ReadTrcvTimeoutFlag(uint8 Transceiver, P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_READTRCVTIMEOUTFLAG_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if ((E_OK == result) && (PN_ENABLED == CanTrcv_PNActivation)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport))
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        if (NULL_PTR == FlagState)
        {
            CANTRCV_DET_REPORT(CANTRCV_READTRCVTIMEOUTFLAG_ID, CANTRCV_E_PARAM_POINTER);
            result = E_NOT_OK;
        }
        else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        {
            result = CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_CANTO, FlagState);
            if (E_OK != result)
            {
                CANTRCV_DET_REPORT_RUNTIME(CANTRCV_READTRCVTIMEOUTFLAG_ID, CANTRCV_E_NO_TRCV_CONTROL);
            }
        }
    }
    else
    {
        result = E_NOT_OK;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears the status of the timeout flag in the transceiver hardware.
 * Service ID: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the timeout flag is successfully cleared.
 *               E_NOT_OK: the timeout flag could not be cleared.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_ClearTrcvTimeoutFlag(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_CLEARTRCVTIMEOUTFLAG_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if ((E_OK == result) && (PN_ENABLED == CanTrcv_PNActivation)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport))
    {
        result = CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_CANTO);
        if (E_OK != result)
        {
            CANTRCV_DET_REPORT_RUNTIME(CANTRCV_CLEARTRCVTIMEOUTFLAG_ID, CANTRCV_E_NO_TRCV_CONTROL);
        }
    }
    else
    {
        result = E_NOT_OK;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function reads the status of the silence flag from the transceiver hardware.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): FlagState: State of the silence flag.
 * Return value: E_OK: status of the silence flag is success-fully read.
 *               E_NOT_OK: status of the silence flag could not be read.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_ReadTrcvSilenceFlag(uint8 Transceiver, P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_READTRCVSILENCEFLAG_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if ((E_OK == result) && (PN_ENABLED == CanTrcv_PNActivation)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport))
    {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
        if (NULL_PTR == FlagState)
        {
            CANTRCV_DET_REPORT(CANTRCV_READTRCVSILENCEFLAG_ID, CANTRCV_E_PARAM_POINTER);
            result = E_NOT_OK;
        }
        else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
        {
            result = CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_CANSIL, FlagState);
            if (E_OK != result)
            {
                CANTRCV_DET_REPORT_RUNTIME(CANTRCV_READTRCVSILENCEFLAG_ID, CANTRCV_E_NO_TRCV_CONTROL);
            }
        }
    }
    else
    {
        result = E_NOT_OK;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function requests to check the status of the wakeup flag from the transceiver hardware.
 * Service ID: 0x0e
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request for checking the wakeup flag has been accepted.
 *               E_NOT_OK: the request for checking the wakeup flag has not been accepted.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_CheckWakeFlag(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    result = CanTrcv_DetCheck(Transceiver, CANTRCV_CHECKWAKEFLAG_ID);
#endif /* (STD_ON == CANTRCV_DEV_ERROR_DETECT) */

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvChannelUsed)
    {
        result = E_NOT_OK;
    }

    if (E_OK == result)
    {
        result = CanTrcv_HandleWakeupEvents(Transceiver);
        if (E_OK != result)
        {
            CANTRCV_DET_REPORT_RUNTIME(CANTRCV_CHECKWAKEFLAG_ID, CANTRCV_E_NO_TRCV_CONTROL);
        }
    }

    if ((E_OK == result) && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef))
    {
        CanIf_CheckTrcvWakeFlagIndication(*(CANTRCV_CHANNEL(Transceiver).CanIfCanTrcvIdRef));
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function configures the wake-up of the transceiver for Standby and Sleep Mode.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): ActivationState: PN_ENABLED: PN wakeup functionality in CanTrcv shall be enabled.
 *                                  PN_DISABLED: PN wakeup functionality in CanTrcv shall be disabled.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the PN has been changed to the requested configuration.
 *               E_NOT_OK: the PN configuration change has failed. The previous configuration has not been changed.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_SetPNActivationState(CanTrcv_PNActivationType ActivationState)
{
    Std_ReturnType result = E_OK;
    uint8 index;

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
    if (CANTRCV_NOT_ACTIVE == CanTrcv_State) /* check module state */
    {
        CANTRCV_DET_REPORT(CANTRCV_SETPNACTIVATIONSTATE_ID, CANTRCV_E_UNINIT);
        result = E_NOT_OK;
    }
    else
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
    {
        CanTrcv_PNActivation = ActivationState;
        for (index = 0; index < CANTRCV_MAX_CHANNELS; index++)
        {
            if ((TRUE == CANTRCV_CHANNEL(index).CanTrcvWakeupByBusUsed)
                && (TRUE == CANTRCV_CHANNEL(index).CanTrcvChannelUsed)
                && (TRUE == CANTRCV_CHANNEL(index).CanTrcvHwPnSupport))
            {
                if (PN_ENABLED == ActivationState)
                {
                    result = CanTrcv_TLE9255W_PNWKSetup(index);
                }
                else
                {
                    result = CanTrcv_TLE9255W_DisablePNWK(index);
                }

                if (E_OK != result)
                {
                    CANTRCV_DET_REPORT_RUNTIME(CANTRCV_SETPNACTIVATIONSTATE_ID, CANTRCV_E_NO_TRCV_CONTROL);
                    break;
                }
            }
        }
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* CANTRCV_HWPN_SUPPORT == STD_ON */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
/**
 * This function performs det check for transceiver index and module state.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 serviceId: the id of the service
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: det check passed
 *               E_NOT_OK: det check failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_DetCheck(uint8 Transceiver, uint8 serviceId)
{
    Std_ReturnType result = E_NOT_OK;
    if (CANTRCV_NOT_ACTIVE == CanTrcv_State) /* check module state */
    {
        CANTRCV_DET_REPORT(serviceId, CANTRCV_E_UNINIT);
    }
    else if (Transceiver >= CANTRCV_MAX_CHANNELS) /* check transceiver index */
    {
        CANTRCV_DET_REPORT(serviceId, CANTRCV_E_INVALID_TRANSCEIVER);
    }
    else
    {
        result = E_OK;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */

/**
 * This function checks and handles wakeup event in the transceiver HW.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: handling is successful
 *               E_NOT_OK: handling failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_HandleWakeupEvents(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    CanTrcv_TrcvFlagStateType FlagState;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
        if ((TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport) && (PN_ENABLED == CanTrcv_PNActivation))
        {
            result = CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_WUF, &FlagState);
        }
        else
#endif
        {
            result = CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_WUP, &FlagState);
        }

        if ((E_OK == result) && (CANTRCV_FLAG_SET == FlagState)
            && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanTrcvBusWakeupSourceRef))
        {
            CanTrcv_WakeupReason[Transceiver] = CANTRCV_WU_BY_BUS;
            if (CANTRCV_WUMODE_DISABLE != CanTrcv_WakeupMode[Transceiver])
            {
                EcuM_SetWakeupEvent(*(CANTRCV_CHANNEL(Transceiver).CanTrcvBusWakeupSourceRef));
            }
        }

        if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByPinUsed)
        {
            result = CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_LWU, &FlagState);
            if ((E_OK == result) && (CANTRCV_FLAG_SET == FlagState)
                && (NULL_PTR != CANTRCV_CHANNEL(Transceiver).CanTrcvPinWakeupSourceRef))
            {
                CanTrcv_WakeupReason[Transceiver] = CANTRCV_WU_BY_PIN;
                if (CANTRCV_WUMODE_DISABLE != CanTrcv_WakeupMode[Transceiver])
                {
                    EcuM_SetWakeupEvent(*(CANTRCV_CHANNEL(Transceiver).CanTrcvPinWakeupSourceRef));
                }
            }
        }

        if (E_OK == result)
        {
            result = CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_WK);
        }
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
