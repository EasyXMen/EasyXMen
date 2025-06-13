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
**  FILENAME    : Dem_OBD.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for OBD                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"

/*******************************************************************************
**                      macros                                                **
*******************************************************************************/
#define DEM_OBD_NUMBER_OF_READINESS_GROUPS_FOR 12u

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
/* PRQA S 3408,1504,1514++ */ /* MISRA Rule 8.4,8.7,8.9 */
#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"
uint16 OBDDistanceMILOn;
uint16 OBDTimeMILOn;
uint16 OBDTimeDTCClear;
uint16 DistSinceDtcCleared;
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
uint16 IgnUpCycleCounter;
#endif
uint16 OBDDistanceMILLastOn;
uint16 OBDTimeMILLastOn;
uint16 OBDDistanceDTCClear;
uint16 OBDTimeDTCLastClear;
#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"
Dem_IndicatorStatusType OBDMilStatus;
#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"
uint8 WarmUpCycleCounter;
#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"

#if (DEM_PTO_SUPPORT == STD_ON)
#define DEM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Dem_MemMap.h"
boolean CurrentPTOStatus;
#define DEM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_VAR_INIT_16
#include "Dem_MemMap.h"
uint16 ContinuousMICounter = 0;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
uint16 MasterContinuousMICounter = 0;
#endif
uint16 OBDB1Counter = 0;
#define DEM_STOP_SEC_VAR_INIT_16
#include "Dem_MemMap.h"
/* PRQA S 3408,1504,1514-- */ /* MISRA Rule 8.4,8.7,8.9 */

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define DEM_START_SEC_VAR_INIT_8
#include "Dem_MemMap.h"
DEM_LOCAL boolean PFCStatu = FALSE;
/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
DEM_LOCAL uint8 WarmUpCycleCounterForMI = 0;
DEM_LOCAL uint8 CycleCounterForMI = 0;
DEM_LOCAL uint8 CycleCounterForB1 = 0;
DEM_LOCAL uint8 TimeCycleCounterForMI = 0;
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */
#define DEM_STOP_SEC_VAR_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"
DEM_LOCAL boolean SetDataOfPid21;
#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
DEM_LOCAL FUNC(void, DEM_CODE) Dem_B1CounterProcess(void);
#endif
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Init the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_OBDInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    WarmUpCycleCounter = 0;
    /*distance related*/
    OBDDistanceMILOn = 0;
    OBDDistanceDTCClear = 0;
    DistSinceDtcCleared = 0;
    OBDDistanceMILLastOn = 0;
    /*time related*/
    OBDTimeMILOn = 0;
    OBDTimeDTCClear = 0;
    OBDTimeDTCLastClear = 0;
    OBDTimeMILLastOn = 0;
    SetDataOfPid21 = FALSE;
    PFCStatu = FALSE;
    OBDMilStatus = DEM_INDICATOR_OFF;
}

#if (DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to forward
                        the condition status to a Dem of a particular ECU
 * ServiceId           0xae
 * Sync/Async          Synchronous /Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
                        ConditionStatus:Status of the IUMPR denominator condition
                        (Notreached, reached, not reachable / inhibited)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of IUMPR denominator condition was successful
                        E_NOT_OK: set of IUMPR denominator condition
                        failed or could not be accepted.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType ConditionStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETIUMPRDENCONDITION, DEM_E_UNINIT);
    }
    else if (ConditionId >= 5u)
    {
        DEM_DET_REPORT(DEM_SID_SETIUMPRDENCONDITION, DEM_E_PARAM_DATA);
    }
    else
    {
        ret = Dem_InterSetIUMPRDenCondition(ConditionId, ConditionStatus);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to retrieve
                        the condition status from the Dem of the ECU where the conditions are
                        computed.
 * ServiceId           0xaf
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
 * Param-Name[out]     ConditionStatus:Status of the IUMPR denominator condition
 *                      (Notreached, reached, not reachable / inhibited)
 * Param-Name[in/out]  None
 * Return              E_OK: get of IUMPR denominator condition status was successful
                        E_NOT_OK: get of condition status failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetIUMPRDenCondition(
    Dem_IumprDenomCondIdType ConditionId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_IumprDenomCondStatusType, AUTOMATIC, DEM_APPL_DATA) ConditionStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_GETIUMPRDENCONDITION, DEM_E_UNINIT);
    }
    else if (ConditionId >= 5u)
    {
        DEM_DET_REPORT(DEM_SID_GETIUMPRDENCONDITION, DEM_E_PARAM_DATA);
    }
    else
    {
        ret = Dem_InterGetIUMPRDenCondition(ConditionId, ConditionStatus);
    }
    return ret;
}
#endif /*DEM_RATIO_NUM > 0 */

