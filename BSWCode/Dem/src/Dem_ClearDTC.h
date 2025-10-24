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
 **  @file               : Dem_ClearDTC.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : Implementation file of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_CLEARDTC_H
#define DEM_CLEARDTC_H

/* =================================================== inclusions =================================================== */
#include "Dem_Client.h"
#include "Dem_MasterInterface.h"

/* ===================================================== macros ===================================================== */
/**
 * @name DEM_CLEAR_REQUEST_TYPE
 * @{
 * @brief Identification of clear request type
 */
#define DEM_CLEAR_REQUEST_TYPE_NONE      (0u)    /**< no clear request */
#define DEM_CLEAR_REQUEST_TYPE_ALL       (1u)    /**< clear all DTC request */
#define DEM_CLEAR_REQUEST_TYPE_GROUP     (2u)    /**< clear group DTC request */
#define DEM_CLEAR_REQUEST_TYPE_SINGLE    (3u)    /**< clear single DTC request */
#define DEM_CLEAR_REQUEST_TYPE_ALL_J1939 (4u)    /**< clear all DTC request */
#define DEM_CLEAR_CODE_NONE              (0xffu) /**< no clear code */
/** @} */

/**
 * @name DEM_CLEARTASK
 * @{
 * @brief Macros encoding the state of the current clear operation
 */
#define DEM_CLEARTASK_IDLE          (0x00u) /**< No clear operation in progress */
#define DEM_CLEARTASK_NEW           (0x01u) /**< Clear operation started */
#define DEM_CLEARTASK_INPROGRESS    (0x02u) /**< Clear operation ongoing */
#define DEM_CLEARTASK_DONE          (0x03u) /**< Clear operation finished */
#define DEM_CLEARTASK_FAILED        (0x04u) /**< Clear operation failed */
#define DEM_CLEARTASK_NVERROR       (0x05u) /**< Write error during clear operation */
#define DEM_CLEARTASK_NV_INPROGRESS (0x06u) /**< Wait for NV operation to finish */
/** @} */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Clear DTC request structure definition
 */
typedef struct Dem_ClearRequestTag /* PRQA S 1536 */ /* VL_Dem_1536 */
{
    Dem_EventIdType   ClearEventId;  /**< Specific EventId to clear @range 0..DEM_MAX_EVENT_ID */
    Dem_MemoryNumType ClearMemIndex; /**< Memory Index to clear @range 0..DEM_MAX_MEMORY_NUMBER */
    uint8             ClearClient;   /**< Client requesting the clear @range 0..DEM_MAX_CLIENT_ID */
    uint8             ClearType;     /**< clear request type @range 0..DEM_CLEAR_REQUEST_TYPE_ALL_J1939 */
#if (DEM_DTC_GROUP_NUMBER > 0u)
    Dem_GroupOfDTCNumType ClearGroupDTCIndex; /**< DTC group to clear @range 0..DEM_DTC_GROUP_NUMBER */
#endif
    Dem_UdsStatusByteType ClearCode; /**< Event status code to clear @range 0..255 */
    Dem_UdsStatusByteType ClearMask; /**< Event status mask to clear @range 0..255 */
} Dem_ClearRequestType;

/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Notification after clear has finished.
 * @param[in]     ClearState: the state of the current clear operation
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotiClearResult(uint8 ClearState);

