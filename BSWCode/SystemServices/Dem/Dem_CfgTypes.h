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
**  FILENAME    : Dem_CfgTypes.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : CfgType definitions of DEM                                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef DEM_CFGTYPES_H_
#define DEM_CFGTYPES_H_

/*******************************************************************************
**                           Include Section                                  **
*******************************************************************************/
#include "Dem_PBcfg.h"

/*******************************************************************************
**                      Macro Definitions                                     **
*******************************************************************************/
/* DemEventMemoryEntryStorageTrigger */
#define DEM_TRIGGER_ON_CONFIRMED     0x00u
#define DEM_TRIGGER_ON_FDC_THRESHOLD 0x01u
#define DEM_TRIGGER_ON_MIRROR        0x02u
#define DEM_TRIGGER_ON_PASSED        0x03u
#define DEM_TRIGGER_ON_PENDING       0x04u
#define DEM_TRIGGER_ON_TEST_FAILED   0x05u

/* DemAvailabilitySupport */
#define DEM_EVENT_AVAILABILITY 1u
#define DEM_NO_AVAILABILITY    2u

/* DemClearDTCBehavior  */
#define DEM_CLRRESP_NONVOLATILE_FINISH  1u
#define DEM_CLRRESP_NONVOLATILE_TRIGGER 2u
#define DEM_CLRRESP_VOLATILE            3u

/* DemClearDTCLimitation */
#define DEM_ALL_SUPPORTED_DTCS  1u
#define DEM_ONLY_CLEAR_ALL_DTCS 2u

/* DemEnvironmentDataCapture */
#define DEM_CAPTURE_ASYNCHRONOUS_TO_REPORTING 1u
#define DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING  2u

/* DemEventCombinationSupport */
#define DEM_EVCOMB_DISABLED    1u
#define DEM_EVCOMB_ONRETRIEVAL 2u
#define DEM_EVCOMB_ONSTORAGE   3u

/* DemEventDisplacementStrategy  */
#define DEM_DISPLACEMENT_FULL     1u
#define DEM_DISPLACEMENT_NONE     2u
#define DEM_DISPLACEMENT_PRIO_OCC 3u

/* DemOBDSupport  */
#define DEM_OBD_DEP_SEC_ECU    1u
#define DEM_OBD_MASTER_ECU     2u
#define DEM_OBD_NO_OBD_SUPPORT 3u
#define DEM_OBD_PRIMARY_ECU    4u

/* DemOccurrenceCounterProcessing  */
#define DEM_PROCESS_OCCCTR_CDTC  1u
#define DEM_PROCESS_OCCCTR_TF    2u
#define DEM_PROCESS_OCCCTR_TFTOC 3u

/* DemStatusBitHandlingTestFailedSinceLastClear  */
#define DEM_STATUS_BIT_AGING_AND_DISPLACEMENT 1u
#define DEM_STATUS_BIT_NORMAL                 2u

/* DemSuppressionSupport  */
#define DEM_DTC_SUPPRESSION 1u
#define DEM_NO_SUPPRESSION  2u

/* DemTypeOfDTCSupported  */
/* DEM_DTC_TRANSLATION_ISO11992_4 1u
 * DEM_DTC_TRANSLATION_ISO14229_1 2u
 * DEM_DTC_TRANSLATION_ISO15031_6 3u
 * DEM_DTC_TRANSLATION_SAEJ1939_73 4u
 */
#define DEM_DTC_TRANSLATION_SAE_J2012_DA_DTCFORMAT_04 5u

/* DemDtrUpdateKind */
#define DEM_DTR_UPDATE_ALWAYS 1u
#define DEM_DTR_UPDATE_STEADY 2u

#define DEM_UPDATE_RECORD_NO  0u
#define DEM_UPDATE_RECORD_YES 1u

/* DemOperationCycleType */
#define DEM_OPCYC_IGNITION 0u
#define DEM_OPCYC_OBD_DCY  1u
#define DEM_OPCYC_OTHER    2u
#define DEM_OPCYC_POWER    3u
#define DEM_OPCYC_TIME     4u
#define DEM_OPCYC_WARMUP   5u

/* DemIUMPRDenGroup */
#define DEM_IUMPR_DEN_500MILL   0u
#define DEM_IUMPR_DEN_COLDSTART 1u
#define DEM_IUMPR_DEN_EVAP      2u
#define DEM_IUMPR_DEN_NONE      3u
#define DEM_IUMPR_DEN_PHYS_API  4u

/* DemInternalDataElement  */
#define DEM_AGINGCTR_DOWNCNT             0u
#define DEM_AGINGCTR_UPCNT               1u
#define DEM_CURRENT_FDC                  2u
#define DEM_CYCLES_SINCE_FIRST_FAILED    3u
#define DEM_CYCLES_SINCE_LAST_FAILED     4u
#define DEM_FAILED_CYCLES                5u
#define DEM_MAX_FDC_DURING_CURRENT_CYCLE 6u
#define DEM_MAX_FDC_SINCE_LAST_CLEAR     7u
#define DEM_OCCCTR                       8u
#define DEM_OVFLIND                      9u
#define DEM_SIGNIFICANCE                 10u

/* DemWWHOBDDTCClass */
#define DEM_DTC_WWHOBD_CLASS_NOCLASS 0u
#define DEM_DTC_WWHOBD_CLASS_A       1u
#define DEM_DTC_WWHOBD_CLASS_B1      2u
#define DEM_DTC_WWHOBD_CLASS_B2      3u
#define DEM_DTC_WWHOBD_CLASS_C       4u