#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
/*************************************************************************/
/*
 * Brief               Calculate the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_SubCalOBDRelatedValue(uint16 OperationId)
{
    switch (DemPbCfgPtr->DemOperationCycle[OperationId].DemOperationCycleType)
    {
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    case DEM_OPCYC_IGNITION:
        IgnUpCycleCounter++;
        break;
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */
    case DEM_OPCYC_OBD_DCY:
        Dem_InterCalOBDRelatedValue();
        break;
    case DEM_OPCYC_WARMUP:
        if (WarmUpCycleCounter < 0xFFu)
        {
            WarmUpCycleCounter++;
        }
        if (OBDMilStatus == DEM_INDICATOR_OFF)
        {
            if (WarmUpCycleCounterForMI < 40u)
            {
                /* req SWS_Dem_01146 */
                WarmUpCycleCounterForMI++;
            }
            else
            {
                WarmUpCycleCounterForMI = 0u;
                ContinuousMICounter = 0u;
            }
        }
        break;
    case DEM_OPCYC_TIME:
        if (OBDMilStatus != DEM_INDICATOR_OFF)
        {
            if (CycleCounterForMI > 3u)
            {
                /* req SWS_Dem_01145 req SWS_Dem_01144 */
                ContinuousMICounter = 0u;
            }
            if (ContinuousMICounter < 0xFFu)
            {
                /* req SWS_Dem_01147 req SWS_Dem_01142 */
                ContinuousMICounter++;
            }
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            if (MasterContinuousMICounter < 0xFFu)
            {
                /* req SWS_Dem_01151  req SWS_Dem_01152 */
                MasterContinuousMICounter++;
            }
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU */
            WarmUpCycleCounterForMI = 0u;
            TimeCycleCounterForMI = 0u;
            CycleCounterForMI = 0u;
        }
        else
        {
            if (TimeCycleCounterForMI < 200u)
            {
                /* req SWS_Dem_01146 */
                TimeCycleCounterForMI++;
            }
            else
            {
                TimeCycleCounterForMI = 0u;
                ContinuousMICounter = 0u;
            }
        }
        Dem_B1CounterProcess();
        break;
    default:
        /*idle*/
        break;
    }
}
#endif
/*************************************************************************/
/*
 * Brief               Calculate the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_CalOBDRelatedValue(uint16 OperationId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 iloop;
    const Dem_EventParameterType* pEventCfg = DemPbCfgPtr->DemEventParameter;
    const Dem_DTCType* pDTC = DemPbCfgPtr->DemDTC;

    if ((OBDMilStatus == DEM_INDICATOR_OFF) && (CycleCounterForMI < 0xFFu))
    {
        CycleCounterForMI++;
    }
    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(iloop);
        uint16 DTCRef = pEventCfg[iloop].DemDTCRef;
        if ((DTCRef != DEM_DTC_REF_INVALID) && (pDTC[DTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B1)
            && ((DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF) != 0x00u)
                && (DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC) != 0x00u)))
        {
            CycleCounterForB1 = 0;
            break;
        }
        CycleCounterForB1++;

        if (CycleCounterForB1 > 200u)
        {
            /* req SWS_Dem_01157 */
            OBDB1Counter = 190u;
        }
        else if (CycleCounterForB1 > 3u)
        {
            /* req SWS_Dem_01156 */
            OBDB1Counter = 0u;
        }
        else
        {
            /*idle*/
        }
    }

    Dem_SubCalOBDRelatedValue(OperationId);
}

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/*************************************************************************/
/*
 * Brief               Update the Current OBD Mil Status.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_UpdateOBDMilStatus(uint8 indicatorRef, uint16 IntID) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Dem_IndicatorStatusType currentMilStatus = DEM_INDICATOR_OFF;
    uint16 CurrentDistance;
    uint16 CurrentTime;
    uint16 DemDTCRef;
    static Dem_IndicatorStatusType lastMilStatus = DEM_INDICATOR_OFF;

    DemDTCRef = DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef;
    uint32 DemDtcValue = DemPbCfgPtr->DemDTC[DemDTCRef].DemDtcValue;
    uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[DemDTCRef].DemObdDTCRef;
    const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
    if ((indicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR) && (DemDTCRef != DEM_DTC_REF_INVALID))
    {
        (void)Dem_InterGetIndicatorStatus(indicatorRef, &currentMilStatus);
        if (currentMilStatus != DEM_INDICATOR_OFF)
        {
            /* MIL recently activated */
            if (((DemDtcValue < 0xFFFF33UL) && (DemDtcValue != DEM_DTC_CFG_INVALID))
                || (pObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
            {
                /* req SWS_Dem_01139 */
                OBDMilStatus = currentMilStatus;
            }
            /* Read current distance information */
            if (lastMilStatus == DEM_INDICATOR_OFF)
            {
                OBDDistanceMILLastOn = Dem_ReadDistanceInformation();
                OBDTimeMILLastOn = Dem_ReadTimeInformation();
            }
        }
        else
        {
            /* MIL recently de-activated */
            if (((DemDtcValue < 0xFFFF33UL) && (DemDtcValue != DEM_DTC_CFG_INVALID))
                || (pObdDTC[DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
            {
                /* req SWS_Dem_01139 */
                OBDMilStatus = currentMilStatus;
            }
            CurrentDistance = Dem_ReadDistanceInformation();
            if (CurrentDistance > OBDDistanceMILLastOn)
            {
                if ((uint16)(CurrentDistance - OBDDistanceMILLastOn + OBDDistanceMILOn) < 0xFFFFu)
                {
                    OBDDistanceMILOn += CurrentDistance - OBDDistanceMILLastOn;
                    OBDDistanceMILLastOn = CurrentDistance;
                }
                else
                {
                    OBDDistanceMILOn = 0xFFFFu;
                }
            }
            CurrentTime = Dem_ReadTimeInformation();
            if (CurrentTime > OBDTimeMILLastOn)
            {
                if ((uint16)(CurrentTime - OBDTimeMILLastOn + OBDTimeMILOn) < 0xFFFFu)
                {
                    OBDTimeMILOn += CurrentTime - OBDTimeMILLastOn;
                    OBDTimeMILLastOn = CurrentTime;
                }
                else
                {
                    OBDTimeMILOn = 0xFFFFu;
                }
            }
        }
        lastMilStatus = currentMilStatus;
    }
}
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU */

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 08.
 * ServiceId           0x6b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata08 Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata08BufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
/* PRQA S 3408++ */ /* MISRA Rule 8.4 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetInfoTypeValue08(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08BufferSize)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE08, DEM_E_UNINIT);
    }
    else if ((Iumprdata08BufferSize == NULL_PTR) || (Iumprdata08 == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE08, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_InterDcmGetInfoTypeValue08(OpStatus, Iumprdata08, Iumprdata08BufferSize);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 0B.
 * ServiceId           0x6c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata0B Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata0BBufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetInfoTypeValue0B(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0B,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0BBufferSize)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE0B, DEM_E_UNINIT);
    }
    else if ((Iumprdata0BBufferSize == NULL_PTR) || (Iumprdata0B == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE0B, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_InterDcmGetInfoTypeValue0B(OpStatus, Iumprdata0B, Iumprdata0BBufferSize);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Reports the value of a requested "availability-OBDMID" to the DCM upon a
 Service 06 request.
 * ServiceId           0xa3
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid Availablity OBDMID (00,20, 40...)
 * Param-Name[out]     Obdmidvalue Bit coded information on the support of OBDMIDs.
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmGetAvailableOBDMIDs(uint8 Obdmid, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) Obdmidvalue)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETAVAILABLEOBDMIDS, DEM_E_UNINIT);
    }
    else if (Obdmidvalue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETAVAILABLEOBDMIDS, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_InterDcmGetAvailableOBDMIDs(Obdmid, Obdmidvalue);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the number of TIDs per (functional) OBDMID.
 * ServiceId           0xa4
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid OBDMID subject of the request to identify the number of assigned TIDs
 * Param-Name[out]     numberOfTIDs Number of assigned TIDs for the requested OBDMID.
 *                      Used as loop value for the DCM to retrieve all OBD/TID result data
 * Param-Name[in/out]  None
 * Return              E_OK: get number of TIDs successful
                        E_NOT_OK: get number of TIDs failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmGetNumTIDsOfOBDMID(uint8 Obdmid, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) numberOfTIDs)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNUMTIDSOFOBDMID, DEM_E_UNINIT);
    }
    if (numberOfTIDs == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNUMTIDSOFOBDMID, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_InterDcmGetNumTIDsOfOBDMID(Obdmid, numberOfTIDs);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 01 computed by the Dem.
 * ServiceId           0x61
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 01 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID01(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 iloop;
    uint8 ReadinessGroupIndex;
    uint8 supportedGroups[2];
    uint8 notCompletedGroups[2];
    uint16 obdDtc;
    uint16 obdDtcRef;
    uint8 OBDDTCNum = 0;
    uint16 tempRef;
    Std_ReturnType ret = E_NOT_OK;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID01, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID01value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID01, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_MemSet(PID01value, 0u, (uint8)4U);
        Dem_MemSet(supportedGroups, 0u, (uint8)2U);
        Dem_MemSet(notCompletedGroups, 0u, (uint8)2U);
        const Dem_EventParameterType* pEventCfg = DemPbCfgPtr->DemEventParameter;
        const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;
        /* Byte A */
        for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
        {
            /* req SWS_Dem_01103 */
            tempRef = pEventCfg[iloop].DemDTCRef;
            const Dem_DTCType* pDTC = &DemPbCfgPtr->DemDTC[tempRef];
            if ((pEventCfg[iloop].DemEventAvailable == TRUE) && (tempRef != DEM_DTC_REF_INVALID))
            {
                obdDtcRef = pDTC->DemObdDTCRef;
                /* req SWS_Dem_01101*/
                /* 0xFFFF33UL <= WWH-OBD <= 0xFFFFFFUL */
                /* req SWS_Dem_01137  confirmedDTC 0x08*/
                if ((DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                    && (((pDTC->DemDtcValue < 0xFFFF33UL) && (pDTC->DemDtcValue > 0UL))
                        || (pObdDTC[pDTC->DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                    && ((OBDDTCNum < 0x7Fu)
                        && (0x00U != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_CDTC))))
                {
                    /* ConfirmedDtcNum : Byte A, bit 0-6  number of OBD confirmed faults req
                     * SWS_Dem_00351 */
                    OBDDTCNum++;
                }
            }
        }
        /* req SWS_Dem_00351 */
        PID01value[0] = OBDDTCNum;
        if (OBDMilStatus != DEM_INDICATOR_OFF)
        {
            /* MIL status : Byte A, bit 7 req SWS_Dem_01138 */
            PID01value[0] |= (uint8)0x80U;
        }
        /* Byte B, C, D*/
        for (ReadinessGroupIndex = 0; ReadinessGroupIndex < DEM_OBD_NUMBER_OF_READINESS_GROUPS_FOR;
             ReadinessGroupIndex++)
        {
            /* req SWS_Dem_00354 */
            for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
            {
                /* req SWS_Dem_01103 */
                tempRef = pEventCfg[iloop].DemDTCRef;
                if ((pEventCfg[iloop].DemEventAvailable == TRUE) && (tempRef != DEM_DTC_REF_INVALID))
                {
                    obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
                    /* req SWS_Dem_01101 */
                    if ((DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                        && (pObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex))
                    {
                        obdDtc = pObdDTC[obdDtcRef].DemDtcValue;
                        if (obdDtc != DEM_OBD_DTC_CFG_INVALID)
                        {
                            const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(iloop);
                            uint8 UdsStatus = pEvent->UdsStatus;
                            DEM_BITS_SET(supportedGroups, ReadinessGroupIndex);
                            if (((0x00u == DEM_FLAGS_ISSET(UdsStatus, DEM_UDS_STATUS_TF))
                                 && (0x00u == DEM_FLAGS_ISSET(UdsStatus, DEM_UDS_STATUS_TNCSLC)))
                                || (0x00u != DEM_FLAGS_ISSET(UdsStatus, DEM_UDS_STATUS_WIR)))
                            {
                                /* idle  req SWS_Dem_00354 */
                            }
                            else
                            {
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
        }
        PID01value[1] = ((supportedGroups[0u] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
        PID01value[2] = ((uint8)((supportedGroups[0u] & 0xF0u) >> 4u) | (uint8)((supportedGroups[1u] & 0xFu) << 4u));
        PID01value[3] =
            ((uint8)((notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint8)((notCompletedGroups[1u] & 0xFu) << 4u));
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 1C computed by the Dem.
 * ServiceId           0x63
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 1C computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/*OBD requirements to which vehicle is designed*/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID1C(
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID1Cvalue
#else
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID1Cvalue
#endif
)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID1C, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID1Cvalue)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID1C, DEM_E_PARAM_POINTER);
    }
    else
    {
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        /* req SWS_Dem_00748 */
        PID1Cvalue[0] = DemGeneralOBD.DemOBDCompliancy;
        ret = E_OK;
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 21 computed by the Dem.
 * ServiceId           0x64
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 21 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID21(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID21, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID21value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID21, DEM_E_PARAM_POINTER);
    }
    else
    {
        if (SetDataOfPid21 == TRUE)
        {
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
            uint16 DistWithMilOn = 0U;
            uint16 currentDistanceInformation;
            /* MIL deactivated req SWS_Dem_00704 */
            if (DEM_INDICATOR_OFF != OBDMilStatus)
            {
                /*the indicator is on*/
                currentDistanceInformation = Dem_ReadDistanceInformation();
                if (currentDistanceInformation > OBDDistanceMILLastOn)
                {
                    DistWithMilOn = currentDistanceInformation - OBDDistanceMILLastOn;
                    OBDDistanceMILLastOn = currentDistanceInformation;
                }
            }
            if ((OBDDistanceMILOn + DistWithMilOn) < 0xFFFFu)
            {
                OBDDistanceMILOn += DistWithMilOn;
            }
            else
            {
                OBDDistanceMILOn = 0xFFFFu;
            }
            PID21value[0] = (uint8)OBDDistanceMILOn;
            PID21value[1] = (uint8)(OBDDistanceMILOn >> 8u);
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU */
        }
        else
        {
            /* req SWS_Dem_01099 */
            PID21value[0] = 0xFFu;
            PID21value[1] = 0xFFu;
        }
        SetDataOfPid21 = FALSE;
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID30 computed by the Dem.
 * ServiceId           0x65
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 30 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID30(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID30value)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID30, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID30value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID30, DEM_E_PARAM_POINTER);
    }
    else
    {
        *PID30value = WarmUpCycleCounter;
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 31 computed by the Dem.
 * ServiceId           0x66
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 31 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID31(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID31value)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID31, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID31value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID31, DEM_E_PARAM_POINTER);
    }
    else
    {
        PID31value[0] = 0;
        PID31value[1] = 0;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
        /* req SWS_Dem_00704 */
        /* Read current distance information */
        uint16 currentDistanceInformation = Dem_ReadDistanceInformation();
        if (currentDistanceInformation > OBDDistanceDTCClear)
        {
            DistSinceDtcCleared = currentDistanceInformation - OBDDistanceDTCClear;
        }
        PID31value[0] = (uint8)DistSinceDtcCleared;
        PID31value[1] = (uint8)(DistSinceDtcCleared >> 8u);
        ret = E_OK;
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 41 computed by the Dem.
 * ServiceId           0x67
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 41 computed by the Dem.>
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID41(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID41value)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 iloop;
    uint16 ReadinessGroupIndex;
    uint8 enabledGroups[2];
    uint8 notCompletedGroups[2];
    uint16 obdDtc;
    uint16 obdDtcRef;
    uint16 tempRef;
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID41, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID41value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID41, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_MemSet(PID41value, 0u, (uint8)4U);
        enabledGroups[0] = 0xffU;
        enabledGroups[1] = 0x07U;
        notCompletedGroups[0] = 0;
        notCompletedGroups[1] = 0;
        for (ReadinessGroupIndex = 0; ReadinessGroupIndex < DEM_OBD_NUMBER_OF_READINESS_GROUPS_FOR;
             ReadinessGroupIndex++)
        {
            for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
            {
                /* req SWS_Dem_01103 */
                const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
                tempRef = pEventCfg->DemDTCRef;
                if ((pEventCfg->DemEventAvailable == TRUE) && (tempRef != DEM_DTC_REF_INVALID))
                {
                    obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
                    const DemObdDTCType* pObdDTC = &DemPbCfgPtr->DemObdDTC[obdDtcRef];
                    /* req SWS_Dem_01101*/
                    if ((DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                        && (pObdDTC->DemEventOBDReadinessGroup == ReadinessGroupIndex))
                    {
                        /* Only include emission related DTCs */
                        obdDtc = pObdDTC->DemDtcValue;
                        if (obdDtc != DEM_OBD_DTC_CFG_INVALID)
                        {
                            pEvent = Dem_GetEventInfo(iloop);
                            uint8 OCRef = pEventCfg->DemOperationCycleRef;
                            if (((DEM_BITS_ISSET(DemOperationCycleStatus, OCRef)))
                                && (DemPbCfgPtr->DemOperationCycle[OCRef].DemOperationCycleType == DEM_OPCYC_POWER)
                                && (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION)))
                            {
                                /* req SWS_Dem_00356 req SWS_Dem_00348 */
                                DEM_BITS_CLR(enabledGroups, ReadinessGroupIndex);
                            }
                            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                            {
                                /* req SWS_Dem_00355 */
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
        }
        PID41value[0] = 0x00;
        PID41value[1] = ((enabledGroups[0] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
        PID41value[2] = ((uint8)((enabledGroups[0u] & 0xF0u) >> 4u) | (uint8)((enabledGroups[1u] & 0xFu) << 4u));
        PID41value[3] =
            ((uint8)((notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint8)((notCompletedGroups[1u] & 0xFu) << 4u));
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4D computed by the Dem.
 * ServiceId           0x68
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 4D computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID4D(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Dvalue)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4D, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID4Dvalue)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4D, DEM_E_PARAM_POINTER);
    }
    else
    {
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
        uint16 TimeWithMilOn = 0U;
        uint16 CurrentTimeInformation;
#endif
        PID4Dvalue[0] = 0;
        PID4Dvalue[1] = 0;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
        /* req SWS_Dem_00704  MIL deactivated */
        if (DEM_INDICATOR_OFF != OBDMilStatus)
        {
            /*the indicator is on*/
            CurrentTimeInformation = Dem_ReadTimeInformation();
            if (CurrentTimeInformation > OBDTimeMILLastOn)
            {
                TimeWithMilOn = CurrentTimeInformation - OBDTimeMILLastOn;
                OBDTimeMILLastOn = CurrentTimeInformation;
            }
        }
        if ((OBDTimeMILOn + TimeWithMilOn) < 0xFFFFu)
        {
            OBDTimeMILOn += TimeWithMilOn;
        }
        else
        {
            OBDTimeMILOn = 0xFFFFu;
        }
        PID4Dvalue[0] = (uint8)OBDTimeMILOn;
        PID4Dvalue[1] = (uint8)(OBDTimeMILOn >> 8u);
        ret = E_OK;
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4E computed by the Dem.
 * ServiceId           0x69
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue Buffer containing the contents of PID 4E
                        computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID4E(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Evalue)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4E, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID4Evalue)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4E, DEM_E_PARAM_POINTER);
    }
    else
    {
        PID4Evalue[0] = 0;
        PID4Evalue[1] = 0;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
        /* MIL still activated  req SWS_Dem_00704 */
        uint16 CurrentTimeInformation = Dem_ReadTimeInformation();
        if (CurrentTimeInformation > OBDTimeDTCLastClear)
        {
            OBDTimeDTCClear = CurrentTimeInformation - OBDTimeDTCLastClear;
        }
        PID4Evalue[0] = (uint8)OBDTimeDTCClear;
        PID4Evalue[1] = (uint8)(OBDTimeDTCClear >> 8u);
        ret = E_OK;
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 91 computed by the Dem.
 * ServiceId           0x6a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID91value Buffer containing the contents of PID 91
                        computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_DcmReadDataOfPID91(
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID91value
#else
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID91value
#endif
)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID91, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID91value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID91, DEM_E_PARAM_POINTER);
    }
    else
    {
        /*TODO: ECU OBD System Information (5 byte)*/
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        ret = DemReadPID91(PID91value);
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets data element per PID and index of the most important freeze frame
                        being selected for the output of service 02. The function stores the data
                        in the provided DestBuffer.
 * ServiceId           0x52
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PID: This parameter is an identifier for a PID as defined
                        in ISO15031-5.
                        DataElementIndexOfPID: Data element index of this PID according
                        to the Dcm configuration of service 02. It is zero-based and
                        consecutive, and ordered by the data element  positions
                        (configured in Dcm, refer to SWS_Dem_00597).>
 * Param-Name[out]     None
 * Param-Name[in/out]  DestBuffer: This parameter contains a byte pointer that points to
                        the buffer, to which the data element of the PID shall
                        be written to. The format is raw hexadecimal values
                        and contains no header-information.
                        BufSize: When the function is called this parameter contains
                        the maximum number of data bytes that can be written
                        to the buffer.
                        The function returns the actual number of written
                        data bytes in this parameter.
 * Return              E_OK Freeze frame data was successfully reported
                        E_NOT_OK Freeze frame data was not successfully reported
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmReadDataOfOBDFreezeFrame(
    uint8 PID,
    uint8 DataElementIndexOfPID,
    /* PRQA S 3673++ */ /* MISRA Rule 8.13 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
/* PRQA S 3673-- */      /* MISRA Rule 8.13 */
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATAOFOBDFREEZEFRAME, DEM_E_UNINIT);
    }
    else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATAOFOBDFREEZEFRAME, DEM_E_PARAM_POINTER);
    }
    else
    {
        ret = Dem_InterDcmReadDataOfOBDFreezeFrame(PID, DataElementIndexOfPID, DestBuffer, BufSize);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets DTC by freeze frame record number.
 * ServiceId           0x53
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FrameNumber: Unique identifier for a freeze frame
                        record as defined in ISO 15031-5. The value 0x00
                        indicates the complete OBD freeze frame. Other values
                        are reserved for future functionality.
                        DTCFormat: Output format of the DTC value.
 * Param-Name[out]     DTC Diagnostic Trouble Code in ODB format. If the return
                        value of the function is other than E_OK this
                        parameter does not contain valid data.
 * Param-Name[in/out]  None
 * Return              E_OK: operation was successful E_NOT_OK: no DTC available
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetDTCOfOBDFreezeFrame(
    uint8 FrameNumber,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    Dem_DTCFormatType DTCFormat)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;
    const Dem_EventMemEntryType* pEntry;
    const Dem_EventParameterType* pEventCfg;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_UNINIT);
    }
    else if (DTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_PARAM_POINTER);
    }
    else
    {
        /* Only record number 0 is supported, SWS_Dem_00291 */
        if ((0u == FrameNumber) && (DEM_DTC_FORMAT_OBD == DTCFormat))
        {
            pEntry = Dem_SelectOBDFreezeFrame(&IntId);
            /* req SWS_Dem_01172 req SWS_Dem_01173 */
            if ((pEntry != NULL_PTR) && (IntId != DemPbCfgPtr->DemEventNum))
            {
                pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
                /* OBD Freeze frame stored, return DTC that caused storage */
                /* req SWS_Dem_01101 */
                if ((pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
                    && (DEM_OBD_DTC_INVALID != DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].DemObdDTCRef)
                    && (DemDTCGeneralStatus[pEventCfg->DemDTCRef].SuppressionStatus == FALSE))
                {
                    *DTC = (uint32)DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].DemObdDTCRef]
                               .DemDtcValue;
                    ret = E_OK;
                }
            }
            else
            {
                /* No DTC found */
                *DTC = 0;
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Reports a DTR data along with TID-value, UaSID, test result with lower and
 upper limit.
 * ServiceId           0xa5
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid: Identification of a DTR element by assigned DTRId.
                        TIDindex: Index of the TID within the DEM. Runs from 0
                        to "numberOfTIDs" obtained in the call to Dem_
                        DcmGetNumTIDsOfOBDMID()
 * Param-Name[out]     TIDvalue: TID to be put on the tester reponse
                        UaSID: UaSID to be put on the tester reponse
                        Testvalue:Latest test result
                        Lowlimvalue:Lower limit value associated to the latest test result
                        Upplimvalue:Upper limit value associated to the latest test result
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetDTRData(
    uint8 Obdmid,
    uint8 TIDindex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) TIDvalue,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) UaSID,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Testvalue,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Lowlimvalue,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Upplimvalue)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_UNINIT);
    }
    else if (
        (TIDvalue == NULL_PTR) || (UaSID == NULL_PTR) || (Testvalue == NULL_PTR) || (Lowlimvalue == NULL_PTR)
        || (Upplimvalue == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_PARAM_POINTER);
    }
    else
    {
        ret = Dem_InterDcmGetDTRData(Obdmid, TIDindex, TIDvalue, UaSID, Testvalue, Lowlimvalue, Upplimvalue);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service for reporting the event as disabled to the
                        Dem for the PID 41 computation.
 * ServiceId           0x51
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds. Non reentrant for the same EventId.
 * Param-Name[in]      Identification of an event by assigned EventId.>
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK set of event to disabled was successfull.
                         E_NOT_OK set of event disabled failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetEventDisabled(Dem_EventIdType EventId)
{
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventRelateInformationType* pEvent;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTDISABLED, DEM_E_UNINIT);
    }
    if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTDISABLED, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
        /* req SWS_Dem_00294 */
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service for reporting that faults are possibly found
                        because all conditions are fullfilled.
 * ServiceId           0x73
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Ratio Identifier reporting that a respective monitor
                        could have found a fault
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR result was successfully reported
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_RepIUMPRFaultDetect(Dem_RatioIdType RatioID)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRFAULTDETECT, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterRepIUMPRFaultDetect(RatioID);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service is used to lock a denominator of a specific monitor.
 * ServiceId           0x71
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is locked
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_RepIUMPRDenLock(Dem_RatioIdType RatioID)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENLOCK, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterRepIUMPRDenLock(RatioID);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service is used to release a denominator of a
                        specific monitor.
 * ServiceId           0x72
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is released
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_RepIUMPRDenRelease(Dem_RatioIdType RatioID)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENRELEASE, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterRepIUMPRDenRelease(RatioID);
    }
    return ret;
}

#if (DEM_PTO_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Set the status of the PTO.
 * ServiceId           0x79
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PtoStatus sets the status of the PTO
                        TRUE:active    FALSE:inactive
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType Returns E_OK when the new PTO-status has been
                        adopted by the Dem; returns E_NOT_OK in all other cases.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetPtoStatus(boolean PtoStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETPTOSTATUS, DEM_E_UNINIT);
    }
    else
    {
        CurrentPTOStatus = PtoStatus;
        ret = E_OK;
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 01 computed by the Dem.
 * ServiceId           0xb3
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID01value:Buffer containing the contents of PID 01 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_ReadDataOfPID01(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 iloop;
    uint8 ReadinessGroupIndex;
    uint8 supportedGroups[2];
    uint8 notCompletedGroups[2];
    uint16 obdDtc;
    uint8 OBDDTCNum = 0;
    uint16 tempRef;
    const Dem_EventParameterType* pEventCfg;
    const Dem_DTCType* pDTC;
    uint16 ObdDTCRef;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_READDATAOFPID01, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID01value)
    {
        DEM_DET_REPORT(DEM_SID_READDATAOFPID01, DEM_E_PARAM_POINTER);
    }
    else
    {
        Dem_MemSet(PID01value, 0u, (uint8)4U);
        Dem_MemSet(supportedGroups, 0u, (uint8)2U);
        Dem_MemSet(notCompletedGroups, 0u, (uint8)2U);
        /* Byte A */
        for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
        {
            pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
            /* req SWS_Dem_01103 */
            if (pEventCfg->DemEventAvailable == TRUE)
            {
                tempRef = pEventCfg->DemDTCRef;
                pDTC = &DemPbCfgPtr->DemDTC[tempRef];
                ObdDTCRef = pDTC->DemObdDTCRef;
                /* req SWS_Dem_01101 req SWS_Dem_01137  confirmedDTC 0x08  number of dtcs  req
                 * SWS_Dem_00351 */
                if ((DEM_DTC_REF_INVALID != tempRef) && (DEM_OBD_DTC_INVALID != ObdDTCRef)
                    && (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
                    && (((pDTC->DemDtcValue < 0xFFFF33UL) && (pDTC->DemDtcValue > 0UL))
                        || (DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                    && ((OBDDTCNum < 0x7Fu)
                        && (0x00U != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_CDTC))))
                {
                    OBDDTCNum++;
                }
            }
        }
        /* req SWS_Dem_00351 */
        PID01value[0] = OBDDTCNum;
        if (OBDMilStatus != DEM_INDICATOR_OFF)
        {
            /* req SWS_Dem_01138 */
            PID01value[0] |= (uint8)0x80U;
        }
        /* Byte B, C, D  in total 11 groups,n events assigned to the group*/
        for (ReadinessGroupIndex = 0; ReadinessGroupIndex < DEM_OBD_NUMBER_OF_READINESS_GROUPS_FOR;
             ReadinessGroupIndex++)
        {
            /* req SWS_Dem_00354 */
            for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
            {
                pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
                /* req SWS_Dem_01103 */
                if (pEventCfg->DemEventAvailable == TRUE)
                {
                    tempRef = pEventCfg->DemDTCRef;
                    pDTC = &DemPbCfgPtr->DemDTC[tempRef];
                    ObdDTCRef = pDTC->DemObdDTCRef;
                    /* req SWS_Dem_01101 */
                    if ((DEM_DTC_REF_INVALID != tempRef) && (DEM_OBD_DTC_INVALID != ObdDTCRef)
                        && (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
                        && (DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemEventOBDReadinessGroup == ReadinessGroupIndex))
                    {
                        obdDtc = DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemDtcValue;
                        if ((obdDtc != DEM_OBD_DTC_CFG_INVALID) && (ObdDTCRef != DEM_OBD_DTC_INVALID))
                        {
                            const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(iloop);
                            DEM_BITS_SET(supportedGroups, ReadinessGroupIndex);
                            if (((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                                 && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC)))
                                || (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)))
                            {
                                /*idle  req SWS_Dem_00354 */
                            }
                            else
                            {
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
        }
        PID01value[1] = ((supportedGroups[0u] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
        PID01value[2] = ((uint8)((supportedGroups[0u] & 0xF0u) >> 4u) | (uint8)((supportedGroups[1u] & 0xFu) << 4u));
        PID01value[3] =
            ((uint8)((notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint8)((notCompletedGroups[1u] & 0xFu) << 4u));
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 21 in the Dem by a software component.
 * ServiceId           0xa6
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PID21value:Buffer containing the contents of PID 21.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */                                                                     /* MISRA Rule 20.7 */
Dem_SetDataOfPID21(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
/* PRQA S 3432-- */                                                                     /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID21value)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_PARAM_POINTER);
    }
    else
    {
        OBDDistanceMILOn = (uint16)PID21value[0u] | (uint16)((uint16)PID21value[1u] << 8u);
        SetDataOfPid21 = TRUE;
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 31 in the Dem by a software component
 * ServiceId           0xa7
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID31value:Buffer containing the contents of PID 31.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */                                                                       /* MISRA Rule 20.7 */
Dem_SetDataOfPID31(P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID31value) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
/* PRQA S 3432-- */                                                                       /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID31, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID31value)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID31, DEM_E_PARAM_POINTER);
    }
    else
    {
        DistSinceDtcCleared = (uint16)PID31value[0] | (uint16)((uint16)PID31value[1] << 8u);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 4D in the Dem by a software component.
 * ServiceId           0xa8
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Dvalue:Buffer containing the contents of PID 4D.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */                                                                       /* MISRA Rule 20.7 */
Dem_SetDataOfPID4D(P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Dvalue) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
/* PRQA S 3432-- */                                                                       /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4D, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID4Dvalue)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4D, DEM_E_PARAM_POINTER);
    }
    else
    {
        OBDTimeMILOn = (uint16)PID4Dvalue[0u] | (uint16)((uint16)PID4Dvalue[1u] << 8u);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 4E in the Dem by a software component.
 * ServiceId           0xa9
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue:Buffer containing the contents of PID 4E.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_SetDataOfPID4E(P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Evalue)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4E, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID4Evalue)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4E, DEM_E_PARAM_POINTER);
    }
    else
    {
        OBDTimeDTCClear = (uint16)PID4Evalue[0u] | (uint16)((uint16)PID4Evalue[1u] << 8u);
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Marks the current OBD driving cycle as having met the criteria for the PFC
 * cycle. ServiceId           0xaa Sync/Async          Synchronous Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetPfcCycleQualified(void)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 iloop;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETPFCCYCLEQUALIFIED, DEM_E_UNINIT);
    }
    else
    {
        PFCStatu = FALSE;
        for (iloop = 0; iloop < DemPbCfgPtr->DemOperationCycleNum; iloop++)
        {
            if (DemPbCfgPtr->DemOperationCycle[iloop].DemOperationCycleType == DEM_OPCYC_OBD_DCY)
            {
                PFCStatu = TRUE;
            }
        }
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Returns TRUE if the criteria for the PFC cycle have been met during the
 current OBD driving cycle.
 * ServiceId           0xab
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     isqualified:TRUE: During the current OBD driving cycle the criteria for the
 PFC cycle have been met. FALSE: During the current OBD driving cycle the criteria for the PFC cycle
 have not been met.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetPfcCycleQualified(P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) isqualified)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 iloop;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_GETPFCCYCLEQUALIFIED, DEM_E_UNINIT);
    }
    else if (NULL_PTR == isqualified)
    {
        DEM_DET_REPORT(DEM_SID_GETPFCCYCLEQUALIFIED, DEM_E_UNINIT);
    }
    else
    {
        for (iloop = 0; iloop < DemPbCfgPtr->DemOperationCycleNum; iloop++)
        {
            /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
            if ((DemPbCfgPtr->DemOperationCycle[iloop].DemOperationCycleType == DEM_OPCYC_OBD_DCY)
                && (DEM_BITS_ISSET(DemOperationCycleStatus, iloop))
                && ((DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
                    && (Dem_GetDemWIRStatus(DEM_MALFUNCTION_LAMP_INDICATOR) != DEM_INDICATOR_OFF)))
            /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
            {
                PFCStatu = TRUE;
            }
        }
        *isqualified = PFCStatu;
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               API to inform the Dem (of a Dependend / Secondary ECU) about the
                       reception of service 04 execution by a software component.
 * ServiceId           0xac
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that has been
 *                     cleared from the event memory.
 *                       DTCFormat:Format of the provided DTC value.
 *                       DTCOrigin:Event memory (e.g. MIRROR)
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Return value unused - only for compatibility with according RTE operation.
 */
/*************************************************************************/
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_SetClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    Dem_ReturnClearDTCType ret = DEM_CLEAR_FAILED;
    Dem_ClearDTCInfoType* pClr;
    boolean ProcessOn = TRUE;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETCLEARDTC, DEM_E_UNINIT);
    }
    else
    {
        pClr = &DemClearDTCInfo;
        if (Dem_ClearDTCLock == DEM_CLEAR_LOCK)
        {
            if ((pClr->SaveDTCIndex == Dem_GetDTCIndex(DTC, DTCFormat)) && (pClr->SaveDTCFormat == DTCFormat)
                && (pClr->SaveDTCOrigin == DTCOrigin) && (pClr->SID == DEM_SID_SETCLEARDTC))
            {
                /* req SWS_Dem_00663 */
                ret = DEM_CLEAR_PENDING;
                ProcessOn = FALSE;
            }
            else
            {
                /* req SWS_Dem_00662  req SWS_Dem_00664 */
                ret = DEM_CLEAR_BUSY;
                ProcessOn = FALSE;
            }
        }
        else if (Dem_ClearDTCLock == DEM_CLEAR_NOT_LOCK)
        {
            /* req SWS_Dem_00661 */
            Dem_ClearDTCLock = DEM_CLEAR_LOCK;
            pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
            pClr->DTCFormat = DTCFormat;
            pClr->DTCOrigin = DTCOrigin;
            pClr->SaveDTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
            pClr->SaveDTCFormat = DTCFormat;
            pClr->SaveDTCOrigin = DTCOrigin;
            pClr->SID = DEM_SID_SETCLEARDTC;
        }
        else
        {
            /*idle*/
        }
        /* req SWS_Dem_00670 */
#if (DEM_CLEAR_DTCLIMITATION == DEM_ONLY_CLEAR_ALL_DTCS)
        if ((ProcessOn == TRUE) && (((DTCFormat != DEM_DTC_FORMAT_UDS) || ((DTC & 0xFFFFFFUL) != 0xFFFFFFUL))))
        {
            Dem_ResetDemClearDTCInfo();
            ret = DEM_CLEAR_WRONG_DTC;
            ProcessOn = FALSE;
        }
#endif /* DEM_CLEAR_DTCLIMITATION == DEM_ONLY_CLEAR_ALL_DTCS */
        if (ProcessOn == TRUE)
        {
            uint8 memDest = Dem_GetInternalMemDest(DTCOrigin);
            if (memDest == DEM_MEM_DEST_INVALID)
            {
                Dem_ResetDemClearDTCInfo();
                ret = DEM_CLEAR_WRONG_DTCORIGIN;
                ProcessOn = FALSE;
            }
            /* req SWS_Dem_00723 req SWS_Dem_01179 */
            if (((DTC & 0xFFFFFFUL) == 0xFFFFFFUL) || ((DTC & 0xFFFFFFUL) == 0xFFFF33UL))
            {
                pClr->ClearAllGroup = TRUE;
            }
            else
            {
                Dem_ResetDemClearDTCInfo();
                ret = DEM_CLEAR_WRONG_DTC;
                ProcessOn = FALSE;
            }
            if (ProcessOn == TRUE)
            {
                pClr->memDest = memDest;
                Dem_Pending = TRUE;
                Dem_ClearDTCProcess();
                Dem_ResetDemClearDTCInfo();
                ret = DEM_CLEAR_OK;
            }
        }
    }
    return ret;
}

#if (DEM_DTR_NUM > 0)
/*************************************************************************/
/*
 * Brief              Reports a DTR result with lower and upper limit. The internal eventstatus
                        serves as master whether the DTR values are forwarded or ignored,
                        also taking the DTRUpdateKind into account.
 * ServiceId           0xa2
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTRId:Identification of a DTR element by assigned DTRId
 *                      TestResult:Test result of DTR
 *                      LowerLimit:Lower limit of DTR
 *                      UpperLimit:Upper limit of DTR
 *                      Ctrlval:Control value of the DTR to support its interpretation
 Dem-internally.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetDTR(uint16 DTRId, sint32 TestResult, sint32 LowerLimit, sint32 UpperLimit, Dem_DTRControlType Ctrlval)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDTR, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterSetDTR(DTRId, TestResult, LowerLimit, UpperLimit, Ctrlval);
    }
    return ret;
}
#endif /*DEM_DTR_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Get the event ID of FreezeFrame.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Identification of an event by assigned EventId.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(Dem_EventMemEntryType*, DEM_CODE)
/* PRQA S 3432,1505++ */ /* MISRA Rule 20.7,8.7 */
Dem_SelectOBDFreezeFrame(P2VAR(Dem_EventIdType, AUTOMATIC, DEM_APPL_DATA) IntId)
/* PRQA S 3432,1505-- */ /* MISRA Rule 20.7,8.7 */
{
    Dem_EventMemEntryType* pEntry;
    Dem_EventMemEntryType* RetpEntry = NULL_PTR;
    uint16 iloop;
    uint8 SelectTimeA = 0xFF;
    uint8 SelectTimeB1 = 0xFF;
    uint8 SelectTimeB2 = 0xFF;
    uint8 SelectTimeC = 0xFF;
    uint8 SelectTime = 0xFF;
    boolean ClassAFind = FALSE;
    boolean ClassB1Find = FALSE;
    boolean ClassB2Find = FALSE;
    boolean ClassCFind = FALSE;
    uint16 DTCRef;
    *IntId = DemPbCfgPtr->DemEventNum;
    const DemObdDTCType* pObdDTC = DemPbCfgPtr->DemObdDTC;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        pEntry = Dem_CheckEventMemEntryExistsAlready(iloop);
        DTCRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
        uint16 DemObdDTCRef = DemPbCfgPtr->DemDTC[DTCRef].DemObdDTCRef;
        if ((pEntry != NULL_PTR) && (pEntry->FFNum != 0u) && (DTCRef != DEM_DTC_REF_INVALID)
            && (DemObdDTCRef != DEM_OBD_DTC_INVALID) && (DEM_OBD_DTC_CFG_INVALID != pObdDTC[DemObdDTCRef].DemDtcValue))
        {
            uint8 DemWWHOBDDTCClass = DemPbCfgPtr->DemDTC[DTCRef].DemWWHOBDDTCClass;
            if (DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_A)
            {
                /* req SWS_Dem_01173 */
                if (pEntry->AbsTime < SelectTimeA)
                {
                    SelectTimeA = pEntry->AbsTime;
                    *IntId = iloop;
                    RetpEntry = pEntry;
                }
                ClassAFind = TRUE;
            }
            if (ClassAFind == FALSE)
            {
                if (DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B1)
                {
                    /* req SWS_Dem_01173 */
                    if (pEntry->AbsTime < SelectTimeB1)
                    {
                        SelectTimeB1 = pEntry->AbsTime;
                        *IntId = iloop;
                        RetpEntry = pEntry;
                    }
                    ClassB1Find = TRUE;
                }
                if (ClassB1Find == FALSE)
                {
                    if (DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B2)
                    {
                        /* req SWS_Dem_01173 */
                        if (pEntry->AbsTime < SelectTimeB2)
                        {
                            SelectTimeB2 = pEntry->AbsTime;
                            *IntId = iloop;
                            RetpEntry = pEntry;
                        }
                        ClassB2Find = TRUE;
                    }
                    if (ClassB2Find == FALSE)
                    {
                        if (DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_C)
                        {
                            /* req SWS_Dem_01173 */
                            if (pEntry->AbsTime < SelectTimeC)
                            {
                                SelectTimeC = pEntry->AbsTime;
                                *IntId = iloop;
                                RetpEntry = pEntry;
                            }
                            ClassCFind = TRUE;
                        }
                        /* req SWS_Dem_01173 */
                        if ((ClassCFind == FALSE) && (pEntry->AbsTime < SelectTime))
                        {
                            SelectTime = pEntry->AbsTime;
                            *IntId = iloop;
                            RetpEntry = pEntry;
                        }
                    }
                }
            }
        }
    }
    return RetpEntry;
}

