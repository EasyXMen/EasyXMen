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
**  FILENAME    : Dem_Internal.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API declaration of DEM for internal                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

#ifndef DEM_INTERNAL_H_
#define DEM_INTERNAL_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_PBcfg.h"
#if (DEM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "Dem_Dcm.h"
#if (STD_ON == DEM_TRIGGER_FIM_REPORTS)
#include "FiM.h"
#endif
#if (DEM_NVRAM_BLOCKID_NUM > 0)
#include "NvM.h"
#endif
#if (STD_ON == DEM_J1939_SUPPORT)
#include "J1939Dcm_Cbk.h"
#endif
#if (DEM_TRIGGER_DCM_REPORTS == STD_ON)
#include "Dcm.h"
#endif

/*******************************************************************************
**                      macros                                                **
*******************************************************************************/
/* Check Event Lock Status */
#define DEM_CHECK_ELOCK_ALL                  0xFFu
#define DEM_CHECK_ELOCK_OPCYC                0x01u
#define DEM_CHECK_ELOCK_ENABLECOND           0x02u
#define DEM_CHECK_ELOCK_DTCSETTING           0x04u
#define DEM_CHECK_ELOCK_AVAILABLE            0x08u
#define DEM_CHECK_ELOCK_COMPONENT            0x10u

#define DEM_CHECK_ELOCK_EXP_OPCYC            0xFEu
#define DEM_CHECK_ELOCK_EXP_ENABLECOND       0xFDu
#define DEM_CHECK_ELOCK_EXP_OPCYC_ENABLECOND 0xFCu

static inline Dem_EventIdType Dem_GetEventInternalId(Dem_EventIdType ExtId)
{
    return ((ExtId)-1u);
}
static inline Dem_EventIdType Dem_GetEventExternalId(Dem_EventIdType ExtId)
{
    return ((ExtId) + 1u);
}

/* Get Bit */
static inline uint8 DEM_GET_BIT(uint8 status, uint8 i)
{
    return (((status & i) == i) ? 1u : 0u);
}

/* FLAGS*/
#define DEM_FLAGS_SET(_status, _val) (_status) |= (_val)
#define DEM_FLAGS_CLR(_status, _val) (_status) &= (uint8)(~(_val))

static inline uint8 DEM_FLAGS_ISSET(uint8 status, uint8 val)
{
    return ((status) & (val));
}

/* Status Bits Functions */
/* _p = &uint8_array[0] */
static inline void DEM_BITS_SET(uint8* p, uint32 n)
{
    ((p)[((n) >> 3u)] |= ((uint8)(1u << ((n) & 7u))));
}
static inline void DEM_BITS_CLR(uint8* p, uint32 n)
{
    ((p)[((n) >> 3u)] &= ((uint8)(~((uint8)(1u << ((n) & 7u))))));
}
static inline boolean DEM_BITS_ISSET(const uint8* p, uint32 n)
{
    return (0x0u != ((p)[((n) >> 3u)] & ((uint8)(1u << ((n) & 7u)))));
}

static inline boolean DEM_BITS_ISNSET(const uint8* p, uint32 n)
{
    return (0x0u == ((p)[((n) >> 3u)] & ((uint8)(1u << ((n) & 7u)))));
}

/*Dem_EventRelateInformationType Status*/
#define DEM_EVENT_STATUS_DTC_SETTING        0x01u
#define DEM_EVENT_STATUS_FDC_TRIGGERED      0x02u
#define DEM_EVENT_STATUS_ENABLED_CONDICTION 0x04u
#define DEM_EVENT_STATUS_STORAGE_CONDICTION 0x08u
#define DEM_EVENT_STATUS_AVAILABLE          0x10u
#define DEM_EVENT_STATUS_TFBITCHANGE        0x20u
#define DEM_EVENT_STATUS_CONSECUTIVE_FAULT  0x40u
#define DEM_EVENT_STATUS_ACTIVE             0x80u

/*Dem_EventRelateInformationType Status*/
#define DEM_EVENT_FLAGS_FDC_TRIGGER    0x01u
#define DEM_EVENT_FLAGS_THISOPISFAILED 0x02u
#define DEM_EVENT_FLAGS_CYCLECOUNTER   0x04u
#define DEM_EVENT_FLAGS_OCC6CLR        0x08u

