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
 **  @file               : Dem_APIImplementation.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : API Implementation of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_APIIMPLEMENTATION_H
#define DEM_APIIMPLEMENTATION_H

/* =================================================== inclusions =================================================== */
#include "Dem_Satellite.h"
#include "Dem_Master.h"
#include "Dem_Event.h"
#include "Dem_Dtc.h"
#include "Dem_ClearDTC.h"
#include "Dem_EnableCondition.h"
#include "Dem_Mem.h"
#include "Dem_Arti.h"
/* ========================================== external function definitions ========================================= */

/* PRQA S 3415 EOF */             /* VL_Dem_3415 */
/* PRQA S 3406, 1252, 1503  ++ */ /* VL_Dem_3406, VL_Dem_1252, VL_QAC_NoUsedApi */
#if (DEM_VERSION_INFO_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief Returns the version information of this module.
 */
void Dem_GetVersionInfo(Std_VersionInfoType* versionInfo)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x00, versionInfo);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Checks ***** */
    if (NULL_PTR == versionInfo)
    {
        Dem_DetReportError(DEM_SID_GETVERSIONINFO, DEM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versionInfo->vendorID         = DEM_VENDOR_ID;
        versionInfo->moduleID         = DEM_MODULE_ID;
        versionInfo->sw_major_version = DEM_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = DEM_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = DEM_SW_PATCH_VERSION;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 1, 0x00);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief Initializes the internal states necessary to process events reported by BSW-modules.
 */
void Dem_PreInit(const Dem_ConfigType* ConfigPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x01, ConfigPtr);
    Dem_MasterPreInit(ConfigPtr);
    if (Dem_CheckMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        Dem_DetReportError(DEM_SID_PREINIT, DEM_E_WRONG_CONDITION);
    }
    else
    {
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
        Dem_SatelliteIdType satelliteId;
        for (satelliteId = 0u; satelliteId < Dem_GetNumOfSatellite(); ++satelliteId)
        {
            if (DEM_MASTER_APPLICATION_ID == Dem_GetSatelliteApplicationId(satelliteId))
            {
                break;
            }
        }
        if (satelliteId >= Dem_GetNumOfSatellite())
        {
            Dem_DetReportError(DEM_SID_PREINIT, DEM_E_WRONG_CONFIGURATION);
        }
        else
        {
            Dem_SatelliteSatPreInit(satelliteId);
        }
#else
        Dem_SatelliteSatPreInit(DEM_SATELLITE_APPLICATION_ZERO);
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 1, 0x01);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief Initializes or reinitializes this module.
 */
void Dem_Init(const Dem_ConfigType* ConfigPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x02, ConfigPtr);
    if (Dem_CheckMasterInitStatus(DEM_SHUTDOWN) == TRUE)
    {
        Dem_MasterInit(ConfigPtr);
    }
    else
    {
        Dem_MasterInit(ConfigPtr);
        /** Dem_SatelliteInit is called directly for initialization in case of single partition, or executed by the
         * corresponding partition in case of multiple partitions. */
        Dem_SatelliteInit();
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 1, 0x02);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/**
 * @brief Shuts down this module.
 */
void Dem_Shutdown(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 1, 0x03);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_SetMasterInitStatus(DEM_SHUTDOWN);

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
        Dem_MemStorageDataDebounce();
#endif

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        Dem_NvmShutdown();
#endif
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SHUTDOWN, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 1, 0x03);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#if (DEM_SUPPORT_CLEARDTC_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief Clears single DTCs, as well as groups of DTCs.
 */