/* DemDTCSignificance */
#define DEM_EVENT_SIGNIFICANCE_FAULT      1u
#define DEM_EVENT_SIGNIFICANCE_OCCURRENCE 0u

/* DemDebounceBehavior */
#define DEM_DEBOUNCE_FREEZE               0u
#define DEM_DEBOUNCE_RESET                1u

#define DEM_NO_STATUS_BYTE_CHANGE         0u
#define DEM_ONLY_THIS_CYCLE_AND_READINESS 1u

/* DemEventKind */
#define DEM_EVENT_KIND_BSW 0u
#define DEM_EVENT_KIND_SWC 1u

/* DemReportBehavior */
#define REPORT_AFTER_INIT  0u
#define REPORT_BEFORE_INIT 1u

/* DemDebounceType */
#define DEM_DEBOUNCE_COUNTER_BASED    0u
#define DEM_DEBOUNCE_TIME_BASE        1u
#define DEM_DEBOUNCE_MONITOR_INTERNAL 2u

/* DemEventOBDReadinessGroup */
#define DEM_OBD_RDY_MISF          0u
#define DEM_OBD_RDY_FLSYS         1u
#define DEM_OBD_RDY_CMPRCMPT      2u
#define DEM_OBD_RDY_CAT           4u
#define DEM_OBD_RDY_HTCAT         5u
#define DEM_OBD_RDY_EVAP          6u
#define DEM_OBD_RDY_SECAIR        7u
#define DEM_OBD_RDY_AC            8u
#define DEM_OBD_RDY_O2SENS        9u
#define DEM_OBD_RDY_O2SENSHT      10u
#define DEM_OBD_RDY_ERG           11u
#define DEM_OBD_RDY_BOOSTPR       13u
#define DEM_OBD_RDY_EGSENS        20u
#define DEM_OBD_RDY_FLSYS_NONCONT 21u
#define DEM_OBD_RDY_HCCAT         22u
#define DEM_OBD_RDY_NONE          23u
#define DEM_OBD_RDY_NOXCAT        24u
#define DEM_OBD_RDY_PMFLT         25u

#define DEM_OBD_RDY_INVAILD       0xFFu

/* DemOBDEngineType */
#define DEM_IGNITION_COMPRESSION 0u
#define DEM_IGNITION_SPARK       1u

/*Invalid*/
#define DEM_DATA_ELEMENT_CLASS_NUM_INVALID  0xFFu
#define DEM_ENABLE_CONDITION_GROUP_INVALID  0xFFu
#define DEM_EXTENDED_DATA_RECORD_INVALID    0xFFu
#define DEM_EXTENDED_DATA_INVALID           0xFFFFu
#define DEM_FREEZE_FRAME_INVALID            0xFFFFu
#define DEM_FREEZE_FRAME_RECORD_INVALID     0xFFu
#define DEM_FREEZE_FRAME_REC_NUM_INVALID    0xFFu
#define DemIndicator_INVALID                0xFFu
#define DEM_GROUP_OF_DTC_INVALID            0xFFu
#define DEM_MEM_DEST_INVALID                0xFFu
#define DEM_STORAGE_CONDITION_GROUP_INVALID 0xFFu
#define DEM_EVENT_PARAMETER_INVALID         0xFFFFu
#define DEM_COMPONENT_INVALID               0xFFFFu
#define DEM_DTC_REF_INVALID                 0xFFFFu
#define DEM_OBD_DTC_INVALID                 0xFFFFu
#define DEM_DTC_CFG_INVALID                 0xFFFFFFu
#define DEM_OBD_DTC_CFG_INVALID             0xFFFFu
#define DEM_J1939_DTC_CFG_INVALID           0xFFFFFFu
#define DEM_OPERATION_CYCLE_INVALID         0xFFu
#define DEM_J1939_NODE_INVALID              0xFFu
#define DEM_J1939_FREEZE_FRAME_INVALID      0xFFu
#define DEM_WWHOBD_FREEZE_FRAME_INVALID     0xFFu

#define DEM_BITS_TO_BYTE(x)                 (((x) + 7u) >> 3u)

/*DEM_BITS_TO_BYTE*/
#define DEM_ENABLE_CONDITION_NUM_BYTE      DEM_BITS_TO_BYTE(DEM_ENABLE_CONDITION_NUM)

#define DEM_EXTENDED_DATA_MAX_REF_NUM_BYTE DEM_BITS_TO_BYTE(DEM_EXTENDED_DATA_MAX_REF_NUM)

#define DEM_GROUP_OF_DTC_NUM_BYTE          DEM_BITS_TO_BYTE(DEM_GROUP_OF_DTC_NUM)

#define DEM_MEM_DEST_MAX_ENTRY_NUM_BYTE    DEM_BITS_TO_BYTE(DEM_MEM_DEST_MAX_ENTRY_NUM)

#define DEM_OPERATION_CYCLE_NUM_BYTE       DEM_BITS_TO_BYTE(DEM_OPERATION_CYCLE_NUM)

#define DEM_STORAGE_CONDITION_NUM_BYTE     DEM_BITS_TO_BYTE(DEM_STORAGE_CONDITION_NUM)

#define DEM_EVENT_PARAMETER_NUM_BYTE       DEM_BITS_TO_BYTE(DEM_EVENT_PARAMETER_NUM)

#define DEM_DTC_NUM_BYTE                   DEM_BITS_TO_BYTE(DEM_DTC_NUM)

#define DEM_OBD_DTC_NUM_BYTE               DEM_BITS_TO_BYTE(DEM_OBD_DTC_NUM)

