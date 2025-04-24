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
**  FILENAME    : Dem.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Implementation for DEM                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Dem<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1504 MISRA Rule 8.7 .<br>
    Reason:Local variables are designed to extern for more clear design structure.

    \li PRQA S 1505 MISRA Rule 8.7 .<br>
    Reason:Local functions are designed to extern for more clear design structure.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:Local functions are designed to extern for more clear design structure.

    \li PRQA S 1533 MISRA Rule 8.9 .<br>
    Reason:variable may be used for different configuration.

    \li PRQA S 2985 MISRA Rule 2.2 .<br>
    Reason:StatusMaskType is not necessarily u8 for every project.

    \li PRQA S 2981 MISRA Rule 2.2 .<br>
    Reason:Init may be necessary for different macro definition.

    \li PRQA S 2982 MISRA Rule 2.2 .<br>
    Reason:Init may be necessary for different macro definition.

    \li PRQA S 2983 MISRA Rule 2.2 .<br>
    Reason:Init may be necessary for different macro definition.

    \li PRQA S 2995 MISRA Rule 2.2 .<br>
    Reason:Condition may be FALSE for different configuration.

    \li PRQA S 3415 MISRA Rule 13.5 .<br>
    Reason:function in && comparison does not need to be called when the first condition fails.

    \li PRQA S 2463 MISRA Rule 14.2 .<br>
    Reason:For loop variable does not necessarily need to be initialized.

    \li PRQA S 2469 MISRA Rule 14.2 .<br>
    Reason:For loop condition involved variable is changed for design purpose.

    \li PRQA S 2812 MISRA Dir 4.1 .<br>
    Reason:Variables varies for different configuration.

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:for loop condition is a macro definition, which varies in different projs.

    \li PRQA S 0777 MISRA Rule 5.4 .<br>
    Reason:Function name, design needs, namelength set to 63 in C99.

    \li PRQA S 0779 MISRA Rule 5.4 .<br>
    Reason:Structure member name, design needs, namelength set to 63 in C99.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:Macro definition, design needs, namelength set to 63 in C99.

    \li PRQA S 2461 MISRA Rule 14.2 .<br>
    Reason:For loop variable does not necessarily need to be initialized.

    \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:Variables are defined in file scope for further development.

    \li PRQA S 1514 MISRA Rule 8.9 .<br>
    Reason:Variables are defined in file scope for further development.

    \li PRQA S 2003 MISRA Rule 2.2 .<br>
    Reason:Switch case is designed to skip over for design purpose.

    \li PRQA S 2755 MISRA CMN 0.4 .<br>
    Reason:Functions name is by design purpose.

    \li PRQA S 4393 MISRA Rule 10.8 .<br>
    Reason:Data conversion to a different range is necessary for design purpose.

    \li PRQA S 0431 MISRA Rule 1.1 .<br>
    Reason:The function parameter type is by design purpose.

    \li PRQA S 3451 MISRA Rule 8.5 .<br>
    Reason:Different configurations produce variations.

    \li PRQA S 3449 MISRA Rule 8.5 .<br>
    Reason:Different configurations produce variations.
 */
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"
#include "SchM_Dem.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef DEM_AR_RELEASE_MAJOR_VERSION
#error " Dem version miss"
#endif
#ifndef DEM_AR_RELEASE_MINOR_VERSION
#error " Dem version miss"
#endif
#ifndef DEM_AR_RELEASE_PATCH_VERSION
#error " Dem version miss"
#endif

#if ((2u != DEM_CFG_H_SW_MAJOR_VERSION) || (1u != DEM_CFG_H_SW_MINOR_VERSION) || (0u != DEM_CFG_H_SW_PATCH_VERSION))
#error " Dem version mismatching"
#endif

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#define DEM_START_SEC_VAR_INIT_PTR
#include "Dem_MemMap.h"
P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) DemPbCfgPtr = NULL_PTR;
#define DEM_STOP_SEC_VAR_INIT_PTR
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_INIT_BOOLEAN
#include "Dem_MemMap.h"
VAR(boolean, AUTOMATIC) Dem_Pending = FALSE;
#define DEM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_ClearDTCInfoType, AUTOMATIC) DemClearDTCInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
VAR(Dem_ClearNonvolatileStatusType, AUTOMATIC)
Dem_ClearNonvolatileStatus = DEM_ClEAR_NONVOLATILE_INIT;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
VAR(Dem_ClearLockStatus, AUTOMATIC) Dem_ClearDTCLock = DEM_CLEAR_NOT_LOCK;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_PreStoreFFInfoType, AUTOMATIC) DemPreStoreFFInfo[DEM_MAX_NUMBER_PRESTORED_FF];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif /* DEM_MAX_NUMBER_PRESTORED_FF > 0 */

#define DEM_START_SEC_VAR_INIT_8
#include "Dem_MemMap.h"
VAR(Dem_StateType, AUTOMATIC) Dem_InitState = DEM_STATE_UNINIT;
#define DEM_STOP_SEC_VAR_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_MemDestInfoType, AUTOMATIC) DemMemDestInfo[DEM_MEM_DEST_TOTAL_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define DEM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
DEM_LOCAL VAR(uint8, AUTOMATIC) Dem_AgingCycle = DEM_OPERATION_CYCLE_INVALID;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"

#if (DEM_NVRAM_BLOCKID_NUM > 0)
#if (DEM_NVRAM_DIVADED == STD_OFF)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_EventMemEntryType, AUTOMATIC) DemEventMemoryEntryStorage[ENTRY_STORAGE_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif /* DEM_NVRAM_DIVADED == STD_OFF */

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_EventRelateInformationStorageType, AUTOMATIC) DemEventRelateInformationStorage;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(DemOBDDataStorageType, AUTOMATIC) DemOBDDataStorage;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */
#endif /* DEM_NVRAM_BLOCKID_NUM > 0 */

#if ((DEM_NVRAM_BLOCKID_NUM > 0u) && (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH))
#define DEM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
DEM_LOCAL VAR(uint8, AUTOMATIC) Dem_ClearCnt = 0u;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
#endif /* DEM_NVRAM_BLOCKID_NUM > 0u && DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH */
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Initializes or reinitializes this module.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Std_ReturnType.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_SubInit(Std_ReturnType NvmResult);

#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
/*************************************************************************/
/*
 * Brief               ClearOneDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ClearOneDTC(uint8 memDest, uint16 DTCIndex);
#endif /* DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS */

/*************************************************************************/
/*
 * Brief               Dem_OperationCycleStart
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId && IsRestart
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_OperationCycleStart(uint8 OpId, boolean IsRestart);

/*************************************************************************/
/*
 * Brief               Dem_OperationCycleEnd
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_OperationCycleEnd(uint16 OpId);

#if (DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
/*************************************************************************/
/*
 * Brief               Dem_GetEventPriority
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) Dem_GetEventPriority(Dem_EventIdType IntId);
#endif /* DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE */

#if (DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
/*************************************************************************/
/*
 * Brief               Dem_DeleteDisplacementEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      InternalId && MemIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_DeleteDisplacementEntry(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pResEntry,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    VAR(uint8, DEM_APPL_DATA) memDest);
#endif /* DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE */

/*************************************************************************/
/*
 * Brief               Dem_EventAgingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_EventAgingProcess(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent, uint16 OpId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_EventHealingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_EventHealingProcess(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent, uint8 OpId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_EventQueueProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_EventQueueProcess(void);

/*************************************************************************/
/*
 * Brief               Dem_IntReadNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_MemEntryDelete(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry, uint8 memDest);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (DEM_NVRAM_BLOCKID_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_IntReadNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE) Dem_IntReadNvRAM(void);
/*************************************************************************/
/*
 * Brief               check if non-volatile event memory data cleared.
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ClearNonvolatile(void);
#endif /* DEM_NVRAM_BLOCKID_NUM > 0 */
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               Initializes the internal states necessary to process events reported by
 * BSW-modules. ServiceId           0x01 Sync/Async          Synchronous Reentrancy          Non
 * Reentrant Param-Name[in]      None Param-Name[out]     None Param-Name[in/out]  None Return None
 */
/******************************************************************************/
FUNC(void, DEM_CODE) Dem_PreInit(void)
{
    Dem_InterPreInit();
    /* req SWS_Dem_00180*/
    /* req SWS_Dem_00438*/
    Dem_DebounceInit();
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
    Dem_OBDInit();
#endif
    Dem_InitState = DEM_STATE_PRE_INIT;
}

/*************************************************************************/
/*
 * Brief               Initializes or reinitializes this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr:Pointer to the configuration set in VARIANT-POSTBUILD.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_Init(P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) ConfigPtr)
{
    if (NULL_PTR == ConfigPtr)
    {
        DEM_DET_REPORT(DEM_SID_INIT, DEM_E_PARAM_POINTER);
    }
    else
    {
        uint16 iloop;
        uint16 CycleNum;
        const Dem_OperationCycleType* pDemOperationCycle;
        Std_ReturnType NvmResult
#if (DEM_NVRAM_BLOCKID_NUM == 0)
            = E_NOT_OK
#endif
            ;
        Dem_EventMemEntryType* pEntry;
        uint16 index;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        static boolean FirstStartUp = FALSE;
#endif
        DemPbCfgPtr = ConfigPtr;
        /* Dem Dcm Init */
        Dem_DTCStatusChangedInd = TRUE;
        DemDTCSettingInfo.AllGroupIsEnabled = TRUE;
        Dem_MemSet((uint8*)&DemDTCFilterInfo, 0x00u, sizeof(Dem_DTCFilterInfoType));
        Dem_MemSet((uint8*)&DemFreezeFrameRecordFilterInfo, 0x00u, sizeof(Dem_FreezeFrameRecordFilterInfoType));
        DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed = DEM_EVENT_PARAMETER_INVALID;
        DemDTCByOccurrenceTimeInfo.FirstFailed = DEM_EVENT_PARAMETER_INVALID;
        DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed = DEM_EVENT_PARAMETER_INVALID;
        DemDTCByOccurrenceTimeInfo.MostRecentFailed = DEM_EVENT_PARAMETER_INVALID;
        DemClearDTCInfo.ClearAllGroup = FALSE;
        DemClearDTCInfo.DTCGroupIndex = DEM_GROUP_OF_DTC_INVALID;
        DemClearDTCInfo.DTCIndex = DEM_DTC_REF_INVALID;
        DemClearDTCInfo.memDest = DEM_MEM_DEST_INVALID;
#if (DEM_NVRAM_BLOCKID_NUM > 0)
        NvmResult = Dem_IntReadNvRAM();
#endif
        if (NvmResult == E_NOT_OK)
        {
            /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
            for (iloop = 0u; iloop < DEM_MEM_DEST_TOTAL_NUM; iloop++)
            /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
            {
                for (index = 0u; index < DemMemDestCfg[iloop].EntryNum; index++)
                {
                    pEntry = &DemMemDestCfg[iloop].EntryList[index];
                    Dem_MemEntryDelete(pEntry, (uint8)iloop);
                }
            }
        }
        Dem_SubInit(NvmResult);
        /* req SWS_Dem_00854 */
        CycleNum = DemPbCfgPtr->DemOperationCycleNum;
        pDemOperationCycle = DemPbCfgPtr->DemOperationCycle;
        for (iloop = 0u; iloop < CycleNum; iloop++)
        {
            if (pDemOperationCycle[iloop].DemOperationCycleAutostart == TRUE)
            {
#if (DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON)
                if ((0x00u != DEM_BITS_ISSET(DemOperationCycleStatus, iloop)))
                {
                    Dem_OperationCycleStart((uint8)iloop, TRUE);
                }
                else
#endif /* DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON */
                {
                    /* req SWS_Dem_00853 */
                    DEM_BITS_SET(DemOperationCycleStatus, iloop);
                    Dem_OperationCycleStart((uint8)iloop, FALSE);
                }
            }
        }
        if (NvmResult == E_NOT_OK)
        {
            Dem_MemSet((uint8*)DemMemDestInfo, 0x00u, (sizeof(Dem_MemDestInfoType) * DEM_MEM_DEST_TOTAL_NUM));
        }
        Dem_InterInit();
        Dem_ResetDemClearDTCInfo();
#if (DEM_J1939_SUPPORT == STD_ON)
        Dem_J1939Init();
#endif
        Dem_InitState = DEM_STATE_INIT;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        if (FirstStartUp == FALSE) /*SWS_Fim_00103*/
        {
            (void)FiM_DemInit();
            FirstStartUp = TRUE;
        }
#endif
    }
}

#if (DEM_VERSION_INFO_API == STD_ON)
/*************************************************************************/
/*
 * Brief               Provide Version information to other BSWs
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versionInfo
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DEM_APPL_DATA) versionInfo)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    if (NULL_PTR == (versionInfo))
    {
        DEM_DET_REPORT(DEM_SID_GETVERSIONINFO, DEM_E_PARAM_POINTER);
    }
    else
    {
        versionInfo->vendorID = DEM_VENDOR_ID;
        versionInfo->moduleID = DEM_MODULE_ID;
        versionInfo->sw_major_version = DEM_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = DEM_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = DEM_SW_PATCH_VERSION;
    }
}
#endif

/*************************************************************************/
/*
 * Brief               Shuts down this module.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_Shutdown(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SHUTDOWN, DEM_E_UNINIT);
    }
    else
    {
        uint16 iloop;
        uint16 CycleNum = DemPbCfgPtr->DemOperationCycleNum;
        /* OperationCycleStatus */
#if (DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON)
        const Dem_OperationCycleType* pDemOperationCycle = DemPbCfgPtr->DemOperationCycle;
        for (iloop = 0; iloop < CycleNum; iloop++)
        {
            /* req SWS_Dem_00697*/
            if ((pDemOperationCycle[iloop].DemOperationCycleAutomaticEnd == TRUE)
                && (0x00u != DEM_BITS_ISSET(DemOperationCycleStatus, iloop)))
            {
                Dem_OperationCycleEnd(iloop);
                DEM_BITS_CLR(DemOperationCycleStatus, iloop);
            }
        }