/**
 * @brief         Set the status of the current clear operation.
 * @param[in]     State: The new status of the current clear operation
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_SetClearState(uint8 State);

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
/**
 * @brief         Notify all cleared blocks are persisted in NV
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_NotifyClearCompleted(void);
#endif

/**
 * @brief         Try to start a clear DTC operation
 * @param[in]     ClearRequest: The ClearDTC request
 * @return        boolean
 * @retval        TRUE: Clear request is successful
 * @retval        FALSE: Clear request is failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_TryNewClear(const Dem_ClearRequestType* ClearRequest);

/**
 * @brief         Try to start a clear DTC operation
 * @param[in]     ClearRequest: The ClearDTC request
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_StartClearOperation(const Dem_ClearRequestType* ClearRequest, uint8 ClientId);

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief         Start clear J1939 DTCs
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCTypeFilter: Defines the type of DTCs to be cleared.
 * @return        Std_ReturnType
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientStartClearJ1939(uint8 ClientId, Dem_J1939DcmSetClearFilterType DTCTypeFilter);
#endif

/**
 * @brief         Start clear DTC for a single event
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist or clearing is restricted by
 *                              configuration to group of all DTCs only.
 * @retval        DEM_CLEAR_FAILED: DTC clearing failed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientStartClearDTC(uint8 ClientId);

/**
 * @brief         Clear the selected DTC/DTC Group/All DTCs
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: DTC successfully cleared
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @retval        DEM_CLEAR_FAILED: DTC clearing failed
 * @retval        DEM_CLEAR_MEMORY_ERROR: An error occurred during erasing a memory location (e.g. if
 *                                        DemClearDTCBehavior is set to DEM_CLRRESP_NON-VOLATILE_FINISH and erasing
 *                                        of non-volatile-block failed).
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL Std_ReturnType Dem_ClientClearDTC(uint8 ClientId);

/**
 * @brief         Clears a DTC
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_DTCInternalCleared(Dem_EventIdType EventId);

/**
 * @brief         Clears an event from the given origin memory, if allowed
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        boolean
 * @retval        TRUE: Clearance of event is allowed
 * @retval        FALSE: Clearance of event is not allowed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_DTCCleared(Dem_EventIdType EventId);

/**
 * @brief         Process UDS clear group dtc requests
 * @param[in]     FirstEvent: Numerically smallest eventid belonging to the range
 * @param[in]     LastEvent: Numerically largest eventid belonging to the range
 * @return        boolean
 * @retval        TRUE: Clear sucessful
 * @retval        FALSE: Clear not sucessful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_ProcessClearGroupDtc(Dem_EventIdType FirstEvent, Dem_EventIdType LastEvent);

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief         Process J1939 clear dtc requests
 * @param[in]     FirstEvent: Numerically smallest eventid belonging to the range
 * @param[in]     LastEvent: Numerically largest eventid belonging to the range
 * @return        boolean
 * @retval        TRUE: Clear sucessful
 * @retval        FALSE: Clear not sucessful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL boolean Dem_ProcessClearJ1939Dtc(Dem_EventIdType FirstEvent, Dem_EventIdType LastEvent);
#endif

/**
 * @brief         Process UDS clear all dtc requests
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ProcessClearAllDtc(void);

/**
 * @brief         Processes clear dtc requests
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ProcessClearDtc(void);

/**
 * @brief         Process the current clear requests
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL void Dem_ClearTaskTask(void);

/* ============================================ internal data definitions =========================================== */
#define DEM_START_SEC_VAR_CLEARED_8
#include "Dem_MemMap.h"
/**
 * @brief State of clear requests
 * @range 0..6
 */
DEM_LOCAL uint8 Dem_ClearTaskState;
#define DEM_STOP_SEC_VAR_CLEARED_8
#include "Dem_MemMap.h"

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
#define DEM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dem_MemMap.h"
/**
 * @brief Flag to indicate write errors during clear
 * @range 0..1
 */
DEM_LOCAL boolean Dem_ClearWriteErrors;
#define DEM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"
/**
 * @brief ClearRequest data
 * @range NA
 */
DEM_LOCAL Dem_ClearRequestType Dem_CurrentProcessClearRequest;
#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dem_MemMap.h"

#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
#define DEM_START_SEC_VAR_CLEARED_16
#include "Dem_MemMap.h"
/**
 * @brief Run-time limitation state for clear requests
 * @range 0..65535
 */
DEM_LOCAL uint16 Dem_ClearTaskIterator;
#define DEM_STOP_SEC_VAR_CLEARED_16
#include "Dem_MemMap.h"
#endif
/* ========================================== internal function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
/**
 * @brief         Check the write errors flag of the current clear operation.
 * @return        boolean
 * @retval        TRUE: Write errors flag set
 * @retval        FALSE: Write errors flag not set
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckClearWriteErrors(void)
{
    return Dem_ClearWriteErrors;
}

/**
 * @brief         Set the write errors flag of the current clear operation.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_SetClearWriteErrors(void)
{
    Dem_ClearWriteErrors = TRUE;
}

/**
 * @brief         Reset the write errors flag of the current clear operation.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ResetClearWriteErrors(void)
{
    Dem_ClearWriteErrors = FALSE;
}
#endif

/**
 * @brief         Get the mem Index of the current clear operation.
 * @return        Dem_MemoryNumType: Memory Index to clear
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_MemoryNumType Dem_GetClearRequestMemIndex(void)
{
    return Dem_CurrentProcessClearRequest.ClearMemIndex;
}

/**
 * @brief         Get the Clear Type of the current clear operation.
 * @return        uint8: Clear request type
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE uint8 Dem_GetClearRequestClearType(void)
{
    return Dem_CurrentProcessClearRequest.ClearType;
}

/**
 * @brief         Get the Clear Code of the current clear operation.
 * @return        Dem_UdsStatusByteType: Event status code to clear
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetClearRequestClearCode(void)
{
    return Dem_CurrentProcessClearRequest.ClearCode;
}

/**
 * @brief         Get the Clear Mask of the current clear operation.
 * @return        Dem_UdsStatusByteType: Event status mask to clear
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Dem_UdsStatusByteType Dem_GetClearRequestClearMask(void)
{
    return Dem_CurrentProcessClearRequest.ClearMask;
}

#if (DEM_CLEAR_DTC_START_NOTIFICATION_NUMBER > 0u)
/**
 * @brief         Call all clear start notification callbacks.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the input-format of the provided DTC value.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_NotiClearDTCStart(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    uint8 memorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
    for (Dem_ClearDTCNotiNumType start = Dem_GetCDSNStartOfMemSet(memorySetRef);
         start < Dem_GetCDSNEndOfMemSet(memorySetRef);
         ++start)
    {
        const DemClearDTCNotificationType fnc = Dem_GetFncOfClearDTCStartNoti(start);
        if (fnc != NULL_PTR)
        {
            (void)fnc(DTC, DTCFormat, DTCOrigin);
        }
    }
}

/**
 * @brief         Call all clear start notification callbacks.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ClearDTCStartNotifications(uint8 ClientId)
{
    Dem_ClientRequestType clientIdRequest;
    clientIdRequest = Dem_GetClientRequest(ClientId);
    Dem_NotiClearDTCStart(ClientId, clientIdRequest.DTC, clientIdRequest.DTCFormat, clientIdRequest.DTCOrigin);
}
#endif

#if (DEM_CLEAR_DTC_END_NOTIFICATION_NUMBER > 0u)
/**
 * @brief         Call all clear end notification callbacks.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the input-format of the provided DTC value.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void
    Dem_NotiClearDTCEnd(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    uint8 memorySetRef = Dem_GetMemorySetRefOfClient(ClientId);
    for (Dem_ClearDTCNotiNumType start = Dem_GetCDENStartOfMemSet(memorySetRef);
         start < Dem_GetCDENEndOfMemSet(memorySetRef);
         ++start)
    {
        const DemClearDTCNotificationType fnc = Dem_GetFncOfClearDTCEndNoti(start);
        if (fnc != NULL_PTR)
        {
            (void)fnc(DTC, DTCFormat, DTCOrigin);
        }
    }
}
#endif

/**
 * @brief         Call all clear end notification callbacks.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ClearDTCEndNotifications(uint8 ClientId)
{
#if (DEM_CLEAR_DTC_END_NOTIFICATION_NUMBER > 0u)
    Dem_ClientRequestType clientIdRequest;
    clientIdRequest = Dem_GetClientRequest(ClientId);
    Dem_NotiClearDTCEnd(ClientId, clientIdRequest.DTC, clientIdRequest.DTCFormat, clientIdRequest.DTCOrigin);
#endif
    Dem_ReleaseClient(ClientId);
}

/**
 * @brief         Notification after clear has finished.
 */