typedef struct
{
    /* All Group */
    boolean AllGroupIsEnabled;
    Dem_DTCKindType DTCKind;
#if (DEM_GROUP_OF_DTC_NUM > 0)
    uint8 DTCGroupStatus[DEM_GROUP_OF_DTC_NUM_BYTE];
#endif
} Dem_DTCSettingInfoType;

/*******************************************************************************
 *                          General Configuration                              **
 *******************************************************************************/
typedef Std_ReturnType (*Dem_ReadDataFncType)(uint8* Buffer, uint16 BufSize);

typedef Std_ReturnType (*DataServicesType)(uint8* Buffer);

/*******************************************************************************
 *                             Global Data Types                               **
 *******************************************************************************/
/* DemGeneral/DemOperationCycle 1-256 */
typedef struct
{
    /* DemOperationCycleAutomaticEnd Range: true or false */
    const boolean DemOperationCycleAutomaticEnd;

    /* DemOperationCycleAutostart Range: true or false */
    const boolean DemOperationCycleAutostart;

    /* DemOperationCycleType type:
    Enum{
    DEM_OPCYC_IGNITION
    DEM_OPCYC_OBD_DCY
    DEM_OPCYC_OTHER
    DEM_OPCYC_POWER
    DEM_OPCYC_TIME
    DEM_OPCYC_WARMUP
    } */
    const uint8 DemOperationCycleType;
} Dem_OperationCycleType;

#if (DEM_STORAGE_CONDITION_NUM > 0)
/* DemGeneral/DemStorageCondition 0-255 */
typedef struct
{
    /* DemStorageConditionReplacementEventRef Reference: DemEventParameter MULTI:0-1*/
    const uint16 DemStorageConditionReplacementEventRef;

    /* DemStorageConditionStatus Range: true or false */
    const boolean DemStorageConditionStatus;
} Dem_StorageConditionType;

extern const Dem_StorageConditionType DemStorageCondition[DEM_STORAGE_CONDITION_NUM];
#endif

/*******************************************************************************
 *                          FreezeFrame Configuration                          **
 *******************************************************************************/
/* DemGeneral/DemPidClass 0-0xFFFF */
typedef struct
{
    const uint16 DemPidIdentifier;
    const uint16 StartIndex;
    const uint8 RefNum;
    const uint8 DataSize; /*added by yt  means the total size of dataelement*/
} Dem_PidClassType;

/* DemGeneral/DemFreezeFrameClass 0-65535 */
typedef struct
{
    const uint16 DataSize;
    const uint16 StartIndex;
    const uint8 RefNum;
} Dem_FreezeFrameClassType;
/* DemGeneral/DemDidClass 0-0xFFFF */
typedef struct
{
    const uint16 DemDidIdentifier;
    const uint16 StartIndex;
    const uint8 RefNum;
    const uint8 DataSize; /*added by yt  means the total size of dataelement*/
} Dem_DidClassType;

#if (DEM_FREEZE_FRAME_CLASS_NUM > 0)
extern CONST(Dem_FreezeFrameClassType, AUTOMATIC) DemFreezeFrameClass[DEM_FREEZE_FRAME_CLASS_NUM];

#if (DEM_DID_CLASS_NUM > 0)
extern CONST(Dem_DidClassType, AUTOMATIC) DemDidClass[DEM_DID_CLASS_NUM];
extern CONST(uint16, AUTOMATIC) DemDidClassRef[DEM_DID_CLASS_REF_TOTAL_NUM];
#endif
#endif

/* DemGeneral/DemDidClass 0-0xFFFF */
typedef struct
{
    const uint16 DemSPNIdentifier;
    const uint16 StartIndex;
    const uint8 DataSize; /*added by yt  means the total size of dataelement*/
} Dem_SPNClassType;

typedef struct
{
    const uint16 DataSize;
    const uint16 StartIndex;
    const uint8 RefNum;
} Dem_J1939FreezeFrameClassType;

#if (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
extern CONST(Dem_J1939FreezeFrameClassType, AUTOMATIC) DemJ1939FreezeFrameClass[DEM_J1939_FREEZE_FRAME_CLASS_NUM];

#if (DEM_SPN_CLASS_NUM > 0)
extern CONST(Dem_SPNClassType, AUTOMATIC) DemSPNClass[DEM_SPN_CLASS_NUM];
extern CONST(uint16, AUTOMATIC) DemSPNClassRef[DEM_SPN_CLASS_REF_TOTAL_NUM];
#endif
#endif

/* DemGeneral/DemFreezeFrameRecordClass 0-255 */
typedef struct
{
    /* DemFreezeFrameRecordNumber Range:0~254 */
    const uint8 DemFreezeFrameRecordNumber;

    /* DemFreezeFrameRecordTrigger type:
    Enum{
    DEM_TRIGGER_ON_CONFIRMED
    DEM_TRIGGER_ON_FDC_THRESHOLD
    DEM_TRIGGER_ON_PENDING
    DEM_TRIGGER_ON_TEST_FAILED
    } */
    const uint8 DemFreezeFrameRecordTrigger;

    /* DemFreezeFrameRecordUpdate type:
    Enum{
    DEM_UPDATE_RECORD_NO
    DEM_UPDATE_RECORD_YES
    } */
    const uint8 DemFreezeFrameRecordUpdate;
} Dem_FreezeFrameRecordClassType;

#if (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0)
extern CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC) DemFreezeFrameRecordClass[DEM_FREEZE_FRAME_RECORD_CLASS_NUM];

#endif