#else
        for (iloop = 0; iloop < CycleNum; iloop++)
        {
            if (DEM_BITS_ISSET(DemOperationCycleStatus, iloop))
            {
                /* req SWS_Dem_01078*/
                Dem_OperationCycleEnd(iloop);
                DEM_BITS_CLR(DemOperationCycleStatus, iloop);
            }
        }
#endif
#if (DEM_NVRAM_BLOCKID_NUM > 0)
        Dem_IntWriteNvRAM();
#endif
        Dem_InitState = DEM_STATE_SHUTDOWN;
    }
}

/*************************************************************************/
/*
 * Brief               Queues the reported events from the BSW modules (API is only used by
                        BSW modules). The interface has an asynchronous behavior, because
                        the processing of the event is done within the Dem main function.
                        OBD Events Suppression shall be ignored for this computation.
 * ServiceId           0x0f
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different EventIds. Non reentrant for the same EventId.
 * Param-Name[in]      EventId: Identification of an event by assigned Event ID.
 *                       EventStatus: Monitor test result
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_ReportErrorStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus)
{
    if (Dem_InitState == DEM_STATE_UNINIT)
    {
        DEM_DET_REPORT(DEM_SID_REPORTERRORSTATUS, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfg.DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_REPORTERRORSTATUS, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        boolean doadd = FALSE;
        Dem_EventIdType IntId = Dem_GetEventInternalId(EventId);
        const Dem_EventParameterType* pEventCfg = &DemPbCfg.DemEventParameter[IntId];
        Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(IntId);
        if ((pEventCfg->DemEventKind != DEM_EVENT_KIND_BSW)
            || ((pEventCfg->AlgorithmType == DEM_DEBOUNCE_MONITOR_INTERNAL) && (EventStatus != DEM_EVENT_STATUS_FAILED)
                && (EventStatus != DEM_EVENT_STATUS_PASSED))
            || ((Dem_InitState == DEM_STATE_PRE_INIT) && (pEventCfg->DemReportBehavior != REPORT_BEFORE_INIT)))
        {
            DEM_DET_REPORT(DEM_SID_REPORTERRORSTATUS, DEM_E_WRONG_CONFIGURATION);
        }
        /* req SWS_Dem_00676*/
        else if (
            (Dem_InitState != DEM_STATE_INIT)
            && (DemPbCfg.DemDebounceCounterBasedClass[pEventCfg->AlgorithmRef].DemDebounceCounterStorage == TRUE))
        {
            DEM_DET_REPORT(DEM_SID_REPORTERRORSTATUS, DEM_E_WRONG_CONDITION);
        }
#if (DEM_ENABLE_SOFT_FILTER_OF_PASS == STD_ON)
        /*soft filter when pass or prepass,if TF/TNCLSC or TNCTOC is flaged then return e_ok whith
         * no deal*/
        else if (
            ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
             && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC))
             && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC)))
            && (EventStatus == DEM_EVENT_STATUS_PASSED))
        {
            /*idle*/
        }
#endif
        /* req SWS_Dem_00851 */
        else if (Dem_InitState != DEM_STATE_PRE_INIT)
        {
#if (DEM_ENABLE_CONDITION_NUM > 0u)
            if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
            {
                /*SWS_Dem_00447 SWS_Dem_00449 SWS_Dem_01091*/
                Dem_DebounceFreeze(IntId);
            }
            else
#endif /* DEM_ENABLE_CONDITION_NUM > 0u */
            {
                /* req SWS_Dem_00700 SWS_Dem_01108*/
                if (((DEM_BITS_ISNSET(DemOperationCycleStatus, pEventCfg->DemOperationCycleRef))
                     && (DemPbCfg.DemOperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType
                         != DEM_OPCYC_OBD_DCY))
                    || (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                    || (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE)))
                {
                    /*idle*/
                }
                else
                {
                    doadd = TRUE;
                }
            }
        }
        else
        {
            doadd = TRUE;
        }
        if (TRUE == doadd)
        {
            Dem_EventStatusType Status = EventStatus;
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
            Status = Dem_DebounceProcessCounter(EventId, Status);
#endif
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_OFF)
            if (!((DEM_DEBOUNCE_COUNTER_BASED == pEventCfg->AlgorithmType)
                  && ((DEM_EVENT_STATUS_PREPASSED == Status) || (DEM_EVENT_STATUS_PREFAILED == Status))))
#endif
            {
                (void)Dem_EventQueueAdd(EventId, Status);
            }
        }
    }
}

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY) /*SWS_Dem_00687*/
/*************************************************************************/
/*
 * Brief               Set the available status of a specific Event.
 * ServiceId           0x37
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                       Event shall be available (TRUE) or not (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful.
 *                       E_NOT_OK:change of available status not accepted
 */
/*************************************************************************/
/* req SWS_Dem_01106 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetEventAvailable(Dem_EventIdType EventId, boolean AvailableStatus)
{
    Dem_EventRelateInformationType* pEvent;
    uint16 dtcIndex;
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    Dem_UdsStatusByteType oldStatus;
#endif

    if (Dem_InitState == DEM_STATE_UNINIT)
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTAVAILABLE, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTAVAILABLE, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        oldStatus = pEvent->UdsStatus;
#endif
        /* req SWS_Dem_01109 */
        if ((NULL_PTR != Dem_CheckEventMemEntryExistsAlready(IntId))
            || (DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF) == 0x01u))
        {
            /*idle*/
        }
        else
        {
            const Dem_EventParameterType* pDemEventParameter = DemPbCfgPtr->DemEventParameter;
            dtcIndex = pDemEventParameter[IntId].DemDTCRef;
            if (AvailableStatus == TRUE)
            {
                DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
                /* req SWS_Dem_01111 bit 4 6 the initialized value 0x50 */
                DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC);
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
                (void)FiM_DemTriggerOnEventStatus(EventId, oldStatus, pEvent->UdsStatus);
#endif
                DemDTCGeneralStatus[dtcIndex].SuppressionStatus = FALSE;
            }
            else
            {
                const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[dtcIndex];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
                const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
                DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
                /* req SWS_Dem_01110 */
                pEvent->UdsStatus = 0x00;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
                (void)FiM_DemTriggerOnEventStatus(EventId, oldStatus, pEvent->UdsStatus);
#endif
                /* req SWS_Dem_00915 */
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                {
                    if (DEM_FLAGS_ISSET(DemEventRelateInformation[*pDTCMapping].Status, DEM_EVENT_STATUS_AVAILABLE)
                        != 0x00u)
                    {
                        dtcIndex = DEM_DTC_REF_INVALID;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                        break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                    }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                    pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                }
                if (dtcIndex != DEM_DTC_REF_INVALID)
                {
                    DemDTCGeneralStatus[dtcIndex].SuppressionStatus = TRUE;
                }
            }
            ret = E_OK;
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               Processes the events reported by SW-Cs via RTE.
 * ServiceId           0x04
 * Sync/Async          Synchronous/Asynchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       EventStatus: Monitor test result
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of event status was successful
 *                       E_NOT_OK: set of event status failed or could not be accepted
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus)
{
    const Dem_EventParameterType* pEventCfg;
    Dem_EventRelateInformationType* pEvent;
    uint8 OperationCycleId;
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTSTATUS, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTSTATUS, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
        pEvent = Dem_GetEventInfo(IntId);
        if (pEventCfg->DemEventKind != DEM_EVENT_KIND_SWC)
        {
            DEM_DET_REPORT(DEM_SID_SETEVENTSTATUS, DEM_E_WRONG_CONFIGURATION);
        }
        else if (
            (pEventCfg->AlgorithmType == DEM_DEBOUNCE_MONITOR_INTERNAL) && (EventStatus != DEM_EVENT_STATUS_FAILED)
            && (EventStatus != DEM_EVENT_STATUS_PASSED))
        {
            DEM_DET_REPORT(DEM_SID_SETEVENTSTATUS, DEM_E_WRONG_CONFIGURATION);
        }
#if (DEM_ENABLE_SOFT_FILTER_OF_PASS == STD_ON)
        /*soft filter when pass or prepass,if TF/TNCLSC or TNCTOC is flaged then return e_ok whith
         * no deal*/
        else if (
            ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
             && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC))
             && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC)))
            && (EventStatus == DEM_EVENT_STATUS_PASSED))
        {
            /*idle*/
            ret = E_OK;
        }
