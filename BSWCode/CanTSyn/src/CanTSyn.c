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
 **  @file               : CanTSyn.c
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/13
 **  @vendor             : isoft
 **  @description        : Handles the distribution of time information over CAN buses
 **
 ***********************************************************************************************************************/
/* PRQA S 6060 ++ */             /* VL_MTR_CanTSyn_STM19 */
/* PRQA S 1503, 1532, 2889 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* =================================================== inclusions =================================================== */
#include "CanTSyn_Cfg.h"
#include "IStdLib.h"
#include "Os.h"
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "CanIf.h"
#include "SchM_CanTSyn.h"
#include "CanTSyn_Internal.h"
#include "CanTSyn.h"

/* ===================================================== macros ===================================================== */
#define CANTSYN_DOMAIN(domainIdx) CanTSyn_configPtr->GlobalTimeDomain[domainIdx]

#define CANTSYN_SC_MAX                 ((uint8)15u)
#define CANTSYN_SYNC_FUP_DOMAIN_ID_MAX ((uint8)15u)

#define CANTSYN_STANDARD_MSG_LEN (8u)
#define CANTSYN_EXTENDED_MSG_LEN (16u)
#define CANTSYN_USERDATA_LEN_MAX (3u)

#define CANTSYN_SHIFT_BIT8  (8u)
#define CANTSYN_SHIFT_BIT16 (16u)
#define CANTSYN_SHIFT_BIT24 (24u)
#define CANTSYN_SHIFT_BIT32 (32u)

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_TransmitSYNC
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_TransmitFUP
#endif
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidatePointer
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidateTxMode
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidateCtrlIdx
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidteTxPduId
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidteRxPduId
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidteInitStatus
#endif
CANTSYN_LOCAL boolean CanTSyn_ValidatePointer(uint8 apiId, const void* pointer);
CANTSYN_LOCAL boolean CanTSyn_ValidateTxMode(CanTSyn_TransmissionModeType mode);
CANTSYN_LOCAL boolean CanTSyn_ValidateCtrlIdx(uint8 ctrlIdx);
CANTSYN_LOCAL boolean CanTSyn_ValidteTxPduId(PduIdType txPduId);
CANTSYN_LOCAL boolean CanTSyn_ValidteRxPduId(PduIdType rxPduId);
CANTSYN_LOCAL boolean CanTSyn_ValidteInitStatus(uint8 apiId);
#endif
CANTSYN_LOCAL uint16  CanTSyn_GetPartitionIndex(void);
CANTSYN_LOCAL uint8   CanTSyn_FindTimeDomainIndexByDomainId(uint8 domainId);
CANTSYN_LOCAL boolean CanTSyn_CheckGlobalTimeBaseBit(uint8 domainIdx);
CANTSYN_LOCAL boolean CanTSyn_PassCRCMode(uint8 domainIdx, const PduInfoType* PduInfoPtr);
#if (CANTSYN_CRC_SUPPORT == STD_ON)
CANTSYN_LOCAL uint8 CanTSyn_CalculateCRC(const PduInfoType* PduInfoPtr, CanTSyn_DataIDListElementType element);
#endif
CANTSYN_LOCAL void           CanTSyn_DealExtendedFormat(boolean useExtendedMsgFormat, PduInfoType* pduInfoPtr);
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitSYNC(uint8 domainIdx, PduIdType pduId);
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitFUP(uint8 domainIdx, PduIdType pduId);
CANTSYN_LOCAL void           CanTSyn_RxSYNC(uint8 domainIdx, const PduInfoType* PduInfoPtr);
CANTSYN_LOCAL void           CanTSyn_RxFUP(uint8 domainIdx, const PduInfoType* PduInfoPtr);
CANTSYN_LOCAL void           CanTSyn_RxPduInfoProcess(
              uint8                              domainIdx,
              const PduInfoType*                 PduInfoPtr,
              const CanTSyn_GlobalTimeSlaveType* timeSlavePtr,
              StbM_VirtualLocalTimeType          localTime);