typedef struct
{
    /* GlobalDemFreezeFrameClassRef  */
    uint16 DemFreezeFrameClassRef;
    /* DemFreezeFrameRecordNumber Range:1~254 */
    const uint8 DemFreezeFrameRecordNumber;

    /* DemFreezeFrameRecordTrigger type:
    Enum{
    DEM_TRIGGER_ON_CONFIRMED
    DEM_TRIGGER_ON_FDC_THRESHOLD
    DEM_TRIGGER_ON_PENDING
    DEM_TRIGGER_ON_TEST_FAILED
    } */
    const uint8 DemFreezeFrameRecordTrigger;

    /*  DEM_UPDATE_RECORD_NO */
    const uint8 DemFreezeFrameRecordUpdate;

} Dem_GlobalFreezeFrameType;

#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
extern CONST(Dem_GlobalFreezeFrameType, AUTOMATIC) DemGlobalFreezeFrame;

#endif

/* DemGeneral/DemFreezeFrameRecNumClass 0-255 */
typedef struct
{
    /* DemFreezeFrameRecordClassRef 1-254 */
    const uint16 StartIndex;
    const uint8 RefNum;
} Dem_FreezeFrameRecNumClassType;

#if (DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM > 0)
/* DemFreezeFrameRecNumClass 0-255 */
extern CONST(Dem_FreezeFrameRecNumClassType, AUTOMATIC) DemFreezeFrameRecNumClass[DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM];

extern CONST(uint8, AUTOMATIC) DemFreezeFrameRecordClassRef[DEM_FREEZE_FRAME_RECORD_CLASS_REF_TOTAL_NUM];
#endif

typedef struct
{
    uint8 UdsStatus[DEM_EVENT_PARAMETER_NUM];
    sint16 DebounceCounter[DEM_EVENT_PARAMETER_NUM];
#if (DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON)
    uint8 MaxFDCSinceLastClear[DEM_DTC_NUM];
#endif
#if (DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON)
    uint8 DemOperationCycleStatus[DEM_OPERATION_CYCLE_NUM_BYTE]; /*SWS_Dem_00577]*/
#endif
    Dem_EventIdType FirstFailed;
    Dem_EventIdType MostRecentFailed;
    Dem_EventIdType FirstDtcConfirmed;
    Dem_EventIdType MostRecDtcConfirmed;
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    uint8 StatusIndicator30[DEM_EVENT_PARAMETER_NUM];
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    uint8 AgedCounter[DEM_EVENT_PARAMETER_NUM];
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
} Dem_EventRelateInformationStorageType;

typedef struct
{
    uint16 IgnUpCycleCounter;
    uint8 WarmUpCycleCounter;
    uint16 OBDDistanceMILOn;
    uint16 DistSinceDtcCleared;
    uint16 OBDTimeMILOn;
    uint16 OBDTimeDTCClear;
    uint16 ContinuousMICounter;
    uint16 OBDB1Counter;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
    uint16 MasterContinuousMICounter;
#endif
} DemOBDDataStorageType;

typedef struct
{
    /* EventId, Status: Dem_ReportErrorStatus/Dem_SetEventStatus()*/
    Dem_EventIdType ExtId;
    Dem_EventStatusType Status;
} Dem_EventDataBufferType;

/* Debounce Time Base Info Define */
typedef struct
{
    uint64 Timeout;
    boolean Triggered;
    Dem_EventIdType IntId;
    Dem_EventStatusType CurStatus;
    boolean IsFreezing;
} Dem_DebounceTimeInfoType;

typedef struct
{
    uint8 RecordNum;
    uint8 DisableDTCRecordUpdate[DEM_DTC_NUM_BYTE]; /* 1, Disable, 0:Enable */
    boolean OverFlow;
} Dem_MemDestInfoType;

typedef struct
{
    Dem_EventIdType ExtId;
#if (DEM_FREEZE_FRAME_MAX_LEN > 0u)
    uint8 FFData[DEM_FREEZE_FRAME_MAX_LEN];
#endif
} Dem_PreStoreFFInfoType;

#if (DEM_BSW_ERROR_BUFFER_SIZE > DEM_MAX_NUMBER_EVENT_ENTRY_EVENT_BUFFER)
#define DEM_EVENT_QUEUE_SIZE DEM_BSW_ERROR_BUFFER_SIZE
#else
#define DEM_EVENT_QUEUE_SIZE DEM_MAX_NUMBER_EVENT_ENTRY_EVENT_BUFFER
#endif

typedef struct
{
    Dem_EventDataBufferType Queue[DEM_EVENT_QUEUE_SIZE];
    uint8 ReadIndex;
    uint8 WriteIndex;
} Dem_EventQueueType;
/*******************************************************************************
 *                          ExtendedData Configuration                         **
 *******************************************************************************/

/* DemGeneral/DemExtendedDataRecordClass 0-253 */
typedef struct
{
    /* DemExtendedDataRecordNumber Range:1~239 */
    const uint8 DemExtendedDataRecordNumber;

    /* DemExtendedDataRecordTrigger type:
     Enum{
     DEM_TRIGGER_ON_CONFIRMED
     DEM_TRIGGER_ON_FDC_THRESHOLD
     DEM_TRIGGER_ON_MIRROR
     DEM_TRIGGER_ON_PASSED
     DEM_TRIGGER_ON_PENDING
     DEM_TRIGGER_ON_TEST_FAILED
     } */
    const uint8 DemExtendedDataRecordTrigger;

    /* DemExtendedDataRecordUpdate type:
     Enum{
     DEM_UPDATE_RECORD_NO
     DEM_UPDATE_RECORD_YES
     } */
    const uint8 DemExtendedDataRecordUpdate;

    /* DemDataElementClassRef Reference: DemDataElementClass MULTI:1-255*/
    const uint16 DemDataElementClassIndex;
    const uint8 DemDataElementClassNum;
    const uint8 DataSize; /*total size of all DataElementClass*/
} Dem_ExtendedDataRecordClassType;