#endif

        else if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
        {
            /* req SWS_Dem_00447 SWS_Dem_00449 In case of Dem-internal debouncing the related fault
             * detection counter will be frozen or reset*/
            Dem_DebounceFreeze(IntId);
        }
        else
        {
            ret = Dem_InterSetEventStatus(&IntId, EventStatus);
            if (ret == E_OK)
            {
                OperationCycleId = pEventCfg->DemOperationCycleRef;
                /* req SWS_Dem_00700 req SWS_Dem_01108*/
                if (((DEM_BITS_ISNSET(DemOperationCycleStatus, OperationCycleId))
                     && (DemPbCfgPtr->DemOperationCycle[OperationCycleId].DemOperationCycleType != DEM_OPCYC_OBD_DCY))
                    || (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                    || (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE)))
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    Dem_EventStatusType Status = EventStatus;
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
                    Status = Dem_DebounceProcessCounter(EventId, Status);
#endif
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_OFF)
                    if (!((DEM_DEBOUNCE_COUNTER_BASED == pEventCfg->AlgorithmType)
                          && ((DEM_EVENT_STATUS_PREPASSED == Status) || (DEM_EVENT_STATUS_PREFAILED == Status))))
#endif
                    {
                        ret = Dem_EventQueueAdd(EventId, Status);
                    }
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Control the internal debounce counter/timer by BSW modules and SW-Cs.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       DebounceResetStatus :  Freeze or reset the internal debounce counter
 *                       /timer of the specified event.
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              None...
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_ResetEventDebounceStatus(Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus)
{
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;

    if (Dem_InitState < DEM_STATE_PRE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_RESETEVENTDEBOUNCESTATUS, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_RESETEVENTDEBOUNCESTATUS, DEM_E_WRONG_CONFIGURATION);
    }
    /*req SWS_Dem_00676*/
    else
    {
        /*SWS_Dem_01108*/
        IntId = Dem_GetEventInternalId(EventId);
        if ((Dem_InitState != DEM_STATE_INIT)
            && (DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef]
                    .DemDebounceCounterStorage
                == TRUE))
        {
            DEM_DET_REPORT(DEM_SID_RESETEVENTDEBOUNCESTATUS, DEM_E_WRONG_CONDITION);
        }
        else
        {
            pEvent = Dem_GetEventInfo(IntId);
            if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
            {
                /*idle*/
            }
            else
            {
                Dem_InterResetEventDebounceStatus(IntId, DebounceResetStatus);
                ret = E_OK;
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Resets the event failed status.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId Identification of an event by assigned EventId.
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              E_OK: reset of event status was successful
 *                       E_NOT_OK: reset of event status failed or is not allowed,
 *                       because the event is already tested in this operation cycle
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_ResetEventStatus(Dem_EventIdType EventId)
{
    Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;

    if (Dem_InitState < DEM_STATE_PRE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_RESETEVENTSTATUS, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_RESETEVENTSTATUS, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        /* req SWS_Dem_00638 SWS_Dem_01108 */
        if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
            || (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE)))
        {
            /*idle*/
        }
        else
        {
            /* req SWS_Dem_00187 */
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TF);
            /* update the combination event dtc status*/
            Dem_UpdateCombinedDtcStatus(IntId);
            Dem_ResponseOnDtcStatusChange(IntId);
            Dem_DebounceReset(IntId);
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Captures the freeze frame data for a specific event.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the sameEventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              None...
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_PrestoreFreezeFrame(Dem_EventIdType EventId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
    const Dem_EventParameterType* pEventCfg;
    Dem_EventIdType IntId;

    if (Dem_InitState < DEM_STATE_PRE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_PRESTOREFREEZEFRAME, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_PRESTOREFREEZEFRAME, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
        if (pEventCfg->DemFFPrestorageSupported == FALSE)
        {
            DEM_DET_REPORT(DEM_SID_PRESTOREFREEZEFRAME, DEM_E_WRONG_CONFIGURATION);
        }
        else
        {
            ret = Dem_InterPrestoreFreezeFrame(IntId);
        }
    }
    return ret;
#else
    DEM_UNUSED(EventId);
    return ret;
#endif
}

/*************************************************************************/
/*
 * Brief               Clears a prestored freeze frame of a specific event.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPrestoredFreezeFrame(Dem_EventIdType EventId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0)
    Dem_EventIdType IntId;

    if (Dem_InitState < DEM_STATE_PRE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_CLEARPRESTOREDFREEZEFRAME, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_CLEARPRESTOREDFREEZEFRAME, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        if (DemPbCfgPtr->DemEventParameter[IntId].DemFFPrestorageSupported == FALSE)
        {
            DEM_DET_REPORT(DEM_SID_CLEARPRESTOREDFREEZEFRAME, DEM_E_WRONG_CONFIGURATION);
        }
        else
        {
            ret = Dem_InterClearPrestoredFreezeFrame(IntId);
        }
    }
    return ret;
#else
    DEM_UNUSED(EventId);
    return ret;
#endif
}

/*************************************************************************/
/*
 * Brief               Sets an operation cycle state.
 * ServiceId           0x08
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OperationCycleId: Identification of operation cycle, like power cycle,
 *                                         driving cycle.
 *                       CycleState: New operation cycle state: (re-)start or end
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of operation cycle was accepted and will be handled asynchronously
 *                       E_NOT_OK: set of operation cycle was rejected>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetOperationCycleState(uint8 OperationCycleId, Dem_OperationCycleStateType CycleState)
{
    boolean IsRestart = FALSE;
    Std_ReturnType ret;
    if (Dem_InitState == DEM_STATE_UNINIT)
    {
        DEM_DET_REPORT(DEM_SID_SETOPERATIONCYCLESTATE, DEM_E_UNINIT);
    }
    else if (OperationCycleId >= DemPbCfgPtr->DemOperationCycleNum)
    {
        DEM_DET_REPORT(DEM_SID_SETOPERATIONCYCLESTATE, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        if (CycleState == DEM_CYCLE_STATE_START)
        {
            /* req SWS_Dem_00853 */
            if (DEM_BITS_ISSET(DemOperationCycleStatus, OperationCycleId))
            {
                /* req SWS_Dem_00483 */
                IsRestart = TRUE;
            }
            else
            {
                DEM_BITS_SET(DemOperationCycleStatus, OperationCycleId);
            }
            Dem_OperationCycleStart(OperationCycleId, IsRestart);
        }
        else
        {
            /* req SWS_Dem_00484 */
            if (DEM_BITS_ISSET(DemOperationCycleStatus, OperationCycleId))
            {
                Dem_OperationCycleEnd(OperationCycleId);
                DEM_BITS_CLR(DemOperationCycleStatus, OperationCycleId);
            }
        }
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets information about the status of a specific operation cycle.
 * ServiceId           0x9e
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OperationCycleId : Identification of operation cycle, like power cycle,
 *                                          driving cycle.
 * Param-Name[out]     CycleState : Cycle status information
 * Param-Name[in/out]  None
 * Return              E_OK: read out of operation cycle was successful
 *                       E_NOT_OK: read out of operation cycle failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetOperationCycleState(
    uint8 OperationCycleId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_OperationCycleStateType, AUTOMATIC, DEM_APPL_DATA) CycleState)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState == DEM_STATE_UNINIT)
    {
        DEM_DET_REPORT(DEM_SID_GETOPERATIONCYCLESTATE, DEM_E_UNINIT);
    }
    else if (OperationCycleId >= DemPbCfgPtr->DemOperationCycleNum)
    {
        DEM_DET_REPORT(DEM_SID_GETOPERATIONCYCLESTATE, DEM_E_WRONG_CONFIGURATION);
    }
    else if (CycleState == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETOPERATIONCYCLESTATE, DEM_E_PARAM_POINTER);
    }
    else
    {
        if (DEM_BITS_ISNSET(DemOperationCycleStatus, OperationCycleId))
        {
            *CycleState = DEM_CYCLE_STATE_END;
        }
        else
        {
            *CycleState = DEM_CYCLE_STATE_START;
        }
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Triggers the next aging cycle state.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OperationCycleId:Identification of aging cycle.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of aging cycle was successful
 *                       E_NOT_OK: set of aging cycle failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_SetAgingCycleState(uint8 OperationCycleId)
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState == DEM_STATE_UNINIT)
    {
        DEM_DET_REPORT(DEM_SID_SETAGINGCYCLESTATE, DEM_E_UNINIT);
    }
    else if (OperationCycleId >= DemPbCfgPtr->DemOperationCycleNum)
    {
        DEM_DET_REPORT(DEM_SID_SETAGINGCYCLESTATE, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        Dem_AgingCycle = OperationCycleId;
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets the WIR status bit via failsafe SW-Cs.
 * ServiceId           0x7a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                       WIRStatus: Requested status of event related WIR-bit
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The request is accepted
 *                       E_NOT_OK: not be accepted
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_SetWIRStatus(Dem_EventIdType EventId, boolean WIRStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventRelateInformationType* pEvent;
    Dem_EventIdType IntId;
    if (Dem_InitState < DEM_STATE_PRE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETWIRSTATUS, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_SETWIRSTATUS, DEM_E_WRONG_CONFIGURATION);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        /* req SWS_Dem_01108 req SWS_Dem_00836*/
        if ((0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
            || (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING)))
        {
            /*idle*/
        }
        else
        {
            if (WIRStatus == TRUE)
            {
                /* req SWS_Dem_00833 */
                DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
                /*The bit is set to 1 when DTC status bit no 7 is set to 1. In all cases, FDC10 has reached the value
                 * +127, since DTC information was latest cleared, before the bit is set to 1 and warring indicator is
                 * requested*/
                if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                {
                    DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_WIRSLC); /*bit 5*/
                }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
            }
            else
            {
                /* req SWS_Dem_00834 */
                DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
            }
            /* update the combination event dtc status*/
            Dem_UpdateCombinedDtcStatus(IntId);
            Dem_ResponseOnDtcStatusChange(IntId);
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the failed status of a DemComponent.
 * ServiceId           0x2a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComponentId:Identification of a DemComponent
 * Param-Name[out]     ComponentFailed: TRUE: failed FALSE: not failed
 * Param-Name[in/out]  None
 * Return              E_OK: getting "ComponentFailed" was successful
 *                       E_NOT_OK: getting "ComponentFailed" was not successful
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432,3673++ */ /* MISRA Rule 20.7,8.13 */
Dem_GetComponentFailed(Dem_ComponentIdType ComponentId, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed)
/* PRQA S 3432,3673-- */ /* MISRA Rule 20.7,8.13 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETCOMPONENTFAILED, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterGetComponentFailed(ComponentId, ComponentFailed);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the current extended event status of an event.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 * Param-Name[out]     EventStatusByte:UDS DTC status byte of the requested event
 * Param-Name[in/out]  None
 * Return              E_OK: get of event status was successful
 *                       E_NOT_OK: get of event status failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetEventStatus(Dem_EventIdType EventId, P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) EventStatusByte)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTSTATUS, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTSTATUS, DEM_E_WRONG_CONFIGURATION);
    }
    else if (EventStatusByte == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTSTATUS, DEM_E_PARAM_POINTER);
    }
    else
    {
        pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
        /* req SWS_Dem_01108*/
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            *EventStatusByte = pEvent->UdsStatus;
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the event failed status of an event.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 * Param-Name[out]     EventFailed: TRUE - Last Failed FALSE - not Last Failed
 * Param-Name[in/out]  None
 * Return              E_OK: get of "EventFailed" was successful
 *                       E_NOT_OK: get of "EventFailed" was not successful
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetEventFailed(Dem_EventIdType EventId, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventFailed)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTFAILED, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTFAILED, DEM_E_WRONG_CONFIGURATION);
    }
    else if (EventFailed == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTFAILED, DEM_E_PARAM_POINTER);
    }
    else
    {
        pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
        /* req SWS_Dem_01108 */
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
            {
                *EventFailed = FALSE;
            }
            else
            {
                *EventFailed = TRUE;
            }
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the event tested status of an event.
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 * Param-Name[out]     EventTested: TRUE - event tested this cycle
 *                                    FALSE - event not tested this cycle
 * Param-Name[in/out]  None
 * Return              E_OK: get of event state "tested" successful
 *                       E_NOT_OK: get of event state "tested" failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetEventTested(Dem_EventIdType EventId, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventTested)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTTESTED, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTTESTED, DEM_E_WRONG_CONFIGURATION);
    }
    else if (EventTested == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTTESTED, DEM_E_PARAM_POINTER);
    }
    else
    {
        pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(EventId));
        /* req SWS_Dem_01108 */
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
            {
                *EventTested = TRUE;
            }
            else
            {
                *EventTested = FALSE;
            }
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the debouncing status of an event.
 * ServiceId           0x9f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 * Param-Name[out]     DebouncingState
 * Param-Name[in/out]  None
 * Return              E_OK: get of debouncing status per event state successful
 *                       E_NOT_OK: get of debouncing per event state failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetDebouncingOfEvent(
    Dem_EventIdType EventId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_DebouncingStateType, AUTOMATIC, DEM_APPL_DATA) DebouncingState)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    sint8 FDC = 0;
    const Dem_EventRelateInformationType* pEvent;
    Dem_EventIdType IntId;
    Std_ReturnType ret = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETDEBOUNCINGOFEVENT, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETDEBOUNCINGOFEVENT, DEM_E_WRONG_CONFIGURATION);
    }
    else if (DebouncingState == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETDEBOUNCINGOFEVENT, DEM_E_PARAM_POINTER);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        /* req SWS_Dem_01108 */
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            ret = Dem_GetInteralFDC(IntId, &FDC);
            *DebouncingState = 0x00;
            if (ret == E_OK)
            {
                /* req SWS_Dem_00844 */
                if ((FDC < 127) && (FDC > 0))
                {
                    *DebouncingState |= DEM_TEMPORARILY_DEFECTIVE;
                }
                if (FDC == 127)
                {
                    *DebouncingState |= DEM_FINALLY_DEFECTIVE;
                }
                if ((FDC < 0) && (FDC > -128))
                {
                    *DebouncingState |= DEM_TEMPORARILY_HEALED;
                }
                if ((FDC == -128) || (FDC == 127))
                {
                    *DebouncingState |= DEM_TEST_COMPLETE;
                }
                if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                    && ((0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
                        || (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_STORAGE_CONDICTION))))
                {
                    *DebouncingState |= DEM_DTR_UPDATE;
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the DTC of an event.
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       DTCFormat: Defines the output-format of the requested DTCvalue.>
 * Param-Name[out]     DTCOfEvent: Receives the DTC value in respective format returned
 *                                   by this function.
 * Param-Name[in/out]  None
 * Return              E_OK: get of DTC was successful
 *                       E_NOT_OK: the call was not successful
 *                       DEM_E_NO_DTC_AVAILABLE: there is no DTC configured in the requested format
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetDTCOfEvent(
    Dem_EventIdType EventId,
    Dem_DTCFormatType DTCFormat,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTCOfEvent)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;
    const Dem_EventParameterType* pEventCfg;
    const Dem_EventRelateInformationType* pEvent;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETDTCOFEVENT, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETDTCOFEVENT, DEM_E_WRONG_CONFIGURATION);
    }
    else if (DTCOfEvent == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETDTCOFEVENT, DEM_E_PARAM_POINTER);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        /* req SWS_Dem_01108 */
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            *DTCOfEvent = 0x00u;
            pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
            uint16 DTCRef = pEventCfg->DemDTCRef;
            if (DTCRef != DEM_DTC_REF_INVALID)
            {
                uint16 ObdDTCRef = DemPbCfgPtr->DemDTC[DTCRef].DemObdDTCRef;
                /* req SWS_Dem_00269 */
                ret = DEM_E_NO_DTC_AVAILABLE;
                switch (DTCFormat)
                {
                case DEM_DTC_FORMAT_OBD:
                    if ((ObdDTCRef != DEM_OBD_DTC_INVALID)
                        && (DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                    {
                        *DTCOfEvent = DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemDtcValue;
                        ret = E_OK;
                    }
                    break;
                case DEM_DTC_FORMAT_UDS:
                    if (DEM_DTC_CFG_INVALID != DemPbCfgPtr->DemDTC[DTCRef].DemDtcValue)
                    {
                        *DTCOfEvent = DemPbCfgPtr->DemDTC[DTCRef].DemDtcValue;
                    }
                    ret = E_OK;
                    break;
                case DEM_DTC_FORMAT_J1939:
                    if ((ObdDTCRef != DEM_OBD_DTC_INVALID)
                        && (DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                    {
                        *DTCOfEvent = DemPbCfgPtr->DemObdDTC[ObdDTCRef].DemJ1939DTCValue;
                        ret = E_OK;
                    }
                    break;
                default:
                    /* idle */
                    break;
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets an enable condition.
 * ServiceId           0x39
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EnableConditionID:This parameter identifies the enable condition.
 *                       ConditionFulfilled: This parameter specifies whether the enable condition
 *                       assigned to the EnableConditionID is fulfilled
 *                       (TRUE) or not fulfilled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              In case the enable condition could be set successfully the API call returns
 * E_OK. If the setting of the enable condition failed the return value of the function is E_NOT_OK.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetEnableCondition(uint8 EnableConditionID, boolean ConditionFulfilled)
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETENABLECONDITION, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterSetEnableCondition(EnableConditionID, ConditionFulfilled);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Sets a storage condition.
 * ServiceId           0x38
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      StorageConditionID: This parameter identifies the storage condition.
 *                       ConditionFulfilled: This parameter specifies whether the storage condition
 *                       assigned to the StorageConditionID is fulfilled (TRUE) or not
 *                       fulfilled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              In case the storage condition could be set successfully the API call
 *                      returns E_OK. If the setting of the storage condition failed the
 *                      return value of the function is E_NOT_OK.
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetStorageCondition(uint8 StorageConditionID, boolean ConditionFulfilled)
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETSTORAGECONDITION, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterSetStorageCondition(StorageConditionID, ConditionFulfilled);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the fault detection counter of an event.
 * ServiceId           0x3e
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.>
 * Param-Name[out]     FaultDetectionCounter: This parameter receives the Fault Detection Counter
 *                      information of the requested EventId. If the return value of the function
 *                      call is other than E_OK this parameter does not contain valid data.
 *                      -128dec...127dec PASSED... FAILED according to ISO 14229-1
 * Param-Name[in/out]  None
 * Return              E_OK: request was successful
 *                       E_NOT_OK: request failed
 *                       DEM_E_NO_FDC_AVAILABLE: there is no fault detection counter available
 *                       for the requested event
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetFaultDetectionCounter(Dem_EventIdType EventId, P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETFAULTDETECTIONCOUNTER, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETFAULTDETECTIONCOUNTER, DEM_E_WRONG_CONFIGURATION);
    }
    else if (FaultDetectionCounter == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETFAULTDETECTIONCOUNTER, DEM_E_PARAM_POINTER);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        /* req SWS_Dem_01108 */
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            ret = Dem_GetInteralFDC(IntId, FaultDetectionCounter);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the indicator status derived from the event status.
 * ServiceId           0x29
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId: Number of indicator
 * Param-Name[out]     IndicatorStatus: Status of the indicator, like off, on, or blinking.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful E_NOT_OK: Operation failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetIndicatorStatus(uint8 IndicatorId, P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETINDICATORSTATUS, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterGetIndicatorStatus(IndicatorId, IndicatorStatus);
    }
    return ret;
}
/*************************************************************************/
/*
 * Brief               Sets the indicator status included in the event status.
 * ServiceId           0xa1
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId : Number of indicator
 *                      IndicatorStatus Status of the indicator, like off, on, or blinking.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_SetIndicatorStatus(uint8 IndicatorId, P2CONST(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETINDICATORSTATUS, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterSetIndicatorStatus(IndicatorId, IndicatorStatus);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the data of a freeze frame by event.
 * ServiceId           0x6e
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                       frame record as defined in ISO15031-5 and ISO14229-1.
 *                       0xFF means most recent freeze frame record is returned.
 *                       DataId : This parameter specifies the PID (ISO15031-5 mapped in UDS range
 *                       0xF400u - 0xF4FFu) or DID (ISO14229-1) that shall be copied to
 *                       the destination buffer.
 * Param-Name[out]     DestBuffer : This parameter contains a byte pointer that points to the
 *                       buffer, to which the freeze frame data record shall be written to.
 *                       The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]   BufSize: When the function is called this parameter contains
                            the maximum number of data bytes that can be written to the buffer.
                            The function returns the actual number of written
                            data bytes in this parameter
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently stored
 *                                              (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported
 *                                              by the event
 *                       DEM_E_WRONG_DIDNUMBER: The requested DID is not supported by the freeze
 frame
 */
/*************************************************************************/
/* PRQA S 0624++ */ /* MISRA Rule 8.3 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetEventFreezeFrameDataEx(
    /* PRQA S 0624-- */ /* MISRA Rule 8.3 */
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    uint16 DataId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint16* BufSize)
{
    const Dem_EventRelateInformationType* pEvent;
    Std_ReturnType ret = E_NOT_OK;
    Dem_EventIdType IntId;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTFREEZEFRAMEDATAEX, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTFREEZEFRAMEDATAEX, DEM_E_WRONG_CONFIGURATION);
    }
    else if (DestBuffer == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTFREEZEFRAMEDATAEX, DEM_E_PARAM_POINTER);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            ret = Dem_InterGetEventFreezeFrameDataEx(EventId, RecordNumber, DataId, DestBuffer, BufSize);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the data of an extended data record by event.
 * ServiceId           0x6d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 *                       RecordNumber: Identification of requested Extended data record.
 *                       Valid values are between 0x01 and 0xEF as defined in ISO14229-1.
 * Param-Name[out]     DestBuffer: This parameter contains a byte pointer that points
 *                          to the buffer, to which the extended data shall bewritten to.
 *                          The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently
 *                              stored (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported by
 * the event
 */
/*************************************************************************/
/* PRQA S 0624++ */ /* MISRA Rule 8.3 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetEventExtendedDataRecordEx(
    /* PRQA S 0624-- */ /* MISRA Rule 8.3 */
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint16* BufSize)
{
    Std_ReturnType ret = E_NOT_OK;
    const Dem_EventRelateInformationType* pEvent;
    Dem_EventIdType IntId;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTEXTENDEDDATARECORDEX, DEM_E_UNINIT);
    }
    else if ((EventId > DemPbCfgPtr->DemEventNum) || (EventId == 0u))
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTEXTENDEDDATARECORDEX, DEM_E_WRONG_CONFIGURATION);
    }
    else if (DestBuffer == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTEXTENDEDDATARECORDEX, DEM_E_PARAM_POINTER);
    }
    else
    {
        IntId = Dem_GetEventInternalId(EventId);
        pEvent = Dem_GetEventInfo(IntId);
        if (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
        {
            ret = Dem_InterGetEventExtendedDataRecordEx(IntId, RecordNumber, DestBuffer, BufSize);
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Gets the event memory overflow indication status.
 * ServiceId           0x32
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin: If the Dem supports more than one event memory this parameter
 *                          is used to select the source memory the overflow indication shall be
 * read from.> Param-Name[out]     OverflowIndication: This parameter returns TRUE if the according
 *                          event memory was overflowed, otherwise it returns FALSE.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_GetEventMemoryOverflow(Dem_DTCOriginType DTCOrigin, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) OverflowIndication)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 memDest;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTMEMORYOVERFLOW, DEM_E_UNINIT);
    }
    else if (OverflowIndication == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETEVENTMEMORYOVERFLOW, DEM_E_PARAM_POINTER);
    }
    else
    {
        memDest = Dem_GetInternalMemDest(DTCOrigin);
        if (memDest == DEM_MEM_DEST_INVALID)
        {
            DEM_DET_REPORT(DEM_SID_GETEVENTMEMORYOVERFLOW, DEM_E_WRONG_CONFIGURATION);
        }
        else
        {
            /* req SWS_Dem_00398 */
            *OverflowIndication = DemMemDestInfo[memDest].OverFlow;
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Returns the number of entries currently stored in the requested event memory.
 * ServiceId           0x35
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin: If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the number of entries shall be read from.
 * Param-Name[out]     NumberOfEventMemoryEntries: Number of entries currently stored in the
 *                          requested event memory.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful E_NOT_OK: Operation failed
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_GetNumberOfEventMemoryEntries(
    Dem_DTCOriginType DTCOrigin,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) NumberOfEventMemoryEntries)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 memDest;
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES, DEM_E_UNINIT);
    }
    else if (NumberOfEventMemoryEntries == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES, DEM_E_PARAM_POINTER);
    }
    else
    {
        memDest = Dem_GetInternalMemDest(DTCOrigin);
        if (memDest == DEM_MEM_DEST_INVALID)
        {
            DEM_DET_REPORT(DEM_SID_GETNUMBEROFEVENTMEMORYENTRIES, DEM_E_WRONG_CONFIGURATION);
        }
        else
        {
            /* req SWS_Dem_00651 */
            *NumberOfEventMemoryEntries = DemMemDestInfo[memDest].RecordNum;
            ret = E_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Set the availability of a specific DemComponent.
 * ServiceId           0x2b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ComponentId: Identification of a DemComponent.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                          Component shall be available (TRUE) or not (FALSE).
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              E_OK: Operation was successful
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetComponentAvailable(Dem_ComponentIdType ComponentId, boolean AvailableStatus)
{
    Std_ReturnType ret = E_NOT_OK;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETCOMPONENTAVAILABLE, DEM_E_UNINIT);
    }
    else
    {
        ret = Dem_InterSetComponentAvailable(ComponentId, AvailableStatus);
    }
    return ret;
}

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/*************************************************************************/
/*
 * Brief               Set the suppression status of a specific DTC.
 * ServiceId           0x33
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC: Diagnostic Trouble Code
 *                       DTCFormat: Defines the input-format of the provided DTC value.
 *                       SuppressionStatus: This parameter specifies whether the respective
 *                       DTC shall be disabled (TRUE) or enabled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK (Operation was successful)
 *                       E_NOT_OK (operation failed or event entry for this DTC still exists)
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_SetDTCSuppression(uint32 DTC, Dem_DTCFormatType DTCFormat, boolean SuppressionStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETDTCSUPPRESSION, DEM_E_UNINIT);
    }
    else
    {
        uint16 DTCRef = Dem_GetDTCIndex(DTC, DTCFormat);
        if (DTCRef == DEM_DTC_REF_INVALID)
        {
            DEM_DET_REPORT(DEM_SID_SETDTCSUPPRESSION, DEM_E_WRONG_RECORDNUMBER);
        }
        else
        {
            const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[DTCRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
            const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
            const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
            const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
            ret = E_OK;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
            for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
            {
                if (NULL_PTR != Dem_CheckEventMemEntryExistsAlready(*pDTCMapping))
                {
                    ret = E_NOT_OK;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                    break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
            }
            if (E_OK == ret)
            {
                DemDTCGeneralStatus[DTCRef].SuppressionStatus = SuppressionStatus;
            }
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               Clears single DTCs, as well as groups of DTCs. This API is intended
                       for complex device driver. It can only be used through the RTE (due
                       to work-around described below SWS_Dem_00659), and therefore no
                       declaration is exported via Dem.h.
 * ServiceId           0x23
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
/* PRQA S 0624++ */ /* MISRA Rule 8.3 */
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_ClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
/* PRQA S 0624-- */ /* MISRA Rule 8.3 */
{
    Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
    Dem_ReturnClearDTCType ret = DEM_CLEAR_FAILED;
    boolean ProcessOn = TRUE;
    uint8 memDest;

    if (Dem_InitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_CLEARDTC, DEM_E_UNINIT);
    }
    else
    {
        if (Dem_ClearDTCLock == DEM_CLEAR_LOCK)
        {
            if ((pClr->SaveDTCIndex == Dem_GetDTCIndex(DTC, DTCFormat)) && (pClr->SaveDTCFormat == DTCFormat)
                && (pClr->SaveDTCOrigin == DTCOrigin) && (pClr->SID == DEM_SID_CLEARDTC))
            {
                /* req SWS_Dem_00663 */
                ret = DEM_CLEAR_PENDING;
                ProcessOn = FALSE;
            }
            else
            {
                /* req SWS_Dem_00662 SWS_Dem_00664 */
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
            pClr->SID = DEM_SID_CLEARDTC;
        }
        else
        {
            /*idle*/
        }

        /* req SWS_Dem_00670 */
#if (DEM_CLEAR_DTCLIMITATION == DEM_ONLY_CLEAR_ALL_DTCS)
        if ((ProcessOn == TRUE) && ((DTCFormat != DEM_DTC_FORMAT_UDS) || ((DTC & 0xFFFFFFUL) != 0xFFFFFFUL)))
        {
            Dem_ResetDemClearDTCInfo();
            ret = DEM_CLEAR_WRONG_DTC;
            ProcessOn = FALSE;
        }
#endif
        if (ProcessOn == TRUE)
        {
            memDest = Dem_GetInternalMemDest(DTCOrigin);
            if (memDest == DEM_MEM_DEST_INVALID)
            {
                Dem_ResetDemClearDTCInfo();
                ret = DEM_CLEAR_WRONG_DTCORIGIN;
            }
            else
            {
                if ((DTC & 0xFFFFFFUL) == 0xFFFFFFUL)
                {
                    pClr->ClearAllGroup = TRUE;
                }
                else
#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
                {
                    pClr->ClearAllGroup = FALSE;
                    pClr->DTCGroupIndex = Dem_GetDTCGroupIndex(DTC);
                    if (pClr->DTCGroupIndex == DEM_GROUP_OF_DTC_INVALID)
                    {
                        pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
                        if (pClr->DTCIndex == DEM_DTC_REF_INVALID)
                        {
                            Dem_ResetDemClearDTCInfo();
                            ret = DEM_CLEAR_WRONG_DTC;
                            ProcessOn = FALSE;
                        }
                    }
                }
#else
                {
                    ret = DEM_CLEAR_WRONG_DTC;
                    ProcessOn = FALSE;
                }
#endif
                if (ProcessOn == TRUE)
                {
                    pClr->memDest = memDest;
                    Dem_Pending = TRUE;
                    Dem_ClearDTCProcess();
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_VOLATILE)
                    /* req SWS_Dem_00570 */
                    Dem_ResetDemClearDTCInfo();
                    ret = DEM_CLEAR_OK;
#elif (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_TRIGGER)
                    /* req SWS_Dem_00571 */
                    Dem_ClearNonvolatileStatus = DEM_CLEAR_NONVOLATILE_START;
                    Dem_ResetDemClearDTCInfo();
                    ret = DEM_CLEAR_OK;
#elif (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
                    /* req SWS_Dem_00663 */
                    Dem_ClearNonvolatileStatus = DEM_CLEAR_NONVOLATILE_START;
                    ret = DEM_CLEAR_PENDING;
#endif
                }
            }
        }
    }
    return ret;
}