DEM_LOCAL void Dem_NotiClearResult(uint8 ClearState)
{
    uint8 clientId = Dem_CurrentProcessClearRequest.ClearClient;
    if (Dem_GetClientClearDTCStatus(clientId) != DEM_CLEARDTC_STATUS_IDLE)
    {
        switch (ClearState)
        {
        case DEM_CLEARTASK_INPROGRESS:
#if (DEM_CLEAR_DTC_START_NOTIFICATION_NUMBER > 0u)
            Dem_ClearDTCStartNotifications(clientId);
#endif
            break;
        case DEM_CLEARTASK_DONE:
            Dem_ClearDTCEndNotifications(clientId);
            Dem_SetClientClearDTCStatus(clientId, DEM_CLEARDTC_STATUS_DONE);
#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
            Dem_ClearTaskIterator = DEM_EVENT_INVALID;
#endif
            break;
        case DEM_CLEARTASK_FAILED:
            Dem_ClearDTCEndNotifications(clientId);
            Dem_SetClientClearDTCStatus(clientId, DEM_CLEARDTC_STATUS_FAILED);
#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
            Dem_ClearTaskIterator = DEM_EVENT_INVALID;
#endif
            break;
        case DEM_CLEARTASK_NVERROR:
            Dem_ClearDTCEndNotifications(clientId);
            Dem_SetClientClearDTCStatus(clientId, DEM_CLEARDTC_STATUS_NV_ERROR);
#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
            Dem_ClearTaskIterator = DEM_EVENT_INVALID;
#endif
            break;
        case DEM_CLEARTASK_IDLE:
        default:
            /** Mirsa C */
            break;
        }
    }
}

/**
 * @brief         Set the status of the current clear operation.
 */
DEM_LOCAL void Dem_SetClearState(uint8 State)
{
    switch (State)
    {
    case DEM_CLEARTASK_IDLE:
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
        Dem_ResetClearWriteErrors();
#endif
        Dem_DisableTask(Dem_Task_ClearTask);
        break;
    case DEM_CLEARTASK_NEW:
        Dem_EnableTask(Dem_Task_ClearTask);
        break;
    default:
        /* idle */
        break;
    }

    Dem_NotiClearResult(State);
    Dem_ClearTaskState = State;
}

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
/**
 * @brief         Notify all cleared blocks are persisted in NV
 */
DEM_LOCAL void Dem_NotifyClearCompleted(void)
{
    if (Dem_ClearTaskState == DEM_CLEARTASK_NV_INPROGRESS)
    {
        if (Dem_CheckClearWriteErrors() == TRUE)
        {
            Dem_SetClearState(DEM_CLEARTASK_NVERROR);
        }
        else
        {
            Dem_SetClearState(DEM_CLEARTASK_DONE);
        }
    }
}
#endif