/* DemGeneral/DemExtendedDataClass 0-* */
typedef struct
{
    /* DemExtendedDataRecordClassRef 1-253 */
    const uint16 StartIndex;
    const uint8 RefNum;
} Dem_ExtendedDataClassType;

#if (DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
extern CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC) DemExtendedDataRecordClass[DEM_EXTENDED_DATA_RECORD_CLASS_NUM];

#endif

#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
extern CONST(Dem_ExtendedDataClassType, AUTOMATIC) DemExtendedDataClass[DEM_EXTENDED_DATA_CLASS_NUM];

extern CONST(uint8, AUTOMATIC) DemExtendedDataRecordClassRef[DEM_EXTENDED_DATA_RECORD_CLASS_REF_TOTAL_NUM];
#endif

/*******************************************************************************
 *                          DTC Configuration                                  **
 *******************************************************************************/
/* DemGeneral/DemCallbackDTCStatusChanged 0..* */
typedef Std_ReturnType (
    *Dem_TriggerOnDTCStatusType)(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew);

/* DemConfigSet/DemDTC 0-65535 */
typedef struct
{
    /* DemDtcValue Range:1~16777214 */
    const uint32 DemDtcValue;
    /* DemDTCAttributesRef Reference: DemDTCAttributes MULTI:1-1*/
    const uint16 DemDTCAttributesRef;
    /*Reference to DemObdDTC MULTI:0-1 */
    const uint16 DemObdDTCRef;

    const Dem_DTCKindType DTCKind;
    /* DemDTCFunctionalUnit Range:0~255 */
    const uint8 DemDTCFunctionalUnit;
    /* DemDTCSeverity type:
     Enum{
     DEM_SEVERITY_CHECK_AT_NEXT_HALT
     DEM_SEVERITY_CHECK_IMMEDIATELY
     DEM_SEVERITY_MAINTENANCE_ONLY
     DEM_SEVERITY_NO_SEVERITY
     } */
    const uint8 DemDTCSeverity; /*TODO: shall include the WWHOBDDTCCLASS info?*/
    /* DemDTCGroupRef */
    const uint8 GroupRef; /* DTC GroupIndex added by yt */

    const uint8 DemWWHOBDDTCClass;
    Dem_EventIdType EventRefNum;
    Dem_EventIdType EventRefStart;
} Dem_DTCType;

typedef struct
{
    const boolean DemConsiderPtoStatus;
    const uint16 DemDtcValue;
    const uint32 DemJ1939DTCValue;
    const uint8 DemEventOBDReadinessGroup;
} DemObdDTCType;

typedef struct
{
#if (DEM_J1939_NODEID_MAX_NUM > 0)
    const uint8 NodeIDRef[DEM_J1939_NODEID_MAX_NUM];
#endif
    const uint8 NodeNum;
} J1939NodeType;

/* DemConfigSet/DemDTCAttributes 0-65535 */
typedef struct
{
    /* DemAgingAllowed Range: true or false */
    const boolean DemAgingAllowed;

    /* DemAgingCycleRef Reference: DemOperationCycle MULTI:0-1*/
    const uint8 DemAgingCycleRef;

    /* DemAgingCycleCounterThreshold Range:1~256 */
    const uint8 DemAgingCycleCounterThreshold;

    /* DemAgingCycleCounterThresholdForTFSLC Range:1~256 */
    const uint8 DemAgingCycleCounterThresholdForTFSLC;

    /* DemDTCPriority Range:1~256 */
    const uint8 DemDTCPriority;

    /* DemEventMemoryEntryFdcThresholdStorageValue Range:1~126 */
    const uint8 DemEventMemoryEntryFdcThresholdStorageValue;

#if (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    /* DemFreezeFrameRecNumClassRef Reference: DemFreezeFrameRecNumClass MULTI:0-1
     */
    const uint8 DemFreezeFrameRecNumClassRef;
#else
    /* DemMaxNumberFreezeFrameRecords Range:0~255 */
    const uint8 DemMaxNumberFreezeFrameRecords;
#endif

    /* DemImmediateNvStorage Range: true or false */
    const boolean DemImmediateNvStorage;

    /* DemDTCSignificance type:
    Enum{
    DEM_EVENT_SIGNIFICANCE_FAULT
    DEM_EVENT_SIGNIFICANCE_OCCURRENCE
    } */
    const uint8 DemDTCSignificance;

    /* DemExtendedDataClassRef Reference: DemExtendedDataClass MULTI:0-1*/
    const uint16 DemExtendedDataClassRef;

    /* DemFreezeFrameClassRef Reference: DemFreezeFrameClass MULTI:0-1*/
    const uint16 DemFreezeFrameClassRef;

    /* DemMemoryDestinationRef ChoiceReference: TOOL shall TODO: 2 The same event shall not be
     * configured two destinations if one is not DemMirrorMemory. */
    const uint8 DemMemoryDestinationRef[DEM_MEM_DEST_MAX_NUM_OF_DTC];

    /*DemJ1939DTC_J1939NodeRef*/
    const uint8 DemJ1939DTC_J1939NodeRef;

    /*DemJ1939ExpandedFreezeFrameClassRef*/
    const uint8 DemJ1939ExpandedFreezeFrameClassRef;

    /*DemJ1939FreezeFrameClassRef*/
    const uint8 DemJ1939FreezeFrameClassRef;

    /*DemWWHOBDFreezeFrameClassRef*/
    const uint8 DemWWHOBDFreezeFrameClassRef;
} Dem_DTCAttributesType;