#if (DEM_NVRAM_BLOCKID_NUM > 0u)
/*************************************************************************/
/*
 * Brief               check if non-volatile event memory data cleared.
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ClearNonvolatile(void)
{
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    uint8 blockIndex;
    NvM_RequestResultType RequestResult;
#endif

    if (Dem_ClearNonvolatileStatus == DEM_CLEAR_NONVOLATILE_START)
    {
        /*clear done ,to write*/
        Dem_IntWriteNvRAM();
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
        Dem_ClearNonvolatileStatus = DEM_CLEAR_NONVOLATILE_PROCESSING;
#else
        Dem_ClearNonvolatileStatus = DEM_ClEAR_NONVOLATILE_INIT;
#endif
    }
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    if (Dem_ClearNonvolatileStatus == DEM_CLEAR_NONVOLATILE_PROCESSING)
    {
        for (blockIndex = Dem_ClearCnt; blockIndex < DEM_NVRAM_BLOCKID_NUM; blockIndex++)
        {
            NvM_GetErrorStatus(DemNvRamBlockId[blockIndex], &RequestResult);
            if (RequestResult == NVM_REQ_OK)
            {
                Dem_ClearCnt++;
            }
            else if (RequestResult == NVM_REQ_PENDING)
            {
                break;
            }
            else
            {
                Dem_ClearCnt = 0u;
                Dem_ClearNonvolatileStatus = DEM_ClEAR_NONVOLATILE_FAILED;
                break;
            }
        }
        if (Dem_ClearCnt == DEM_NVRAM_BLOCKID_NUM)
        {
            Dem_ClearCnt = 0u;
            Dem_ClearNonvolatileStatus = DEM_ClEAR_NONVOLATILE_FINISHED;
        }
    }