/**
 * @brief         Initialize the Clear Task
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE void Dem_ClearTaskInit(void)
{
    Dem_SetClearState(DEM_CLEARTASK_IDLE);
}

/**
 * @brief         Try to start a clear DTC operation
 */
DEM_LOCAL boolean Dem_TryNewClear(const Dem_ClearRequestType* ClearRequest)
{
    boolean ret = FALSE;

    if (Dem_ClearTaskState == DEM_CLEARTASK_IDLE)
    {
        /** Enter Critical Section: DcmApi */
        Dem_EnterCritical_DcmApi();
        if (Dem_ClearTaskState == DEM_CLEARTASK_IDLE)
        {
            Dem_CurrentProcessClearRequest = *ClearRequest;
            Dem_SetClearState(DEM_CLEARTASK_NEW);
            ret = TRUE;
        }
        Dem_LeaveCritical_DcmApi();
        /** Leave Critical Section: DcmApi */
    }
    return ret;
}

/**
 * @brief         Try to start a clear DTC operation
 */
DEM_LOCAL Std_ReturnType Dem_StartClearOperation(const Dem_ClearRequestType* ClearRequest, uint8 ClientId)
{
    Std_ReturnType ret;

    if (Dem_TryNewClear(ClearRequest) == TRUE)
    {
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_PENDING);
        Dem_LockClient(ClientId);
        ret = DEM_PENDING;
    }
    else
    {
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
        ret = DEM_PENDING;
    }
    return ret;
}

#if (DEM_CLEAR_DTC_LIMITATION == DEM_ALL_SUPPORTED_DTCS)
/**
 * @brief         Start clear single DTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientStartClearEvent(uint8 ClientId)
{
    Std_ReturnType       ret;
    Dem_ClearRequestType clearRequest;
    clearRequest.ClearClient  = ClientId;
    clearRequest.ClearEventId = Dem_ClientInfo[ClientId].Selection.EventId;

    clearRequest.ClearType     = DEM_CLEAR_REQUEST_TYPE_SINGLE;
    clearRequest.ClearCode     = DEM_CLEAR_CODE_NONE;
    clearRequest.ClearMask     = 0x00u;
    clearRequest.ClearMemIndex = Dem_ClientInfo[ClientId].MemIndex;
#if (DEM_DTC_GROUP_NUMBER > 0u)
    clearRequest.ClearGroupDTCIndex = DEM_CLIENT_GROUP_INVALID;
#endif
    if (Dem_CheckDTCSuppressed(clearRequest.ClearEventId) == TRUE)
    {
        ret = DEM_WRONG_DTC;
    }
    else
    {
        ret = Dem_StartClearOperation(&clearRequest, ClientId);
    }
    return ret;
}
#endif

#if ((DEM_DTC_GROUP_NUMBER > 0u) && (DEM_CLEAR_DTC_LIMITATION == DEM_ALL_SUPPORTED_DTCS))
/**
 * @brief         Start clear group DTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientStartClearGroup(uint8 ClientId)
{
    Dem_ClearRequestType clearRequest;
    clearRequest.ClearClient        = ClientId;
    clearRequest.ClearEventId       = DEM_EVENT_INVALID;
    clearRequest.ClearType          = DEM_CLEAR_REQUEST_TYPE_GROUP;
    clearRequest.ClearCode          = DEM_CLEAR_CODE_NONE;
    clearRequest.ClearMask          = 0x00u;
    clearRequest.ClearMemIndex      = Dem_ClientInfo[ClientId].MemIndex;
    clearRequest.ClearGroupDTCIndex = Dem_ClientInfo[ClientId].Selection.GroupDTCIndex;
    return Dem_StartClearOperation(&clearRequest, ClientId);
}

/**
 * @brief         Get if a DTC number encodes a specific DTC group (not ALLDTCs).
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     GroupId: Identification of DTC Group.
 * @return        boolean
 * @retval        TRUE: Dtc Group Match
 * @retval        FALSE: Dtc Group not Match
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_CheckDtcGroupMatch(Dem_EventIdType EventId, Dem_GroupOfDTCNumType GroupId)
{
    boolean match = FALSE;
    uint32  DTC   = Dem_GetEventDTC(EventId);
    if (DTC != DEM_DTC_UDS_INVALID) /* PRQA S 1258, 1252 */ /* VL_Dem_1258, VL_Dem_1252 */
    {
        Dem_GroupOfDTCNumType nextGroupId = GroupId + 1u;
        uint32                GroupDTC    = Dem_GetDTCOfGroupDTCs(GroupId);
        if (((DTC >= GroupDTC) && (nextGroupId == DEM_DTC_GROUP_NUMBER))
            || ((DTC >= GroupDTC) && (nextGroupId < DEM_DTC_GROUP_NUMBER)
                && (DTC < Dem_GetDTCOfGroupDTCs(nextGroupId))))
        {
            match = TRUE;
        }
    }
    return match;
}
#endif