typedef struct
{
    /* DemIndicatorFailureCycleCounterThreshold Range:0~255 */
    const uint8 DemIndicatorFailureCycleCounterThreshold;

    /* DemIndicatorHealingCycleCounterThreshold Range:0~255 */
    const uint8 DemIndicatorHealingCycleCounterThreshold;

    /* DemIndicatorRef Reference: DemIndicator MULTI:1-1*/
    const uint8 DemIndicatorRef;

    /* DemIndicatorBehaviour type:
    Enum{
    DEM_INDICATOR_BLINKING
    DEM_INDICATOR_BLINK_CONT
    DEM_INDICATOR_CONTINUOUS
    DEM_INDICATOR_FAST_FLASH
    DEM_INDICATOR_SLOW_FLASH
    } */
    const uint8 DemIndicatorBehaviour;
} Dem_IndicatorAttributeType;

/*******************************************************************************
 *                          Indicator Configuration                            **
 *******************************************************************************/
#if (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0)
/* DemConfigSet/DemEventParameter/DemIndicatorAttribute */
extern CONST(Dem_IndicatorAttributeType, AUTOMATIC) DemIndicatorAttribute[DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM];
#endif

/*******************************************************************************
 *                          Debounce Configuration                             **
 *******************************************************************************/
/* DemConfigSet/DemDebounceCounterBasedClass 0-65535 */
typedef struct
{
    /* DemDebounceCounterDecrementStepSize Range:1~32768 */
    const uint16 DemDebounceCounterDecrementStepSize;

    /* DemDebounceCounterIncrementStepSize Range:1~32767 */
    const uint16 DemDebounceCounterIncrementStepSize;

    /* DemDebounceCounterJumpDownValue Range:-32768~32767 */
    const sint16 DemDebounceCounterJumpDownValue;

    /* DemDebounceCounterJumpUpValue Range:-32768~32767 */
    const sint16 DemDebounceCounterJumpUpValue;

    /* DemDebounceCounterFailedThreshold Range:1~32767 */
    const sint16 DemDebounceCounterFailedThreshold;

    /* DemDebounceCounterPassedThreshold Range:-32768~-1 */
    const sint16 DemDebounceCounterPassedThreshold;

    /* DemDebounceCounterJumpDown Range: true or false */
    const boolean DemDebounceCounterJumpDown;

    /* DemDebounceCounterJumpUp Range: true or false */
    const boolean DemDebounceCounterJumpUp;

    /* DemDebounceCounterStorage Range: true or false */
    const boolean DemDebounceCounterStorage;

    /* DemDebounceBehavior type:
    Enum{
    DEM_DEBOUNCE_FREEZE
    DEM_DEBOUNCE_RESET
    } */
    const uint8 DemDebounceBehavior;
} Dem_DebounceCounterBasedClassType;

/* DemConfigSet/DemDebounceTimeBaseClass 0-65535 */
typedef struct
{
    /* DemDebounceTimeFailedThreshold Range:0.001~3600 */
    const uint32 DemDebounceTimeFailedThreshold;

    /* DemDebounceTimePassedThreshold Range:0.001~3600 */
    const uint32 DemDebounceTimePassedThreshold;

    /* DemDebounceBehavior type:
     Enum{
     DEM_DEBOUNCE_FREEZE
     DEM_DEBOUNCE_RESET
     } */
    const uint8 DemDebounceBehavior;
} Dem_DebounceTimeBaseClassType;

/* DemConfigSet/DemEventParameter/DemDebounceAlgorithmClass/DemDebounceMonitorInternal */
typedef Std_ReturnType (*Dem_GetFDCFncType)(sint8* FDC);

#if (DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
extern CONST(Dem_GetFDCFncType, AUTOMATIC) DemGetFDCFncs[DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM];
#endif
/*******************************************************************************
 *                          Event Configuration                                **
 *******************************************************************************/

/* DemConfigSet/DemEventParameter/DemCallbackInitMForE */
typedef Std_ReturnType (*Dem_CallbackInitMForEType)(Dem_InitMonitorReasonType InitMonitorReason);

/* DemConfigSet/DemEventParameter/DemCallbackEventDataChanged */
typedef Std_ReturnType (*Dem_CallbackEventDataChangedType)(Dem_EventIdType EventId);

/* DemConfigSet/DemEventParameter/DemCallbackEventStatusChanged */
typedef Std_ReturnType (*Dem_TriggerOnEventStatusType)(
    Dem_EventIdType EventId,
    Dem_UdsStatusByteType EventStatusByteOld,
    Dem_UdsStatusByteType EventStatusByteNew);

#if (DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
extern CONST(Dem_TriggerOnEventStatusType, AUTOMATIC)
    DemCallbackEventStatusChanged[DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM];
#endif

typedef Std_ReturnType (*ClearEventAllowedType)(boolean* Allowed);

/* DemConfigSet/DemEventParameter/DemCallbackClearEventAllowed */
typedef struct
{
    /* DemCallbackClearEventAllowedFnc type: FunctionName */
    const ClearEventAllowedType ClearEventAllowed;
    /* DemClearEventAllowedBehavior type:
    Enum{
    DEM_NO_STATUS_BYTE_CHANGE
    DEM_ONLY_THIS_CYCLE_AND_READINESS
    } */
    const uint8 DemClearEventAllowedBehavior;
} Dem_CallbackClearEventAllowedType;