#if ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) && (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON))
/*************************************************************************/
/*
 * Brief               Service to get the value of PID 21 from the Dem by a software component.
 * ServiceId           0xb1
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID21value:Content of PID 21 as raw hex value.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetDataOfPID21(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value)
{
    /* req SWS_Dem_01098 */
    uint16 DistWithMilOn = 0U;
    uint16 currentDistanceInformation;
    Std_ReturnType ret = E_NOT_OK;

    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_UNINIT);
    }
    else if (NULL_PTR == PID21value)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_PARAM_POINTER);
    }
    else
    {
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
        /* MIL deactivated req SWS_Dem_00704 */
        if (DEM_INDICATOR_OFF != OBDMilStatus)
        {
            /*the indicator is on*/
            currentDistanceInformation = Dem_ReadDistanceInformation();
            if (currentDistanceInformation > OBDDistanceMILLastOn)
            {
                DistWithMilOn = currentDistanceInformation - OBDDistanceMILLastOn;
            }
        }
        if (OBDDistanceMILOn + DistWithMilOn < 0xFFFFu)
        {
            OBDDistanceMILOn += DistWithMilOn;
        }
        else
        {
            OBDDistanceMILOn = 0xFFFFu;
        }
        PID21value[0] = (uint8)OBDDistanceMILOn;
        PID21value[1] = (uint8)(OBDDistanceMILOn >> 8);
        ret = E_OK;
#endif
    }
    return ret;
}
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU && DEM_OBD_CENTRALIZED_PID21_HANDLING == \
          STD_ON */