/**
 * @brief         Start clear all DTC
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE Std_ReturnType Dem_ClientStartClearAllDTCs(uint8 ClientId)
{
    Dem_ClearRequestType clearRequest;
    clearRequest.ClearClient   = ClientId;
    clearRequest.ClearEventId  = DEM_EVENT_INVALID;
    clearRequest.ClearType     = DEM_CLEAR_REQUEST_TYPE_ALL;
    clearRequest.ClearCode     = DEM_CLEAR_CODE_NONE;
    clearRequest.ClearMask     = 0x00u;
    clearRequest.ClearMemIndex = Dem_ClientInfo[ClientId].MemIndex;
#if (DEM_DTC_GROUP_NUMBER > 0u)
    clearRequest.ClearGroupDTCIndex = DEM_CLIENT_GROUP_INVALID;
#endif
    return Dem_StartClearOperation(&clearRequest, ClientId);
}

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief         Start clear J1939 DTCs
 */
DEM_LOCAL Std_ReturnType Dem_ClientStartClearJ1939(uint8 ClientId, Dem_J1939DcmSetClearFilterType DTCTypeFilter)
{
    Dem_ClearRequestType clearRequest;
    clearRequest.ClearClient   = ClientId;
    clearRequest.ClearEventId  = DEM_EVENT_INVALID;
    clearRequest.ClearType     = DEM_CLEAR_REQUEST_TYPE_ALL_J1939;
    clearRequest.ClearMemIndex = Dem_ClientInfo[ClientId].MemIndex;
#if (DEM_DTC_GROUP_NUMBER > 0u)
    clearRequest.ClearGroupDTCIndex = DEM_CLIENT_GROUP_INVALID;
#endif

    if (DTCTypeFilter == DEM_J1939DTC_CLEAR_ACTIVE)
    {
        /** Clear all active DTCs */
        clearRequest.ClearCode = (uint8)(DEM_UDS_STATUS_CDTC | DEM_UDS_STATUS_TF);
        clearRequest.ClearMask = (uint8)(DEM_UDS_STATUS_CDTC | DEM_UDS_STATUS_TF);
    }
    else if (DTCTypeFilter == DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE)
    {
        /** Clear previously active DTCs */
        clearRequest.ClearCode = (uint8)(DEM_UDS_STATUS_CDTC);
        clearRequest.ClearMask = (uint8)(DEM_UDS_STATUS_CDTC | DEM_UDS_STATUS_TF);
    }
    else
    {
        /** Clear active and previously active DTCs */
        clearRequest.ClearCode = (uint8)(DEM_UDS_STATUS_CDTC);
        clearRequest.ClearMask = (uint8)(DEM_UDS_STATUS_CDTC);
    }
    return Dem_StartClearOperation(&clearRequest, ClientId);
}
#endif

/**
 * @brief         Start clear DTC for a single event
 */
DEM_LOCAL Std_ReturnType Dem_ClientStartClearDTC(uint8 ClientId)
{
    Std_ReturnType ret = DEM_WRONG_DTC;
    switch (Dem_GetClientResult(ClientId))
    {
    case DEM_CLIENT_RESULT_ALL_DTCS:
        ret = Dem_ClientStartClearAllDTCs(ClientId);
        break;
#if (DEM_DTC_GROUP_NUMBER > 0u)
    case DEM_CLIENT_RESULT_GROUP:
#if (DEM_CLEAR_DTC_LIMITATION == DEM_ALL_SUPPORTED_DTCS)
        ret = Dem_ClientStartClearGroup(ClientId);
#endif
        break;
#endif
    case DEM_CLIENT_RESULT_DTC:
#if (DEM_CLEAR_DTC_LIMITATION == DEM_ALL_SUPPORTED_DTCS)
        ret = Dem_ClientStartClearEvent(ClientId);
#endif
        break;
    case DEM_CLIENT_RESULT_NO_MATCH:
    case DEM_CLIENT_RESULT_WRONG_ORIGIN:
    case DEM_CLIENT_RESULT_INIT:
    default:
        /** cannot reach: cases already handled by caller */
        ret = DEM_CLEAR_FAILED;
        break;
    }
    return ret;
}

/**
 * @brief         Clear the selected DTC/DTC Group/All DTCs
 */
DEM_LOCAL Std_ReturnType Dem_ClientClearDTC(uint8 ClientId)
{
    Std_ReturnType ret = DEM_CLEAR_FAILED;
    switch (Dem_GetClientClearDTCStatus(ClientId))
    {
    case DEM_CLEARDTC_STATUS_IDLE:
        ret = Dem_ClientStartClearDTC(ClientId);
        break;
    case DEM_CLEARDTC_STATUS_PENDING:
        ret = DEM_PENDING;
        break;
    case DEM_CLEARDTC_STATUS_DONE:
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
        ret = E_OK;
        break;
    case DEM_CLEARDTC_STATUS_FAILED:
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
        /** return DEM_CLEAR_FAILED */
        break;
    case DEM_CLEARDTC_STATUS_NV_ERROR:
        Dem_SetClientClearDTCStatus(ClientId, DEM_CLEARDTC_STATUS_IDLE);
        ret = DEM_CLEAR_MEMORY_ERROR;
        break;
    default:
        /** return DEM_CLEAR_FAILED */
        break;
    }
    return ret;
}