#if (DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM > 0)
extern CONST(Dem_CallbackClearEventAllowedType, AUTOMATIC)
    DemCallbackClearEventAllowed[DEM_CALLBACK_CLEAR_EVENT_ALLOWED_TOTAL_NUM];
#endif

typedef Std_ReturnType (*DemComponentFailedCallbackFnc)(Dem_ComponentIdType ComponentId, boolean ComponentFailedStatus);
typedef struct
{
    const DemComponentFailedCallbackFnc DemComponentFailedCallbackFnc;
    const boolean DemComponentIgnoresPriority; /*whether the priority of events at this component
                                                  shall be ignored*/
    const uint16 DemParentComponentRef;        /* parent component added by yt*/
#if (DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM > 0)
    const uint16
        DemImmediateChildComponentRef[DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM]; /*Reference to all immediate children of
                                                                                the current component*/
#endif /* DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM > 0 */
} DemComponentTypes;

/* DemConfigSet/DemEventParameter 0-65535 */
typedef struct
{
    /* DemCallbackInitMForE 0-1 */
    const Dem_CallbackInitMForEType DemCallbackInitMForE;

    /* DemCallbackEventDataChanged 0-1 */
    const Dem_CallbackEventDataChangedType DemCallbackEventDataChanged;

    /* DemCallbackClearEventAllowed 0-1 */
    P2CONST(Dem_CallbackClearEventAllowedType, AUTOMATIC, DEM_CONST_PBCFG)
    DemCallbackClearEventAllowed;

    /* DemCallbackEventStatusChanged 0-* */
    const uint16 StatusChangedCbkStartIndex;
    const uint8 StatusChangedCbkNum;

    /* DemDTCRef Reference: DemDTC MULTI:0-1*/
    const uint16 DemDTCRef;
    /* DemDebounceAlgorithmClass 1-1 -CHOICES */
    const uint16 AlgorithmRef; /* Index Of AlgirithmClass */
    const uint16 AlgorithmIndex;
    const uint8 AlgorithmType;

    /* DemIndicatorAttribute 0-255 */
    const uint16 AttrStartIndex;
    const uint8 AttrNum;

    /* DemEventFailureCycleCounterThreshold Range:0~255 */
    const uint8 DemEventFailureCycleCounterThreshold;
    /* DemEventAvailable Range: true or false */
    const boolean DemEventAvailable;
    /* DemFFPrestorageSupported Range: true or false */
    const boolean DemFFPrestorageSupported; /*SWS_Dem_00002*/

    /* DemEventKind type:
    Enum{
    DEM_EVENT_KIND_BSW
    DEM_EVENT_KIND_SWC
    } */
    const uint8 DemEventKind;

    /* DemReportBehavior type:
    Enum{
    REPORT_AFTER_INIT
    REPORT_BEFORE_INIT
    } */
    const uint8 DemReportBehavior;

    /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
    const uint8 DemOperationCycleRef;

    /* DemEnableConditionGroupRef Reference: DemEnableConditionGroup MULTI:0-1*/
    const uint8 DemEnableConditionGroupRef;

    /* DemStorageConditionGroupRef Reference: DemStorageConditionGroup MULTI:0-1*/
    const uint8 DemStorageConditionGroupRef;

    /*DemComponentClassRef Reference: DemComponent */
    const uint16 DemComponentClassRef;

    const uint8 DemComponentPriority;

    /*The "reverence event" must refer to it self. Note: One event is only allowed
     * to be reverenced to only one group of associate events.*/
    const uint16 DemOBDGroupingAssociativeEventsRef;
} Dem_EventParameterType;

/*******************************************************************************
 *                          Memory Configuration                               **
 *******************************************************************************/
/* FreezeFrameInfo Define */
typedef struct
{
    uint8 RecordNum; /*total number = DEM_MAX_NUMBER_FF_RECORDS*/
#if (DEM_FREEZE_FRAME_MAX_LEN > 0u)
    uint8 Data[DEM_FREEZE_FRAME_MAX_LEN];
#endif
} Dem_FreezeFrameInfoType;

#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
/* GlobalFreezeFrameInfo Define */
typedef struct
{
    uint8 RecordNum; /* GlobalDemFreezeFrameRecordNumber */
    uint8 Data[DEM_GLOBAL_FREEZE_FRAME_LENGTH];
} Dem_GlobalFreezeFrameInfoType;
#endif

/* EventMemoryEntry Define */
typedef struct
{
    uint8 AbsTime;
    Dem_EventIdType EventId;
    uint8 UdsStatus;
    uint8 AgingCounter;
    uint8 HealingCounter;
    uint8 FailureCounter;
#if (DEM_OCC6EANLE == STD_ON)
    uint8 OCC6;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    uint8 OCC4;
#endif /* DEM_OCC4EANLE == STD_ON */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    uint8 StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
    uint8 OccurrenceCounter;
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    uint8 AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
    /* Operation Cycle Counter */
/* Operation Cycle Counter */
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    uint8 CyclesSinceFirstFailed;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    uint8 CyclesSinceLastFailed;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
    uint8 EventStatusFlag;
#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
    uint8 FFNum;
    uint8 LatestFF;
    Dem_FreezeFrameInfoType FFList[DEM_MAX_NUMBER_FF_RECORDS];
#endif
#if (DEM_GLOBAL_FREEZE_FRAME_SUPPORT == STD_ON)
    Dem_GlobalFreezeFrameInfoType GlobalFFList;
#endif
#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
    uint8 ExtStatus[DEM_EXTENDED_DATA_MAX_REF_NUM_BYTE];
#if (DEM_EXTENDED_DATA_MAX_LEN > 0u)
    uint8 ExtData[DEM_EXTENDED_DATA_MAX_LEN];
#endif
#endif
} Dem_EventMemEntryType;