#endif
}
#endif /* DEM_NVRAM_BLOCKID_NUM > 0u */

/********[Scheduled functions]*********/
/*************************************************************************/
/*
 * Brief               Processes all not event based Dem internal functions.
 * ServiceId           0x55
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 3408++ */ /* MISRA Rule 8.4 */
FUNC(void, DEM_CODE) Dem_MainFunction(void)
/* PRQA S 3408-- */ /* MISRA Rule 8.4 */
{
    if (Dem_InitState == DEM_STATE_INIT)
    {
        Dem_SetEnableCondictionProcess();
        Dem_SetStorageCondictionProcess();
        Dem_DebounceTimerMain();
        Dem_EventQueueProcess();
#if (DEM_NVRAM_BLOCKID_NUM > 0)
        Dem_ClearNonvolatile();
#endif
    }
}

/********[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]*****/

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Initializes or reinitializes this module.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Std_ReturnType.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_SubInit(Std_ReturnType NvmResult)
{
    uint16 iloop;
    Dem_EventRelateInformationType* pEvent;
    uint16 dtcRef;
    uint16 EventNum = DemPbCfgPtr->DemEventNum;
    const Dem_EventParameterType* pDemEventParameter = DemPbCfgPtr->DemEventParameter;

    for (iloop = 0u; iloop < EventNum; iloop++)
    {
        pEvent = &DemEventRelateInformation[iloop];
        pEvent->IntId = iloop;
        const Dem_EventParameterType* pEventCfg = &pDemEventParameter[iloop];
#if (DEM_STATUS_BIT_STORAGE_TEST_FAILED == STD_OFF)
        /* req SWS_Dem_00525 clear bit0 */
        DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TF);
#else
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ACTIVE);
        }
        else
        {
            DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ACTIVE);
        }
#endif /* DEM_STATUS_BIT_STORAGE_TEST_FAILED == STD_OFF */

        pEvent->OldUdsStatus = pEvent->UdsStatus;
        dtcRef = pEventCfg->DemDTCRef;
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
        if (pEventCfg->DemEventAvailable == TRUE)
        {
            DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE);
        }
        if (DEM_DTC_REF_INVALID != dtcRef)
        {
            if (pEventCfg->DemEventAvailable == TRUE)
            {
                DemDTCGeneralStatus[dtcRef].SuppressionStatus = FALSE;
            }
            else
            {
                DemDTCGeneralStatus[dtcRef].SuppressionStatus = TRUE;
                /* req SWS_Dem_00915 */
                const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
                const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
                const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                {
                    if (pEventParameter[*pDTCMapping].DemEventAvailable == TRUE)
                    {
                        DemDTCGeneralStatus[dtcRef].SuppressionStatus = FALSE;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                        break;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                    }
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                    pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                }
            }
            Dem_UpdateCombinedDtcStatus(iloop);
        }
        if (E_NOT_OK == NvmResult)
        {
            Dem_ClearEventRelateInformation(pEvent);
        }
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_CONSECUTIVE_FAULT);
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
        DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
        /*bit 6: The bit is set to 0 when the DTC is not emission related.The bit is set to 1 when the DTC is emission
         * related.*/
        const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef];
        if ((pDemDTC->DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS))
        {
            DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_ER);
        }
        else
        {
            DEM_FLAGS_CLR(pEvent->StatusIndicator30, DEM_SI30_DTC_ER);
        }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
    }
}
#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
/*************************************************************************/
/*
 * Brief               ClearOneDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_ClearOneDTC(uint8 memDest, uint16 DTCIndex)
{
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[DTCIndex];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
        Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(*pDTCMapping);
        Dem_Clear(pEvent, memDest);
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    }
    /* req SWS_Dem_00399 */
    if (DemMemDestInfo[memDest].RecordNum == 0u)
    {
        DemMemDestInfo[memDest].OverFlow = FALSE;
    }
}
#endif

/*************************************************************************/
/*
 * Brief               Dem_EventQueueProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_EventQueueProcess(void)
{
    Dem_EventQueueType* pQueue = &DemEventQueue;
    Dem_EventDataBufferType* pBuffer = &(pQueue->Queue[pQueue->ReadIndex]);
    Dem_EventIdType IntId;
    uint8 OperationCycleId;
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON)
    Dem_EventRelateInformationType* pEvent;
#else
    const Dem_EventRelateInformationType* pEvent;
#endif /* DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON */
#if (DEM_DEAL_MAINFUNCTION_ENABLE == STD_ON)
    uint8 Deal_Cnt = 0;
#endif
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    uint8 OldStatus;
#endif
    /* Proccess Event */
    while ((pBuffer->ExtId != 0x00u)
#if (DEM_DEAL_MAINFUNCTION_ENABLE == STD_ON)
           && (Deal_Cnt < DEM_DEAL_MAINFUNCTION_COUNTER)
#endif
    )
    {
#if (DEM_DEAL_MAINFUNCTION_ENABLE == STD_ON)
        Deal_Cnt++;
#endif
        IntId = Dem_GetEventInternalId(pBuffer->ExtId);
        pEvent = Dem_GetEventInfo(IntId);
        OperationCycleId = DemPbCfgPtr->DemEventParameter[IntId].DemOperationCycleRef;
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        OldStatus = pEvent->UdsStatus;
#endif
        if ((0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
            && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
            && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION)))
        {
            /* req SWS_Dem_00700 */
            if (((DEM_BITS_ISSET(DemOperationCycleStatus, OperationCycleId))
                 || (DemPbCfgPtr->DemOperationCycle[OperationCycleId].DemOperationCycleType == DEM_OPCYC_OBD_DCY)))
            {
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON)
                if ((pBuffer->Status == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED)
                    && (0x00u == DEM_FLAGS_ISSET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_FDC_TRIGGER)))
                {
                    /* req SWS_Dem_01068 req SWS_Dem_01069 */
                    DEM_FLAGS_SET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_FDC_TRIGGER);
                    Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_FDC_THRESHOLD);
                }
                else
#endif /* DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON */
                {
                    /* Debounce */
                    /* PRQA S 0431++ */ /* MISRA Rule 1.1 */
                    switch (Dem_DebounceProcess(*pBuffer))
                    /* PRQA S 0431-- */ /* MISRA Rule 1.1 */
                    {
                    case DEM_EVENT_STATUS_PASSED:
                        /* PRQA S 0431++ */ /* MISRA Rule 1.1 */
                        Dem_EventTestPassed(IntId);
                        /* PRQA S 0431-- */ /* MISRA Rule 1.1 */
                        break;
                    case DEM_EVENT_STATUS_FAILED:
                        /* PRQA S 0431++ */ /* MISRA Rule 1.1 */
                        Dem_EventTestFailed(IntId);
                        /* PRQA S 0431-- */ /* MISRA Rule 1.1 */
                        break;
                    default:
                        /*idle*/
                        break;
                    }
#if (DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON)
                    uint16 DemDTCRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
                    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST)
                    pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
                    Dem_UpdateFDC(IntId);
                    /*SWS_Dem_00786,SWS_Dem_01068,SWS_Dem_00811,SWS_Dem_01069*/
                    if ((DemDTCRef != DEM_DTC_REF_INVALID) && (FDCInfo[DemDTCRef].FDC > 0)
                        && (DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[DemDTCRef].DemDTCAttributesRef]
                                .DemEventMemoryEntryFdcThresholdStorageValue
                            <= (uint8)FDCInfo[DemDTCRef].FDC)
                        && (DEM_DEBOUNCE_COUNTER_BASED == pEventCfg->AlgorithmType))
                    {
                        /*SWS_Dem_00799]*/
                        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
                        Dem_EventDataStorageTrigger(IntId, DEM_TRIGGER_ON_FDC_THRESHOLD);
                    }
#endif /* DEM_TRIGGERONFDCTHRESHOLDEANLE == STD_ON */
                }
            }
            else
            {
                Dem_EventQueueProcessWithDebounce(IntId);
            }
        }
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        if (OldStatus != pEvent->UdsStatus)
        {
            (void)FiM_DemTriggerOnEventStatus(pEvent->IntId + 1u, OldStatus, pEvent->UdsStatus);
        }
#endif
        /* Clear Buffer */
        pBuffer->ExtId = 0x00;
        pBuffer->Status = 0x00;

        /* Move ReadIndex */
        pQueue->ReadIndex++;
        if (pQueue->ReadIndex >= DEM_EVENT_QUEUE_SIZE)
        {
            pQueue->ReadIndex = 0x00;
        }
        pBuffer = &(pQueue->Queue[pQueue->ReadIndex]);
    }
}

/*************************************************************************/
/*
 * Brief               Dem_OperationCycleCounterProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_OperationCycleCounterProcess(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
    {
        if (pEvent->FailureCounter < 0xFFu)
        {
            pEvent->FailureCounter++; /*SWS_Dem_00782*/
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
            /* Test Failed This Operation Cycle */
            pEvent->CyclesSinceLastFailed = 0u;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
        }
    }
#if ((DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON) || (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON))
    if (0x00u != (DEM_FLAGS_ISSET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_CYCLECOUNTER)))
    {
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
        if (pEvent->CyclesSinceFirstFailed < 0xFFu)
        {
            /* SWS_Dem_00778 */
            pEvent->CyclesSinceFirstFailed++;
        }
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
        if (pEvent->CyclesSinceLastFailed < 0xFFu)
        {
            /* SWS_Dem_00774 */
            pEvent->CyclesSinceLastFailed++;
        }
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
    }
#endif /*DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)|| (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
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
FUNC(void, DEM_CODE) Dem_UpdateCombinedDtcStatus(Dem_EventIdType IntId)
{
    uint16 dtcRef = DemPbCfg.DemEventParameter[IntId].DemDTCRef;
    if (dtcRef != DEM_DTC_REF_INVALID)
    {
        uint8 CbUdsStatus;
        Dem_DTCGeneralStatusType* pDTCGeneralStatus = &DemDTCGeneralStatus[dtcRef];
        const Dem_DTCType* pDemDTC = &DemPbCfg.DemDTC[dtcRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
        const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
        const Dem_EventIdType* pDTCMapping = &DemPbCfg.DemDTCMapping[EventRefStart];
        pDTCGeneralStatus->CbUdsStatus = 0;

#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        for (uint16 iloop = 0; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
        /* SWS_Dem_00441*/
        {
            /*bit0 - bit7 or-operation */
            DemDTCGeneralStatus[dtcRef].CbUdsStatus |= DemEventRelateInformation[*pDTCMapping].UdsStatus;
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
            pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
        }
        /* bit4 calculation */
        CbUdsStatus = ~((DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_UDS_STATUS_TFSLC) >> 1u);
        DemDTCGeneralStatus[dtcRef].CbUdsStatus &= CbUdsStatus;
        /* bit6 calculation */
        CbUdsStatus = (uint8)(~(uint8)((DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_UDS_STATUS_TFTOC) << 5u));
        DemDTCGeneralStatus[dtcRef].CbUdsStatus &= CbUdsStatus;
    }
}

/*************************************************************************/
/*
 * Brief               Dem_OperationCycleStart
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId && IsRestart
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_OperationCycleStart(uint8 OpId, boolean IsRestart)
{
    const Dem_EventParameterType* pEventCfg;
    Dem_EventRelateInformationType* pEvent;
    uint16 iloop;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
        pEvent = &DemEventRelateInformation[iloop];
        /* req SWS_Dem_00506 */
        Dem_EventHealingProcess(pEvent, OpId);
#if (DEM_OCC6EANLE == STD_ON)
        if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TNCTOC))
        {
            /* In last cycle, no fail occured. */
            DEM_FLAGS_SET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_OCC6CLR);
        }
        else if (
            (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
            && (0x00u == DEM_FLAGS_ISSET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_OCC6CLR)))
        {
            /* In last cycle , fail occured and OCC6 is not cleared */
            DEM_FLAGS_SET(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_OCC6CLR);
        }
        else
        {
            /* idle */
        }
#endif /* DEM_OCC6EANLE == STD_ON */
        if (IsRestart == TRUE)
        {
            Dem_EventAgingProcess(pEvent, OpId);
            if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
                && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC)))
            {
                DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC);
            }
        }
        if (pEventCfg->DemOperationCycleRef == OpId)
        {
            /* req SWS_Dem_00679 Monitor re-initialization */
            if (pEventCfg->DemCallbackInitMForE != NULL_PTR)
            {
                pEventCfg->DemCallbackInitMForE(DEM_INIT_MONITOR_RESTART);
            }
            DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
            DEM_FLAGS_CLR(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_FDC_TRIGGER);
            if (IsRestart == TRUE)
            {
                /* req SWS_Dem_00773 req SWS_Dem_00781 req SWS_Dem_00777 */
                Dem_OperationCycleCounterProcess(pEvent);
            }
#if ((DEM_OCC6EANLE == STD_ON) || (DEM_OCC4EANLE == STD_ON))
            DEM_FLAGS_CLR(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_THISOPISFAILED);
#endif /* DEM_OCC6EANLE == STD_ON || DEM_OCC4EANLE == STD_ON */
            /* bit1 1 => 0 req SWS_Dem_00389 */
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC);
            /* bit6 0 => 1 req SWS_Dem_00394 */
            DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC);
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
            /*The bit is reset to 0 at start of an operation cycle.*/
            DEM_FLAGS_CLR(pEvent->StatusIndicator30, DEM_SI30_DTC_UCTOC);
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
            /* req SWS_Dem_00344*/
            Dem_InterOperationCycleStart(iloop);
            uint16 DemDTCRef = pEventCfg->DemDTCRef;
            if (DemDTCRef != DEM_DTC_REF_INVALID)
            {
                /* req SWS_Dem_00790 */
                FDCInfo[DemDTCRef].MaxFDCDuringCurrentCycle = 0;
            }
        }
        /* update the combination event dtc status*/
        Dem_UpdateCombinedDtcStatus(iloop);
        /*first init shall not update and trigger the callback*/
        if (Dem_InitState != DEM_STATE_PRE_INIT)
        {
            Dem_ResponseOnDtcStatusChange(iloop);
        }
    }
    if (Dem_AgingCycle == OpId)
    {
        Dem_AgingCycle = DEM_OPERATION_CYCLE_INVALID;
    }
}