CANTSYN_LOCAL void CanTSyn_MasterSyncRequired(uint8 domainIdx);
CANTSYN_LOCAL void CanTSyn_MasterFupRequired(uint8 domainIdx);
CANTSYN_LOCAL void CanTSyn_MasterSyncNotRequired(uint8 domainIdx);
CANTSYN_LOCAL void CanTSyn_MasterMain(uint8 domainIdx);
CANTSYN_LOCAL void CanTSyn_DecrementCounter(
    uint32*                         counter,
    uint8                           domainIdx,
    const CanTSyn_MasterStatusType* masterStatus,
    const CanTSyn_SlaveStatusType*  slaveStatus);
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left8(uint32 x);
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left16(uint32 x);
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left24(uint32 x);
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Left32(uint64 x);
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Right32(uint64 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_HiHiByte(uint32 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_HiLoByte(uint32 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_LoHiByte(uint32 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_LoLoByte(uint32 x);
CANTSYN_LOCAL_INLINE void   CanTSyn_Det_Report(uint8 apiId, uint8 errorId);
/* ============================================ internal data definitions =========================================== */
/* CanTSyn init status, at first define it as CANTSYN_UNINIT */
#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL CanTSyn_InitStatusType       CanTSyn_InitStatus[CANTSYN_PARTITION_NUMBER];
CANTSYN_LOCAL boolean                      CanTSyn_ImmediateSent[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
CANTSYN_LOCAL uint8                        CanTSyn_SequenceCounters[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
CANTSYN_LOCAL uint8                        CanTSyn_BaseCounterValues[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
CANTSYN_LOCAL CanTSyn_MasterStatusType     CanTSyn_MasterStatus[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
CANTSYN_LOCAL CanTSyn_SlaveStatusType      CanTSyn_SlaveStatus[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
CANTSYN_LOCAL CanTSyn_TransmissionModeType CanTSyn_TransmissionMode[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_PTR
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL const CanTSyn_ConfigType* CanTSyn_configPtr;
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_PTR
#include "CanTSyn_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"

/**
 * The implementation of CanTSyn_Init.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTSyn_STMIF */
void CanTSyn_Init(const CanTSyn_ConfigType* configPtr)
/* PRQA S 6030 -- */
{
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (CanTSyn_GetPartitionIndex() >= CANTSYN_PARTITION_NUMBER)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_INVALID_PARTITION);
    }
    else
#endif
        if (configPtr == NULL_PTR)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_NULL_POINTER);
    }
    else if (CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] == CANTSYN_INITED)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_ALREADY_INITIALIZED);
    }
    else
#endif
    {
        if (CanTSyn_configPtr == NULL_PTR)
        {
            CanTSyn_configPtr = configPtr;
        }

        /* Initialize base update counter and time counters */
        for (uint8 domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
        {
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (GetApplicationID() != CANTSYN_DOMAIN(domainIdx).ApplicationId)
            {
                continue; /* PRQA S 0770 */ /* VL_CanTSyn_0770 */
            }
#endif
            CanTSyn_BaseCounterValues[domainIdx] =
                StbM_GetTimeBaseUpdateCounter(CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId);
            CanTSyn_MasterTimeInfoPtr[domainIdx]->CyclicResumeCounter    = 0u;
            CanTSyn_MasterTimeInfoPtr[domainIdx]->DebounceCounter        = 0u;
            CanTSyn_MasterTimeInfoPtr[domainIdx]->PeriodCounter          = 1u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->ReceptionTimeoutCounter = 0u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->SequenceCounter         = 0u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->FirstlyReceivedSync     = TRUE;
            CanTSyn_MasterStatus[domainIdx]                              = CANTSYN_SYNC_NOT_REQUIRED;
            CanTSyn_SlaveStatus[domainIdx]                               = CANTSYN_SYNC_EXPECTED;
            CanTSyn_TransmissionMode[domainIdx]                          = CANTSYN_TX_ON;
        }
        CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] = CANTSYN_INITED;
    }
}

#if (CANTSYN_VERSIONINFO_API == STD_ON)
/**
 * The implementation of CanTSyn_GetVersionInfo.
 */
void CanTSyn_GetVersionInfo(Std_VersionInfoType* versioninfoPtr)
{
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidatePointer(CANTSYN_SID_GETVERSIONINFO, versioninfoPtr))
#endif
    {
        versioninfoPtr->vendorID         = CANTSYN_VENDOR_ID;
        versioninfoPtr->moduleID         = CANTSYN_MODULE_ID;
        versioninfoPtr->sw_major_version = CANTSYN_SW_MAJOR_VERSION;
        versioninfoPtr->sw_minor_version = CANTSYN_SW_MINOR_VERSION;
        versioninfoPtr->sw_patch_version = CANTSYN_SW_PATCH_VERSION;
    }
}
#endif

/**
 * The implementation of CanTSyn_SetTransmissionMode.
 */
void CanTSyn_SetTransmissionMode(uint8 CtrlIdx, CanTSyn_TransmissionModeType Mode)
{
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidateTxMode(Mode) && CanTSyn_ValidateCtrlIdx(CtrlIdx)
        && CanTSyn_ValidteInitStatus(CANTSYN_SID_SETTRANSMISSIONMODE))
#endif
    {
        CanTSyn_TransmissionMode[CtrlIdx] = Mode;
    }
}

/**
 * The implementation of CanTSyn_MainFunction.
 */
void CanTSyn_MainFunction(void)
{
    /* look through each time domain */
    for (uint8 domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
    {
        const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (GetApplicationID() != timeDomainPtr->ApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_CanTSyn_0770 */
        }
#endif
        if (CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] == CANTSYN_INITED)
        {
            /* check whether configured as a time master */
            if (timeDomainPtr->GlobalTimeMaster != NULL_PTR)
            {
                /* deal with functionality as a time master */
                CanTSyn_MasterMain(domainIdx);
            }
            /* check whether configured as a time slave */
            if (timeDomainPtr->GlobalTimeSlave != NULL_PTR)
            {
                /* reception timeout counter operation */
                if (CanTSyn_SlaveTimeInfoPtr[domainIdx]->ReceptionTimeoutCounter != 0UL)
                {
                    CanTSyn_SlaveStatusType slaveStatus = CANTSYN_SYNC_EXPECTED;
                    CanTSyn_DecrementCounter(
                        &CanTSyn_SlaveTimeInfoPtr[domainIdx]->ReceptionTimeoutCounter,
                        domainIdx,
                        NULL_PTR,
                        &slaveStatus);
                }
            }
        }
    }
}

#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

/**
 * The implementation of CanTSyn_TxConfirmation.
 */
void CanTSyn_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidteTxPduId(TxPduId) && CanTSyn_ValidteInitStatus(CANTSYN_SID_TXCONFIRMATION))
#endif
    {
        StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(TxPduId).SynchronizedTimeBaseId;
        StbM_VirtualLocalTimeType     localTime;

        SchM_Enter_CanTSyn_Context();
        Std_ReturnType tvValid = StbM_GetCurrentVirtualLocalTime(timeBaseId, &localTime);
        SchM_Exit_CanTSyn_Context();

        const CanTSyn_GlobalTimeMasterType* timeMasterPtr     = CANTSYN_DOMAIN(TxPduId).GlobalTimeMaster;
        CanTSyn_MasterTimeInfoType*         timeMasterInfoPtr = CanTSyn_MasterTimeInfoPtr[TxPduId];

        if (result == E_OK)
        {
            if ((CanTSyn_MasterStatus[TxPduId] == CANTSYN_SYNC_SENT) && (tvValid == E_OK))
            {
                uint64 T1VLT = localTime.nanosecondsHi;
                T1VLT        = CanTSyn_Shift_Left32(T1VLT);
                T1VLT += localTime.nanosecondsLo;
                uint64 T4             = timeMasterInfoPtr->T0SYNCns + (T1VLT - timeMasterInfoPtr->T0VLT);
                timeMasterInfoPtr->T4 = T4;

                /* Start period counter */
                timeMasterInfoPtr->PeriodCounter = timeMasterPtr->GlobalTimeTxPeriod;

                /* check if immediate sent, if yes, start cyclic msg resume counter, stop period counter */
                if (CanTSyn_ImmediateSent[TxPduId])
                {
                    timeMasterInfoPtr->CyclicResumeCounter = timeMasterPtr->CyclicMsgResumeTime;
                    CanTSyn_ImmediateSent[TxPduId]         = FALSE;
                    timeMasterInfoPtr->PeriodCounter       = 0u;
                }

                CanTSyn_MasterStatus[TxPduId] = CANTSYN_FUP_REQUIRED;
                /* Start debounce counter */
                timeMasterInfoPtr->DebounceCounter = timeMasterPtr->GlobalTimeDebounceTime;
            }
            else if (CanTSyn_MasterStatus[TxPduId] != CANTSYN_SYNC_SENT)
            {
                tvValid = E_OK;
                /* increment sequence counter */
                CanTSyn_SequenceCounters[TxPduId] = (CanTSyn_SequenceCounters[TxPduId] + 1u) % (CANTSYN_SC_MAX + 1u);
            }
            else
            {
                /* do nothing */
            }
        }

        if ((result != E_OK) || (tvValid != E_OK))
        {
            CanTSyn_MasterStatus[TxPduId]    = CANTSYN_SYNC_NOT_REQUIRED;
            timeMasterInfoPtr->PeriodCounter = timeMasterPtr->GlobalTimeTxPeriod;
        }
    }
}