#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
#define DEM_SI30_DTC_UC     0x01u /*bit 0: UnconfirmedDTC*/
#define DEM_SI30_DTC_UCTOC  0x02u /*bit 1: UnconfirmedDTCThisOperationCyle*/
#define DEM_SI30_DTC_UCSLC  0x04u /*bit 2: UnconfirmedDTCSinceLastClear*/
#define DEM_SI30_DTC_AGED   0x08u /*bit 3: AgedDTC*/
#define DEM_SI30_DTC_SSLC   0x10u /*bit 4: SymptomSinceLastClear*/
#define DEM_SI30_DTC_WIRSLC 0x20u /*bit 5: WarningIndicatorRequestedSinceLastClear*/
#define DEM_SI30_DTC_ER     0x40u /*bit 6: EmissionRelatedDTC*/
#define DEM_SI30_DTC_TSFLC  0x80u /*bit 7: TestFailedSinceLastClear/Aged*/
#endif                            /* DEM_STATUSINDICATOR30ENABLE == STD_ON */

/* total length = priEntry + perEntry + eventdata + obddata + crc*/
#define ENTRY_STORAGE_LEN       (sizeof(Dem_EventMemEntryType))
#define ENTRY_STORAGE_NUM       (DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY + DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT)
#define TOTAL_ENTRY_STORAGE_LEN (ENTRY_STORAGE_LEN * ENTRY_STORAGE_NUM) /* ENTRY_STORAGE_LEN * ENTRY_STORAGE_NUM */

#if (STD_ON == DEM_DEV_ERROR_DETECT)
static inline void DEM_DET_REPORT(uint8 ApiId, uint8 ErrorId) /* PRQA S 3219 */ /* MISRA Rule 2.1 */
{
    (void)Det_ReportError(DEM_MODULE_ID, DEM_INSTANCE_ID, (ApiId), (ErrorId));
}
#else
#define DEM_DET_REPORT(ApiId, ErrorId)
#endif /* STD_ON == DEM_DEV_ERROR_DETECT */

#if !defined(DEM_LOCAL)
#define DEM_LOCAL static
#endif /* !defined(DEM_LOCAL */

#if !defined(DEM_LOCAL_INLINE)
#define DEM_LOCAL_INLINE static inline
#endif /* !defined(DEM_LOCAL_INLINE */

/*
 * Function Pointer type define, with void return value.
 */
typedef P2FUNC(void, NVM_CODE, Dem_ProcessEventPtr)(Dem_EventRelateInformationType* pEvent);
/*******************************************************************************
**                      Global Variable declaration                           **
*******************************************************************************/
extern P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) DemPbCfgPtr;

extern VAR(boolean, AUTOMATIC) Dem_Pending;

extern VAR(Dem_StateType, AUTOMATIC) Dem_InitState;

extern VAR(FDCInfoType, AUTOMATIC) FDCInfo[DEM_DTC_NUM];

extern VAR(Dem_ClearDTCInfoType, AUTOMATIC) DemClearDTCInfo;

extern VAR(Dem_MemDestInfoType, AUTOMATIC) DemMemDestInfo[DEM_MEM_DEST_TOTAL_NUM];

extern VAR(Dem_EventRelateInformationType, AUTOMATIC) DemEventRelateInformation[DEM_EVENT_PARAMETER_NUM];

extern VAR(Dem_DTCGeneralStatusType, AUTOMATIC) DemDTCGeneralStatus[DEM_DTC_NUM];

#if (DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
extern VAR(Dem_DebounceTimeInfoType, AUTOMATIC) DemDebounceTimerInfo[DEM_DEBOUNCE_TIME_BASED_EVENT_NUM];
#endif

extern VAR(boolean, AUTOMATIC) Dem_DTCStatusChangedInd;

extern VAR(Dem_ClearLockStatus, AUTOMATIC) Dem_ClearDTCLock;

extern Dem_IndicatorStatusType OBDMilStatus;

#if (DEM_J1939_DM31_SUPPORT == STD_ON)
extern VAR(DemJ1939DTCGetNodeType, AUTOMATIC) DemJ1939DTCGetNode;
#endif

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
extern uint16 OBDDistanceMILOn;
extern uint16 OBDTimeMILOn;
extern uint16 OBDTimeDTCClear;
extern uint16 DistSinceDtcCleared;
extern uint16 IgnUpCycleCounter;
extern uint16 OBDDistanceMILLastOn;
extern uint16 OBDTimeMILLastOn;
extern uint16 OBDDistanceDTCClear;
extern uint16 OBDTimeDTCLastClear;

extern uint8 WarmUpCycleCounter;

extern uint16 ContinuousMICounter;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
extern uint16 MasterContinuousMICounter;
#endif
extern uint16 OBDB1Counter;
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

extern VAR(Dem_ClearNonvolatileStatusType, AUTOMATIC) Dem_ClearNonvolatileStatus;

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
extern VAR(Dem_PreStoreFFInfoType, AUTOMATIC) DemPreStoreFFInfo[DEM_MAX_NUMBER_PRESTORED_FF];
#endif