/* EventMemoryDest Define */
typedef struct
{
    Dem_EventMemEntryType* EntryList;
    const uint8 EntryNum;
    const Dem_DTCOriginType ExtId;
} Dem_MemDestConfigType;

#if (DEM_MEM_DEST_TOTAL_NUM > 0)
extern CONST(Dem_MemDestConfigType, AUTOMATIC) DemMemDestCfg[DEM_MEM_DEST_TOTAL_NUM];
#endif

#if (DEM_NVRAM_BLOCKID_NUM > 0)
extern CONST(uint16, AUTOMATIC) DemNvRamBlockId[DEM_NVRAM_BLOCKID_NUM];
#endif

/*************************************************************************************
                                OBD Configuration                                   **
**************************************************************************************/
typedef struct
{
    /*Configuration value to define the appropriate value to PID1C "OBD
      requirements to which vehicle or engine is certified*/
    const uint8 DemOBDCompliancy;

    /*Switch to provide either Gasoline or Diesel parameters*/
    const uint8 DemOBDEngineType;

    /*Input variable for the accelerator padal information, which is assigned
      to a specific data element used as interface for the Dem-internal PID calculations. */
    const uint8 DemOBDInputAcceleratorPedalInformation;

    /*Input variable for the ambient pressure*/
    const uint8 DemOBDInputAmbientPressure;

    /*Input variable for the ambient temperature*/
    const uint8 DemOBDInputAmbientTemperature;

    /*Input variable for the distance information*/
    const uint8 DemOBDInputDistanceInformation;

    /*Input variable for the engine speed*/
    const uint8 DemOBDInputEngineSpeed;

    /*Input variable for the engine temperature*/
    const uint8 DemOBDInputEngineTemperature;

    /*Input variable for the programming event*/
    const uint8 DemOBDInputProgrammingEvent;

    /*Input variable for the vehicle speed*/
    const uint8 DemOBDInputVehicleSpeed;

    /*Input variable for the Time Since Engine Start information*/
    const uint8 DemOBDTimeSinceEngineStart;

} DemGeneralOBDType;

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
extern CONST(DemGeneralOBDType, AUTOMATIC) DemGeneralOBD;
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

typedef struct
{
    const sint16 DemDtrCompuDenominator0;
    const sint16 DemDtrCompuNumerator0;
    const sint16 DemDtrCompuNumerator1;
    const uint16 DemDtrEventRef;
    const uint8 DemDtrMid;
    const uint8 DemDtrTid;
    const uint8 DemDtrUasid;
    const uint8 DemDtrUpdateKind;
} DTRType;

typedef Std_ReturnType (*DemDataElementReadFncType)(uint8* Buffer);

typedef struct
{
    boolean DemInterFlag;
    uint8 DemDataElementDataSize;
    DemDataElementReadFncType DemDataElementReadFnc;
} DemDataElementClassTypes;

typedef struct
{
    const uint8 DemSecondaryFunctionIdRefNum;
    const uint8 DemSecondaryFunctionIdRef[DEM_SECONDARY_FUNCTIONID_REF_MAX_NUM];
} DemSecondaryFunctionIdType;

typedef struct
{
    const uint8 DemDiagnosticEventRef;
    const uint8 DemFunctionIdRef;
    const uint8 DemIUMPRDenGroup;
    const uint8 DemIUMPRGroup; /*Dem_IUMPRGroupType*/
    const uint8 DemRatioId;
    const uint8 DemRatioKind;
    P2CONST(DemSecondaryFunctionIdType, AUTOMATIC, DEM_CONST_PBCFG) DemSecondaryFunctionIdRef;
} DemRatioType;

/*******************************************************************************
 *                          PostBuild Configuration                            **
 *******************************************************************************/

typedef struct
{
    P2CONST(DemComponentTypes, AUTOMATIC, DEM_CONST_PBCFG) DemComponent;
    P2CONST(Dem_DTCType, AUTOMATIC, DEM_CONST_PBCFG) DemDTC;
    P2CONST(uint32, AUTOMATIC, DEM_CONST_PBCFG) DemGroupOfDTC;
    uint16 DemDTCNum;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST_PBCFG) DemDTCAttributes;
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST_PBCFG)
    DemDebounceCounterBasedClass;
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST_PBCFG) DemDebounceTimeBaseClass;
    P2CONST(DTRType, AUTOMATIC, DEM_CONST_PBCFG) DTR;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST_PBCFG) DemEventParameter;
    uint16 DemEventNum;
    P2CONST(J1939NodeType, AUTOMATIC, DEM_CONST_PBCFG) J1939Node;
    P2CONST(DemObdDTCType, AUTOMATIC, DEM_CONST_PBCFG) DemObdDTC;
    P2CONST(Dem_PidClassType, AUTOMATIC, DEM_CONST_PBCFG) DemPidClass;
    P2CONST(Dem_OperationCycleType, AUTOMATIC, DEM_CONST_PBCFG) DemOperationCycle;
    uint16 DemOperationCycleNum;
    P2CONST(DemRatioType, AUTOMATIC, DEM_CONST_PBCFG) DemRatio;
    P2CONST(DemDataElementClassTypes, AUTOMATIC, DEM_CONST_PBCFG) DemDataElementClass;
    P2CONST(Dem_EventIdType, AUTOMATIC, DEM_CONST_PBCFG) DemDTCMapping;
} Dem_ConfigType;

#endif /* DEM_CFGNEW_DEM_CFGTYPES_H_ */

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