/**
 * The implementation of CanTSyn_RxIndication.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTSyn_STCAL */
void CanTSyn_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6070 -- */
{
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidatePointer(CANTSYN_SID_RXINDICATION, PduInfoPtr) && CanTSyn_ValidteRxPduId(RxPduId)
        && CanTSyn_ValidteInitStatus(CANTSYN_SID_RXINDICATION))
#endif
    {
        StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(RxPduId).SynchronizedTimeBaseId;
        StbM_VirtualLocalTimeType     localTime;

        SchM_Enter_CanTSyn_Context();
        Std_ReturnType result = StbM_GetCurrentVirtualLocalTime(timeBaseId, &localTime);
        SchM_Exit_CanTSyn_Context();

        uint8 domainId  = PduInfoPtr->SduDataPtr[2] >> 4u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        uint8 domainIdx = CanTSyn_FindTimeDomainIndexByDomainId(domainId);

        if (domainIdx != RxPduId)
        {
            return;
        }

        uint8 type = PduInfoPtr->SduDataPtr[0];

        if ((result == E_OK) || (type == CANTSYN_FUP_NOT_CRC_TYPE) || (type == CANTSYN_FUP_CRC_TYPE))
        {
            CanTSyn_RxPduInfoProcess(domainIdx, PduInfoPtr, CANTSYN_DOMAIN(domainIdx).GlobalTimeSlave, localTime);
        }
    }
}

#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Development error validation of null pointer
 * @param[in]    apiId   ID of API service in which error is detected
 * @param[in]    pointer pointer
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-77097
 */