extern VAR(Dem_DTCSettingInfoType, AUTOMATIC) DemDTCSettingInfo;

extern VAR(Dem_FreezeFrameRecordFilterInfoType, AUTOMATIC) DemFreezeFrameRecordFilterInfo;

#if (DEM_J1939_SUPPORT == STD_ON)
extern VAR(Dem_J1939DTCFilterInfoType, AUTOMATIC) DemJ1939DTCFilter;

extern VAR(Dem_J1939DcmFreezeFrameFilterType, AUTOMATIC) DemJ1939DcmFreezeFrameFilter;

extern VAR(Dem_J1939DcmRatioFilterType, AUTOMATIC) DemJ1939DcmRatioFilter;

#define DEM_NUMBER_OF_READINESS_GROUPS_FORJ1939 20u

#endif /* DEM_J1939_SUPPORT == STD_ON */

#if (DEM_NVRAM_BLOCKID_NUM > 0)
#if (DEM_NVRAM_DIVADED == STD_OFF)
extern VAR(Dem_EventMemEntryType, AUTOMATIC) DemEventMemoryEntryStorage[ENTRY_STORAGE_NUM];
#else
extern VAR(Dem_EventMemEntryType, AUTOMATIC) DemPrimaryMemory[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
/* DemGeneral/DemMaxNumberEventEntryPermanent [0-255]*/
extern VAR(Dem_EventMemEntryType, AUTOMATIC) DemPermanentMemory[DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT];
#endif /* DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0 */
#endif /* DEM_NVRAM_DIVADED == STD_OFF */

extern VAR(Dem_EventRelateInformationStorageType, AUTOMATIC) DemEventRelateInformationStorage;

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
extern VAR(DemOBDDataStorageType, AUTOMATIC) DemOBDDataStorage;
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */
#endif /* DEM_NVRAM_BLOCKID_NUM > 0 */

/*******************************************************************************
**                      Global Function declaration                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               MemSet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Val && Size
 * Param-Name[out]     none
 * Param-Name[in/out]  Dest
 * Return              none
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) Dem_MemSet(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Dest, uint8 Val, uint32 Size)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 iloop;
    for (iloop = 0; iloop < Size; iloop++)
    {
        Dest[iloop] = Val;
    }
}

/*************************************************************************/
/*
 * Brief               MemCopy
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Src && Size
 * Param-Name[out]     Dest
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) Dem_MemCopy(uint8* Dest, const uint8* Src, uint32 Size)
{
    for (uint32 iloop = 0; iloop < Size; iloop++)
    {
        Dest[iloop] = Src[iloop];
    }
}

/*************************************************************************/
/*
 * Brief               Get EventDTCAttributesCfg
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              const Dem_DTCAttributesType*
 */
/*************************************************************************/
DEM_LOCAL_INLINE FUNC(P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST), DEM_CODE)
    Dem_EventDTCAttributesCfg(Dem_EventIdType IntId)
{
    uint16 Ref;
    const Dem_DTCAttributesType* pRes = NULL_PTR;

    /* DtcRef */
    Ref = DemPbCfg.DemEventParameter[IntId].DemDTCRef;
    if (Ref != DEM_DTC_REF_INVALID)
    {
        /* DTCAttributesRef */
        Ref = DemPbCfg.DemDTC[Ref].DemDTCAttributesRef;
        pRes = &DemPbCfg.DemDTCAttributes[Ref];
    }
    return pRes;
}
/*************************************************************************/
/*
 * Brief               Dem_ResetDemClearDTCInfo
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      Id
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) Dem_ResetDemClearDTCInfo(void)
{
    Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
    pClr->DTCIndex = DEM_DTC_REF_INVALID;
    pClr->DTCGroupIndex = DEM_GROUP_OF_DTC_INVALID;
    pClr->ClearAllGroup = FALSE;
    Dem_ClearDTCLock = DEM_CLEAR_NOT_LOCK;
}

/*************************************************************************/
/*
 * Brief               Dem_UpdateCombinedDtcStatus
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_UpdateCombinedDtcStatus(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && Status
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SetOccurrenceEvent(Dem_EventIdType IntId, uint8 Status);

/*************************************************************************/
/*
 * Brief               Dem_DTCGetFaultDetectionCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId
 * Param-Name[out]     FaultDetectionCounter
 * Param-Name[in/out]  none
 * Return              Std_ReturnType
 */
/*************************************************************************/

extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_DTCGetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter);