/**
 * @brief         Calls user defined clear allowed callback
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        boolean
 * @retval        TRUE: Clearance of event is allowed
 * @retval        FALSE: Clearance of event is not allowed
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ClearEventAllowed(Dem_EventIdType EventId)
{
    boolean clearAllowed = TRUE;
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
    Dem_ClearEventFncNumType clearEventAllowedRef = Dem_GetCEARefOfEvent(EventId);
    if (clearEventAllowedRef != DEM_EVENT_CALLBACK_CLEAR_ALLOWED)
    {
        const DemClearEventAllowedType fnc = Dem_GetCEAFncOfEvent(clearEventAllowedRef);
        if (fnc != NULL_PTR)
        {
            if ((*fnc)(&clearAllowed) != E_OK)
            {
                /** Make sure a failed call does not influence lDoClear */
                clearAllowed = TRUE;
            }
        }
    }
#else
    DEM_PARAM_UNUSED(EventId);
#endif
    return clearAllowed;
}

/**
 * @brief         Clears a DTC
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_DTCInternalCleared(Dem_EventIdType EventId)
{
    Dem_CombinedArgType   lArgs;
    Dem_UdsStatusByteType oldDtcStatus;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
#endif

    Dem_EventInternalStatusType internalStatus = Dem_GetDtcInterStatus(EventId);
    if (Dem_GetEventstoreStatus(internalStatus) != Dem_InternalStoredStatus_None)
    {
        Dem_MemStateInfoConstPtrType memoryInfo  = Dem_MemStateInfoInit(Dem_GetMemRef(Dem_GetDTCAttr(EventId)));
        Dem_NvBlockNumType           memoryIndex = Dem_MemoryFindIndex(memoryInfo, EventId);
        /** Remove the event from the memory */
        Dem_MemoryFreeIndex(memoryInfo, memoryIndex);
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
        Dem_NvmSetSingleBlockState(memoryIndex, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
#endif
    }

#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        /** Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();
        Dem_SetCombinedEventStatusByGroupId(
            groupId,
            Dem_SetEventstoredStatus(internalStatus, Dem_InternalStoredStatus_None));
        Dem_LeaveCritical_DiagMonitor();
        /** Leave Critical Section: DiagMonitor */
    }
#endif

    oldDtcStatus = Dem_DTCApplyExternalStatus(EventId, Dem_GetDTCUDSStatus(EventId));
    Dem_CombinedEvent(EventId, Dem_EventCleared, &lArgs);

    Dem_NotiDtcStatusChanged(EventId, oldDtcStatus, DEM_UDS_STATUS_INITIALIZE);
}
/* PRQA S 6070 -- */

/**
 * @brief         Clears an event from the given origin memory, if allowed
 */
DEM_LOCAL boolean Dem_DTCCleared(Dem_EventIdType EventId)
{
    boolean doClear;
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    Dem_CombinationNumType groupId = Dem_GetGroupIdOfEvent(EventId);
#endif

#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        doClear = TRUE;
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if ((Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
                && (Dem_GetEventAvailableStatusByEventId(subEventId) == TRUE))
            {
                doClear = Dem_ClearEventAllowed(subEventId);
            }
            if (doClear == FALSE)
            {
                break;
            }
        }
    }
    else
#endif
    {
        doClear = Dem_ClearEventAllowed(EventId);
    }

    if (doClear == TRUE)
    {
        Dem_DTCInternalCleared(EventId);
        /** Always call init monitor callbacks */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
        if (groupId != DEM_COMBINATION_NUMBER)
        {
            for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
                 start < Dem_GetEndRefOfCombinationDTC(groupId);
                 ++start)
            {
                Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
                if ((Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
                    && (Dem_GetEventAvailableStatusByEventId(subEventId) == TRUE))
                {
                    Dem_NotiInitMonitorForEvent(subEventId, DEM_INIT_MONITOR_CLEAR);
                }
            }
        }
        else
#endif
        {
            Dem_NotiInitMonitorForEvent(EventId, DEM_INIT_MONITOR_CLEAR);
        }
    }
    else
    {
        /** If the out-parameter Allowed of the callback DemClearEventAllowed returns false and the configuration
         * parameter DemClearEventAllowedBehavior is set to DEM_ONLY_THIS_CYCLE_AND_READINESS, the related UDS status
         * bits 1 (TestFailedThisOperationCycle), 4 (TestNotCompletedSinceLastClear), 5 (TestFailedSinceLastClear), and
         * 6 (TestNotCompletedThisOperationCycle) shall be reset. */
        Dem_ClearEventFncNumType clearEventAllowedRef = Dem_GetCEARefOfEvent(EventId);
        if (clearEventAllowedRef != DEM_EVENT_CALLBACK_CLEAR_ALLOWED)
        {
            if (Dem_GetCEABehaviorOfEvent(clearEventAllowedRef) == DEM_ONLY_THIS_CYCLE_AND_READINESS)
            {
                Dem_EventClearReset(EventId);
            }
        }
    }