/*************************************************************************/
/*
 * Brief               Dem_OperationCycleEnd
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_OperationCycleEnd(uint16 OpId)
{
    const Dem_EventParameterType* pEventCfg;
    Dem_EventRelateInformationType* pEvent;
    uint16 iloop;

    for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
    {
        pEventCfg = &DemPbCfgPtr->DemEventParameter[iloop];
        pEvent = &DemEventRelateInformation[iloop];
        if (pEventCfg->DemOperationCycleRef == OpId)
        {
            /* Process Operation Cycle Counters */
            /* req SWS_Dem_00773 req SWS_Dem_00781 req SWS_Dem_00777 */
            Dem_OperationCycleCounterProcess(pEvent);
        }
        Dem_EventAgingProcess(pEvent, OpId);
        if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
            && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
            && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC)))
        {
            DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC);
        }
        /*update the combination event dtc status*/
        Dem_UpdateCombinedDtcStatus(iloop);
        /*trigger ROE invoking*/
        Dem_ResponseOnDtcStatusChange(iloop);
    }
    if (Dem_AgingCycle == OpId)
    {
        Dem_AgingCycle = DEM_OPERATION_CYCLE_INVALID;
    }
#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
    Dem_CalOBDRelatedValue(OpId);
#endif
}

#if (DEM_NVRAM_BLOCKID_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_CopyInfoFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_CopyInfoFromEntry(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) Info,
    P2CONST(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) Entry)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Info->UdsStatus = Entry->UdsStatus;
    Info->AgingCounter = Entry->AgingCounter;
    Info->OccurrenceCounter = Entry->OccurrenceCounter;
    Info->HealingCounter = Entry->HealingCounter;
    Info->FailureCounter = Entry->FailureCounter;
#if (DEM_OCC6EANLE == STD_ON)
    Info->OCC6 = Entry->OCC6;
#endif /* DEM_OCC6EANLE == STD_ON */
#if (DEM_OCC4EANLE == STD_ON)
    Info->OCC4 = Entry->OCC4;
#endif /* DEM_OCC4EANLE == STD_ON */
#if (DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)
    Info->CyclesSinceFirstFailed = Entry->CyclesSinceFirstFailed;
#endif /* DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON */
#if (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON)
    Info->CyclesSinceLastFailed = Entry->CyclesSinceLastFailed;
#endif /* DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON */
#if ((DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON) || (DEM_CYCLESSINCELASTFAILEDEANLE == STD_ON))
    Info->EventStatusFlag = Entry->EventStatusFlag;
#endif /*DEM_CYCLESSINCEFRISTFAILEDEANLE == STD_ON)|| (DEM_CYCLESSINCELASTFAIL*/
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    Info->StatusIndicator30 = Entry->StatusIndicator30;
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
    Info->AgedCounter = Entry->AgedCounter;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
}

/*************************************************************************/
/*
 * Brief               Dem_IntReadNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE) Dem_IntReadNvRAM(void)
{
    Std_ReturnType retVal = E_OK;
    uint16 iloop;
    uint16 tempEventId = 0;
    uint16 blockIndex = 0;
    uint16 blockRef;
    NvM_RequestResultType RequestResultPtr;
    uint8 DemMemDestindex;
#if (DEM_NVRAM_DIVADED == STD_ON)
    /*the NvRAM data should be validated*/
    for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY; iloop++)
    {
        blockRef = DemNvRamBlockId[blockIndex];
        blockIndex++;
        (void)NvM_GetErrorStatus(blockRef, &RequestResultPtr);
        if (RequestResultPtr == NVM_REQ_OK)
        {
            for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
            {
                if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
                {
                    Dem_MemCopy(
                        (uint8*)&DemMemDestCfg[DemMemDestindex].EntryList[iloop],
                        (const uint8*)NvM_BlockDescriptor[blockRef - 1u].NvmRamBlockDataAddress,
                        ENTRY_STORAGE_LEN);
                    tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                    break;
                }
            }
            if ((tempEventId != 0u) && (tempEventId <= DemPbCfgPtr->DemEventNum))
            {
                tempEventId = tempEventId - 1u;
                Dem_CopyInfoFromEntry(
                    &DemEventRelateInformation[tempEventId],
                    &DemMemDestCfg[DemMemDestindex].EntryList[iloop]);
                DemMemDestInfo[DemMemDestindex].RecordNum++;
            }
            else
            {
                DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId = 0u;
            }
        }
        else
        {
            retVal = E_NOT_OK;
        }
    }
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
    for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT; iloop++)
    {
        blockRef = DemNvRamBlockId[blockIndex];
        blockIndex++;
        (void)NvM_GetErrorStatus(blockRef, &RequestResultPtr);
        if (RequestResultPtr == NVM_REQ_OK)
        {
            for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
            {
                if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
                {
                    Dem_MemCopy(
                        (uint8*)&DemMemDestCfg[DemMemDestindex].EntryList[iloop],
                        (const uint8*)NvM_BlockDescriptor[blockRef - 1u].NvmRamBlockDataAddress,
                        ENTRY_STORAGE_LEN);
                    tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                    break;
                }
            }
            if ((tempEventId != 0u) && (tempEventId <= DemPbCfgPtr->DemEventNum))
            {
                tempEventId = tempEventId - 1u;
                Dem_CopyInfoFromEntry(
                    &DemEventRelateInformation[tempEventId],
                    &DemMemDestCfg[DemMemDestindex].EntryList[iloop]);
                DemMemDestInfo[DemMemDestindex].RecordNum++;
            }
            else
            {
                DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId = 0u;
            }
        }
        else
        {
            retVal = E_NOT_OK;
        }
    }
#endif
#else
    /*the NvRAM data should be validated*/
    blockRef = DemNvRamBlockId[blockIndex];
    blockIndex++;
    (void)NvM_GetErrorStatus(blockRef, &RequestResultPtr);
    if (RequestResultPtr == NVM_REQ_OK)
    {
        for (iloop = 0; iloop < DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY; iloop++)
        {
            /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
            for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
            /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
            {
                if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
                {
                    DemMemDestCfg[DemMemDestindex].EntryList[iloop] = DemEventMemoryEntryStorage[iloop];
                    tempEventId = DemMemDestCfg[DemMemDestindex].EntryList[iloop].EventId;
                    break;
                }
            }
            if ((tempEventId != 0u) && (tempEventId <= DemPbCfgPtr->DemEventNum))
            {
                tempEventId = tempEventId - 1u;
                Dem_CopyInfoFromEntry(&DemEventRelateInformation[tempEventId], &DemEventMemoryEntryStorage[iloop]);
                DemMemDestInfo[DemMemDestindex].RecordNum++;
            }
            else
            {
                DemEventMemoryEntryStorage[iloop].EventId = 0u;
            }
        }
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
        for (; iloop < ENTRY_STORAGE_NUM; iloop++)
        {
            for (DemMemDestindex = 0; DemMemDestindex < DEM_MEM_DEST_TOTAL_NUM; DemMemDestindex++)
            {
                if (DemMemDestCfg[DemMemDestindex].ExtId == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
                {
                    DemMemDestCfg[DemMemDestindex].EntryList[iloop - DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY] =
                        DemEventMemoryEntryStorage[iloop];
                    tempEventId =
                        DemMemDestCfg[DemMemDestindex].EntryList[iloop - DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY].EventId;
                    break;
                }
            }
            if ((tempEventId != 0u) && (tempEventId <= DemPbCfgPtr->DemEventNum))
            {
                tempEventId = tempEventId - 1u;
                Dem_CopyInfoFromEntry(&DemEventRelateInformation[tempEventId], &DemEventMemoryEntryStorage[iloop]);
                DemMemDestInfo[DemMemDestindex].RecordNum++;
            }
            else
            {
                DemEventMemoryEntryStorage[iloop].EventId = 0u;
            }
        }
#endif
    }
    else
    {
        retVal = E_NOT_OK;
    }
#endif

    blockRef = DemNvRamBlockId[blockIndex];
    (void)NvM_GetErrorStatus(blockRef, &RequestResultPtr);
    if (RequestResultPtr == NVM_REQ_OK)
    {
        for (iloop = 0; iloop < DemPbCfgPtr->DemEventNum; iloop++)
        {
            DemEventRelateInformation[iloop].UdsStatus = DemEventRelateInformationStorage.UdsStatus[iloop];
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
            DemEventRelateInformation[iloop].StatusIndicator30 =
                DemEventRelateInformationStorage.StatusIndicator30[iloop];
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
            DemEventRelateInformation[iloop].AgedCounter = DemEventRelateInformationStorage.AgedCounter[iloop];
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
            Dem_InterIntReadNvRAM(iloop, &DemEventRelateInformationStorage);
        }
#if (DEM_MAXFDCSINCELASTCLEAR_EANLE == STD_ON)
        for (iloop = 0; iloop < DEM_DTC_NUM; iloop++)
        {
            FDCInfo[iloop].MaxFDCSinceLastClear = DemEventRelateInformationStorage.MaxFDCSinceLastClear[iloop];
        }
#endif
#if (DEM_OPERATION_CYCLE_STATUS_STORAGE == STD_ON) /* req SWS_Dem_00577 */
        for (iloop = 0; iloop < DEM_OPERATION_CYCLE_NUM_BYTE; iloop++)
        {
            DemOperationCycleStatus[iloop] = DemEventRelateInformationStorage.DemOperationCycleStatus[iloop];
        }
#endif
        DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed = DemEventRelateInformationStorage.FirstDtcConfirmed;
        DemDTCByOccurrenceTimeInfo.FirstFailed = DemEventRelateInformationStorage.FirstFailed;
        DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed = DemEventRelateInformationStorage.MostRecDtcConfirmed;
        DemDTCByOccurrenceTimeInfo.MostRecentFailed = DemEventRelateInformationStorage.MostRecentFailed;
    }
    else
    {
        retVal = E_NOT_OK;
    }

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    blockIndex++;
    blockRef = DemNvRamBlockId[blockIndex];
    (void)NvM_GetErrorStatus(blockRef, &RequestResultPtr);
    if (RequestResultPtr == NVM_REQ_OK)
    {
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        IgnUpCycleCounter = DemOBDDataStorage.IgnUpCycleCounter;
#endif
        WarmUpCycleCounter = DemOBDDataStorage.WarmUpCycleCounter;
        OBDDistanceMILOn = DemOBDDataStorage.OBDDistanceMILOn;
        DistSinceDtcCleared = DemOBDDataStorage.DistSinceDtcCleared;
        OBDTimeMILOn = DemOBDDataStorage.OBDTimeMILOn;
        OBDTimeDTCClear = DemOBDDataStorage.OBDTimeDTCClear;
        ContinuousMICounter = DemOBDDataStorage.ContinuousMICounter;
        OBDB1Counter = DemOBDDataStorage.OBDB1Counter;
#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
        MasterContinuousMICounter = DemOBDDataStorage.MasterContinuousMICounter;
#endif
    }
    else
    {
        retVal = E_NOT_OK;
    }
#endif
    return retVal;
}
#endif /* DEM_NVRAM_BLOCKID_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_IntReadNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_MemEntryDelete(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry, uint8 memDest)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    if (pEntry != NULL_PTR)
    {
        const Dem_MemDestConfigType* pMemDestCfg = &DemMemDestCfg[memDest];
        uint8 EntryNum = pMemDestCfg->EntryNum;
        uint8 AbsTime = pEntry->AbsTime;
        Dem_EventMemEntryType* pEntryList = pMemDestCfg->EntryList;

        /*delete the time order and update others*/
        for (uint8 index = 0; index < EntryNum; index++)
        {
            if (AbsTime < pEntryList[index].AbsTime)
            {
                pEntryList[index].AbsTime--;
            }
        }
        pEntry->AbsTime = 0;
        Dem_DeleteRelatedData(pEntry);
        pEntry->EventId = 0x00;
        if (DemMemDestInfo[memDest].RecordNum > 0u)
        {
            DemMemDestInfo[memDest].RecordNum--;
        }
    }
}

#if (DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
/*************************************************************************/
/*
 * Brief               Dem_GetEventPriority
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) Dem_GetEventPriority(Dem_EventIdType IntId)
{
    uint8 res = 0xFF;
    uint16 ref = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;

    if (ref != DEM_DTC_REF_INVALID)
    {
        res = DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[ref].DemDTCAttributesRef].DemDTCPriority;
    }
    return res;
}
#endif /* DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE */

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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8,7 */
FUNC(Std_ReturnType, DEM_CODE)
Dem_CheckEntryAlreadyAllocated(
    Dem_EventIdType IntId,
    uint8 memDest,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) * pEntry)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 ret = E_OK;
    Dem_EventIdType EventId = Dem_GetEventExternalId(IntId);

#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
    uint8 iloop;
    Dem_EventMemEntryType* lEntry = DemMemDestCfg[memDest].EntryList; /* PRQA S 3678 */ /* MISRA Rule 8.13 */
    const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
    uint16 DemDTCRef = pEventParameter[IntId].DemDTCRef;
    for (iloop = 0; (iloop < DemMemDestCfg[memDest].EntryNum) && (*pEntry == NULL_PTR); iloop++)
    {
        Dem_EventIdType EntryIntId = lEntry->EventId;
        Dem_EventIdType EntryEventId = Dem_GetEventInternalId(EntryIntId);
        /*check if event is already stored*/
        if (lEntry->EventId == EventId)
        {
            *pEntry = lEntry;
        }
        else
        {
            if ((EntryIntId != 0u) && (DemDTCRef == pEventParameter[EntryEventId].DemDTCRef))
            {
                *pEntry = lEntry;
                /* req SWS_Dem_01051 */
                if (DEM_FLAGS_ISSET(DemEventRelateInformation[EntryEventId].UdsStatus, DEM_UDS_STATUS_CDTC) != 0x00u)
                {
                    ret = E_NOT_OK;
                }
            }
        }
        lEntry++;
    }