/*************************************************************************/
/*
 * Brief               Dem_CheckEntryAlreadyAllocated
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_ReturnCheckEntryIsStoredType
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_CheckEntryAlreadyAllocated(
    Dem_EventIdType IntId,
    uint8 memDest,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
        P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) * pEntry);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
/*************************************************************************/
/*
 * Brief               Dem_EventDisplacementProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      InternalId && MemIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_EventMemEntryType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_EventDisplacementProcess(Dem_EventIdType InternalId, uint8 memDest);
#endif /* DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE */

/*************************************************************************/
/*
 * Brief               Dem_ClearDTCProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_ClearDTCProcess(void);

/*************************************************************************/
/*
 * Brief               Dem_GetEventDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && DTCFormat
 * Param-Name[out]     DTC
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetEventDTC(uint16 DTCRef, Dem_DTCFormatType DTCFormat, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC && DTCFormat
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              DTC Index
 */
/*************************************************************************/
extern FUNC(uint16, DEM_CODE) Dem_GetDTCIndex(uint32 DTC, Dem_DTCFormatType DTCFormat);

/*************************************************************************/
/*
 * Brief               Dem_UpdateFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_UpdateFDC(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               GetDTCGroupIndex
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      Id
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              DTCGroupIndex
 */
/*************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) Dem_GetDTCGroupIndex(uint32 DTCGroup)
{
    uint8 ret = DEM_GROUP_OF_DTC_INVALID;
#if (DEM_GROUP_OF_DTC_NUM > 0u)
    uint8 iloop;
    for (iloop = 0; iloop < DEM_GROUP_OF_DTC_NUM; iloop++) /* PRQA S 2877,2994,2996 */ /* MISRA Dir 4.1,Rule 14.3,2.2 */
    {
        if (DemPbCfg.DemGroupOfDTC[iloop] == DTCGroup)
        {
            ret = iloop;
            break;
        }
    }
#else
    DEM_UNUSED(DTCGroup);
#endif
    return ret;
}

#if (DEM_DATA_ELEMENT_CLASS_NUM > 0u)
/*************************************************************************/
/*
 * Brief               Dem_GetFncData>
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DataReadFnc,DataSize
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_GetFncData(DemDataElementReadFncType DataReadFnc, uint8* pBuffer, uint8 DataSize);
#endif /* DEM_DATA_ELEMENT_CLASS_NUM > 0u */

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * Brief               Dem_PreStoreFFGet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_PreStoreFFInfoType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE) Dem_PreStoreFFGet(Dem_EventIdType ExtId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_MAX_NUMBER_PRESTORED_FF > 0 */

/*************************************************************************/
/*
 * Brief               Dem_UpdateInternalData
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_UpdateInternalData(uint16 IntId);
/*******************************************************************************
**                      EventQueue                                            **
*******************************************************************************/
/*******************************************************************************
**                      EventMemory                                           **
*******************************************************************************/
/*************************************************************************/
/*
 *  Brief               Dem_CheckEventMemEntryExistsAlready
 *  ServiceId           --
 *  Sync/Async          Synchronous
 *  Reentrancy          Reentrant
 *  Param-Name[in]      IntId
 *  Param-Name[out]     none
 *  Param-Name[in/out]  none
 *  Return              Dem_EventMemEntryType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_CheckEventMemEntryExistsAlready(Dem_EventIdType IntId);

/*************************************************************************/
/*
 *  Brief               Dem_GetEventInfo
 *  ServiceId           --
 *  Sync/Async          Synchronous
 *  Reentrancy          Reentrant
 *  Param-Name[in]      IntId
 *  Param-Name[out]     none
 *  Param-Name[in/out]  none
 *  Return              Dem_EventRelateInformationType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_GetEventInfo(Dem_EventIdType IntId);

/*************************************************************************/
/*
 *  Brief               Dem_EventTestFailed
 *  ServiceId           --
 *  Sync/Async          Synchronous
 *  Reentrancy          Reentrant
 *  Param-Name[in]      IntId && pEventBuffer
 *  Param-Name[out]     none
 *  Param-Name[in/out]  none
 *  Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_EventTestFailed(Dem_EventIdType IntId);

/*************************************************************************/
/*
 *  Brief               Dem_EventTestPassed
 *  ServiceId           --
 *  Sync/Async          Synchronous
 *  Reentrancy          Reentrant
 *  Param-Name[in]      IntId && pEventBuffer
 *  Param-Name[out]     none
 *  Param-Name[in/out]  none
 *  Return              none
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DEM_CODE) Dem_EventTestPassed(Dem_EventIdType IntId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_MemEntryGet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId && MemDestIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_EventMemEntryType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DEM_LOCAL_INLINE FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_MemEntryGet(Dem_EventIdType ExtId, uint8 memDest)
{
    Dem_EventMemEntryType* pEntry = NULL_PTR;
    uint8 iloop;
    Dem_EventMemEntryType* pEntryList = DemMemDestCfg[memDest].EntryList;
    const uint8 EntryNum = DemMemDestCfg[memDest].EntryNum;
    /*primary total number*/
    for (iloop = 0; iloop < EntryNum; iloop++)
    {
        if (pEntryList[iloop].EventId == ExtId)
        {
            pEntry = &pEntryList[iloop];
            break;
        }
    }
    return pEntry;
}