#else
    doClear = TRUE;
    Dem_DTCInternalCleared(EventId);
    /** Always call init monitor callbacks */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    if (groupId != DEM_COMBINATION_NUMBER)
    {
        for (Dem_ComDTCEventRefNumType start = Dem_GetStartRefOfCombinationDTC(groupId);
             start < Dem_GetEndRefOfCombinationDTC(groupId);
             ++start)
        {
            Dem_EventIdType subEventId = Dem_GetventIdOfCombinationDTC(start);
            if ((Dem_GetAvailableInVariantByEvent(subEventId) == TRUE)
                && (Dem_GetEventAvailableStatusByEventId(subEventId) == TRUE))
            {
                Dem_NotiInitMonitorForEvent(subEventId, DEM_INIT_MONITOR_CLEAR);
            }
        }
    }
    else
#endif
    {
        Dem_NotiInitMonitorForEvent(EventId, DEM_INIT_MONITOR_CLEAR);
    }
#endif
    return doClear;
}

/**
 * @brief         Process UDS clear single dtc requests
 * @return        boolean
 * @retval        TRUE: Clear sucessful
 * @retval        FALSE: Clear not sucessful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDER
 */
DEM_LOCAL_INLINE boolean Dem_ProcessClearSingleDtc(void)
{
    return Dem_DTCCleared(Dem_CurrentProcessClearRequest.ClearEventId);
}

/**
 * @brief         Process UDS clear group dtc requests
 */
DEM_LOCAL boolean Dem_ProcessClearGroupDtc(Dem_EventIdType FirstEvent, Dem_EventIdType LastEvent)
{
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
    boolean ret = FALSE;
#else
    boolean ret = TRUE;
#endif
    uint8 clearType = Dem_GetClearRequestClearType();
#if ((DEM_DTC_GROUP_NUMBER > 0u) && (DEM_CLEAR_DTC_LIMITATION == DEM_ALL_SUPPORTED_DTCS))
    Dem_GroupOfDTCNumType groupId;

    if (clearType == DEM_CLEAR_REQUEST_TYPE_GROUP)
    {
        /** Clear group */
        groupId = Dem_CurrentProcessClearRequest.ClearGroupDTCIndex;
    }
#endif

    for (Dem_EventIdType eventId = FirstEvent; eventId < LastEvent; ++eventId)
    {
        if ((DEM_CLEAR_REQUEST_TYPE_ALL == clearType)
#if ((DEM_DTC_GROUP_NUMBER > 0u) && (DEM_CLEAR_DTC_LIMITATION == DEM_ALL_SUPPORTED_DTCS))
            || ((clearType == DEM_CLEAR_REQUEST_TYPE_GROUP) && (Dem_CheckDtcGroupMatch(eventId, groupId) == TRUE))
#endif
        )
        {
            if ((Dem_GetAvailableInVariantByEvent(eventId) == TRUE)
                && (Dem_GetEventAvailableStatusByEventId(eventId) == TRUE))

            {
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
                if (Dem_DTCCleared(eventId) == TRUE)
                {
                    ret = TRUE;
                }
#else
                (void)Dem_DTCCleared(eventId);
#endif
            }
        }
    }
    return ret;
}

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/**
 * @brief         Process J1939 clear dtc requests
 */
DEM_LOCAL boolean Dem_ProcessClearJ1939Dtc(Dem_EventIdType FirstEvent, Dem_EventIdType LastEvent)
{
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
    boolean ret = FALSE;
#else
    boolean ret = TRUE;
#endif
    for (Dem_EventIdType eventId = FirstEvent; eventId < LastEvent; ++eventId)
    {
        /** Event must be a J1939 related and match the ClearCode */
        /** only match events that support J1939 */
        /** DTC status needs to match the request */
        /* PRQA S 1258, 1252 ++ */ /* VL_Dem_1258, VL_Dem_1252 */
        if ((Dem_GetEventJ1939DTC(eventId) != DEM_DTC_J1939_INVALID)
            && (Dem_CheckDTCAvailableInVariant(eventId) == TRUE)
            && (Dem_GetEventAvailableStatus(Dem_GetDtcInterStatus(eventId)) == TRUE)
            && ((Dem_GetDTCUDSStatus(eventId) & (Dem_GetClearRequestClearMask())) == Dem_GetClearRequestClearCode()))
        {
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
            if (Dem_DTCCleared(eventId) == TRUE)
            {
                ret = TRUE;
            }
#else
            (void)Dem_DTCCleared(eventId);
#endif
        }
        /* PRQA S 1258, 1252 -- */
    }
    return ret;
}
#endif

/**
 * @brief         Process UDS clear all dtc requests
 */