CANTSYN_LOCAL boolean CanTSyn_ValidatePointer(uint8 apiId, const void* pointer)
{
    if (pointer == NULL_PTR)
    {
        CanTSyn_Det_Report(apiId, CANTSYN_E_NULL_POINTER);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Development error validation of Mode
 * @param[in]    mode    Transmit mode
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-77096
 */
CANTSYN_LOCAL boolean CanTSyn_ValidateTxMode(CanTSyn_TransmissionModeType mode)
{
    if ((mode != CANTSYN_TX_OFF) && (mode != CANTSYN_TX_ON))
    {
        CanTSyn_Det_Report(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_PARAM);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Development error validation of ctrlIdx
 * @param[in]    ctrlIdx Index of the CAN channel
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-77095
 */
CANTSYN_LOCAL boolean CanTSyn_ValidateCtrlIdx(uint8 ctrlIdx)
{
    if (ctrlIdx >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER)
    {
        CanTSyn_Det_Report(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_INV_CTRL_IDX);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief       Check if the txPduId of the CanTSyn_TxConfirmation is valid.
 * @param[in]   txPduId  ID of the PDU that has been transmitted
 * @return      boolean
 * @retval      TRUE   Development error not occurred
 * @retval      FALSE  Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-77094
 */
CANTSYN_LOCAL boolean CanTSyn_ValidteTxPduId(PduIdType txPduId)
{
    if ((txPduId >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER) || (CANTSYN_DOMAIN(txPduId).GlobalTimeMaster == NULL_PTR))
    {
        CanTSyn_Det_Report(CANTSYN_SID_TXCONFIRMATION, CANTSYN_E_INVALID_PDUID);
        return FALSE;
    }

#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (GetApplicationID() != CANTSYN_DOMAIN(txPduId).ApplicationId)
    {
        CanTSyn_Det_Report(CANTSYN_SID_TXCONFIRMATION, CANTSYN_E_INVALID_PDUID);
        return FALSE;
    }
#endif

    return TRUE;
}

/**
 * @brief       Check if the rxPduId of the CanTSyn_RxIndication is valid.
 * @param[in]   rxPduId  ID of the PDU that has been received
 * @return      boolean
 * @retval      TRUE   Development error not occurred
 * @retval      FALSE  Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-77093
 */
CANTSYN_LOCAL boolean CanTSyn_ValidteRxPduId(PduIdType rxPduId)
{
    if ((rxPduId >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER) || (CANTSYN_DOMAIN(rxPduId).GlobalTimeSlave == NULL_PTR))
    {
        CanTSyn_Det_Report(CANTSYN_SID_RXINDICATION, CANTSYN_E_INVALID_PDUID);
        return FALSE;
    }

#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (GetApplicationID() != CANTSYN_DOMAIN(rxPduId).ApplicationId)
    {
        CanTSyn_Det_Report(CANTSYN_SID_RXINDICATION, CANTSYN_E_INVALID_PDUID);
        return FALSE;
    }
#endif

    return TRUE;
}

/**
 * @brief       Check whether the CanTSyn of the current partition have been initialized.
 * @param[in]   apiId  ID of API service
 * @return      boolean
 * @retval      TRUE   Development error not occurred
 * @retval      FALSE  Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-77092
 */
CANTSYN_LOCAL boolean CanTSyn_ValidteInitStatus(uint8 apiId)
{
    if (CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] == CANTSYN_UNINIT)
    {
        CanTSyn_Det_Report(apiId, CANTSYN_E_UNINIT);
        return FALSE;
    }
    return TRUE;
}
#endif

/**
 * @brief       Returns the index of current partition in the CanTSyn configuration.
 * @return      The index of current partition in the CanTSyn configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75630
 */
CANTSYN_LOCAL uint16 CanTSyn_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType applicationId = GetApplicationID();
    for (; index < CANTSYN_PARTITION_NUMBER; ++index)
    {
        if (CanTSyn_OsApplicationList[index] == applicationId)
        {
            break;
        }
    }
#endif
    return index;
}

/**
 * @brief       Find the timedomain index by the dommain ID.
 * @param[in]   domainId  the ID of global time domain
 * @return      uint8
 * @retval      The index of the domain in the CanTSyn configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75631
 */
CANTSYN_LOCAL uint8 CanTSyn_FindTimeDomainIndexByDomainId(uint8 domainId)
{
    uint8 domainIdx = 0u;

    for (; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
    {
        if (CANTSYN_DOMAIN(domainIdx).GlobalTimeDomainId == domainId)
        {
            break;
        }
    }

    return domainIdx;
}

/**
 * @brief       Check if the GLOBAL_TIME_BASE bit is set.
 * @param[in]   domainIdx  the index of global time domain
 * @return      TRUE: the GLOBAL_TIME_BASE bit is set
 *              FALSE: the GLOBAL_TIME_BASE bit is not set
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75632
 */
CANTSYN_LOCAL boolean CanTSyn_CheckGlobalTimeBaseBit(uint8 domainIdx)
{
    boolean                 result = TRUE;
    StbM_TimeBaseStatusType timeBaseStatus;

    /* get current time base status */
    Std_ReturnType ret = StbM_GetTimeBaseStatus(CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId, &timeBaseStatus);
    if (ret == E_OK)
    {
        /* check if it is a synchronized time base */
        if (CANTSYN_DOMAIN(domainIdx).GlobalTimeDomainId <= CANTSYN_SYNC_FUP_DOMAIN_ID_MAX)
        {
            if ((timeBaseStatus & CANTSYN_GLOBAL_TIME_BASE_MASK) == 0u)
            {
                result = FALSE;
            }
        }
        else /* it is an offset time base */
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/**
 * @brief       Check whether pass CRC validation mode or not.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   PduInfoPtr the pdu info pointer
 * @return      boolean
 * @retval      TURE       passed CRC validation mode
 * @retval      FALSE      not passed CRC validation mode
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75633
 */
CANTSYN_LOCAL boolean CanTSyn_PassCRCMode(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    const CanTSyn_GlobalTimeDomainType* globalTimeDomain = &CANTSYN_DOMAIN(domainIdx);
    uint8                               type             = PduInfoPtr->SduDataPtr[0];
    CrcValidatedType                    crcMode          = globalTimeDomain->GlobalTimeSlave->RxCrcValidated;
    boolean                             result           = TRUE;

    /* check whether it is a CRC message */
    if ((type == CANTSYN_SYNC_CRC_TYPE) || (type == CANTSYN_FUP_CRC_TYPE))
    {
        if (crcMode == CRC_NOT_VALIDATED)
        {
            result = FALSE;
        }
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        else if ((crcMode == CRC_OPTIONAL) || (crcMode == CRC_VALIDATED)) /* need to validate CRC */
        {
            uint8 sequenceCounter = PduInfoPtr->SduDataPtr[2] & CANTSYN_SEQUENCE_COUNTER_BIT_MASK;
            CanTSyn_DataIDListElementType element;
            switch (type)
            {
            case CANTSYN_SYNC_CRC_TYPE:
            {
                element = globalTimeDomain->GlobalTimeSyncDataIDList[sequenceCounter];
                break;
            }
            case CANTSYN_FUP_CRC_TYPE:
            {
                element = globalTimeDomain->GlobalTimeFupDataIDList[sequenceCounter];
                break;
            }
            default:
                /* do nothing */
                break;
            }
            uint8 CRC = CanTSyn_CalculateCRC(PduInfoPtr, element);
            if (CRC != PduInfoPtr->SduDataPtr[1])
            {
                result = FALSE;
            }
        }
#endif
        else
        {
            /* do nothing */
        }
    }
    else
    {
        if (crcMode == CRC_VALIDATED)
        {
            result = FALSE;
        }
    }
    return result;
}

#if (CANTSYN_CRC_SUPPORT == STD_ON)
/**
 * @brief       calculate CRC for a specific message if CRC is supported.
 * @param[in]   PduInfoPtr the pdu info
 * @param[in]   element    the DataID list element
 * @return      uint8
 * @retval      CRC result
 * @reentrant   FALSE(not reentrant for hardware CRC, reentrant for software CRC)
 * @synchronous TRUE
 * @trace       CPD-75634
 */
CANTSYN_LOCAL uint8 CanTSyn_CalculateCRC(const PduInfoType* PduInfoPtr, CanTSyn_DataIDListElementType element)
{
    uint8 data[15];
    (void)IStdLib_MemCpy(data, &PduInfoPtr->SduDataPtr[2], (uint32)PduInfoPtr->SduLength - 2u);
    data[PduInfoPtr->SduLength - 2u] = element.GlobalTimeDataIDListValue;

    uint32 crcLength = (PduInfoPtr->SduLength == CANTSYN_EXTENDED_MSG_LEN) ? (CANTSYN_EXTENDED_MSG_LEN - 1u)
                                                                           : (CANTSYN_STANDARD_MSG_LEN - 1u);
    uint8  result    = Crc_CalculateCRC8H2F(data, crcLength, CANTSYN_CRC_DEFAULT_VALUE, TRUE);
    return result;
}
#endif

/**
 * @brief       Deal with extended message format for SYNC/FUP.
 * @param[in]   useExtendedMsgFormat whether to use can FD or not
 * @param[in]   pduInfoPtr           the pdu info ptr
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75635
 */
CANTSYN_LOCAL void CanTSyn_DealExtendedFormat(boolean useExtendedMsgFormat, PduInfoType* pduInfoPtr)
{
    if (useExtendedMsgFormat)
    {
        pduInfoPtr->SduLength = CANTSYN_EXTENDED_MSG_LEN;
        for (uint8 extendedIdx = CANTSYN_STANDARD_MSG_LEN; extendedIdx < CANTSYN_EXTENDED_MSG_LEN; extendedIdx++)
        {
            pduInfoPtr->SduDataPtr[extendedIdx] = 0u;
        }
    }
    else
    {
        pduInfoPtr->SduLength = CANTSYN_STANDARD_MSG_LEN;
    }
}

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/**
 * @brief       Transmit a SYNC message as a time master.
 * @param[in]   domainIdx the index of global time domain
 * @param[in]   pduId     the pdu id
 * @return      Std_ReturnType
 * @retval      E_OK      Sent successfully
 * @retval      E_NOT_OK  Send failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75636
 */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTSyn_STCAL */
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitSYNC(uint8 domainIdx, PduIdType pduId)
/* PRQA S 6070 -- */
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    StbM_TimeTupleType                  timeTuple;
    StbM_UserDataType                   userData;
    uint8                               userByte[CANTSYN_USERDATA_LEN_MAX] = {0};
    uint8                               sduData[CANTSYN_EXTENDED_MSG_LEN]  = {0};
    PduInfoType                         pduInfo;
    uint8                               seq = CanTSyn_SequenceCounters[domainIdx];

    Std_ReturnType result = StbM_GetCurrentTime(timeDomainPtr->SynchronizedTimeBaseId, &timeTuple, &userData);
    if (result == E_OK)
    {
        /* extract userdata based on given length */
        if (userData.userDataLength >= 1u)
        {
            userByte[0] = userData.userByte0;
        }
        if (userData.userDataLength >= 2u)
        {
            userByte[1] = userData.userByte1;
        }
        if (userData.userDataLength == 3u)
        {
            userByte[2] = userData.userByte2;
        }

        /* construct pdu */
        sduData[0] = CANTSYN_SYNC_NOT_CRC_TYPE;
        sduData[1] = userByte[1];
        sduData[2] = (uint8)(timeDomainPtr->GlobalTimeDomainId << 4u) + seq;
        sduData[3] = userByte[0];

        /* retrieve time info */
        uint32 T0SYNC = timeTuple.globalTime.seconds;

        sduData[4] = CanTSyn_HiHiByte(T0SYNC);
        sduData[5] = CanTSyn_HiLoByte(T0SYNC);
        sduData[6] = CanTSyn_LoHiByte(T0SYNC);
        sduData[7] = CanTSyn_LoLoByte(T0SYNC);

        pduInfo.SduDataPtr = sduData;

        CanTSyn_MasterTimeInfoType* masterTimeInfoPtr = CanTSyn_MasterTimeInfoPtr[domainIdx];
        /* store extra time information */
        masterTimeInfoPtr->T0SYNCns = timeTuple.globalTime.nanoseconds;
        masterTimeInfoPtr->T0VLT    = timeTuple.virtualLocalTime.nanosecondsHi;
        masterTimeInfoPtr->T0VLT    = CanTSyn_Shift_Left32(masterTimeInfoPtr->T0VLT);
        masterTimeInfoPtr->T0VLT += timeTuple.virtualLocalTime.nanosecondsLo;
        masterTimeInfoPtr->UserByte2      = userByte[2];
        masterTimeInfoPtr->TimeBaseStatus = timeTuple.timeBaseStatus;

        /* deal with extented format */
        CanTSyn_DealExtendedFormat(timeDomainPtr->UseExtendedMsgFormat, &pduInfo);
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        if (timeDomainPtr->GlobalTimeMaster->GlobalTimeTxCrcSecured == CRC_SUPPORTED)
        {
            CanTSyn_DataIDListElementType dataIdElement;
            dataIdElement         = timeDomainPtr->GlobalTimeSyncDataIDList[seq];
            pduInfo.SduDataPtr[0] = CANTSYN_SYNC_CRC_TYPE;
            pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(&pduInfo, dataIdElement);
        }
#endif
        result = CanIf_Transmit(pduId, &pduInfo);
    }

    return result;
}
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/**
 * @brief       Transmit a FUP message as a time master.
 * @param[in]   domainIdx the index of global time domain
 * @param[in]   pduId     the pdu id
 * @return      Std_ReturnType
 * @retval      E_OK      Sent successfully
 * @retval      E_NOT_OK  Send failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75637
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitFUP(uint8 domainIdx, PduIdType pduId)
{
    uint64                              T4             = CanTSyn_MasterTimeInfoPtr[domainIdx]->T4;
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr  = &CANTSYN_DOMAIN(domainIdx);
    uint8                               timeDomainId   = timeDomainPtr->GlobalTimeDomainId;
    StbM_TimeBaseStatusType             timeBaseStatus = CanTSyn_MasterTimeInfoPtr[domainIdx]->TimeBaseStatus;
    uint8                               sduData[CANTSYN_EXTENDED_MSG_LEN] = {0};
    uint8                               seq                               = CanTSyn_SequenceCounters[domainIdx];

    /* write overflow part to OVS */
    uint8 OVS = (uint8)(T4 / CANTSYN_1NS_SECOND);
    T4 -= OVS * CANTSYN_1NS_SECOND;

    /* Calculate SGW */
    if ((timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) == 0u) /* sync to global time master */
    {
        sduData[3] = 0x00u;
    }
    else /* sync to sub domain */
    {
        sduData[3] = 0x04u;
    }

    /* construct pdu */
    sduData[0] = CANTSYN_FUP_NOT_CRC_TYPE;
    sduData[1] = CanTSyn_MasterTimeInfoPtr[domainIdx]->UserByte2;
    sduData[2] = (uint8)(timeDomainId << 4u) + seq;
    sduData[3] += OVS;
    sduData[4] = CanTSyn_HiHiByte((uint32)T4);
    sduData[5] = CanTSyn_HiLoByte((uint32)T4);
    sduData[6] = CanTSyn_LoHiByte((uint32)T4);
    sduData[7] = CanTSyn_LoLoByte((uint32)T4);

    PduInfoType pduInfo;
    pduInfo.SduDataPtr = sduData;
    /* deal with extented format */
    CanTSyn_DealExtendedFormat(timeDomainPtr->UseExtendedMsgFormat, &pduInfo);

    /* deal with CRC format */
#if (CANTSYN_CRC_SUPPORT == STD_ON)
    if (timeDomainPtr->GlobalTimeMaster->GlobalTimeTxCrcSecured == CRC_SUPPORTED)
    {
        CanTSyn_DataIDListElementType dataIdElement;
        dataIdElement         = timeDomainPtr->GlobalTimeFupDataIDList[seq];
        pduInfo.SduDataPtr[0] = CANTSYN_FUP_CRC_TYPE;
        pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(&pduInfo, dataIdElement);
    }
#endif
    return CanIf_Transmit(pduId, &pduInfo);
}
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/**
 * @brief       Receive a SYNC message as a time slave.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   PduInfoPtr the pdu info pointer
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75652
 */
CANTSYN_LOCAL void CanTSyn_RxSYNC(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    StbM_TimeBaseStatusType             timeBaseStatus;
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    StbM_SynchronizedTimeBaseType       timeBaseId    = timeDomainPtr->SynchronizedTimeBaseId;
    uint8                               type          = PduInfoPtr->SduDataPtr[0];
    uint8                      maxJumpWidth     = timeDomainPtr->GlobalTimeSlave->GlobalTimeSequenceCounterJumpWidth;
    CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    Std_ReturnType             result           = E_OK;
    uint8                      sequenceCounter  = (PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK;

    if ((maxJumpWidth > 0u) && (!slaveTimeInfoPtr->FirstlyReceivedSync))
    {
        /* Check whether sequence counter diffrence goes over maxJumpWidth if this message is not firstly received
        and Sequence Counter Jump Width Check enabled*/
        if (StbM_GetTimeBaseStatus(timeBaseId, &timeBaseStatus) != E_OK)
        {
            result = E_NOT_OK;
        }
        else if (
            ((timeBaseStatus & CANTSYN_TIMEOUT_MASK) == 0u)
            && ((sequenceCounter == slaveTimeInfoPtr->SequenceCounter)
                || ((sequenceCounter > slaveTimeInfoPtr->SequenceCounter)
                    && ((sequenceCounter - slaveTimeInfoPtr->SequenceCounter) > maxJumpWidth))
                || ((sequenceCounter < slaveTimeInfoPtr->SequenceCounter)
                    && ((CANTSYN_SEQUENCE_COUNTER_MAX_VALUE - slaveTimeInfoPtr->SequenceCounter + sequenceCounter + 1u)
                        > maxJumpWidth))))
        {
            result = E_NOT_OK;
        }
        else
        {
            /* do nothing */
        }
    }

    if (result == E_OK)
    {
        slaveTimeInfoPtr->FirstlyReceivedSync = FALSE;
        CanTSyn_SlaveStatus[domainIdx]        = CANTSYN_FUP_EXPECTED;
        /* extract T0SYNC from pdu info */
        uint32 T0SYNC = PduInfoPtr->SduDataPtr[4];
        T0SYNC        = CanTSyn_Shift_Left24(T0SYNC);
        uint32 temp   = PduInfoPtr->SduDataPtr[5];
        temp          = CanTSyn_Shift_Left16(temp);
        T0SYNC += temp;
        temp = PduInfoPtr->SduDataPtr[6];
        temp = CanTSyn_Shift_Left8(temp);
        T0SYNC += temp;
        T0SYNC += PduInfoPtr->SduDataPtr[7];

        /* store extra information for follow up messages */
        slaveTimeInfoPtr->T0SYNC          = T0SYNC;
        slaveTimeInfoPtr->UserByte0       = PduInfoPtr->SduDataPtr[3];
        slaveTimeInfoPtr->SequenceCounter = sequenceCounter;

        /* deal with CRC format */
        if (type == CANTSYN_SYNC_NOT_CRC_TYPE)
        {
            slaveTimeInfoPtr->UserByte1 = PduInfoPtr->SduDataPtr[1];
            slaveTimeInfoPtr->IsCRC     = FALSE;
        }
        else
        {
            slaveTimeInfoPtr->IsCRC = TRUE;
        }
    }
}
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/**
 * @brief       Receive a FUP message as a time slave.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   PduInfoPtr the pdu info pointer
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75654
 */
CANTSYN_LOCAL void CanTSyn_RxFUP(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    StbM_SynchronizedTimeBaseType timeBaseId       = CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId;
    CanTSyn_SlaveTimeInfoType*    slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];

    /* check if SC matches */
    if (((PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK) == slaveTimeInfoPtr->SequenceCounter)
    {
        StbM_TimeTupleType timeTuple;
        StbM_UserDataType  userData;
        Std_ReturnType     result = StbM_GetCurrentTime(timeBaseId, &timeTuple, &userData);
        if (result == E_OK)
        {
            /* extract information from pdu info */
            uint32 SyncTimeNSec = PduInfoPtr->SduDataPtr[4];
            SyncTimeNSec        = CanTSyn_Shift_Left24(SyncTimeNSec);
            uint32 temp         = PduInfoPtr->SduDataPtr[5];
            temp                = CanTSyn_Shift_Left16(temp);
            SyncTimeNSec += temp;
            temp = PduInfoPtr->SduDataPtr[6];
            temp = CanTSyn_Shift_Left8(temp);
            SyncTimeNSec += temp;
            SyncTimeNSec += PduInfoPtr->SduDataPtr[7];
            uint8 OVS = (PduInfoPtr->SduDataPtr[3] & CANTSYN_OVS_BIT_MASK);

            if ((PduInfoPtr->SduDataPtr[3] & CANTSYN_SGW_BIT_MASK) == 0x00u)
            {
                timeTuple.timeBaseStatus &= (uint8)~STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
            }
            else
            {
                timeTuple.timeBaseStatus |= STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
            }

            userData.userByte0      = slaveTimeInfoPtr->UserByte0;
            userData.userDataLength = 1u;

            if (!slaveTimeInfoPtr->IsCRC)
            {
                userData.userByte1 = slaveTimeInfoPtr->UserByte1;
                userData.userDataLength++;
            }

            /* deal with CRC format */
            uint8 type = PduInfoPtr->SduDataPtr[0];
            if (type == CANTSYN_FUP_NOT_CRC_TYPE)
            {
                userData.userByte2 = PduInfoPtr->SduDataPtr[1];
                userData.userDataLength++;
            }

            timeTuple.globalTime.seconds             = OVS + slaveTimeInfoPtr->T0SYNC;
            timeTuple.globalTime.nanoseconds         = SyncTimeNSec;
            timeTuple.virtualLocalTime.nanosecondsLo = (uint32)slaveTimeInfoPtr->T2VLT;
            timeTuple.virtualLocalTime.nanosecondsHi = (uint32)CanTSyn_Shift_Right32(slaveTimeInfoPtr->T2VLT);

            /* set new global time */
            StbM_MeasurementType measurmentData;
            measurmentData.pathDelay = 0u;
            (void)StbM_BusSetGlobalTime(timeBaseId, &timeTuple, &userData, &measurmentData);
        }
    }
    else
    {
        if (slaveTimeInfoPtr->SequenceCounter == 0u)
        {
            slaveTimeInfoPtr->SequenceCounter = CANTSYN_SC_MAX;
        }
        else
        {
            slaveTimeInfoPtr->SequenceCounter--;
        }
    }
}
/* PRQA S 3120 -- */

/**
 * @brief        Process the received Pdu data packets.
 * @param[in]    domainIdx    The index of global time domain
 * @param[in]    PduInfoPtr   Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 *               containing   The PDU, and the MetaData related to this PDU.
 * @param[in]    timeSlavePtr The time salve of the global time domain refered by domainIdx.
 * @param[in]    localTime    The current virtual local time
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-75655
 */
CANTSYN_LOCAL void CanTSyn_RxPduInfoProcess(
    uint8                              domainIdx,
    const PduInfoType*                 PduInfoPtr,
    const CanTSyn_GlobalTimeSlaveType* timeSlavePtr,
    StbM_VirtualLocalTimeType          localTime)
{
    uint8 type = PduInfoPtr->SduDataPtr[0];

    if (CanTSyn_PassCRCMode(domainIdx, PduInfoPtr))
    {
        switch (type)
        {
        case CANTSYN_SYNC_NOT_CRC_TYPE:
        case CANTSYN_SYNC_CRC_TYPE:
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->ReceptionTimeoutCounter = timeSlavePtr->GlobalTimeFollowUpTimeout;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->T2VLT                   = localTime.nanosecondsHi;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->T2VLT =
                CanTSyn_Shift_Left32(CanTSyn_SlaveTimeInfoPtr[domainIdx]->T2VLT);
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->T2VLT += localTime.nanosecondsLo;
            CanTSyn_RxSYNC(domainIdx, PduInfoPtr);
            break;
        case CANTSYN_FUP_NOT_CRC_TYPE:
        case CANTSYN_FUP_CRC_TYPE:
            if (CanTSyn_SlaveStatus[domainIdx] == CANTSYN_FUP_EXPECTED)
            {
                CanTSyn_SlaveTimeInfoPtr[domainIdx]->ReceptionTimeoutCounter = 0u;
                CanTSyn_SlaveStatus[domainIdx]                               = CANTSYN_SYNC_EXPECTED;
                CanTSyn_RxFUP(domainIdx, PduInfoPtr);
            }
            break;
        default:
            /* do nothing */
            break;
        }
    }
}

/**
 * @brief       Handle the situation where Master is in CANTSYN_SYNC_REQUIRED state.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75656
 */
CANTSYN_LOCAL void CanTSyn_MasterSyncRequired(uint8 domainIdx)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = timeDomainPtr->GlobalTimeMaster;

    if (CanTSyn_TransmissionMode[domainIdx] == CANTSYN_TX_ON)
    {
        /* Start trasnmitting */
        if ((timeDomainPtr->GlobalTimeDomainId <= CANTSYN_SYNC_FUP_DOMAIN_ID_MAX)
            && (CanTSyn_TransmitSYNC(domainIdx, timeMasterPtr->GlobalTimePduId) == E_OK))
        {
            CanTSyn_MasterStatus[domainIdx] = CANTSYN_SYNC_SENT;
        }
    }
}

/**
 * @brief       Handle the situation where Master is in CANTSYN_FUP_REQUIRED state.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75657
 */
CANTSYN_LOCAL void CanTSyn_MasterFupRequired(uint8 domainIdx)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = timeDomainPtr->GlobalTimeMaster;
    CanTSyn_MasterTimeInfoType*         timeInfoPtr   = CanTSyn_MasterTimeInfoPtr[domainIdx];

    if (timeInfoPtr->DebounceCounter > CANTSYN_MAIN_FUNCTION_PERIOD)
    {
        timeInfoPtr->DebounceCounter -= CANTSYN_MAIN_FUNCTION_PERIOD;
    }
    else
    {
        if (CanTSyn_TransmissionMode[domainIdx] == CANTSYN_TX_ON)
        {
            /* Start Transimitting */
            if ((timeDomainPtr->GlobalTimeDomainId <= CANTSYN_SYNC_FUP_DOMAIN_ID_MAX)
                && (CanTSyn_TransmitFUP(domainIdx, timeMasterPtr->GlobalTimePduId) == E_OK))
            {
                CanTSyn_MasterStatus[domainIdx] = CANTSYN_SYNC_NOT_REQUIRED;
            }
        }
    }
}

/**
 * @brief       Handle the situation where Master is in CANTSYN_SYNC_NOT_REQUIRED state.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75658
 */
CANTSYN_LOCAL void CanTSyn_MasterSyncNotRequired(uint8 domainIdx)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = timeDomainPtr->GlobalTimeMaster;

    if (timeMasterPtr->ImmediateTimeSync)
    {
        /* check whether update counter has changed */
        uint8 baseCounterCurrValue = StbM_GetTimeBaseUpdateCounter(timeDomainPtr->SynchronizedTimeBaseId);
        if (baseCounterCurrValue != CanTSyn_BaseCounterValues[domainIdx])
        {
            CanTSyn_BaseCounterValues[domainIdx] = baseCounterCurrValue;

            /* check whether GLOBAL_TIME_BASE bit is set */
            boolean result = CanTSyn_CheckGlobalTimeBaseBit(domainIdx);
            if (result)
            {
                CanTSyn_MasterStatus[domainIdx]  = CANTSYN_SYNC_REQUIRED;
                CanTSyn_ImmediateSent[domainIdx] = TRUE;
            }
        }
    }
}

/**
 * @brief       Deal with time master main functionality.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75662
 */
CANTSYN_LOCAL void CanTSyn_MasterMain(uint8 domainIdx)
{
    CanTSyn_MasterTimeInfoType* timeInfoPtr = CanTSyn_MasterTimeInfoPtr[domainIdx];
    CanTSyn_MasterStatusType    masterStatus;
    boolean                     result = CanTSyn_CheckGlobalTimeBaseBit(domainIdx);

    /* periodic time sync counter operation */
    if (result && (timeInfoPtr->PeriodCounter != 0UL))
    {
        masterStatus = CANTSYN_SYNC_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfoPtr->PeriodCounter, domainIdx, &masterStatus, NULL_PTR);
    }

    /* cyclic resume counter operation */
    if (timeInfoPtr->CyclicResumeCounter != 0UL)
    {
        masterStatus = CANTSYN_SYNC_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfoPtr->CyclicResumeCounter, domainIdx, &masterStatus, NULL_PTR);
        if ((timeInfoPtr->CyclicResumeCounter == 0UL) && !result)
        {
            CanTSyn_MasterStatus[domainIdx] = CANTSYN_SYNC_NOT_REQUIRED;
        }
    }

    switch (CanTSyn_MasterStatus[domainIdx])
    {
    case CANTSYN_SYNC_REQUIRED:
    {
        CanTSyn_MasterSyncRequired(domainIdx);
        break;
    }
    case CANTSYN_FUP_REQUIRED:
    {
        CanTSyn_MasterFupRequired(domainIdx);
        break;
    }
    case CANTSYN_SYNC_NOT_REQUIRED:
    {
        CanTSyn_MasterSyncNotRequired(domainIdx);
        break;
    }
    default:
        /* do nothing */
        break;
    }
}

/**
 * @brief         Decrement counter value.
 * @param[in,out] counter      the counter to decrement
 * @param[in]     domainIdx    the index of global time domain
 * @param[in]     masterStatus master target status
 * @param[in]     slaveStatus  slave target status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-75665
 */
CANTSYN_LOCAL void CanTSyn_DecrementCounter(
    uint32*                         counter,
    uint8                           domainIdx,
    const CanTSyn_MasterStatusType* masterStatus,
    const CanTSyn_SlaveStatusType*  slaveStatus)
{
    if (*counter <= CANTSYN_MAIN_FUNCTION_PERIOD)
    {
        if ((CanTSyn_MasterStatus[domainIdx] != CANTSYN_FUP_REQUIRED) && (NULL_PTR != masterStatus))
        {
            CanTSyn_MasterStatus[domainIdx] = *masterStatus;
            *counter                        = 0u;
        }
        if (NULL_PTR != slaveStatus)
        {
            CanTSyn_SlaveStatus[domainIdx] = *slaveStatus;
        }
    }
    else
    {
        *counter -= CANTSYN_MAIN_FUNCTION_PERIOD;
    }
}
/**
 * @brief       Shift a 32bit integer to the left by 8 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint32
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75666
 */
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left8(uint32 x)
{
    return x << CANTSYN_SHIFT_BIT8;
}

/**
 * @brief       Shift a 32bit integer to the left by 16 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint32
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75667
 */
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left16(uint32 x)
{
    return x << CANTSYN_SHIFT_BIT16;
}

/**
 * @brief       Shift a 32bit integer to the left by 24 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint32
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75668
 */
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left24(uint32 x)
{
    return x << CANTSYN_SHIFT_BIT24;
}

/**
 * @brief       Shift a 64bit integer to the left by 32 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint64
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75669
 */
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Left32(uint64 x)
{
    return x << CANTSYN_SHIFT_BIT32;
}

/**
 * @brief       Shift a 64bit integer to the right by 32 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint64
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75670
 */
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Right32(uint64 x)
{
    return x >> CANTSYN_SHIFT_BIT32;
}

/**
 * @brief       Get the 24-31 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75671
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_HiHiByte(uint32 x)
{
    return (uint8)(x >> CANTSYN_SHIFT_BIT24);
}

/**
 * @brief       Get the 16-23 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75672
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_HiLoByte(uint32 x)
{
    return (uint8)(x >> CANTSYN_SHIFT_BIT16);
}

/**
 * @brief       Get the 8-15 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75673
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_LoHiByte(uint32 x)
{
    return (uint8)(x >> CANTSYN_SHIFT_BIT8);
}

/**
 * @brief       Get the 0-8 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75674
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_LoLoByte(uint32 x)
{
    return (uint8)x;
}

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief       Report a CanTSyn diagnostic error.
 * @param[in]   apiId   The application ID
 * @param[in]   errorId The error ID
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75675
 */
CANTSYN_LOCAL_INLINE void CanTSyn_Det_Report(uint8 apiId, uint8 errorId)
{
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID, apiId, errorId);
}
#endif
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 6060 -- */
/* PRQA S 1503, 1532, 2889 -- */