/*************************************************************************/
/*
 * Brief               Service to report the value of the B1 counter computed by the Dem.
 * ServiceId           0xb4
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     B1Counter:Buffer containing the B1 counter.
 * Param-Name[in/out]  none
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetB1Counter(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) B1Counter)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dem_ReturnClearDTCType ret = E_NOT_OK;
    if (DEM_STATE_INIT != Dem_InitState)
    {
        DEM_DET_REPORT(DEM_SID_GETB1COUNTER, DEM_E_UNINIT);
    }
    else if (NULL_PTR == B1Counter)
    {
        DEM_DET_REPORT(DEM_SID_GETB1COUNTER, DEM_E_PARAM_POINTER);
    }
    else
    {
        *B1Counter = OBDB1Counter;
        ret = E_OK;
    }
    return ret;
}
/* PRQA S 3408-- */ /* MISRA Rule 8.4 */

#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
/*************************************************************************/
/*
 * Brief               Process the B1 Counter
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_B1CounterProcess(void)
{
    uint16 iloop;
    const Dem_EventParameterType* pEventCfg = DemPbCfgPtr->DemEventParameter;
    const Dem_EventRelateInformationType* pEvent = DemEventRelateInformation;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        uint16 DTCRef = pEventCfg[iloop].DemDTCRef;
        if ((DEM_DTC_REF_INVALID != DTCRef)
            && (DemPbCfgPtr->DemDTC[DTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B1)
            && ((DEM_FLAGS_ISSET(pEvent[iloop].UdsStatus, DEM_UDS_STATUS_TF) != 0x00u)
                && (DEM_FLAGS_ISSET(pEvent[iloop].UdsStatus, DEM_UDS_STATUS_CDTC) != 0x00u)))
        {
            /* req SWS_Dem_01154 */
            if (OBDB1Counter < 0xFFu)
            {
                /* req SWS_Dem_01158 */
                OBDB1Counter++;
            }
            if (OBDB1Counter > 200u)
            {
                /* req SWS_Dem_01141 */
                ContinuousMICounter = 0u;
            }
            break;
        }
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif /*DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT || DEM_J1939_SUPPORT == STD_ON*/

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