#if ((DEM_ENABLE_CONDITION_NUM > 0) || (DEM_STORAGE_CONDITION_NUM > 0))
/*************************************************************************/
/*
 * Brief               CheckCondictionFulfilled
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cond && group && len
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              boolean
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) Dem_CheckCondictionFulfilled(
    P2CONST(uint8, AUTOMATIC, DEM_CONST) cond,
    P2CONST(uint8, AUTOMATIC, DEM_CONST) group,
    uint8 len)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    boolean res = TRUE;
    uint8 iloop;
    for (iloop = 0; (iloop < len) && (res != FALSE); iloop++)
    {
        if ((cond[iloop] & group[iloop]) != group[iloop])
        {
            res = FALSE;
        }
    }
    return res;
}
#endif /* DEM_ENABLE_CONDITION_NUM > 0 || DEM_STORAGE_CONDITION_NUM > 0 */

#if (DEM_NVRAM_BLOCKID_NUM > 0)
/*************************************************************************/
/*
 *  Brief               Dem_IntWriteNvRAM
 *  ServiceId           --
 *  Sync/Async          Synchronous
 *  Reentrancy          Reentrant
 *  Param-Name[in]      None
 *  Param-Name[out]     None
 *  Param-Name[in/out]  pEvent
 *  Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_IntWriteNvRAM(void);
#endif

/*************************************************************************/
/*
 * Brief               Dem_Clear
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      memDest && pEntry && pEvent
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_Clear(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent, uint8 memDest);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_MemEntryAllocate
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && MemDestIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_EventMemEntryType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_MemEntryAllocate(Dem_EventIdType IntId, uint8 memDest);

/*******************************************************************************
**                      OBD                                                   **
*******************************************************************************/
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
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
extern FUNC(void, DEM_CODE) Dem_OBDInit(void);

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
extern FUNC(Dem_EventMemEntryType*, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_SelectOBDFreezeFrame(P2VAR(Dem_EventIdType, AUTOMATIC, DEM_APPL_DATA) IntId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Get the Current Distance Information.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(uint16, DEM_CODE) Dem_ReadDistanceInformation(void);

/*************************************************************************/
/*
 * Brief               Get the Current Time Since Engine Start.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(uint16, DEM_CODE) Dem_ReadTimeInformation(void);

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
extern FUNC(void, DEM_CODE) Dem_UpdateOBDMilStatus(uint8 indicatorRef, uint16 IntID);
#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU */

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
extern FUNC(void, DEM_CODE) Dem_CalOBDRelatedValue(uint16 OperationId);
#endif
#endif

#if (DEM_J1939_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Init the J1939 related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_J1939Init(void);
#endif /* DEM_J1939_SUPPORT == STD_ON */

/*************************************************************************/
/*
 * Brief               Dem_EventQueueAdd
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ExtId && Status
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_EventQueueAdd(Dem_EventIdType ExtId, Dem_EventStatusType Status);

#if ((DEM_NVRAM_DIVADED == STD_ON) && (DEM_NVRAM_BLOCKID_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_IntWriteOneNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_IntWriteOneNvRAM(uint16 IntId);
#endif

/*************************************************************************/
/*
 * Brief               Dem_SetDTCSettingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SetDTCSettingProcess(void);

#if (DEM_J1939_NODE_NUM > 0u)
/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(boolean, DEM_CODE) Dem_J1939CeckNode(uint16 dtcRef, uint8 node);
#endif /* DEM_J1939_NODE_NUM > 0u */

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
/*************************************************************************/
/*
 * Brief               Dem_CDTCOverFlowCheck
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
extern void Dem_CDTCOverFlowCheck(Dem_EventIdType IntId);
#endif /* DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON */

#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_DebounceProcessCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer && pEventCfg
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessCounter(Dem_EventIdType ExtId, Dem_EventStatusType Status);
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
#endif

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
#endif /* DEM_INTERNAL_H_ */