Std_ReturnType Dem_ClearDTC(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x23, ClientId);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        switch (Dem_GetClientResult(ClientId))
        {
        case DEM_CLIENT_RESULT_DTC:
        case DEM_CLIENT_RESULT_GROUP:
        case DEM_CLIENT_RESULT_ALL_DTCS:
            ret = Dem_ClientClearDTC(ClientId);
            break;
        case DEM_CLIENT_RESULT_NO_MATCH:
            ret = DEM_WRONG_DTC;
            break;
        case DEM_CLIENT_RESULT_WRONG_ORIGIN:
            ret = DEM_WRONG_DTCORIGIN;
            break;
        case DEM_CLIENT_RESULT_INIT:
            /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            errorId = DEM_E_WRONG_CONDITION;
#endif
            break;
        default:
            /** E_NOT_OK */
            break;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_CLEARDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x23, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Clears a prestored freeze frame of a specific event. This API can only be used through the RTE
 *                and therefore no declaration is exported via Dem.h. API Availability: This API will be available
 *                only if ({ecuc(Dem/DemConfigSet/DemEventParameter.DemFFPrestorageSupported)} == true)
 */
Std_ReturnType Dem_ClearPrestoredFreezeFrame(Dem_EventIdType EventId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x07, EventId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckLeastSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_EventClearPrestoreFF(EventId);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_CLEARPRESTOREDFREEZEFRAME, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x07, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_COMPONENT_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the failed status of a DemComponent.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType Dem_GetComponentFailed(Dem_ComponentIdType ComponentId, boolean* ComponentFailed)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x2A, ComponentId, ComponentFailed);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(ComponentId);
    DEM_PARAM_UNUSED(ComponentFailed);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x2A, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (STD_ON == DCM_UDS_0X14)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs.
 */
Std_ReturnType Dem_GetDTCSelectionResult(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xB8, ClientId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientGetDTCSelectionResult(
            ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCSELECTIONRESULT, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB8, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (STD_ON == DCM_UDS_0X14)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs,
 * respecting the settings if the Dem shall clear only all DTCs.
 */
Std_ReturnType Dem_GetDTCSelectionResultForClearDTC(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xBB, ClientId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientResultForClearDTCGetDTCSelection(
            ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCSELECTIONRESULTFORCLEARDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xBB, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the current UDS status byte assigned to the DTC for the event
 */
Std_ReturnType Dem_GetEventUdsStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType* UDSStatusByte)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xB6, EventId, UDSStatusByte);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (UDSStatusByte == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
        ret = Dem_EventGetUDSStatus(EventId, UDSStatusByte);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETEVENTUDSSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB6, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the current monitor status for an event.
 */
Std_ReturnType Dem_GetMonitorStatus(Dem_EventIdType EventId, Dem_MonitorStatusType* MonitorStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xB5, EventId, MonitorStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
#else
    if (Dem_CheckLeastMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
#endif
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (MonitorStatus == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
#if (DEM_TRIGGER_FIM_REPORTS == STD_OFF)
        if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == TRUE)
#endif
        {
            /* return not ok, may be after shutdown be call by fim in cycle mode */
            ret = Dem_MonitorGetStatus(EventId, MonitorStatus);
        }
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETMONOTORSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB5, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the debouncing status of an event. This function shall not be used for EventId with native
 *                debouncing within their functions. It is rather for EventIds using debouncing within the Dem.
 */
Std_ReturnType Dem_GetDebouncingOfEvent(Dem_EventIdType EventId, Dem_DebouncingStateType* DebouncingState)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x9F, EventId, DebouncingState);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (DebouncingState == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
        ret = Dem_SatelliteGetDebouncingState(EventId, DebouncingState);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDEBOUNCINGEVENT, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x9F, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the DTC of an event.
 */
Std_ReturnType Dem_GetDTCOfEvent(Dem_EventIdType EventId, Dem_DTCFormatType DTCFormat, uint32* DTCOfEvent)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x0D, EventId, DTCFormat, DTCOfEvent);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (DTCFormat > DEM_DTC_FORMAT_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (DTCOfEvent == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
        ret = Dem_GetDTCByEvent(EventId, DTCFormat, DTCOfEvent);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCOFEVENT, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x0D, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Returns the suppression status of a specific DTC. API Availability: This API will be available
 *                only if ({ecuc(Dem/DemGeneral.DemSuppressionSupport)} == DEM_DTC_SUPPRESSION)
 */
Std_ReturnType Dem_GetDTCSuppression(uint8 ClientId, boolean* SuppressionStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xBC, ClientId, SuppressionStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (SuppressionStatus == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        ret = Dem_ClientGetDTCSuppression(
            ClientId,
            SuppressionStatus
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCSUPPRESSION, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xBC, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Get the Event availability.
 */
Std_ReturnType Dem_GetEventAvailable(Dem_EventIdType EventId, boolean* AvailableStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xBE, EventId, AvailableStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (AvailableStatus == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
        {
            *AvailableStatus = TRUE;
        }
        else
        {
            *AvailableStatus = FALSE;
        }
        ret = E_OK;
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETEVENTAVAILABLE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xBE, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the fault detection counter of an event. This API can only be used through the RTE, and
 *                therefore no declaration is exported via Dem.h.
 */
Std_ReturnType Dem_GetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x3E, EventId, FaultDetectionCounter);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (FaultDetectionCounter == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else
#endif
    {
        ret = Dem_SatelliteGetFaultDetectionCounter(EventId, FaultDetectionCounter);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETFAULTDETECTIONCOUNTER, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x3E, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#if (DEM_INDICATOR_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the indicator status derived from the UDS status. API Availability: This API will be
 *                available only if ({ecuc(Dem/DemGeneral/DemEventMemorySet/DemIndicator)} != NULL)
 */
Std_ReturnType Dem_GetIndicatorStatus(uint8 IndicatorId, Dem_IndicatorStatusType* IndicatorStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x29, IndicatorId, IndicatorStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (IndicatorId >= (uint8)DEM_INDICATOR_NUMBER)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (IndicatorStatus == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        *IndicatorStatus = Dem_GetStatusOfIndicator(IndicatorId);
        ret              = E_OK;
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETINDICATORSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x29, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the data of a freeze frame by event.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType Dem_GetEventFreezeFrameDataEx(
    Dem_EventIdType EventId,
    uint8           RecordNumber,
    uint16          DataId,
    uint8*          DestBuffer,
    uint16*         BufSize)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 6, 0x6E, EventId, RecordNumber, DataId, DestBuffer, BufSize);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_EnvDataSwcApiGetEventFreezeFrameDataEx(EventId, RecordNumber, DataId, DestBuffer, BufSize);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETEVENTFREEZEFRAMEDATAEX, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x6E, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the data of an extended data record by event.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType
    Dem_GetEventExtendedDataRecordEx(Dem_EventIdType EventId, uint8 RecordNumber, uint8* DestBuffer, uint16* BufSize)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 5, 0x6D, EventId, RecordNumber, DestBuffer, BufSize);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_EnvDataSwcApiGetEventExtendedDataEx(EventId, RecordNumber, DestBuffer, BufSize);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETEVENTEXTENDEDDATARECORDEX, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x6D, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_USER_SUPPORT_OVFLIND_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the event memory overflow indication status.
 */
Std_ReturnType Dem_GetEventMemoryOverflow(uint8 ClientId, Dem_DTCOriginType DTCOrigin, boolean* OverflowIndication)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x32, ClientId, DTCOrigin, OverflowIndication);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (OverflowIndication == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientGetEventMemoryOverflow(ClientId, DTCOrigin, OverflowIndication);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETEVENTMEMORYOVERFLOW, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x32, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Returns the number of entries currently stored in the requested event memory.
 */
Std_ReturnType
    Dem_GetNumberOfEventMemoryEntries(uint8 ClientId, Dem_DTCOriginType DTCOrigin, uint8* NumberOfEventMemoryEntries)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x35, ClientId, DTCOrigin, NumberOfEventMemoryEntries);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (NumberOfEventMemoryEntries == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientGetNumberOfEventMemoryEntries(ClientId, DTCOrigin, NumberOfEventMemoryEntries);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x35, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Control the internal debounce counter/timer by BSW modules and SW-Cs. The event qualification
 *                will not be affected by these debounce state changes. This API is available for BSW modules as
 *                soon as Dem_PreInit has been completed (refer to SWS_Dem_00438 and SWS_Dem_00167).
 */
/* PRQA S 6070,6030 ++ */ /* VL_MTR_Dem_STCAL,VL_MTR_Dem_STMIF */
Std_ReturnType Dem_ResetEventDebounceStatus(Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x09, EventId, DebounceResetStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (
        (Dem_CheckSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_PREINITIALIZED) == FALSE)
        && (Dem_CheckSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_INITIALIZED) == FALSE))
    {
        errorId = DEM_E_UNINIT;
    }
    else if (DebounceResetStatus > (Dem_DebounceResetStatusType)DEM_DEBOUNCE_STATUS_RESET)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
    else if ((Dem_GetDebounceStorageByEvent(EventId) == TRUE) && (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
        {
            if (DebounceResetStatus == (Dem_DebounceResetStatusType)DEM_DEBOUNCE_STATUS_RESET)
            {
                Dem_SatelliteResetDebounceStatus(EventId);
            }
            else
            {
                Dem_SatelliteFreezeDebounceStatus(EventId);
            }
            ret = E_OK;
        }
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_RESETEVENTDEBOUNCESTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x09, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 6070,6030 -- */

/**
 * @brief         Resets the event failed status. This API can only be used through the RTE and therefore no
 *                declaration is exported via Dem.h.
 */
Std_ReturnType Dem_ResetEventStatus(Dem_EventIdType EventId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x05, EventId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_SatelliteResetEventStatus(EventId);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_RESETEVENTSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x05, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Resets the monitor failed status.
 */
Std_ReturnType Dem_ResetMonitorStatus(Dem_EventIdType EventId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xC5, EventId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_SatelliteResetMonitorStatus(EventId);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_RESETMONITORSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xC5, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets an operation cycle state. This API can only be used through the RTE and therefore no
 *                declaration is exported via Dem.h. The interface has an asynchronous behavior to avoid exceeding
 *                of typical timing requirements on APIs if a large number of events has to be processed and
 *                during the re-initializations of the related monitors. The asynchronous acknowledgements are the
 *                related InitMonitorForEvent callbacks.
 */
Std_ReturnType Dem_RestartOperationCycle(uint8 OperationCycleId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x08, OperationCycleId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckLeastMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (OperationCycleId >= (uint8)DEM_OPERATION_CYCLE_NUMBER)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_OperationCycleSetReStart(OperationCycleId);
        ret = E_OK;
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_RESTARTOPERATIONCYCLE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x08, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Captures the freeze frame data for a specific event. This API can only be used through the RTE
 *                and therefore no declaration is exported via Dem.h. API Availability: This API will be available
 *                only if ({ecuc(Dem/DemConfigSet/DemEvent Parameter.DemFFPrestorageSupported)} == true)
 */
Std_ReturnType Dem_PrestoreFreezeFrame(Dem_EventIdType EventId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x06, EventId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckLeastSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_EventPrestoreFF(EventId);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_PRESTOREFREEZEFRAME, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x06, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_SELECTDTC_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Selects a DTC or DTC group as target for further operations.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
Std_ReturnType Dem_SelectDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 5, 0xB7, ClientId, DTC, DTCFormat, DTCOrigin);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    /* PRQA S 1258 ++ */                                /* VL_Dem_1258 */
    else if (DTC > DEM_DTC_MAX_VALUE) /* PRQA S 3673 */ /* VL_QAC_3673 */
    {
        errorId = DEM_E_PARAM_DATA;
    }
    /* PRQA S 1258 -- */
    else if (DTCFormat > DEM_DTC_FORMAT_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
#if (DEM_DCM_CLIENT_NUMBER > 0u)
        (void)Dem_ClientEnableDTCRecordUpdate(ClientId);
#endif
        Dem_ClientSelectDTC(ClientId, DTC, DTCFormat, DTCOrigin);
        ret = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SELECTDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB7, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 6030 -- */
#endif

#if (DEM_COMPONENT_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Set the availability of a specific DemComponent.
 */
Std_ReturnType Dem_SetComponentAvailable(Dem_ComponentIdType ComponentId, boolean AvailableStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x2B, ComponentId, AvailableStatus);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(ComponentId);
    DEM_PARAM_UNUSED(AvailableStatus);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x2B, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Set the suppression status of a specific DTC. API Availability: This API will be available only
 *                if ({ecuc(Dem/DemGeneral.DemSuppressionSupport)} == DEM_DTC_SUPPRESSION)
 */
Std_ReturnType Dem_SetDTCSuppression(uint8 ClientId, boolean SuppressionStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x33, ClientId, SuppressionStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        switch (Dem_GetClientResult(ClientId))
        {
        case DEM_CLIENT_RESULT_DTC:
            ret = Dem_ClientSetDTCSuppression(ClientId, SuppressionStatus);
            break;
        case DEM_CLIENT_RESULT_GROUP:
        case DEM_CLIENT_RESULT_ALL_DTCS:
        case DEM_CLIENT_RESULT_NO_MATCH:
            ret = DEM_WRONG_DTC;
            break;
        case DEM_CLIENT_RESULT_WRONG_ORIGIN:
            ret = DEM_WRONG_DTCORIGIN;
            break;
        case DEM_CLIENT_RESULT_INIT:
            /** E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            errorId = DEM_E_WRONG_CONDITION;
#endif
            break;
        default:
            /** E_NOT_OK */
            break;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETDTCSUPPRESSION, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x33, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets an enable condition. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral/DemEnableCondition)} != NULL)
 */
Std_ReturnType Dem_SetEnableCondition(uint8 EnableConditionID, boolean ConditionFulfilled)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x39, EnableConditionID, ConditionFulfilled);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckLeastMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (EnableConditionID > (uint8)DEM_ENABLE_CONDITION_ID_NUMBER)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if (ConditionFulfilled == TRUE)
        {
            Dem_SetEnableConditionFulfilled(EnableConditionID);
        }
        else
        {
            Dem_ResetEnableConditionFulfilled(EnableConditionID);
        }
        ret = E_OK;
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETENABLECONDITION, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x39, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Set the available status of a specific Event.
 */
Std_ReturnType Dem_SetEventAvailable(Dem_EventIdType EventId, boolean AvailableStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x37, EventId, AvailableStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckLeastMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if (Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
        {
            if (AvailableStatus == TRUE)
            {
                Dem_EventEnable(EventId);
                ret = E_OK;
            }
            else
            {
                ret = Dem_EventDisable(EventId);
            }
        }
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETEVENTAVAILABLE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x37, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Set the failure confirmation threshold of an event.
 */
Std_ReturnType Dem_SetEventConfirmationThresholdCounter(Dem_EventIdType EventId, uint8 FailureCycleCounterThreshold)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x57, EventId, FailureCycleCounterThreshold);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckLeastMasterInitStatus(DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckEventThrAdapt(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if ((Dem_GetAvailableInVariantByEvent(EventId) == TRUE)
            && (Dem_GetEventAvailableStatusByEventId(EventId) == TRUE))
        {
            if ((Dem_GetConfirmationThresholdByEvent(EventId) < FailureCycleCounterThreshold)
                && (Dem_DTCCheckStoredStatus(EventId) == TRUE))
            {
                /** If Dem_SetEventConfirmationThresholdCounter is called and the corresponding event is already stored
                 * in event memory and the new failure threshold is less or equal to the current failure counter, then
                 * the event will get confirmed. This implies, that the event related data will also be updated
                 * according their defined triggers. */
                Dem_EventConfirmed(EventId);
            }
            Dem_SetEventConfirmationThreshold(EventId, FailureCycleCounterThreshold);
            ret = E_OK;
        }
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETEVENTCONFIRMATIONTHRESHOLDCOUNTER, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x57, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_EVENT_REPORTING_STANDARD == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Called by SW-Cs or BSW modules to report monitor status information to the Dem. BSW modules
 *                calling Dem_SetEventStatus can safely ignore the return value. This API will be available only
 *                if ({Dem/DemConfigSet/DemEventParameter/DemEventReportingType} == STANDARD_REPORTING)
 */
Std_ReturnType Dem_SetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x04, EventId, EventStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckLeastSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (EventStatus >= DEM_EVENT_STATUS_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckEventReportingType(EventId) == TRUE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
    else if ((Dem_GetDebounceStorageByEvent(EventId) == TRUE) && (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_SatelliteSetEventStatus(EventId, EventStatus);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETEVENTSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x04, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_EVENT_REPORTING_WITH_MONITOR == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         This API will be available only if ({Dem/DemConfigSet/DemEventParameter/DemEventReportingType}
 *                == STANDARD_REPORTING_WITH_MONITOR_DATA)
 */
Std_ReturnType Dem_SetEventStatusWithMonitorData(
    Dem_EventIdType     EventId,
    Dem_EventStatusType EventStatus,
    Dem_MonitorDataType monitorData0,
    Dem_MonitorDataType monitorData1)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 5, 0xBD, EventId, EventStatus, monitorData0, monitorData1);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckLeastSatelliteInitStatus(Dem_GetSatelliteIdByEventId(EventId), DEM_PREINITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (EventStatus >= DEM_EVENT_STATUS_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_CheckEventReportingType(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
    else if ((Dem_GetDebounceStorageByEvent(EventId) == TRUE) && (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (Dem_SatelliteCheckEventPartition(EventId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_SatelliteSetEventStatus(EventId, EventStatus);
        if ((Std_ReturnType)E_OK == ret)
        {
            Dem_SatelliteSetMonitorData(EventId, monitorData0, monitorData1);
        }
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETEVENTSTATUSWITHMONITORDATA, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xBD, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets a storage condition.API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral/DemStorageCondition)} != NULL)
 */
Std_ReturnType Dem_SetStorageCondition(uint8 StorageConditionID, boolean ConditionFulfilled)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x38, StorageConditionID, ConditionFulfilled);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckLeastMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (StorageConditionID > (uint8)DEM_STORAGE_CONDITION_GROUP_NUMBER)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if (ConditionFulfilled == TRUE)
        {
            Dem_SetStorageConditionFulfilled(StorageConditionID);
        }
        else
        {
            Dem_ResetStorageConditionFulfilled(StorageConditionID);
        }
        ret = E_OK;
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETSTORAGECONDITION, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x38, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_USER_CONTROLLED_WIR == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets the WIR status bit via failsafe SW-Cs. This API can only be used through the RTE and
 *                therefore no declaration is exported via Dem.h.
 */
Std_ReturnType Dem_SetWIRStatus(Dem_EventIdType EventId, boolean WIRStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x7A, EventId, WIRStatus);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckLeastMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_CheckEventIdInValid(EventId) == TRUE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_DTCSetWIRStatus(EventId, WIRStatus);
    }

#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETWIRSTATUS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x7A, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETTRANSLATIONTYPE_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the supported DTC formats of the ECU.The supported formats are configured via Dem
 *                TypeOfDTCSupported.
 */
Dem_DTCTranslationFormatType Dem_GetTranslationType(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x3C, ClientId);
    Dem_DTCTranslationFormatType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = (Dem_DTCTranslationFormatType)0;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_GetDtcTypeOfMemSet(Dem_GetMemorySetRefOfClient(ClientId));
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETTRANSLATIONTYPE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x3C, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETDTCSTATUSAVAILABILITYMASK_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the DTC Status availability mask of the selected fault memory.
 */
Std_ReturnType
    Dem_GetDTCStatusAvailabilityMask(uint8 ClientId, Dem_UdsStatusByteType* DTCStatusMask, Dem_DTCOriginType DTCOrigin)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x16, ClientId, DTCOrigin, DTCStatusMask);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCStatusMask == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientGetDTCStatusAvailabilityMask(ClientId, DTCStatusMask, DTCOrigin);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCSTATUSAVAILABILITYMASK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x16, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETSTATUSOFDTC_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the status of a DTC. For large configurations and DTC-calibration, the interface behavior
 *                can be asynchronous (splitting the DTC-search into segments). The DTCs of OBD Events Suppression
 *                shall be reported as Dem_WRONG_DTC.
 */
Std_ReturnType Dem_GetStatusOfDTC(uint8 ClientId, uint8* DTCStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x15, ClientId, DTCStatus);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCStatus == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        ret = Dem_ClientGetStatusOfDTC(
            ClientId,
            DTCStatus
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETSTATUSOFDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x15, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETSEVERITYOFDTC_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the severity of the requested DTC. For large configurations and DTC-calibration, the
 *                interface behavior can be asynchronous (splitting the DTC-search into segments).
 */
Std_ReturnType Dem_GetSeverityOfDTC(Dem_DTCSeverityType* DTCSeverity, uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x0E, DTCSeverity, ClientId);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCSeverity == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        ret = Dem_ClientGetSeverityOfDTC(
            ClientId,
            DTCSeverity
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETSEVERITYOFDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x0E, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the functional unit of the requested DTC.
 */
Std_ReturnType Dem_GetFunctionalUnitOfDTC(uint8 ClientId, uint8* DTCFunctionalUnit)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x34, DTCFunctionalUnit, ClientId);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCFunctionalUnit == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        ret = Dem_ClientGetFunctionalUnitOfDTC(
            ClientId,
            DTCFunctionalUnit
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETFUNCTIONALUNITOFDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x34, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_SETDTCFILTER_AND_GETNUMBER_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets the DTC Filter. The server shall perform a bit-wise logical AND-ing operation between the
 *                parameter DTCStatusMask and the current UDS status in the server. In addition to the DTCStatus
 *                AvailabilityMask, the server shall return all DTCs for which the result of the AND-ing operation
 *                is non-zero [i.e. (statusOfDTC & DTCStatusMask) != 0]. The server shall process only the DTC
 *                Status bits that it is supporting. OBD Events Suppression shall be ignored for this computation.
 *                If no DTCs within the server match the masking criteria specified in the client request, no
 *                DTC or status information shall be provided following the DTCStatusAvailabilityMask byte in the
 *                positive response message
 *                (((statusOfDTC & DTCStatusMask) != 0) && ((severity & DTCSeverityMask) != 0)) == TRUE
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Dem_STPAR */
Std_ReturnType Dem_SetDTCFilter(
    uint8               ClientId,
    uint8               DTCStatusMask,
    Dem_DTCFormatType   DTCFormat,
    Dem_DTCOriginType   DTCOrigin,
    boolean             FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean             FilterForFaultDetectionCounter)
{
    ARTI_TRACE_N(
        USER,
        AR_CP_BSW_API,
        Dem,
        0,
        Bsw_Start,
        8,
        0x13,
        ClientId,
        DTCStatusMask,
        DTCFormat,
        DTCOrigin,
        FilterWithSeverity,
        DTCSeverityMask,
        FilterForFaultDetectionCounter);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCFormat > DEM_DTC_FORMAT_INVALID)
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (Dem_DTCOriginCheckValid(DTCOrigin) == FALSE)
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientSetDTCFilter(
            ClientId,
            DTCStatusMask,
            DTCFormat,
            DTCOrigin,
            FilterWithSeverity,
            DTCSeverityMask,
            FilterForFaultDetectionCounter);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETDTCFILTER, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x13, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 6040 -- */

/**
 * @brief         Gets the number of a filtered DTC.
 */
Std_ReturnType Dem_GetNumberOfFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x17, ClientId, NumberOfFilteredDTC);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (NumberOfFilteredDTC == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_DTC) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        *NumberOfFilteredDTC = Dem_ClientFilterNumberMemory(ClientId);
        ret                  = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNUMBEROFFILTEREDDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x17, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETNEXT_FILTEREDDTC_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the next filtered DTC matching the filter criteria. For UDS services, the interface has an
 *                asynchronous behavior, because a large number of DTCs has to be processed.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Dem_STMIF */
Std_ReturnType Dem_GetNextFilteredDTC(uint8 ClientId, uint32* DTC, uint8* DTCStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x18, ClientId, DTC, DTCStatus);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((DTCStatus == NULL_PTR) || (DTC == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_DTC) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_FilterDataResultType filterResult;
        ret = Dem_ClientGetNextFilteredDTC(ClientId, &filterResult);
        if (ret == (Std_ReturnType)E_OK)
        {
            /** Set out parameter only if a filtered DTC was found*/
            *DTC       = filterResult.DTC;
            *DTCStatus = filterResult.DTCStatus;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNEXTFILTEREDDTC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x18, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETNEXT_FILTEREDDTCANDFDC_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the next filtered DTC and its associated Fault Detection Counter (FDC) matching the filter
 *                criteria. The interface has an asynchronous behavior, because a large number of DTCs has to be
 *                processed and the FDC might be received asynchronously from a SW-C, too.
 */
Std_ReturnType Dem_GetNextFilteredDTCAndFDC(uint8 ClientId, uint32* DTC, sint8* DTCFaultDetectionCounter)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x3B, ClientId, DTC, DTCFaultDetectionCounter);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((DTCFaultDetectionCounter == NULL_PTR) || (DTC == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_DTC) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_FilterDataResultType filterResult;
        ret = Dem_ClientGetNextFilteredDTC(ClientId, &filterResult);
        if (ret == (Std_ReturnType)E_OK)
        {
            /** Set out parameter only if a filtered DTC was found*/
            *DTC                      = filterResult.DTC;
            *DTCFaultDetectionCounter = filterResult.FDC;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNEXTFILTEREDDTCANDFDC, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x3B, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETNEXT_FILTEREDDTCANDSEVERITY_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the next filtered DTC and its associated Severity matching the filter criteria. The
 *                interface has an asynchronous behavior, because a large number of DTCs has to be processed.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType Dem_GetNextFilteredDTCAndSeverity(
    uint8                ClientId,
    uint32*              DTC,
    uint8*               DTCStatus,
    Dem_DTCSeverityType* DTCSeverity,
    uint8*               DTCFunctionalUnit)
{
    ARTI_TRACE_N(
        USER,
        AR_CP_BSW_API,
        Dem,
        0,
        Bsw_Start,
        6,
        0x3D,
        ClientId,
        DTC,
        DTCStatus,
        DTCSeverity,
        DTCFunctionalUnit);
    /* PRQA S 3673 -- */
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (
        (DTCStatus == NULL_PTR) || (DTC == NULL_PTR) || (DTCSeverity == NULL_PTR) || (DTCFunctionalUnit == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCFilterSet(ClientId, DEM_FILTERDATA_FILTER_DTC) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        Dem_FilterDataResultType filterResult;
        ret = Dem_ClientGetNextFilteredDTC(ClientId, &filterResult);
        if (ret == (Std_ReturnType)E_OK)
        {
            /** Set out parameter only if a filtered DTC was found*/
            *DTC               = filterResult.DTC;
            *DTCStatus         = filterResult.DTCStatus;
            *DTCSeverity       = filterResult.Severity;
            *DTCFunctionalUnit = filterResult.FunctionalUnit;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNEXTFILTEREDDTCANDSEVERITY, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x3D, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 6030 -- */
#endif
#if (DEM_SUPPORT_SETFREEZEFRAMERECORD_AND_GET_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets a freeze frame record filter.
 */
Std_ReturnType Dem_SetFreezeFrameRecordFilter(uint8 ClientId, Dem_DTCFormatType DTCFormat)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x3F, ClientId, DTCFormat);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (
        (DTCFormat != (Dem_DTCFormatType)DEM_DTC_FORMAT_UDS)
#if (DEM_OBDII_SUPPORT == STD_ON)
#if (DEM_SUPPORTED_OBD_UDS_DTC_SEPARATION == STD_ON)
        && (DTCFormat != (Dem_DTCFormatType)DEM_DTC_FORMAT_OBD_3BYTE)
#else
        && (DTCFormat != (Dem_DTCFormatType)DEM_DTC_FORMAT_OBD)
#endif
#endif
#if (DEM_J1939_SUPPORT == STD_ON)
        && (DTCFormat != (Dem_DTCFormatType)DEM_DTC_FORMAT_J1939)
#endif
    )
    {
        errorId = DEM_E_PARAM_DATA;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        ret = Dem_ClientStartFreezeFrameIterator(ClientId, DTCFormat);
#else
        ret = E_OK;
#endif
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SETFREEZEFRAMERECORDFILTER, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x3F, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         This function returns the number of all freeze frame records currently stored in the primary
 *                event memory
 */
Std_ReturnType Dem_GetNumberOfFreezeFrameRecords(uint8 ClientId, uint16* NumberOfFilteredRecords)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x5A, ClientId, NumberOfFilteredRecords);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (NumberOfFilteredRecords == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckFreezeFrameFilterSet(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        *NumberOfFilteredRecords = Dem_ClientGetNumberOfFreezeFrameRecords(ClientId);
#else
        *NumberOfFilteredRecords = 0;
#endif
        ret = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNUMBEROFFREEZEFRAMERECORDS, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x5A, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the next freeze frame record number and its associated DTC stored in the event memory. The
 *                interface has an asynchronous behavior, because NvRAM access might be required.
 */
Std_ReturnType Dem_GetNextFilteredRecord(uint8 ClientId, uint32* DTC, uint8* RecordNumber)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x3A, ClientId, DTC, RecordNumber);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((DTC == NULL_PTR) || (RecordNumber == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckFreezeFrameFilterSet(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
#if (DEM_MAX_SIZE_FREEZEFRAME > 0u)
        ret = Dem_ClientGetNextFreezeFrameRecord(ClientId, DTC, RecordNumber);
#else
        ret = DEM_NO_SUCH_ELEMENT;
#endif
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNEXTFILTEREDRECORD, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x3A, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_GETDTCOCCURRENCETIME_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the DTC by occurrence time. There is no explicit parameter for the DTC-origin as the origin
 *                always is DEM_DTC_ORIGIN_PRIMARY_MEMORY.
 */
Std_ReturnType Dem_GetDTCByOccurrenceTime(uint8 ClientId, Dem_DTCRequestType DTCRequest, uint32* DTC)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x19, ClientId, DTCRequest, DTC);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Checks ***** */
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((DTCRequest < DEM_DTC_REQUEST_FIRST_VALID) || (DTCRequest > DEM_DTC_REQUEST_INVALID))
    {
        errorId = DEM_E_PARAM_DATA;
    }
    else if (DTC == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientGetDTCByOccurrenceTime(ClientId, DTCRequest, DTC);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCBYOCCURRENCETIME, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x19, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_DISABLE_ENABLE_DTCRECORD_UPDATE_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Disables the event memory update of a specific DTC (only one at one time).
 */
Std_ReturnType Dem_DisableDTCRecordUpdate(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x1A, ClientId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_GetClientResult(ClientId) == DEM_CLIENT_RESULT_INIT)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        ret = Dem_ClientDisableDTCRecordUpdate(
            ClientId
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_DISABLEDTCRECORDUPDATE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x1A, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Enables the event memory update of the DTC disabled by Dem_DisableDTCRecordUpdate() before.
 *                Note: As the call to Dem_EnableDTCRecordUpdate is the last action in the sequence of
 *                disabling/enabling a DTC, the caller is not interested in the return value. Therefore E_OK
 *                should be returned even if the DTC is enabled asynchronously.
 */
Std_ReturnType Dem_EnableDTCRecordUpdate(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x1B, ClientId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        ret = Dem_ClientEnableDTCRecordUpdate(ClientId);
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_ENABLEDTCRECORDUPDATE, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x1B, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_EXTENDEDDATARECORD_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets the filter to be used by Dem_GetNextExtendedDataRecord and
 *                Dem_GetSizeOfExtendedDataRecordSelection.
 */
Std_ReturnType Dem_SelectExtendedDataRecord(uint8 ClientId, uint8 ExtendedDataNumber)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xBA, ClientId, ExtendedDataNumber);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCRecordUpdateRequested(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        ret = Dem_ClientSelectExtendedDataRecord(
            ClientId,
            ExtendedDataNumber
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SELSECTEXTENDEDDATARECORD, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xBA, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the size of Extended Data Record by DTC selected by the call of Dem_SelectExtended DataRecord.
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Dem_STMIF,VL_MTR_Dem_STCAL */
Std_ReturnType Dem_GetSizeOfExtendedDataRecordSelection(uint8 ClientId, uint32* SizeOfExtendedDataRecord)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x21, ClientId, SizeOfExtendedDataRecord);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (SizeOfExtendedDataRecord == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (Dem_GetClientResult(ClientId) == DEM_CLIENT_RESULT_INIT)
        || (Dem_ClientCheckDTCRecordUpdateRequested(ClientId) == FALSE)
        || (Dem_ClientCheckExtendedRecordSelected(ClientId) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        if (Dem_ClientCheckDTCRecordUpdateDisabled(ClientId) == TRUE)
        {
            ret = Dem_ClientGetSizeOfExtendedDataRecordSelection(ClientId, SizeOfExtendedDataRecord);
        }
        else
        {
            ret = DEM_PENDING;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETSIZEOFEXTENDEDDATARECORDSELECTION, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x21, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets extended data record for the DTC selected by Dem_SelectExtendedDataRecord. The function
 *                stores the data in the provided DestBuffer.
 */
Std_ReturnType Dem_GetNextExtendedDataRecord(uint8 ClientId, uint8* DestBuffer, uint16* BufSize)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x20, ClientId, DestBuffer, BufSize);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (Dem_GetClientResult(ClientId) == DEM_CLIENT_RESULT_INIT)
        || (Dem_ClientCheckDTCRecordUpdateRequested(ClientId) == FALSE)
        || (Dem_ClientCheckExtendedRecordSelected(ClientId) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        if (Dem_ClientCheckDTCRecordUpdateDisabled(ClientId) == TRUE)
        {
            ret = Dem_ClientGetNextExtendedDataRecord(ClientId, DestBuffer, BufSize);
        }
        else
        {
            ret = DEM_PENDING;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNEXTEXTENDEDDATARECORD, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x20, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 6030,6070 -- */
#endif
#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_FREEZEFRAMEDATA_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets the filter to be used by Dem_GetNextFreezeFrameData and Dem_GetSizeOfFreezeFrame Selection.
 */
Std_ReturnType Dem_SelectFreezeFrameData(uint8 ClientId, uint8 RecordNumber)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xB9, ClientId, RecordNumber);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (Dem_ClientCheckDTCRecordUpdateRequested(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
        if (Dem_CheckClientLocked(ClientId) == TRUE)
    {
        ret = DEM_BUSY;
    }
    else
    {
        ret = Dem_ClientSelectFirstSource(
            ClientId,
            RecordNumber
#if (DEM_DEV_ERROR_DETECT == STD_ON)
            ,
            &errorId
#endif
        );
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_SELECTFREEZEFRAMEDATA, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB9, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the size of Extended Data Record by DTC selected by the call of Dem_SelectExtended DataRecord.
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Dem_STMIF,VL_MTR_Dem_STCAL */
Std_ReturnType Dem_GetSizeOfFreezeFrameSelection(uint8 ClientId, uint32* SizeOfFreezeFrame)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0x1F, ClientId, SizeOfFreezeFrame);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (SizeOfFreezeFrame == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (Dem_GetClientResult(ClientId) == DEM_CLIENT_RESULT_INIT)
        || (Dem_ClientCheckDTCRecordUpdateRequested(ClientId) == FALSE)
        || (Dem_ClientCheckFFRecordSelected(ClientId) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        if (Dem_ClientCheckDTCRecordUpdateDisabled(ClientId) == TRUE)
        {
            ret = Dem_ClientGetSizeOfFFSelection(ClientId, SizeOfFreezeFrame);
        }
        else
        {
            ret = DEM_PENDING;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETSIZEOFFREEZEFRAMESELECTION, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x1F, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets extended data record for the DTC selected by Dem_SelectExtendedDataRecord. The function
 *                stores the data in the provided DestBuffer.
 */
Std_ReturnType Dem_GetNextFreezeFrameData(uint8 ClientId, uint8* DestBuffer, uint16* BufSize)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 4, 0x1D, ClientId, DestBuffer, BufSize);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (Dem_GetClientResult(ClientId) == DEM_CLIENT_RESULT_INIT)
        || (Dem_ClientCheckDTCRecordUpdateRequested(ClientId) == FALSE)
        || (Dem_ClientCheckFFRecordSelected(ClientId) == FALSE))
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
    else
#endif
    {
        if (Dem_ClientCheckDTCRecordUpdateDisabled(ClientId) == TRUE)
        {
            ret = Dem_ClientGetNextFreezeFrameData(ClientId, DestBuffer, BufSize);
        }
        else
        {
            ret = DEM_PENDING;
        }
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETNEXTFREEZEFRAMEDATA, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x1D, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/* PRQA S 6030,6070 -- */

#if (STD_ON == DCM_UDS_0X85)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Disables the DTC setting for all DTCs assigned to the DemEventMemorySet of the addressed client.
 */
Std_ReturnType Dem_DisableDTCSetting(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x24, ClientId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_DTCSettingDisable(ClientId);
        ret = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_DISABLEDTCSETTING, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x24, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         (Re)-Enables the DTC setting for all DTCs assigned to the DemEventMemorySet of the addressed client.
 */
Std_ReturnType Dem_EnableDTCSetting(uint8 ClientId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x25, ClientId);
    Std_ReturnType ret;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    ret           = E_NOT_OK;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        Dem_DTCSettingEnable(ClientId);
        ret = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_ENABLEDTCSETTING, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x25, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if (DEM_OBD_READINESS_GROUP_HANDING == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service for reporting the event as disabled to the Dem for the PID $41 computation. API is
 *                needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_SetEventDisabled(Dem_EventIdType EventId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x51, EventId);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(EventId);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x51, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_RATIO_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service for reporting that faults are possibly found because all conditions are fullfilled. API
 *                is needed in OBD-relevant ECUs only API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_RepIUMPRFaultDetect(Dem_RatioIdType RatioID)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x73, RatioID);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(RatioID);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x73, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         In order to communicate the status of the (additional) denominator conditions among the OBD
 *                relevant ECUs, the API is used to forward the condition status to a Dem of a particular ECU.
 *                API is needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType
    Dem_SetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType ConditionStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xAE, ConditionId, ConditionStatus);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(ConditionId);
    DEM_PARAM_UNUSED(ConditionStatus);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xAE, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         In order to communicate the status of the (additional) denominator conditions among the OBD
 *                relevant ECUs, the API is used to forward the condition status to a Dem of a particular ECU.
 *                API is needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType
    Dem_GetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType* ConditionStatus)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xAF, ConditionId, ConditionStatus);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(ConditionId);
    DEM_PARAM_UNUSED(ConditionStatus);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xAF, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service is used to release a denominator of a specific monitor. API is needed in OBD-relevant
 *                ECUs only API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_RepIUMPRDenRelease(Dem_RatioIdType RatioID)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x72, RatioID);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(RatioID);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x72, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_USER_SUPPORT_SET_PTO_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         API is needed in OBD-relevant ECUs only API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_SetPtoStatus(boolean PtoStatus)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x79, PtoStatus);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PtoStatus);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x79, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to report the value of PID $01 computed by the Dem. API is needed in OBD relevant ECUs only
 */
Std_ReturnType Dem_ReadDataOfPID01(uint8* PID01value) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xB3, PID01value);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID01value);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB3, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#if ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) && (DEM_OBD_CENTRALIZED_PID21_HANDING == STD_ON))
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to get the value of PID $21 from the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral/DemGeneral OBD.DemOBDCentralizedPID21Handling)} == true) &&
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 */
Std_ReturnType Dem_GetDataOfPID21(uint8* PID21value) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xB1, PID21value);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID21value);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB1, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#if (DEM_USER_SUPPORT_SET_PID_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to set the value of PID $21 in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_SetDataOfPID21(const uint8* PID21value)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xA6, PID21value);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID21value);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xA6, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to set the value of PID $31 in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_SetDataOfPID31(const uint8* PID31value)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xA7, PID31value);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID31value);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xA7, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to set the value of PID $4D in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_SetDataOfPID4D(const uint8* PID4Dvalue)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xA8, PID4Dvalue);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID4Dvalue);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xA8, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to set the value of PID $4E in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_SetDataOfPID4E(const uint8* PID4Evalue)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xA9, PID4Evalue);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID4Evalue);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xA9, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Returns the qualification state of the dependent operation cycle. API Availability: This API
 *                will be available only if any of the
 *                ({ecuc(Dem/DemGeneral/DemOperationCycle.DemLeadingCycleRef)} != NULL)
 */
Std_ReturnType Dem_GetCycleQualified(uint8 OperationCycleId, boolean* isQualified) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xAB, OperationCycleId, isQualified);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(OperationCycleId);
    DEM_PARAM_UNUSED(isQualified);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xAB, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Sets a dependent operation cycle as qualified, so it may be processed along with its leading cycle.
 */
Std_ReturnType Dem_SetCycleQualified(uint8 OperationCycleId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0x56, OperationCycleId);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(OperationCycleId);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0x56, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#if (DEM_DCM_CLIENT_NUMBER > 0u)
#if (DEM_SUPPORT_GETDTCSERVERITYAVAILABILITYMASK_API == STD_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Gets the DTC Severity availability mask.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
Std_ReturnType Dem_GetDTCSeverityAvailabilityMask(uint8 ClientId, Dem_DTCSeverityType* DTCSeverityMask)
/* PRQA S 3673 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 3, 0xB2, ClientId, DTCSeverityMask);
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = DEM_E_NO_ERROR;
    /** ***** Development Error Checks ***** */
    if (Dem_CheckMasterInitStatus(DEM_INITIALIZED) == FALSE)
    {
        errorId = DEM_E_UNINIT;
    }
    else if (Dem_ClientCheckDcmClientIdValid(ClientId) == FALSE)
    {
        errorId = DEM_E_WRONG_CONFIGURATION;
    }
    else if (DTCSeverityMask == NULL_PTR)
    {
        errorId = DEM_E_PARAM_POINTER;
    }
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
    else if (GetApplicationID() != DEM_MASTER_APPLICATION_ID)
    {
        errorId = DEM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        *DTCSeverityMask = DEM_SEVERITY_MASK;
        ret              = E_OK;
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /** ***** Development Error Report ***** */
    if (errorId != DEM_E_NO_ERROR)
    {
        Dem_DetReportError(DEM_SID_GETDTCSEVERITYAVAILABILITYMASK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB2, ret);
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif
#if (DEM_OBDII_SUPPORT == STD_ON)
#if (DEM_DTR_NUMBER > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Service to report the value of the B1 counter computed by the Dem. API is needed in WWH-OBD
 *                relevant ECUs only
 */
Std_ReturnType Dem_GetB1Counter(uint16* B1Counter) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 2, 0xB4, B1Counter);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(B1Counter);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xB4, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Reports a DTR result with lower and upper limit. The internal eventstatus serves as master
 *                whether the DTR values are forwarded or ignored, also taking the DTRUpdateKind into account.
 *                The EventId that is related to the DTR is assigned per configuration (and derived from
 *                ServiceNeeds). Processing takes enable/storage conditions into account. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType
    Dem_SetDTR(uint16 DTRId, sint32 TestResult, sint32 LowerLimit, sint32 UpperLimit, Dem_DTRControlType Ctrlval)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 6, 0xA2, DTRId, TestResult, LowerLimit, UpperLimit, Ctrlval);
    /** API not yet implemented */
    DEM_PARAM_UNUSED(DTRId);
    DEM_PARAM_UNUSED(TestResult);
    DEM_PARAM_UNUSED(LowerLimit);
    DEM_PARAM_UNUSED(UpperLimit);
    DEM_PARAM_UNUSED(Ctrlval);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 2, 0xA2, E_NOT_OK);
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief         Processes all not event based Dem internal functions.
 */
void Dem_MainFunction(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Start, 1, 0x55);
    /** Dem_SatelliteMainFunction is called directly in case of single partition */
    Dem_SatelliteMainFunction();
    Dem_MasterMainFunction();
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Dem, 0, Bsw_Return, 1, 0x55);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/* PRQA S 1532 -- */
/* PRQA S 3406, 1252, 1503 -- */

#endif