DEM_LOCAL void Dem_ProcessClearAllDtc(void)
{
    Dem_MemoryNumType memIndex = Dem_GetClearRequestMemIndex();
    uint8             clientId = Dem_CurrentProcessClearRequest.ClearClient;
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_ResetMemoryOverflow(memIndex);
#endif
    if (memIndex == Dem_GetPrimaryRefOfMemSet(Dem_GetMemorySetRefOfClient(clientId)))
    {
        Dem_SetFirstFailedEvent(DEM_EVENT_INVALID, clientId);
        Dem_SetFirstConfirmedEvent(DEM_EVENT_INVALID, clientId);
        Dem_SetMostRecentFailedEvent(DEM_EVENT_INVALID, clientId);
        Dem_SetMostRecentConfirmdEvent(DEM_EVENT_INVALID, clientId);
    }
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
    Dem_NvmSetSingleBlockState(DEM_NV_ADMINBLOCKINDEX, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
#endif
}

/**
 * @brief         Processes clear dtc requests
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Dem_STCAL */
DEM_LOCAL void Dem_ProcessClearDtc(void)
{
    boolean clearSuccess = FALSE;
#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
    boolean clearEnd = FALSE;
#endif

    if (Dem_GetClearRequestClearType() == DEM_CLEAR_REQUEST_TYPE_SINGLE)
    {
        /** Clear single event */
        clearSuccess = Dem_ProcessClearSingleDtc();
    }
    else
    {
        Dem_MemoryNumType           memIndex   = Dem_GetClearRequestMemIndex();
        const Dem_MemStateInfoType* memoryInfo = Dem_MemStateInfoInit(memIndex);
        Dem_EventIdType             firstEvent = memoryInfo->FirstEvent;
        Dem_EventIdType             lastEvent  = memoryInfo->LastEvent;
#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
        if (Dem_ClearTaskIterator == DEM_EVENT_INVALID)
        {
            /* Not yet started */
            Dem_ClearTaskIterator = firstEvent;
        }
        /** Check that the number of DTC group events to be cleared is less than the clearing number limit. */
        firstEvent = Dem_ClearTaskIterator;
        if ((lastEvent - Dem_ClearTaskIterator) > DEM_CLEAR_DTC_LIMIT_NUMBER)
        {
            lastEvent = Dem_ClearTaskIterator + DEM_CLEAR_DTC_LIMIT_NUMBER;
            Dem_ClearTaskIterator += DEM_CLEAR_DTC_LIMIT_NUMBER;
        }
#endif
        /** Clear group of events */
#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
#if (DEM_J1939_SUPPORT == STD_ON)
        if (Dem_GetClearRequestClearType() == DEM_CLEAR_REQUEST_TYPE_ALL_J1939)
        {
            clearSuccess = Dem_ProcessClearJ1939Dtc(firstEvent, lastEvent);
        }
        else
#endif
#endif
        {
            clearSuccess = Dem_ProcessClearGroupDtc(firstEvent, lastEvent);
        }
    }

#if (DEM_CLEAR_DTC_LIMIT_NUMBER > 0u)
    if (clearEnd == TRUE)
#endif
    {
        if (clearSuccess == TRUE)
        {
            if (Dem_GetClearRequestClearType() == DEM_CLEAR_REQUEST_TYPE_ALL)
            {
                Dem_ProcessClearAllDtc();
            }
#if (DEM_NV_RAM_BLOCK_NUMBER > 0u) && (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
            Dem_SetClearState(DEM_CLEARTASK_NV_INPROGRESS);
#else
            Dem_SetClearState(DEM_CLEARTASK_DONE);
#endif

#if (DEM_DEBOUNCE_COUNTER_STORAGE_NUM > 0u)
            Dem_MemStorageDataDebounce();
#endif

#if (DEM_NV_RAM_BLOCK_NUMBER > 0u)
            Dem_NvmSetSingleBlockState(DEM_NV_EVENTSTATUSBLOCKINDEX, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
#endif
        }
#if (DEM_EVENT_CALLBACK_CLEAR_ALLOWED > 0u)
        else
        {
            Dem_SetClearState(DEM_CLEARTASK_FAILED);
        }
#endif
    }
}
/* PRQA S 6070 -- */

/**
 * @brief         Process the current clear requests
 */
DEM_LOCAL void Dem_ClearTaskTask(void)
{
    switch (Dem_ClearTaskState)
    {
    case DEM_CLEARTASK_NEW:
        Dem_SetClearState(DEM_CLEARTASK_INPROGRESS);
        /** FALL THROUGH */
    case DEM_CLEARTASK_INPROGRESS: /* PRQA S 2003 */ /* VL_Dem_2003 */
        Dem_ProcessClearDtc();
        if (Dem_ClearTaskState == DEM_CLEARTASK_DONE)
        {
            Dem_SetClearState(DEM_CLEARTASK_IDLE);
        }
        break;
    case DEM_CLEARTASK_DONE:
    case DEM_CLEARTASK_FAILED:
    case DEM_CLEARTASK_NVERROR:
        Dem_SetClearState(DEM_CLEARTASK_IDLE);
        break;
    default:
        /** nothing to do */
        break;
    }
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