#else
    *pEntry = Dem_MemEntryGet(EventId, memDest);
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE */
    return ret;
}

#if (DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE)
/*************************************************************************/
/*
 * Brief               Dem_DeleteDisplacementEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      InternalId && MemIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) Dem_DeleteDisplacementEntry(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pResEntry,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    VAR(uint8, DEM_APPL_DATA) memDest)
{
    Dem_EventIdType IntId = Dem_GetEventInternalId(pResEntry->EventId);
#if (                                                                                                          \
    (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE) || (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON) \
    || (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT))
    Dem_EventRelateInformationType* NewpEvent = Dem_GetEventInfo(IntId);
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE || DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON \
    || DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT */
       /* SWS_Dem_00409 Reset bit2 bit3 to 0 */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
    const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
    uint16 DTCRef = pEventParameter[IntId].DemDTCRef;
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[DTCRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
    const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
    for (uint16 iloop = 0; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    {
        NewpEvent = Dem_GetEventInfo(*pDTCMapping);
#endif /* DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED */
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
        DEM_FLAGS_CLR(NewpEvent->UdsStatus, (DEM_UDS_STATUS_PDTC | DEM_UDS_STATUS_CDTC));
#endif /* DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON */
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT)
        DEM_FLAGS_CLR(NewpEvent->UdsStatus, DEM_UDS_STATUS_TFSLC);
#endif /* DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT */
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_DISABLED)
        Dem_ResponseOnDtcStatusChange(IntId);
#endif /* DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_DISABLED */
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
        Dem_ResponseOnDtcStatusChange(*pDTCMapping);
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
        pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
    }
#endif /* DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED */

    /* update the combination event dtc status*/
    Dem_UpdateCombinedDtcStatus(IntId);
    Dem_MemEntryDelete(pResEntry, memDest);
}

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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
Dem_EventDisplacementProcess(Dem_EventIdType InternalId, uint8 memDest)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    /* Search entries with lowest priority */
    Dem_EventMemEntryType* pResEntry = NULL_PTR;
    const Dem_MemDestConfigType* pMemDestCfg = &DemMemDestCfg[memDest];
    Dem_EventMemEntryType* pEntry = pMemDestCfg->EntryList;
    uint8 EntryNum = pMemDestCfg->EntryNum;
    uint8 EventNewPriority = Dem_GetEventPriority(InternalId);
    const Dem_EventParameterType* pEventParameter = DemPbCfgPtr->DemEventParameter;
#if (DEM_EVENT_DISPLACEMENT_STRATEGY == DEM_DISPLACEMENT_FULL)
    boolean PassiveFound = FALSE;
#endif
    boolean LowPriorityFound = FALSE;
    const Dem_MemDestInfoType* pMemDestInfo = &DemMemDestInfo[memDest];

    for (uint8 iloop = 0; iloop < EntryNum; iloop++)
    {
        /* find the needed entry in the Entry list */
        uint16 TempEventIntId = Dem_GetEventInternalId(pEntry->EventId);
        uint8 EntryPriority = Dem_GetEventPriority(TempEventIntId);
        uint16 DTCRef = pEventParameter[TempEventIntId].DemDTCRef;
        const Dem_EventRelateInformationType* pEvent = Dem_GetEventInfo(TempEventIntId);
        /* Check CurrentDTC Disable Record Update */
        if (DEM_BITS_ISSET(pMemDestInfo->DisableDTCRecordUpdate, DTCRef))
        {
            pEntry++;
        }
#if (                                                                                     \
    ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) || (DEM_OBD_SUPPORT == DEM_OBD_PRIMARY_ECU)) \
    && (DEM_OBD_EVENT_DISPLACEMENT == STD_ON))
        /*req SWS_Dem_00695 */
        /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
        else if (
            (DTCRef != DEM_DTC_REF_INVALID) && (DemPbCfgPtr->DemDTC[DTCRef].DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS)
            && (
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
                (Dem_GetDemWIRStatus(
                     DemIndicatorAttribute[pEventParameter[TempEventIntId].AttrStartIndex].DemIndicatorRef)
                 != DEM_INDICATOR_OFF)
                ||
#endif /* DEM_INDICATOR_NUM > 0 && DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0 */
                ((pEntry->FFNum != 0u) && (EntryPriority <= EventNewPriority))
                || (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC)))

        )
        /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
        {
            pEntry++;
        }
#endif
        else
        {
#if (DEM_EVENT_DISPLACEMENT_STRATEGY == DEM_DISPLACEMENT_FULL)
            /* req SWS_Dem_00403 */
            if (EventNewPriority <= EntryPriority)
            {
                /*req SWS_Dem_00404 */
                if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ACTIVE))
                {
                    if (PassiveFound == FALSE)
                    {
                        /* The First Passive Mode Entry Found */
                        pResEntry = pEntry;
                        PassiveFound = TRUE;
                    }
                    else
                    {
                        /* req SWS_Dem_00405 Already found  Get The Oldest One */
                        if (pResEntry->AbsTime > pEntry->AbsTime)
                        {
                            pResEntry = pEntry;
                        }
                    }
                }
                /* not found passive */
                if (PassiveFound == FALSE)
                {
#endif
                    if (EventNewPriority < EntryPriority)
                    {
                        /* req SWS_Dem_00403 */
                        if (LowPriorityFound == FALSE)
                        {
                            LowPriorityFound = TRUE;
                            pResEntry = pEntry;
                        }
                        /* req SWS_Dem_00405 Already found Get The Oldest One*/
                        if (pResEntry->AbsTime > pEntry->AbsTime)
                        {
                            pResEntry = pEntry;
                        }
                    }
                    else
                    {
                        /* req SWS_Dem_00692 TestNotCompletedThisOperationCycle bit == 1 */
                        /* req SWS_Dem_00405 Already found  Get The Oldest One*/
                        if ((LowPriorityFound == FALSE)
                            && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                            && ((pResEntry == NULL_PTR) || ((pResEntry->AbsTime > pEntry->AbsTime))))
                        {
                            pResEntry = pEntry;
                        }
                    }
#if (DEM_EVENT_DISPLACEMENT_STRATEGY == DEM_DISPLACEMENT_FULL)
                }
            }
#endif
            pEntry++;
        }
    }
    /*SWS_Dem_00408*/
    if ((pResEntry != NULL_PTR)
        && (pEventParameter[Dem_GetEventInternalId(pResEntry->EventId)].DemDTCRef != DEM_DTC_REF_INVALID))
    {
        Dem_DeleteDisplacementEntry(pResEntry, memDest);
    }
    return pResEntry;
}
#endif /* DEM_EVENT_DISPLACEMENT_STRATEGY != DEM_DISPLACEMENT_NONE */

/*************************************************************************/
/*
 * Brief               Dem_EventAgingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_EventAgingProcess(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent, uint16 OpId)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const uint8* pMemDest;
    Dem_EventMemEntryType* pEntry = NULL_PTR;
    const Dem_DTCAttributesType* pDTCAttrCfg = NULL_PTR;
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[pEvent->IntId];
    uint8 MemDestMaxNumOfDtc = Dem_GetMemDestMaxNumOfDtc();
    uint8 AgingCycleCounterThreshold;
    uint16 DemDTCRef = pEventCfg->DemDTCRef;

    Dem_Pending = TRUE;
    if (DemDTCRef != DEM_DTC_REF_INVALID)
    {
        pDTCAttrCfg = &DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[DemDTCRef].DemDTCAttributesRef];
    }
    if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC))
    {
        if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)) && (pDTCAttrCfg != NULL_PTR))
        {
            /* req SWS_Dem_00698 aging*/
            pMemDest = pDTCAttrCfg->DemMemoryDestinationRef;
            uint8 DemOperationCycleRef = pEventCfg->DemOperationCycleRef;
            for (uint8 iloop = 0; iloop < MemDestMaxNumOfDtc; iloop++)
            {
                if (pMemDest[iloop] != DEM_MEM_DEST_INVALID)
                {
                    pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(pEvent->IntId), pMemDest[iloop]);
                }
                if ((pEntry != NULL_PTR))
                {
                    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
                    if (((pDTCAttrCfg->DemAgingCycleRef == OpId) || (Dem_AgingCycle == OpId)
                         || ((DemPbCfgPtr->DemOperationCycle[DemOperationCycleRef].DemOperationCycleType
                              == DEM_OPCYC_OBD_DCY)
                             && (DEM_BITS_ISNSET(DemOperationCycleStatus, DemOperationCycleRef))
                             && (DemPbCfgPtr->DemOperationCycle[OpId].DemOperationCycleType == DEM_OPCYC_WARMUP)))
                        && (pDTCAttrCfg->DemAgingAllowed == TRUE)
                    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
#if (DEM_AGING_REQUIERES_TESTED_CYCLE == STD_ON)
                        /* req SWS_Dem_00490 req SWS_Dem_00826 */
                        && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
#endif
                    )
                    {
                        /* req SWS_Dem_00985 req SWS_Dem_00494 */
                        if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                        {
                            /* req SWS_Dem_00489 */
                            pEvent->AgingCounter++;
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT)
                            AgingCycleCounterThreshold = (pDTCAttrCfg->DemAgingCycleCounterThreshold
                                                          > pDTCAttrCfg->DemAgingCycleCounterThresholdForTFSLC)
                                                             ? pDTCAttrCfg->DemAgingCycleCounterThreshold
                                                             : pDTCAttrCfg->DemAgingCycleCounterThresholdForTFSLC;
#else
                            AgingCycleCounterThreshold = pDTCAttrCfg->DemAgingCycleCounterThreshold;
#endif
#if (DEM_STATUS_BIT_HANDLING_TEST_FAILED_SINCE_LAST_CLEAR == DEM_STATUS_BIT_AGING_AND_DISPLACEMENT)
                            if (pEvent->AgingCounter >= pDTCAttrCfg->DemAgingCycleCounterThresholdForTFSLC)
                            {
                                /* req SWS_Dem_01054 */
                                DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TFSLC);
                            }
#endif
                            /* req SWS_Dem_00391 */

                            /* SWS_Dem_00391 */
                            if (pEvent->AgingCounter >= pDTCAttrCfg->DemAgingCycleCounterThreshold)
                            {
#if (DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED)
                                /* clear bit3 at next operation cycle end */
                                /* SWS_Dem_00443 */
                                const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[DemDTCRef];
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                                const Dem_EventIdType EventRefNum = pDemDTC->EventRefNum;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                                const Dem_EventIdType EventRefStart = pDemDTC->EventRefStart;
                                const Dem_EventIdType* pDTCMapping = &DemPbCfgPtr->DemDTCMapping[EventRefStart];
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
                                if ((pEvent->AgedCounter < 0xFFu)
                                    && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC)))
                                {
                                    pEvent->AgedCounter++;
                                }
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                                for (uint16 iloop = 0u; iloop < EventRefNum; iloop++)
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                                {
                                    DEM_FLAGS_CLR(
                                        DemEventRelateInformation[*pDTCMapping].UdsStatus,
                                        DEM_UDS_STATUS_CDTC);
#if (DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON)
                                    pDTCMapping++;
#endif /* DEM_ONEDTCMAPPINGMUTILEVENT == STD_ON */
                                }
#endif /* DEM_EVENT_COMBINATION_SUPPORT != DEM_EVCOMB_DISABLED */
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
                                /*bit 3: set to 1 when the DTC is aged*/
                                DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_AGED);
                                /*bit 7: The bit is reset to 0 when DTC is aged*/
                                DEM_FLAGS_CLR(pEvent->StatusIndicator30, DEM_SI30_DTC_TSFLC);
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
                                /* req SWS_Dem_00498 */
                                DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC);
                            }
                            if (pEvent->AgingCounter >= AgingCycleCounterThreshold)
                            {
                                if (pMemDest[iloop] != DEM_MEM_DEST_INVALID)
                                {
                                    /* SWS_Dem_00493 SWS_Dem_01075 */
                                    Dem_MemEntryDelete(pEntry, pMemDest[iloop]);
                                }
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF)
                                for (uint16 index = 0; index < DEM_EVENT_PARAMETER_NUM; index++)
                                {
                                    /* SWS_Dem_00824 */
                                    Dem_CDTCOverFlowCheck(index);
                                }
#endif /* DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_OFF */
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        pEvent->HealingCounter = 0;
        pEvent->AgingCounter = 0;
    }
    Dem_Pending = FALSE;
}

/*************************************************************************/
/*
 * Brief               Dem_EventHealingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OpId
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_EventHealingProcess(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent, uint8 OpId)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[pEvent->IntId];
    if ((0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC)) && (pEventCfg != NULL_PTR))
    {
        if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR))
        {
            /* healing req SWS_Dem_01056 */
            pEvent->AgingCounter = 0;
            uint8 DemOperationCycleRef = pEventCfg->DemOperationCycleRef;
            /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
            if (((DemOperationCycleRef == OpId)
                 || ((DemPbCfgPtr->DemOperationCycle[DemOperationCycleRef].DemOperationCycleType == DEM_OPCYC_OBD_DCY)
                     && (DEM_BITS_ISNSET(DemOperationCycleStatus, DemOperationCycleRef))
                     && (DemPbCfgPtr->DemOperationCycle[OpId].DemOperationCycleType == DEM_OPCYC_WARMUP)))
                && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC)))
            /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
            {
                pEvent->HealingCounter++;
#if ((DEM_INDICATOR_NUM > 0u) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0u))
                if (TRUE == Dem_CheckWIROff(pEvent, pEventCfg))
                {
                    /* SWS_Dem_00533 */
                    DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_WIR);
                }
#endif /* DEM_INDICATOR_NUM > 0u && DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0u */
            }
        }
    }
    else
    {
        pEvent->HealingCounter = 0;
        pEvent->AgingCounter = 0;
    }
}

/*******************************************************************************
**                      Global Function Definitions                          **
*******************************************************************************/

/*************************************************************************/
/*
 * Brief               ClearDTCProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
FUNC(void, DEM_CODE) Dem_ClearDTCProcess(void)
{
    const Dem_ClearDTCInfoType* pClr = &DemClearDTCInfo;
    uint8 memDest = pClr->memDest;

    /* req SWS_Dem_00171 */
    if (memDest != DEM_MEM_DEST_INVALID)
    {
        if (pClr->ClearAllGroup == TRUE)
        {
            Dem_ClearAllDTC(memDest);
        }
#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
        else
        {
#if (DEM_GROUP_OF_DTC_NUM > 0u)
            uint8 DTCGroupIndex = pClr->DTCGroupIndex;
            if (DTCGroupIndex != DEM_GROUP_OF_DTC_INVALID)
            {
                Dem_ClearGroupDTC(memDest, DTCGroupIndex);
            }
            else
#endif /* DEM_GROUP_OF_DTC_NUM > 0u */
            {
                uint16 DTCIndex = pClr->DTCIndex;
                if (DTCIndex != DEM_DTC_REF_INVALID)
                {
                    Dem_ClearOneDTC(memDest, DTCIndex);
                }
            }
        }
#endif /* DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS */
        Dem_Pending = FALSE;
#if (DEM_CLEAR_DTCBEHAVIOR != DEM_CLRRESP_NONVOLATILE_FINISH)
        Dem_ClearDTCLock = DEM_CLEAR_NOT_LOCK;
#endif
    }
}

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
FUNC(uint16, DEM_CODE) Dem_GetDTCIndex(uint32 DTC, Dem_DTCFormatType DTCFormat)
{
    uint16 index;
    uint16 ret = DEM_DTC_REF_INVALID;
    const Dem_DTCType* pDTC = DemPbCfg.DemDTC;
    const DemObdDTCType* pObdDTC = DemPbCfg.DemObdDTC;

    switch (DTCFormat)
    {
    case DEM_DTC_FORMAT_UDS:
        for (index = 0; index < DEM_DTC_NUM; index++)
        {
            if (pDTC[index].DemDtcValue == DTC)
            {
                ret = index;
                break;
            }
        }
        break;
    case DEM_DTC_FORMAT_OBD:
        for (index = 0; index < DEM_DTC_NUM; index++)
        {
            uint16 ObdDTCRef = pDTC->DemObdDTCRef;
            if ((ObdDTCRef != DEM_OBD_DTC_INVALID) && (pObdDTC[ObdDTCRef].DemDtcValue == DTC))
            {
                ret = index;
                break;
            }
            pDTC++;
        }
        break;
    case DEM_DTC_FORMAT_J1939:
        for (index = 0; index < DEM_DTC_NUM; index++)
        {
            uint16 ObdDTCRef = pDTC->DemObdDTCRef;
            if ((ObdDTCRef != DEM_OBD_DTC_INVALID) && (pObdDTC[ObdDTCRef].DemJ1939DTCValue == DTC))
            {
                ret = index;
                break;
            }
            pDTC++;
        }
        break;
    default:
        /*idle*/
        break;
    }
    return ret;
}

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
FUNC(void, DEM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
Dem_Clear(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent, uint8 memDest)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    boolean allowed = FALSE;
    boolean clear = FALSE;
    Dem_EventMemEntryType* pEntry;
    Dem_EventIdType IntId = pEvent->IntId;
    const Dem_EventParameterType* pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    const Dem_CallbackInitMForEType InitMForE = pEventCfg->DemCallbackInitMForE;
    const Dem_CallbackClearEventAllowedType* pClearEventAllowed = pEventCfg->DemCallbackClearEventAllowed;
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_CONST) pWIRAttr;
    uint8 DemIndicatorRef;
    uint8 AttrNum = pEventCfg->AttrNum;
#endif
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
    uint8 OldStatus = pEvent->UdsStatus;
#endif

#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    const Dem_DTCType* pDemDTC = &DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef];
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */

    /*SWS_Dem_00514,SWS_Dem_00515*/
    /* req SWS_Dem_00680 Monitor re-initialization  */
    if (InitMForE != NULL_PTR)
    {
        (void)InitMForE(DEM_INIT_MONITOR_CLEAR);
    }

    if ((pClearEventAllowed != NULL_PTR) && (pClearEventAllowed->ClearEventAllowed != NULL_PTR))
    {
        if (E_OK == pClearEventAllowed->ClearEventAllowed(&allowed))
        {
            if (allowed == TRUE)
            {
                /*SWS_Dem_00385] */
                pEvent->UdsStatus = 0x00;
                DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC);
                /* bit 4 6 the initialized value 0x50 */
            }
            else
            {
                clear = TRUE; /*SWS_Dem_00667]*/
                if (pClearEventAllowed->DemClearEventAllowedBehavior == DEM_ONLY_THIS_CYCLE_AND_READINESS)
                {
                    DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC); /* bit 4 6 reset */
                    DEM_FLAGS_CLR(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TFSLC);   /* bit 1 5 reset */
                }
            }
        }
        /*SWS_Dem_00516] */
        else
        {
            /*SWS_Dem_00385] */
            pEvent->UdsStatus = 0x00;
            DEM_FLAGS_SET(
                pEvent->UdsStatus,
                DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC); /* bit 4 6 the initialized value 0x50 */
        }
    }
    else
    {
        /*SWS_Dem_00385] */
        pEvent->UdsStatus = 0x00; /* All other bits are set to 0 */
        DEM_FLAGS_SET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC); /* bit 4 6 set to 1 */
    }
#if (DEM_STATUSINDICATOR30ENABLE == STD_ON)
    /* All other bits are set to 0 */
    DEM_FLAGS_CLR(
        pEvent->StatusIndicator30,
        DEM_SI30_DTC_UCSLC | DEM_SI30_DTC_SSLC | DEM_SI30_DTC_WIRSLC); /*bit 2,4,5*/

    /*bit 6: The bit is set to 0 when the DTC is not emission related.The bit is set to 1 when the DTC is emission
     * related.*/
    if ((pDemDTC->DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS))
    {
        DEM_FLAGS_SET(pEvent->StatusIndicator30, DEM_SI30_DTC_ER);
    }
    else
    {
        DEM_FLAGS_CLR(pEvent->StatusIndicator30, DEM_SI30_DTC_ER);
    }
#endif /* DEM_STATUSINDICATOR30ENABLE == STD_ON */
    /* update the combination event dtc status*/
    Dem_UpdateCombinedDtcStatus(IntId);
    Dem_ResponseOnDtcStatusChange(IntId);
    if (clear == FALSE)
    {
        uint16 DTCRef = pEventCfg->DemDTCRef;
#if ((DEM_OCC6EANLE == STD_ON) || (DEM_OCC4EANLE == STD_ON))
        DEM_FLAGS_CLR(pEvent->EventStatusFlag, DEM_EVENT_FLAGS_THISOPISFAILED);
#endif /* DEM_OCC6EANLE == STD_ON || DEM_OCC4EANLE == STD_ON */
        /*SWS_Dem_00343]*/
        Dem_DebounceReset(IntId);
        if (DTCRef != DEM_DTC_REF_INVALID)
        {
            FDCInfo[DTCRef].MaxFDCSinceLastClear = 0; /*SWS_Dem_00794]*/
            FDCInfo[DTCRef].MaxFDCDuringCurrentCycle = 0;
        }
        /* SWS_Dem_00763 */
        Dem_ClearDTRInfoByEventId(IntId);
#if ((DEM_INDICATOR_NUM > 0) && (DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0))
        allowed = TRUE;
        for (uint8 kloop = 0; (kloop < AttrNum) && (TRUE == allowed); kloop++)
        {
            pWIRAttr = &DemIndicatorAttribute[pEventCfg->AttrStartIndex + kloop];
            DemIndicatorRef = pWIRAttr->DemIndicatorRef;
            for (uint16 iloop = 0; (iloop < DEM_EVENT_PARAMETER_NUM) && (TRUE == allowed); iloop++)
            {
                const Dem_EventRelateInformationType* EventPtr = &DemEventRelateInformation[iloop];
                P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST)
                pEventCfg2 = &DemPbCfgPtr->DemEventParameter[iloop];
                for (uint8 jloop = 0; (jloop < pEventCfg2->AttrNum) && (TRUE == allowed); jloop++)
                {
                    if ((0x00u != DEM_FLAGS_ISSET(EventPtr->UdsStatus, DEM_UDS_STATUS_WIR))
                        && (DemIndicatorRef
                            == DemIndicatorAttribute[pEventCfg2->AttrStartIndex + jloop].DemIndicatorRef))
                    {
                        allowed = FALSE;
                    }
                }
            }
            if (TRUE == allowed)
            {
                DemWIRStatus[DemIndicatorRef] = DEM_INDICATOR_OFF;
            }
        }
#endif /* DEM_INDICATOR_NUM > 0 && DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0 */
        /*SWS_Dem_00667]*/
        pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(IntId), memDest);
        if (pEntry != NULL_PTR)
        {
            Dem_MemEntryDelete(pEntry, memDest); /*SWS_Dem_00660] */
        }
#if (DEM_TRIGGER_FIM_REPORTS == STD_ON)
        (void)FiM_DemTriggerOnEventStatus(Dem_GetEventExternalId(IntId), OldStatus, pEvent->UdsStatus);
#endif
#if (DEM_AGEDCOUNT_SAIC == STD_ON)
        pEvent->AgedCounter = 0;
#endif /* DEM_AGEDCOUNT_SAIC == STD_ON */
    }
}

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
FUNC(P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
Dem_PreStoreFFGet(Dem_EventIdType ExtId)
{
    uint8 iloop;
    Dem_PreStoreFFInfoType* pPreFF = NULL_PTR;

    for (iloop = 0; (iloop < DEM_MAX_NUMBER_PRESTORED_FF) && (pPreFF == NULL_PTR); iloop++)
    {
        if (DemPreStoreFFInfo[iloop].ExtId == ExtId)
        {
            pPreFF = &DemPreStoreFFInfo[iloop];
        }
    }
    return pPreFF;
}
#endif /* DEM_MAX_NUMBER_PRESTORED_FF > 0 */

/*************************************************************************/
/*
 * Brief               Dem_EventQueueAdd
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId && Status
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/*************************************************************************/
FUNC(Std_ReturnType, DEM_CODE) Dem_EventQueueAdd(Dem_EventIdType ExtId, Dem_EventStatusType Status)
{
    Std_ReturnType ret = E_NOT_OK;
    SchM_Enter_Dem_ExclusiveArea();
    Dem_EventQueueType* pQueue = &DemEventQueue;
    Dem_EventDataBufferType* pBuffer = &(pQueue->Queue[pQueue->WriteIndex]);

    if (pBuffer->ExtId == 0x00u)
    {
        pQueue->WriteIndex++;
        if (pQueue->WriteIndex >= DEM_EVENT_QUEUE_SIZE)
        {
            pQueue->WriteIndex = 0x00;
        }
        SchM_Exit_Dem_ExclusiveArea();
        pBuffer->Status = Status;
        pBuffer->ExtId = ExtId;
        ret = E_OK;
    }
    else
    {
        SchM_Exit_Dem_ExclusiveArea();
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Dem_GetEventInfo
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_EventRelateInformationType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
Dem_GetEventInfo(Dem_EventIdType IntId)
{
    return &DemEventRelateInformation[IntId];
}

/*************************************************************************/
/*
 * Brief               Dem_CheckEventMemEntryExistsAlready
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_EventMemEntryType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
Dem_CheckEventMemEntryExistsAlready(Dem_EventIdType IntId)
{
    Dem_EventMemEntryType* ret = NULL_PTR;
    const uint8* pMemDest;
    uint8 iloop;
    uint16 TempRef = DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef;
    Dem_EventMemEntryType* pEntry;
    uint8 MemDestMaxNumOfDtc = Dem_GetMemDestMaxNumOfDtc();

    if (TempRef != DEM_DTC_REF_INVALID)
    {
        pMemDest =
            DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[TempRef].DemDTCAttributesRef].DemMemoryDestinationRef;
        for (iloop = 0; iloop < MemDestMaxNumOfDtc; iloop++)
        {
            if (pMemDest[iloop] != DEM_MEM_DEST_INVALID)
            {
                pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(IntId), pMemDest[iloop]);
                if (NULL_PTR != pEntry)
                {
                    ret = pEntry;
                }
            }
        }
    }
    return ret;
}

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
FUNC(void, DEM_CODE) Dem_SetOccurrenceEvent(Dem_EventIdType IntId, uint8 Status)
{
    switch (Status)
    {
    case DEM_UDS_STATUS_TF:
        if (DemDTCByOccurrenceTimeInfo.FirstFailed == DEM_EVENT_PARAMETER_INVALID)
        {
            DemDTCByOccurrenceTimeInfo.FirstFailed = IntId;
        }
        DemDTCByOccurrenceTimeInfo.MostRecentFailed = IntId;
        break;
    case DEM_UDS_STATUS_CDTC:
        if (DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed == DEM_EVENT_PARAMETER_INVALID)
        {
            DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed = IntId;
        }
        DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed = IntId;
        break;
    default:
        /*idle*/
        break;
    }
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
