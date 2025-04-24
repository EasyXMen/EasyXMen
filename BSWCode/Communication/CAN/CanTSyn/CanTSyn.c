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
**  FILENAME    : CanTSyn.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>       <REVISION LOG>
 * V2.0.0      20200824   Xinrun.Wang    Initial version
 * V2.0.1      20221215   Xinrun.Wang    Handle OFS type to call StbM_SetOffset
 * V2.0.2      20230309   Xinrun.Wang    Clear ConfirmTimeout when TxConfirm FollowUp
 * V2.0.3      20230321   Xinrun.Wang    Reset Periodic Transmission Timer after ConfirmTimeout
 * V2.0.4      20230407   Xinrun.Wang    OFS/OFNS type message domainId offset adjustment
 * V2.0.5      20230719   Xinrun.Wang    OFNS msg Rx modify get time method to getOffset
 * V2.0.6      20230814   xiaojian.liang fixed CPT-6487, Compatibility checks on big endian systems
 * V2.0.7      20231124   xiaojian.liang Performance optimization
 * V2.0.8      20240312   xiaojian.liang Support multiple partition feature.
               20240403   xiaojian.liang The default transmission mode is set to CANTSYN_TX_ON.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanTSyn<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 1531, 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is global configuration data(1531) and API(1532).
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTSyn.h"
#include "SchM_CanTSyn.h"
#include "istd_lib.h"
#include "CanIf.h"
#include "Os.h"
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define CANTSYN_C_AR_MAJOR_VERSION 4U
#define CANTSYN_C_AR_MINOR_VERSION 5U
#define CANTSYN_C_AR_PATCH_VERSION 0U
#define CANTSYN_C_SW_MAJOR_VERSION 2U
#define CANTSYN_C_SW_MINOR_VERSION 0U
#define CANTSYN_C_SW_PATCH_VERSION 8U

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                             \
    (CANTSYN_C_AR_MAJOR_VERSION != CANTSYN_H_AR_MAJOR_VERSION)    \
    || (CANTSYN_C_AR_MINOR_VERSION != CANTSYN_H_AR_MINOR_VERSION) \
    || (CANTSYN_C_AR_PATCH_VERSION != CANTSYN_H_AR_PATCH_VERSION) \
    || (CANTSYN_C_SW_MAJOR_VERSION != CANTSYN_H_SW_MAJOR_VERSION) \
    || (CANTSYN_C_SW_MINOR_VERSION != CANTSYN_H_SW_MINOR_VERSION) \
    || (CANTSYN_C_SW_PATCH_VERSION != CANTSYN_H_SW_PATCH_VERSION))

#error "CanTSyn.c version mismatching with CanTSyn.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CANTSYN_LOCAL_INLINE static inline

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#define CANTSYN_DET_REPORT(ApiId, ErrorId) \
    ((void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID, (ApiId), (ErrorId)))
#endif

#define CANTSYN_DOMAIN(domainIdx) CanTSyn_configPtr->CanTSynGlobalTimeDomain[domainIdx]

CANTSYN_LOCAL_INLINE uint8 CanTSyn_HiHiByte(uint32 x)
{
    return (uint8)(x >> 24);
}

CANTSYN_LOCAL_INLINE uint8 CanTSyn_HiLoByte(uint32 x)
{
    return (uint8)(x >> 16);
}

CANTSYN_LOCAL_INLINE uint8 CanTSyn_LoHiByte(uint32 x)
{
    return (uint8)(x >> 8);
}

CANTSYN_LOCAL_INLINE uint8 CanTSyn_LoLoByte(uint32 x)
{
    return (uint8)x;
}
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/* Check if the GLOBAL_TIME_BASE bit is set */
static boolean CanTSyn_CheckGlobalTimeBaseBit(uint8 domainIdx);

/* Check whether pass crc validation mode */
static boolean CanTSyn_PassCRCMode(uint8 domainIdx, const PduInfoType* PduInfoPtr);

/* Deal with time master main functionality */
static void CanTSyn_MasterMain(uint8 domainIdx);

/* decrement the specific counter */
static void CanTSyn_DecrementCounter(
    uint32* counter,
    uint8 domainIdx,
    const CanTSyn_MasterStatusType* masterStatus,
    const CanTSyn_SlaveStatusType* slaveStatus);

/* deal with extended message format for SYNC/FUP */
static void CanTSyn_DealExtendedFormat(boolean useExtendedMsgFormat, PduInfoType* pduInfoPtr);

/* calculate CRC for a specific message if CRC is supported*/
#if (CANTSYN_CRC_SUPPORT == STD_ON)
static uint8 CanTSyn_CalculateCRC(const PduInfoType* PduInfoPtr, CanTSyn_DataIDListElementType element);
#endif

/* Transmit a SYNC message as a time master */
static void CanTSyn_TransmitSYNC(uint8 domainIdx, PduIdType pduId);

/* Transmit a FUP message as a time master */
static void CanTSyn_TransmitFUP(uint8 domainIdx, PduIdType pduId);

/* Transmit an OFS message as a time master */
static void CanTSyn_TransmitOFS(uint8 domainIdx, PduIdType pduId);

/* Transmit an OFNS message as a time master */
static void CanTSyn_TransmitOFNS(uint8 domainIdx, PduIdType pduId);

/* Receive a SYNC message as a time slave */
static void CanTSyn_RxSYNC(uint8 domainIdx, const PduInfoType* PduInfoPtr);

/* Receive a FUP message as a time slave */
static void CanTSyn_RxFUP(uint8 domainIdx, const PduInfoType* PduInfoPtr);

/* Receive a OFS message as a time slave */
static void CanTSyn_RxOFS(uint8 domainIdx, const PduInfoType* PduInfoPtr);

/* Receive a OFNS message as a time slave */
static void CanTSyn_RxOFNS(uint8 domainIdx, const PduInfoType* PduInfoPtr);

/* Receive a OFS FD message as a time slave */
static void CanTSyn_RxOFSFD(uint8 domainIdx, const PduInfoType* PduInfoPtr);

#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/* CanTSyn Transmission mode, at first define it as CANTSYN_TX_OFF */
#define CANTSYN_START_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"
static CanTSyn_TransmissionModeType CanTSyn_TransmissionMode[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/* CanTSyn time information for a time slave */
#define CANTSYN_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "CanTSyn_MemMap.h"
static CanTSyn_SlaveTimeInfoType CanTSyn_SlaveTimeInfo[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "CanTSyn_MemMap.h"

/* CanTSyn whether sent immediate message */
#define CANTSYN_START_SEC_VAR_INIT_BOOLEAN
#include "CanTSyn_MemMap.h"
static boolean CanTSyn_ImmediateSent[CANTSYN_GLOBALTIMEDOMAIN_NUMBER] = {0};
#define CANTSYN_STOP_SEC_VAR_INIT_BOOLEAN
#include "CanTSyn_MemMap.h"

/* CanTSyn config data */
#define CANTSYN_START_SEC_VAR_INIT_PTR
#include "CanTSyn_MemMap.h"
static const CanTSyn_ConfigType* CanTSyn_configPtr = NULL_PTR;
#define CANTSYN_STOP_SEC_VAR_INIT_PTR
#include "CanTSyn_MemMap.h"

/* CanTSyn init status, at first define it as CANTSYN_UNINIT */
#define CANTSYN_START_SEC_VAR_INIT_8
#include "CanTSyn_MemMap.h"
static CanTSyn_InitStatusType CanTSyn_InitStatus = CANTSYN_UNINIT;
#define CANTSYN_STOP_SEC_VAR_INIT_8
#include "CanTSyn_MemMap.h"

/* CanTSyn time information for time master */
#define CANTSYN_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "CanTSyn_MemMap.h"
static CanTSyn_MasterTimeInfoType CanTSyn_MasterTimeInfo[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "CanTSyn_MemMap.h"

/* CanTSyn global variable for runtime Global Time Domain control */
#define CANTSYN_START_SEC_VAR_INIT_8
#include "CanTSyn_MemMap.h"
static uint8 CanTSyn_SequenceCounters[CANTSYN_GLOBALTIMEDOMAIN_NUMBER] = {0};
#define CANTSYN_STOP_SEC_VAR_INIT_8
#include "CanTSyn_MemMap.h"

/* CanTSyn counter values for time bases from stbm */
#define CANTSYN_START_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"
static uint8 CanTSyn_BaseCounterValues[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"

/* CanTSyn indication of master transmitting status */
#define CANTSYN_START_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"
static CanTSyn_MasterStatusType CanTSyn_MasterStatus[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"

/* CanTSyn indication of slave receving status */
#define CANTSYN_START_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"
static CanTSyn_SlaveStatusType CanTSyn_SlaveStatus[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_NO_INIT_8
#include "CanTSyn_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#if (CANTSYN_VERSIONINFO_API == STD_ON)

/**
 * This service returns the version information of this module.
 * Service ID: 0x02
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo: Pointer to where to store the version information of this module
 * Return value: NA
 *
 */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
void CanTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTSYN_APPL_DATA) versioninfo)
{
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {

        CANTSYN_DET_REPORT(CANTSYN_GETVERSIONINFO_ID, CANTSYN_E_NULL_POINTER);
    }
    else
#endif
    {
        versioninfo->vendorID = CANTSYN_VENDOR_ID;
        versioninfo->moduleID = CANTSYN_MODULE_ID;
        versioninfo->sw_major_version = CANTSYN_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANTSYN_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANTSYN_H_SW_PATCH_VERSION;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

/**
 * This function initializes the Time Synchronization over CAN.
 * Service ID: 0x01
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): configPtr: Pointer to selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
void CanTSyn_Init(const CanTSyn_ConfigType* configPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 domainIdx;
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
    /* Detect null-ptr */
    if (configPtr == NULL_PTR)
    {

        CANTSYN_DET_REPORT(CANTSYN_INIT_ID, CANTSYN_E_NULL_POINTER);
    }
    else
#endif
    {
        CanTSyn_configPtr = configPtr;

        /* Initialize base update counter and time counters */
        for (domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
        {
            CanTSyn_BaseCounterValues[domainIdx] =
                StbM_GetTimeBaseUpdateCounter(CANTSYN_DOMAIN(domainIdx).CanTSynSynchronizedTimeBaseId);
            CanTSyn_MasterTimeInfo[domainIdx].cyclicResumeCounter = 0u;
            CanTSyn_MasterTimeInfo[domainIdx].debounceCounter = 0u;
            CanTSyn_MasterTimeInfo[domainIdx].periodCounter = 1u;
            CanTSyn_MasterTimeInfo[domainIdx].confirmationTimeoutCounter = 0u;
            CanTSyn_SlaveTimeInfo[domainIdx].receptionTimeoutCounter = 0u;
            CanTSyn_SlaveTimeInfo[domainIdx].sequenceCounter = 0u;
            CanTSyn_MasterStatus[domainIdx] = SYNC_NOT_REQUIRED;
            CanTSyn_SlaveStatus[domainIdx] = SYNC_EXPECTED;
            CanTSyn_TransmissionMode[domainIdx] = CANTSYN_TX_ON;
        }
        CanTSyn_InitStatus = CANTSYN_INITED;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

/**
 * This API is used to turn on and off the TX capabilities of the CanTSyn.
 * Service ID: 0x03
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx: Index of the CAN channel
 *                 Mode: CANTSYN_TX_OFF CANTSYN_TX_ON
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
void CanTSyn_SetTransmissionMode(
    uint8 CtrlIdx,
    CanTSyn_TransmissionModeType Mode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
/* detect null_ptr */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
    if ((Mode != CANTSYN_TX_OFF) && (Mode != CANTSYN_TX_ON))
    {
        CANTSYN_DET_REPORT(CANTSYN_SETTRANSMISSIONMODE_ID, CANTSYN_E_PARAM);
    }
    else if (CtrlIdx >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER) /* check validity of CtrlIdx */
    {
        CANTSYN_DET_REPORT(CANTSYN_SETTRANSMISSIONMODE_ID, CANTSYN_E_INV_CTRL_IDX);
    }
    else if (CanTSyn_InitStatus == CANTSYN_UNINIT) /* check if module initialized */
    {
        CANTSYN_DET_REPORT(CANTSYN_SETTRANSMISSIONMODE_ID, CANTSYN_E_UNINIT);
    }
    else
#endif
    {
        CanTSyn_TransmissionMode[CtrlIdx] = Mode;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

/**
 * Main function for cyclic call / resp. Timesync message transmission
 * Service ID: 0x06
 * Sync/Async: synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
void CanTSyn_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 domainIdx;
    CanTSyn_SlaveStatusType slaveStatus;

    if (CanTSyn_InitStatus == CANTSYN_INITED) /* check if module initialized */
    {
        /* look through each time domain */
        for (domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
        {
            const CanTSyn_GlobalTimeDomainType* globalTimeDomain = &CANTSYN_DOMAIN(domainIdx);
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (GetApplicationID() != timeDomain.applicationID)
            {
                continue;
            }
#endif
            /* check whether configured as a time master */
            if (globalTimeDomain->CanTSynGlobalTimeMaster != NULL_PTR)
            {
                /* deal with functionality as a time master */
                CanTSyn_MasterMain(domainIdx);
            }

            /* check whether configured as a time slave */
            if (globalTimeDomain->CanTSynGlobalTimeSlave != NULL_PTR)
            {
                /* reception timeout counter operation */
                if (CanTSyn_SlaveTimeInfo[domainIdx].receptionTimeoutCounter != 0u)
                {
                    slaveStatus = SYNC_EXPECTED;
                    CanTSyn_DecrementCounter(
                        &CanTSyn_SlaveTimeInfo[domainIdx].receptionTimeoutCounter,
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

/**
 * Indication of a received PDU from a lower layer communication interface module.
 * Service ID: 0x42
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): RxPduId: ID of the received PDU.
 *                 PduInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 * containing the PDU, and the MetaData related to this PDU. Parameters(INOUT): NA Parameters(OUT): NA Return value: NA
 *
 */
#define CANTSYN_START_SEC_CANTSYNRXINDICATION_CALLBACK_CODE
#include "CanTSyn_MemMap.h"
void CanTSyn_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    SchM_Enter_CanTSyn_Context(); /*lock state*/
    const CanTSyn_GlobalTimeDomainType* globalTimeDomain = NULL_PTR;
    StbM_SynchronizedTimeBaseType timeBaseId;
    Std_ReturnType result = E_OK;
    StbM_VirtualLocalTimeType localTimePtr;
    uint8 domainIdx;
    uint8 domainId;
    uint8 type;
    CanTSynGlobalTimeSlaveType timeSlave;
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
    /* DET Detect */
    if (PduInfoPtr == NULL_PTR)
    {
        CANTSYN_DET_REPORT(CANTSYN_RXINDICATION_ID, CANTSYN_E_NULL_POINTER);
        SchM_Exit_CanTSyn_Context(); /*unlock state*/
    }
    else if (CanTSyn_InitStatus == CANTSYN_UNINIT)
    {
        CANTSYN_DET_REPORT(CANTSYN_RXINDICATION_ID, CANTSYN_E_UNINIT);
        SchM_Exit_CanTSyn_Context(); /*unlock state*/
    }
    else
#endif
    {
        domainId = PduInfoPtr->SduDataPtr[2] >> 4;
        type = PduInfoPtr->SduDataPtr[0];

        if ((type == CANTSYN_OFS_NOT_CRC_TYPE) || (type == CANTSYN_OFS_CRC_TYPE)
            || (type == CANTSYN_OFS_NOT_CRC_FD_TYPE) || (type == CANTSYN_OFS_CRC_FD_TYPE)
            || (type == CANTSYN_OFNS_NOT_CRC_TYPE) || (type == CANTSYN_OFNS_CRC_TYPE))
        {
            domainId += 16u;
        }

        /* Search for matched domainIdx */
        for (domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
        {
            if (CANTSYN_DOMAIN(domainIdx).CanTSynGlobalTimeDomainId == domainId)
            {
                globalTimeDomain = &CANTSYN_DOMAIN(domainIdx);
                break;
            }
        }

        if ((globalTimeDomain != NULL_PTR) && (globalTimeDomain->CanTSynGlobalTimeSlave != NULL_PTR))
        {
            timeBaseId = globalTimeDomain->CanTSynSynchronizedTimeBaseId;
            timeSlave = *(globalTimeDomain->CanTSynGlobalTimeSlave);

            if (RxPduId != timeSlave.CanTSynGlobalTimePduId)
            {
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
                CANTSYN_DET_REPORT(CANTSYN_RXINDICATION_ID, CANTSYN_E_INVALID_PDUID);
#endif
                SchM_Exit_CanTSyn_Context(); /*unlock state*/
            }
            else
            {
                /* Check whether it is a SYNC msg */
                if ((type != CANTSYN_SYNC_NOT_CRC_TYPE) && (type != CANTSYN_SYNC_CRC_TYPE)
                    && (type != CANTSYN_OFS_NOT_CRC_TYPE) && (type != CANTSYN_OFS_CRC_TYPE))
                {
                    SchM_Exit_CanTSyn_Context(); /*unlock state*/
                }
                else
                {
                    result = StbM_GetCurrentVirtualLocalTime(timeBaseId, &localTimePtr);
                    SchM_Exit_CanTSyn_Context(); /*unlock state*/
                }

                if ((result == E_OK) && (globalTimeDomain->CanTSynGlobalTimeDomainId <= 31u))
                {
                    if (CanTSyn_PassCRCMode(domainIdx, PduInfoPtr) == TRUE)
                    {
                        switch (type)
                        {
                        case CANTSYN_SYNC_NOT_CRC_TYPE:
                        case CANTSYN_SYNC_CRC_TYPE:
                            /* it is a SYNC message */
                            CanTSyn_SlaveTimeInfo[domainIdx].receptionTimeoutCounter =
                                timeSlave.CanTSynGlobalTimeFollowUpTimeout;
                            CanTSyn_SlaveTimeInfo[domainIdx].T2VLT = localTimePtr.nanosecondsHi;
                            CanTSyn_SlaveTimeInfo[domainIdx].T2VLT = CanTSyn_SlaveTimeInfo[domainIdx].T2VLT << 32;
                            CanTSyn_SlaveTimeInfo[domainIdx].T2VLT += localTimePtr.nanosecondsLo;
                            CanTSyn_RxSYNC(domainIdx, PduInfoPtr);
                            break;
                        case CANTSYN_FUP_NOT_CRC_TYPE:
                        case CANTSYN_FUP_CRC_TYPE:
                            /* it is a FUP message */
                            if (CanTSyn_SlaveStatus[domainIdx] == FUP_EXPECTED)
                            {
                                /* Release Interruption */
                                CanTSyn_SlaveTimeInfo[domainIdx].receptionTimeoutCounter = 0u;
                                CanTSyn_SlaveStatus[domainIdx] = SYNC_EXPECTED;
                                CanTSyn_RxFUP(domainIdx, PduInfoPtr);
                            }
                            break;
                        case CANTSYN_OFS_NOT_CRC_TYPE:
                        case CANTSYN_OFS_CRC_TYPE:
                            /* it is an OFS regular message */
                            CanTSyn_RxOFS(domainIdx, PduInfoPtr);
                            break;
                        case CANTSYN_OFNS_NOT_CRC_TYPE:
                        case CANTSYN_OFNS_CRC_TYPE:
                            /* it is an OFNS message */
                            if (CanTSyn_SlaveStatus[domainIdx] == FUP_EXPECTED)
                            {
                                CanTSyn_SlaveTimeInfo[domainIdx].receptionTimeoutCounter = 0u;
                                CanTSyn_SlaveStatus[domainIdx] = SYNC_EXPECTED;
                                CanTSyn_RxOFNS(domainIdx, PduInfoPtr);
                            }
                            break;
                        case CANTSYN_OFS_NOT_CRC_FD_TYPE:
                        case CANTSYN_OFS_CRC_FD_TYPE:
                            /* it is an OFS FD message */
                            if (CanTSyn_SlaveStatus[domainIdx] == SYNC_EXPECTED)
                            {
                                CanTSyn_RxOFSFD(domainIdx, PduInfoPtr);
                            }
                            break;
                        default:
                            /* do nothing */
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            SchM_Exit_CanTSyn_Context(); /*unlock state*/
        }
    }
}
#define CANTSYN_STOP_SEC_CANTSYNRXINDICATION_CALLBACK_CODE
#include "CanTSyn_MemMap.h"

/**
 * The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 * Service ID: 0x40
 * Sync/Async: synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): TxPduId: ID of the PDU that has been transmitted.
 *                 result: E_OK: The PDU was transmitted.
 *                         E_NOT_OK: Transmission of the PDU failed.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define CANTSYN_START_SEC_CANTSYNTXCONFIRMATION_CALLBACK_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void CanTSyn_TxConfirmation(
    PduIdType TxPduId
#if (CANTSYN_VER_R19_11 == STD_ON)
    ,
    Std_ReturnType result
#endif
)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    SchM_Enter_CanTSyn_Context(); /*lock state*/
    Std_ReturnType curResult;
    StbM_SynchronizedTimeBaseType timeBaseId;
    uint64 T1VLT;
    uint64 T4;
    StbM_VirtualLocalTimeType localTimePtr;
    CanTSynGlobalTimeMasterType timeMaster;

#if (CANTSYN_VER_R19_11 == STD_ON)
    if (result == E_OK)
    {
#endif
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
        if (TxPduId >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER)
        {
            CANTSYN_DET_REPORT(CANTSYN_TXCONFIRMATION_ID, CANTSYN_E_INVALID_PDUID);
            SchM_Exit_CanTSyn_Context(); /*unlock state*/
        }
        else if (CanTSyn_InitStatus == CANTSYN_UNINIT)
        {
            CANTSYN_DET_REPORT(CANTSYN_TXCONFIRMATION_ID, CANTSYN_E_UNINIT);
            SchM_Exit_CanTSyn_Context(); /*unlock state*/
        }
        else
#endif
        {
            CanTSyn_MasterTimeInfoType* masterTimeInfo = &CanTSyn_MasterTimeInfo[TxPduId];

            if ((NULL_PTR != CANTSYN_DOMAIN(TxPduId).CanTSynGlobalTimeMaster)
                && (CanTSyn_MasterStatus[TxPduId] == SYNC_SENT))
            {
                const CanTSyn_GlobalTimeDomainType* globalTimeDomain = &CANTSYN_DOMAIN(TxPduId);
                timeMaster = *(globalTimeDomain->CanTSynGlobalTimeMaster);
                /* Check whether it is a synchrnized time base */
                if (globalTimeDomain->CanTSynGlobalTimeDomainId <= 15u)
                {
                    timeBaseId = globalTimeDomain->CanTSynSynchronizedTimeBaseId;
                    curResult = StbM_GetCurrentVirtualLocalTime(timeBaseId, &localTimePtr);
                    SchM_Exit_CanTSyn_Context(); /*unlock state*/

                    if (curResult == E_OK)
                    {
                        T1VLT = localTimePtr.nanosecondsHi;
                        T1VLT = T1VLT << 32;
                        T1VLT += localTimePtr.nanosecondsLo;
                        T4 = masterTimeInfo->T0SYNCns + (T1VLT - masterTimeInfo->T0VLT);
                        masterTimeInfo->T4 = T4;
                    }
                }
                else
                {
                    SchM_Exit_CanTSyn_Context(); /*unlock state*/
                }

                /* Start period counter */
                masterTimeInfo->periodCounter = timeMaster.CanTSynGlobalTimeTxPeriod;

                /* Stop confirmation timeout counter */
                masterTimeInfo->confirmationTimeoutCounter = 0u;

                /* check if immediate sent, if yes, start cyclic msg resume counter, stop period counter */
                if (CanTSyn_ImmediateSent[TxPduId] == TRUE)
                {
                    masterTimeInfo->cyclicResumeCounter = timeMaster.CanTSynCyclicMsgResumeTime;
                    CanTSyn_ImmediateSent[TxPduId] = FALSE;
                    masterTimeInfo->periodCounter = 0u;
                }

                CanTSyn_MasterStatus[TxPduId] = FUP_REQUIRED;
                /* Start debounce counter */
                masterTimeInfo->debounceCounter = timeMaster.CanTSynGlobalTimeDebounceTime;
            }
            else if (
                (CANTSYN_DOMAIN(TxPduId).CanTSynGlobalTimeMaster != NULL_PTR)
                && (SYNC_SENT != CanTSyn_MasterStatus[TxPduId]))
            {
                SchM_Exit_CanTSyn_Context();
                /* Stop confirmation timeout counter */
                masterTimeInfo->confirmationTimeoutCounter = 0u;
                /* increment sequence counter */
                CanTSyn_SequenceCounters[TxPduId] = (CanTSyn_SequenceCounters[TxPduId] + 1u) % 16u;
            }
            else
            {
                SchM_Exit_CanTSyn_Context();
            }
        }
#if (CANTSYN_VER_R19_11 == STD_ON)
    }
    else
    {
        SchM_Exit_CanTSyn_Context(); /*unlock state*/
    }
#endif
}
#define CANTSYN_STOP_SEC_CANTSYNTXCONFIRMATION_CALLBACK_CODE
#include "CanTSyn_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/**
 * Check whether pass CRC validation mode or not
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  PduInfoPtr: the pdu info pointer
 *
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: passed CRC validation mode
 *               E_NOT_OK: not passed CRC validation mode
 *
 */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
static boolean CanTSyn_PassCRCMode(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    const CanTSyn_GlobalTimeDomainType* globalTimeDomain = &CANTSYN_DOMAIN(domainIdx);
    uint8 type = PduInfoPtr->SduDataPtr[0];
    CrcValidatedType CrcMode = globalTimeDomain->CanTSynGlobalTimeSlave->CanTSynRxCrcValidated;
    CanTSyn_DataIDListElementType element;
    boolean result = TRUE;

    uint8 CRC;
    uint8 sequenceCounter = PduInfoPtr->SduDataPtr[2] & CANTSYN_SEQUENCE_COUNTER_BIT_MASK;

    /* check whether it is a CRC message */
    if ((type == CANTSYN_SYNC_CRC_TYPE) || (type == CANTSYN_FUP_CRC_TYPE) || (type == CANTSYN_OFS_CRC_TYPE)
        || (type == CANTSYN_OFS_CRC_FD_TYPE) || (type == CANTSYN_OFNS_CRC_TYPE))
    { /* it is a CRC message */
        if (CrcMode == CRC_NOT_VALIDATED)
        {
            result = FALSE;
        }
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        else if ((CrcMode == CRC_OPTIONAL) || (CrcMode == CRC_VALIDATED)) /* need to validate CRC */
        {
            switch (type)
            {
            case CANTSYN_SYNC_CRC_TYPE: {
                element = globalTimeDomain->CanTSynGlobalTimeSyncDataIDList[sequenceCounter];
                break;
            }
            case CANTSYN_FUP_CRC_TYPE: {
                element = globalTimeDomain->CanTSynGlobalTimeFupDataIDList[sequenceCounter];
                break;
            }
            case CANTSYN_OFS_CRC_TYPE: {
                element = globalTimeDomain->CanTSynGlobalTimeOfsDataIDList[sequenceCounter];
                break;
            }
            case CANTSYN_OFS_CRC_FD_TYPE: {
                element = globalTimeDomain->CanTSynGlobalTimeOfsDataIDList[sequenceCounter];
                break;
            }
            case CANTSYN_OFNS_CRC_TYPE: {
                element = globalTimeDomain->CanTSynGlobalTimeOfnsDataIDList[sequenceCounter];
                break;
            }
            default:
                /* do nothing */
                break;
            }
            CRC = CanTSyn_CalculateCRC(PduInfoPtr, element);
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
        if (CrcMode == CRC_VALIDATED)
        {
            result = FALSE;
        }
    }
    return result;
}

/**
 * Deal with time master main functionality
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
static void CanTSyn_MasterMain(uint8 domainIdx)
{
    boolean result;
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    CanTSynGlobalTimeMasterType timeMaster = *(timeDomain.CanTSynGlobalTimeMaster);
    uint8 baseCounterCurrValue;
    CanTSyn_MasterStatusType masterStatus;
    CanTSyn_MasterTimeInfoType* timeInfo = &CanTSyn_MasterTimeInfo[domainIdx];

    /* get the time base status and check whether to cyclic time sync */
    result = CanTSyn_CheckGlobalTimeBaseBit(domainIdx);
    /* periodic time sync counter operation */
    if (result && (timeInfo->periodCounter != 0u))
    {
        masterStatus = SYNC_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfo->periodCounter, domainIdx, &masterStatus, NULL_PTR);
    }

    /* cyclic resume counter operation */
    if (timeInfo->cyclicResumeCounter != 0u)
    {
        masterStatus = SYNC_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfo->cyclicResumeCounter, domainIdx, &masterStatus, NULL_PTR);
        if ((timeInfo->cyclicResumeCounter == 0u) && !result)
        {
            CanTSyn_MasterStatus[domainIdx] = SYNC_NOT_REQUIRED;
        }
    }

    /* confirmation time out counter operation */
    if (timeInfo->confirmationTimeoutCounter != 0u)
    {
        masterStatus = SYNC_NOT_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfo->confirmationTimeoutCounter, domainIdx, &masterStatus, NULL_PTR);
        if (timeInfo->confirmationTimeoutCounter == 0u)
        {
            timeInfo->periodCounter = timeMaster.CanTSynGlobalTimeTxPeriod;
        }
    }

    switch (CanTSyn_MasterStatus[domainIdx])
    {
    case SYNC_REQUIRED: {
        if (CanTSyn_TransmissionMode[domainIdx] == CANTSYN_TX_ON)
        {
            CanTSyn_MasterStatus[domainIdx] = SYNC_SENT;

            /* Start trasnmitting */
            if (timeDomain.CanTSynGlobalTimeDomainId <= 15u)
            {
                CanTSyn_TransmitSYNC(domainIdx, timeMaster.CanTSynGlobalTimePduId);
            }
            else
            {
                CanTSyn_TransmitOFS(domainIdx, timeMaster.CanTSynGlobalTimePduId);
            }

            /* Start timeout counter */
            timeInfo->confirmationTimeoutCounter = timeMaster.CanTSynMasterConfirmationTimeout;
        }
        break;
    }
    case FUP_REQUIRED: {
        if (timeInfo->debounceCounter > CANTSYN_MAIN_FUNCTION_PERIOD)
        {
            timeInfo->debounceCounter -= CANTSYN_MAIN_FUNCTION_PERIOD;
            break;
        }

        if (CanTSyn_TransmissionMode[domainIdx] == CANTSYN_TX_ON)
        {
            CanTSyn_MasterStatus[domainIdx] = SYNC_NOT_REQUIRED;

            /* Start timeout counter */
            timeInfo->confirmationTimeoutCounter = timeMaster.CanTSynMasterConfirmationTimeout;

            /* Start Transimitting */
            if (timeDomain.CanTSynGlobalTimeDomainId <= 15u)
            {
                CanTSyn_TransmitFUP(domainIdx, timeMaster.CanTSynGlobalTimePduId);
            }
            else if (timeDomain.CanTSynUseExtendedMsgFormat == FALSE)
            {
                CanTSyn_TransmitOFNS(domainIdx, timeMaster.CanTSynGlobalTimePduId);
            }
            else
            {
                break;
            }
        }
        break;
    }
    case SYNC_NOT_REQUIRED: {
        if (timeMaster.CanTSynImmediateTimeSync == TRUE)
        {
            /* check whether update counter has changed */
            baseCounterCurrValue = StbM_GetTimeBaseUpdateCounter(timeDomain.CanTSynSynchronizedTimeBaseId);
            if (baseCounterCurrValue != CanTSyn_BaseCounterValues[domainIdx])
            {
                CanTSyn_BaseCounterValues[domainIdx] = baseCounterCurrValue;

                /* check whether GLOBAL_TIME_BASE bit is set */
                result = CanTSyn_CheckGlobalTimeBaseBit(domainIdx);
                if (result == TRUE)
                {
                    CanTSyn_MasterStatus[domainIdx] = SYNC_REQUIRED;
                    CanTSyn_ImmediateSent[domainIdx] = TRUE;
                }
            }
        }
        break;
    }
    default:
        /* do nothing */
        break;
    }
}

/**
 * Decrement counter value
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  counter: the counter to decrement
 *                  domainIdx: the index of global time domain
 *                  masterStatus: whether switch status to sync or fup
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_DecrementCounter(
    uint32* counter,
    uint8 domainIdx,
    const CanTSyn_MasterStatusType* masterStatus,
    const CanTSyn_SlaveStatusType* slaveStatus)
{
    if (*counter <= CANTSYN_MAIN_FUNCTION_PERIOD)
    {
        if ((CanTSyn_MasterStatus[domainIdx] != FUP_REQUIRED) && (NULL_PTR != masterStatus))
        {
            CanTSyn_MasterStatus[domainIdx] = *masterStatus;
            *counter = 0u;
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
 * Deal with extended message format for SYNC/FUP
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  useExtendedMsgFormat: whether to use can FD or not
 *                  pduInfoPtr: the pdu info ptr
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_DealExtendedFormat(boolean useExtendedMsgFormat, PduInfoType* pduInfoPtr)
{
    uint8 extendedIdx;

    if (useExtendedMsgFormat == TRUE)
    {
        pduInfoPtr->SduLength = 16u;
        for (extendedIdx = 8u; extendedIdx < 16u; extendedIdx++)
        {
            pduInfoPtr->SduDataPtr[extendedIdx] = 0u;
        }
    }
    else
    {
        pduInfoPtr->SduLength = 8u;
    }
}

/**
 * Calculate CRC for a message
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  pduInfo: the pdu info
 *                  element: the DataID list element
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: CRC result
 *
 */
#if (CANTSYN_CRC_SUPPORT == STD_ON)
static uint8 CanTSyn_CalculateCRC(const PduInfoType* PduInfoPtr, CanTSyn_DataIDListElementType element)
{
    uint8 data[15];
    (void)ILib_memcpy(data, &PduInfoPtr->SduDataPtr[2], (uint32)PduInfoPtr->SduLength - 2u);
    data[PduInfoPtr->SduLength - 2u] = element.CanTSynGlobalTimeDataIDListValue;

    uint32 Crc_length = (PduInfoPtr->SduLength == 16u) ? 15u : 7u;
    uint8 result = Crc_CalculateCRC8H2F(data, Crc_length, CANTSYN_CRC_DEFAULT_VALUE, TRUE);
    return result;
}
#endif

/**
 * Transmit a SYNC message as a time master
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  pduId: the pdu id
 *                  useExtendedMsgFormat: whether use can FD or not
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_TransmitSYNC(uint8 domainIdx, PduIdType pduId)
{
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    Std_ReturnType result;
    StbM_TimeStampType globalTimePtr;
    StbM_VirtualLocalTimeType localTimePtr;
    StbM_UserDataType userData;
    uint8 userByte[3] = {0};
    uint8 SduData[16] = {0};
    PduInfoType pduInfo;

    /* get current bus time */
    result = StbM_BusGetCurrentTime(timeDomain.CanTSynSynchronizedTimeBaseId, &globalTimePtr, &localTimePtr, &userData);
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
        SduData[0] = CANTSYN_SYNC_NOT_CRC_TYPE;
        SduData[1] = userByte[1];
        SduData[2] = (timeDomain.CanTSynGlobalTimeDomainId << 4) + CanTSyn_SequenceCounters[domainIdx];
        SduData[3] = userByte[0];

        /* retrieve time info */
        uint32 T0SYNC = globalTimePtr.seconds;

        SduData[4] = CanTSyn_HiHiByte(T0SYNC);
        SduData[5] = CanTSyn_HiLoByte(T0SYNC);
        SduData[6] = CanTSyn_LoHiByte(T0SYNC);
        SduData[7] = CanTSyn_LoLoByte(T0SYNC);

        pduInfo.SduDataPtr = SduData;

        CanTSyn_MasterTimeInfoType* masterTimeInfo = &CanTSyn_MasterTimeInfo[domainIdx];
        /* store extra time information */
        masterTimeInfo->T0SYNCns = globalTimePtr.nanoseconds;
        masterTimeInfo->T0VLT = localTimePtr.nanosecondsHi;
        masterTimeInfo->T0VLT = masterTimeInfo->T0VLT << 32;
        masterTimeInfo->T0VLT += localTimePtr.nanosecondsLo;
        masterTimeInfo->userByte2 = userByte[2];
        masterTimeInfo->timeBaseStatus = globalTimePtr.timeBaseStatus;

        /* deal with extented format */
        CanTSyn_DealExtendedFormat(timeDomain.CanTSynUseExtendedMsgFormat, &pduInfo);
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        if (timeDomain.CanTSynGlobalTimeMaster->CanTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED)
        {
            CanTSyn_DataIDListElementType DataIDElement;
            DataIDElement = timeDomain.CanTSynGlobalTimeSyncDataIDList[CanTSyn_SequenceCounters[domainIdx]];
            pduInfo.SduDataPtr[0] = CANTSYN_SYNC_CRC_TYPE;
            pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(&pduInfo, DataIDElement);
        }
#endif
        (void)CanIf_Transmit(pduId, &pduInfo);
    }
}

/**
 * Transmit a FUP message as a time master
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  pduId: the pdu id
 *                  useExtendedMsgFormat: whether use can FD or not
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_TransmitFUP(uint8 domainIdx, PduIdType pduId)
{
    uint8 OVS;
    uint64 T4 = CanTSyn_MasterTimeInfo[domainIdx].T4;
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    uint8 timeDomainId = timeDomain.CanTSynGlobalTimeDomainId;
    StbM_TimeBaseStatusType timeBaseStatus = CanTSyn_MasterTimeInfo[domainIdx].timeBaseStatus;
    uint8 SduData[16] = {0};
    PduInfoType pduInfo;

    /* write overflow part to OVS */
    OVS = (uint8)(T4 / CANTSYN_1NS_SECOND);
    T4 -= (uint64)OVS * (uint64)CANTSYN_1NS_SECOND;

    /* Calculate SGW */
    if ((timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) == 0u) /* sync to global time master */
    {
        SduData[3] = 0x00;
    }
    else /* sync to sub domain */
    {
        SduData[3] = 0x04;
    }

    /* construct pdu */
    SduData[0] = CANTSYN_FUP_NOT_CRC_TYPE;
    SduData[1] = CanTSyn_MasterTimeInfo[domainIdx].userByte2;
    SduData[2] = (timeDomainId << 4) + CanTSyn_SequenceCounters[domainIdx];
    SduData[3] += OVS;
    SduData[4] = CanTSyn_HiHiByte((uint32)T4);
    SduData[5] = CanTSyn_HiLoByte((uint32)T4);
    SduData[6] = CanTSyn_LoHiByte((uint32)T4);
    SduData[7] = CanTSyn_LoLoByte((uint32)T4);

    pduInfo.SduDataPtr = SduData;
    /* deal with extented format */
    CanTSyn_DealExtendedFormat(timeDomain.CanTSynUseExtendedMsgFormat, &pduInfo);

    /* deal with CRC format */
#if (CANTSYN_CRC_SUPPORT == STD_ON)
    if (timeDomain.CanTSynGlobalTimeMaster->CanTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED)
    {
        CanTSyn_DataIDListElementType DataIDElement;
        DataIDElement = timeDomain.CanTSynGlobalTimeFupDataIDList[CanTSyn_SequenceCounters[domainIdx]];
        pduInfo.SduDataPtr[0] = CANTSYN_FUP_CRC_TYPE;
        pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(&pduInfo, DataIDElement);
    }

#endif
    (void)CanIf_Transmit(pduId, &pduInfo);
}

/**
 * Transmit a OFS message as a time master
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  pduId: the pdu id
 *                  useExtendedMsgFormat: whether use can FD or not
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_TransmitOFS(uint8 domainIdx, PduIdType pduId)
{
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    StbM_SynchronizedTimeBaseType timeBaseId = timeDomain.CanTSynSynchronizedTimeBaseId;
    uint8 timeDomainId = timeDomain.CanTSynGlobalTimeDomainId;
    StbM_TimeBaseStatusType timeBaseStatus;
    StbM_TimeStampType timeStamp;
    StbM_UserDataType userData;
    uint8 userByte[3] = {0};
    Std_ReturnType result;
    uint8 SduData[16] = {0};
    PduInfoType pduInfo;
    uint32 OfsTimeSec;
    uint32 OfsTimeNSec;

    result = StbM_GetOffset(timeBaseId, &timeStamp, &userData);

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

        OfsTimeSec = timeStamp.seconds;
        OfsTimeNSec = timeStamp.nanoseconds;
        timeBaseStatus = timeStamp.timeBaseStatus;

        /* deal with extented format */
        if (timeDomain.CanTSynUseExtendedMsgFormat == TRUE)
        {
            pduInfo.SduLength = 16u;

            /* Calculate SGW */
            if ((timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) == 0u) /* sync to global time master */
            {
                SduData[3] = 0x00;
            }
            else /* sync to sub domain */
            {
                SduData[3] = 0x01;
            }

            /* construct pdu */
            SduData[0] = CANTSYN_OFS_NOT_CRC_FD_TYPE;
            SduData[1] = userByte[2];
            SduData[2] = (timeDomainId << 4) + CanTSyn_SequenceCounters[domainIdx];
            SduData[4] = userByte[0];
            SduData[5] = userByte[1];
            SduData[6] = 0u;
            SduData[7] = 0u;
            SduData[8] = CanTSyn_HiHiByte(OfsTimeSec);
            SduData[9] = CanTSyn_HiLoByte(OfsTimeSec);
            SduData[10] = CanTSyn_LoHiByte(OfsTimeSec);
            SduData[11] = CanTSyn_LoLoByte(OfsTimeSec);
            SduData[12] = CanTSyn_HiHiByte(OfsTimeNSec);
            SduData[13] = CanTSyn_HiLoByte(OfsTimeNSec);
            SduData[14] = CanTSyn_LoHiByte(OfsTimeNSec);
            SduData[15] = CanTSyn_LoLoByte(OfsTimeNSec);
        }
        else
        {
            pduInfo.SduLength = 8u;

            /* construct pdu */
            SduData[0] = CANTSYN_OFS_NOT_CRC_TYPE;
            SduData[1] = userByte[1];
            SduData[2] = (timeDomainId << 4) + CanTSyn_SequenceCounters[domainIdx];
            SduData[3] = userByte[0];
            SduData[4] = CanTSyn_HiHiByte(OfsTimeSec);
            SduData[5] = CanTSyn_HiLoByte(OfsTimeSec);
            SduData[6] = CanTSyn_LoHiByte(OfsTimeSec);
            SduData[7] = CanTSyn_LoLoByte(OfsTimeSec);

            CanTSyn_MasterTimeInfo[domainIdx].timeBaseStatus = timeBaseStatus;
            CanTSyn_MasterTimeInfo[domainIdx].OfsTimeNSec = OfsTimeNSec;
            CanTSyn_MasterTimeInfo[domainIdx].userByte2 = userByte[2];
        }

        pduInfo.SduDataPtr = SduData;

        /* deal with CRC format */
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        if (timeDomain.CanTSynGlobalTimeMaster->CanTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED)
        {
            CanTSyn_DataIDListElementType DataIDElement;
            DataIDElement = timeDomain.CanTSynGlobalTimeOfsDataIDList[CanTSyn_SequenceCounters[domainIdx]];
            if (timeDomain.CanTSynUseExtendedMsgFormat == TRUE)
            {
                pduInfo.SduDataPtr[0] = CANTSYN_OFS_CRC_FD_TYPE;
            }
            else
            {
                pduInfo.SduDataPtr[0] = CANTSYN_OFS_CRC_TYPE;
            }
            pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(&pduInfo, DataIDElement);
        }
#endif
        (void)CanIf_Transmit(pduId, &pduInfo);
    }
}

/**
 * Transmit a OFNS message as a time master
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  pduId: the pdu id
 *                  useExtendedMsgFormat: whether use can FD or not
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_TransmitOFNS(uint8 domainIdx, PduIdType pduId)
{
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    uint8 timeDomainId = timeDomain.CanTSynGlobalTimeDomainId;
    StbM_TimeBaseStatusType timeBaseStatus = CanTSyn_MasterTimeInfo[domainIdx].timeBaseStatus;
    uint32 OfsTimeNSec = CanTSyn_MasterTimeInfo[domainIdx].OfsTimeNSec;
    uint8 SduData[16] = {0};
    PduInfoType pduInfo;

    /* Calculate SGW */
    if ((timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) == 0u) /* sync to global time master */
    {
        SduData[3] = 0x00;
    }
    else /* sync to sub domain */
    {
        SduData[3] = 0x01;
    }

    pduInfo.SduLength = 8u;

    /* construct pdu */
    SduData[0] = CANTSYN_OFNS_NOT_CRC_TYPE;
    SduData[1] = CanTSyn_MasterTimeInfo[domainIdx].userByte2;
    SduData[2] = (timeDomainId << 4) + CanTSyn_SequenceCounters[domainIdx];
    SduData[4] = CanTSyn_HiHiByte(OfsTimeNSec);
    SduData[5] = CanTSyn_HiLoByte(OfsTimeNSec);
    SduData[6] = CanTSyn_LoHiByte(OfsTimeNSec);
    SduData[7] = CanTSyn_LoLoByte(OfsTimeNSec);

    pduInfo.SduDataPtr = SduData;
    /* deal with CRC format */
#if (CANTSYN_CRC_SUPPORT == STD_ON)
    if (timeDomain.CanTSynGlobalTimeMaster->CanTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED)
    {
        CanTSyn_DataIDListElementType DataIDElement;
        DataIDElement = timeDomain.CanTSynGlobalTimeOfnsDataIDList[CanTSyn_SequenceCounters[domainIdx]];
        pduInfo.SduDataPtr[0] = CANTSYN_OFNS_CRC_TYPE;
        pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(&pduInfo, DataIDElement);
    }
#endif
    (void)CanIf_Transmit(pduId, &pduInfo);
}

/**
 * Receive a SYNC message as a time slave
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  PduInfoPtr: the pdu info pointer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_RxSYNC(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result;
    StbM_TimeBaseStatusType syncTimeBaseStatus;
    StbM_TimeBaseStatusType offsetTimeBaseStatus;
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    StbM_SynchronizedTimeBaseType timeBaseId = timeDomain.CanTSynSynchronizedTimeBaseId;
    uint32 T0SYNC;
    uint8 type = PduInfoPtr->SduDataPtr[0];
    uint8 sequenceCounter;
    uint8 maxJumpWidth = timeDomain.CanTSynGlobalTimeSlave->CanTSynGlobalTimeSequenceCounterJumpWidth;
    uint32 temp;

    result = StbM_GetTimeBaseStatus(timeBaseId, &syncTimeBaseStatus, &offsetTimeBaseStatus);
    if (result == E_OK)
    {
        /* Check whether sequence counter diffrence goes over maxJumpWidth */
        sequenceCounter = (PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK;
        CanTSyn_SlaveTimeInfoType* slaveTimeInfo = &CanTSyn_SlaveTimeInfo[domainIdx];
        if ((sequenceCounter <= slaveTimeInfo->sequenceCounter)
            || ((sequenceCounter - slaveTimeInfo->sequenceCounter) > maxJumpWidth))
        {
            if ((sequenceCounter != 0u) && ((syncTimeBaseStatus & CANTSYN_TIMEOUT_MASK) == 0u))
            {
                result = E_NOT_OK;
            }
        }

        if (result == E_OK)
        {
            CanTSyn_SlaveStatus[domainIdx] = FUP_EXPECTED;
            /* extract T0SYNC from pdu info */
            T0SYNC = PduInfoPtr->SduDataPtr[4];
            T0SYNC <<= 24;
            temp = PduInfoPtr->SduDataPtr[5];
            temp <<= 16;
            T0SYNC += temp;
            temp = PduInfoPtr->SduDataPtr[6];
            temp <<= 8;
            T0SYNC += temp;
            T0SYNC += PduInfoPtr->SduDataPtr[7];

            /* store extra information for follow up messages */
            slaveTimeInfo->T0SYNC = T0SYNC;
            slaveTimeInfo->userByte0 = PduInfoPtr->SduDataPtr[3];
            slaveTimeInfo->sequenceCounter = sequenceCounter;

            /* deal with CRC format */
            if (type == CANTSYN_SYNC_NOT_CRC_TYPE)
            {
                slaveTimeInfo->userByte1 = PduInfoPtr->SduDataPtr[1];
                slaveTimeInfo->isCRC = FALSE;
            }
            else
            {
                slaveTimeInfo->isCRC = TRUE;
            }
        }
    }
}

/**
 * Receive a FUP message as a time slave
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  PduInfoPtr: the pdu info pointer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_RxFUP(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result;
    StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIdx).CanTSynSynchronizedTimeBaseId;
    StbM_VirtualLocalTimeType localTimePtr;
    StbM_TimeStampType globalTimePtr;
    StbM_UserDataType userData;
    StbM_MeasurementType measurmentData;
    uint64 T5VLT;
    uint32 SyncTimeNSec;
    uint8 OVS;
    uint64 T3;
    uint32 T3Sec;
    uint8 type = PduInfoPtr->SduDataPtr[0];
    uint32 temp;

    measurmentData.pathDelay = 0u;

    CanTSyn_SlaveTimeInfoType* slaveTimeInfo = &CanTSyn_SlaveTimeInfo[domainIdx];
    /* check if SC matches */
    if (((PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK) == slaveTimeInfo->sequenceCounter)
    {
        /* get current virtual local time */
        result = StbM_BusGetCurrentTime(timeBaseId, &globalTimePtr, &localTimePtr, &userData);
        if (result == E_OK)
        {

            /* extract information from pdu info */
            SyncTimeNSec = PduInfoPtr->SduDataPtr[4];
            SyncTimeNSec <<= 24;
            temp = PduInfoPtr->SduDataPtr[5];
            temp <<= 16;
            SyncTimeNSec += temp;
            temp = PduInfoPtr->SduDataPtr[6];
            temp <<= 8;
            SyncTimeNSec += temp;
            SyncTimeNSec += PduInfoPtr->SduDataPtr[7];
            OVS = (PduInfoPtr->SduDataPtr[3] & CANTSYN_OVS_BIT_MASK);

            if ((PduInfoPtr->SduDataPtr[3] & ~CANTSYN_OVS_BIT_MASK) == 0x00u)
            {
                globalTimePtr.timeBaseStatus &= ~0x04u;
            }
            else
            {
                globalTimePtr.timeBaseStatus |= 0x04u;
            }

            userData.userByte0 = slaveTimeInfo->userByte0;
            userData.userDataLength = 1u;

            if (slaveTimeInfo->isCRC == FALSE)
            {
                userData.userByte1 = slaveTimeInfo->userByte1;
                userData.userDataLength++;
            }

            /* deal with CRC format */
            if (type == CANTSYN_FUP_NOT_CRC_TYPE)
            {
                userData.userByte2 = PduInfoPtr->SduDataPtr[1];
                userData.userDataLength++;
            }

            (void)StbM_GetCurrentVirtualLocalTime(timeBaseId, &localTimePtr);

            T5VLT = localTimePtr.nanosecondsHi;
            T5VLT = T5VLT << 32;
            T5VLT += localTimePtr.nanosecondsLo;
            T3 = T5VLT - slaveTimeInfo->T2VLT;
            T3Sec = (uint32)(T3 / CANTSYN_1NS_SECOND);
            T3 -= (uint64)T3Sec * (uint64)CANTSYN_1NS_SECOND;
            globalTimePtr.seconds = OVS + slaveTimeInfo->T0SYNC + T3Sec;
            globalTimePtr.nanoseconds = SyncTimeNSec + (uint32)T3;

            /* set new global time */
            (void)StbM_BusSetGlobalTime(timeBaseId, &globalTimePtr, &userData, &measurmentData, &localTimePtr);
        }
    }
    else
    {
        if (slaveTimeInfo->sequenceCounter == 0u)
        {
            slaveTimeInfo->sequenceCounter = 15;
        }
        else
        {
            slaveTimeInfo->sequenceCounter--;
        }
    }
}

/**
 * Receive a regular OFS message as a time slave
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  PduInfoPtr: the pdu info pointer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_RxOFS(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    CanTSyn_GlobalTimeDomainType timeDomain = CANTSYN_DOMAIN(domainIdx);
    Std_ReturnType result;
    StbM_TimeBaseStatusType syncTimeBaseStatus;
    StbM_TimeBaseStatusType offsetTimeBaseStatus;
    uint32 OfsTimeSec;
    uint8 type = PduInfoPtr->SduDataPtr[0];
    uint8 sequenceCounter = (PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK;
    uint8 maxJumpWidth = timeDomain.CanTSynGlobalTimeSlave->CanTSynGlobalTimeSequenceCounterJumpWidth;
    uint32 temp;

    /* get current time base status */
    result = StbM_GetTimeBaseStatus(
        CANTSYN_DOMAIN(domainIdx).CanTSynSynchronizedTimeBaseId,
        &syncTimeBaseStatus,
        &offsetTimeBaseStatus);

    if (result == E_OK)
    {
        CanTSyn_SlaveTimeInfoType* slaveTimeInfo = &CanTSyn_SlaveTimeInfo[domainIdx];
        /* check if SC difference goes over maxJumpWidth */
        if ((sequenceCounter <= slaveTimeInfo->sequenceCounter)
            || ((sequenceCounter - slaveTimeInfo->sequenceCounter) > maxJumpWidth))
        {
            if ((sequenceCounter != 0u) && ((offsetTimeBaseStatus & CANTSYN_TIMEOUT_MASK) == 0u))
            {
                result = E_NOT_OK;
            }
        }

        if (result == E_OK)
        {
            CanTSyn_SlaveStatus[domainIdx] = FUP_EXPECTED;
            /* extract OfsTimeSec from pdu info */
            OfsTimeSec = PduInfoPtr->SduDataPtr[4];
            OfsTimeSec <<= 24;
            temp = PduInfoPtr->SduDataPtr[5];
            temp <<= 16;
            OfsTimeSec += temp;
            temp = PduInfoPtr->SduDataPtr[6];
            temp <<= 8;
            OfsTimeSec += temp;
            OfsTimeSec += PduInfoPtr->SduDataPtr[7];

            /* store extra information into SlaveTimeInfo */
            slaveTimeInfo->OfsTimeSec = OfsTimeSec;
            slaveTimeInfo->userByte0 = PduInfoPtr->SduDataPtr[3];
            slaveTimeInfo->sequenceCounter = sequenceCounter;

            /* deal with CRC format */
            if (type == CANTSYN_OFS_NOT_CRC_TYPE)
            {
                slaveTimeInfo->userByte1 = PduInfoPtr->SduDataPtr[1];
                slaveTimeInfo->isCRC = FALSE;
            }
            else
            {
                slaveTimeInfo->isCRC = TRUE;
            }
        }
    }
}

/**
 * Receive a OFNS message as a time slave
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  PduInfoPtr: the pdu info pointer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_RxOFNS(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result;
    StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIdx).CanTSynSynchronizedTimeBaseId;
    uint8 type = PduInfoPtr->SduDataPtr[0];
    StbM_TimeStampType timeStamp;
    StbM_UserDataType userData;
    uint32 OfsTimeNSec;
    uint32 temp;

    const CanTSyn_SlaveTimeInfoType* slaveTimeInfo = &CanTSyn_SlaveTimeInfo[domainIdx];
    /* check if SC matches */
    if (((PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK) == slaveTimeInfo->sequenceCounter)
    {
        /* get current globalTimePtr, localTimePtr and userData */
        result = StbM_GetOffset(timeBaseId, &timeStamp, &userData);
        if (result == E_OK)
        {
            /* extract OfsTimeNSec from pdu info */
            OfsTimeNSec = PduInfoPtr->SduDataPtr[4];
            OfsTimeNSec <<= 24;
            temp = PduInfoPtr->SduDataPtr[5];
            temp <<= 16;
            OfsTimeNSec += temp;
            temp = PduInfoPtr->SduDataPtr[6];
            temp <<= 8;
            OfsTimeNSec += temp;
            OfsTimeNSec += PduInfoPtr->SduDataPtr[7];

            /* update globaltimePtr and userData */
            timeStamp.secondsHi = 0u;
            timeStamp.seconds = slaveTimeInfo->OfsTimeSec;
            timeStamp.nanoseconds = OfsTimeNSec;
            if (PduInfoPtr->SduDataPtr[3] == 0x00u)
            {
                timeStamp.timeBaseStatus &= ~0x04u;
            }
            else
            {
                timeStamp.timeBaseStatus |= 0x04u;
            }

            userData.userByte0 = slaveTimeInfo->userByte0;
            userData.userDataLength = 1u;

            /* deal with CRC format */
            if (slaveTimeInfo->isCRC == FALSE)
            {
                userData.userByte1 = slaveTimeInfo->userByte1;
                userData.userDataLength++;
            }
            if (type == CANTSYN_OFNS_NOT_CRC_TYPE)
            {
                userData.userByte2 = PduInfoPtr->SduDataPtr[1];
                userData.userDataLength++;
            }

            /* set new global time */
            (void)StbM_SetOffset(timeBaseId, &timeStamp, &userData);
        }
    }
}

/**
 * Receive a OFS FD message as a time slave
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 *                  PduInfoPtr: the pdu info pointer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

static void CanTSyn_RxOFSFD(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result;
    StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIdx).CanTSynSynchronizedTimeBaseId;
    uint8 type = PduInfoPtr->SduDataPtr[0];
    uint32 OfsTimeSec;
    uint32 OfsTimeNSec;
    uint32 temp;
    StbM_VirtualLocalTimeType localTimePtr;
    StbM_TimeStampType globalTimePtr;
    StbM_UserDataType userData;
    uint8 sequenceCounter = (PduInfoPtr->SduDataPtr[2]) & CANTSYN_SEQUENCE_COUNTER_BIT_MASK;
    uint8 maxJumpWidth = CANTSYN_DOMAIN(domainIdx).CanTSynGlobalTimeSlave->CanTSynGlobalTimeSequenceCounterJumpWidth;

    /* get current globalTimePtr, localTimePtr and userData */
    result = StbM_BusGetCurrentTime(timeBaseId, &globalTimePtr, &localTimePtr, &userData);

    if (result == E_OK)
    {
        CanTSyn_SlaveTimeInfoType* slaveTimeInfo = &CanTSyn_SlaveTimeInfo[domainIdx];
        /* check if SC difference goes over maxJumpWidth */
        if ((sequenceCounter <= slaveTimeInfo->sequenceCounter)
            || ((sequenceCounter - slaveTimeInfo->sequenceCounter) > maxJumpWidth))
        {
            if ((sequenceCounter != 0u) && ((globalTimePtr.timeBaseStatus & CANTSYN_TIMEOUT_MASK) == 0u))
            {
                result = E_NOT_OK;
            }
        }

        if (result == E_OK)
        {
            slaveTimeInfo->sequenceCounter = sequenceCounter;

            /* extract OfsTimeSec and OfsTimeNSec from pdu info */
            OfsTimeSec = PduInfoPtr->SduDataPtr[8];
            OfsTimeSec <<= 24;
            temp = PduInfoPtr->SduDataPtr[9];
            temp <<= 16;
            OfsTimeSec += temp;
            temp = PduInfoPtr->SduDataPtr[10];
            temp <<= 8;
            OfsTimeSec += temp;
            OfsTimeSec += PduInfoPtr->SduDataPtr[11];
            OfsTimeNSec = PduInfoPtr->SduDataPtr[12];
            OfsTimeNSec <<= 24;
            temp = PduInfoPtr->SduDataPtr[13];
            temp <<= 16;
            OfsTimeNSec += temp;
            temp = PduInfoPtr->SduDataPtr[14];
            temp <<= 8;
            OfsTimeNSec += temp;
            OfsTimeNSec += PduInfoPtr->SduDataPtr[15];

            /* update globalTimePtr and userData */
            globalTimePtr.seconds = OfsTimeSec;
            globalTimePtr.nanoseconds = OfsTimeNSec;
            if (PduInfoPtr->SduDataPtr[3] == 0x00u)
            {
                globalTimePtr.timeBaseStatus &= ~0x04u;
            }
            else
            {
                globalTimePtr.timeBaseStatus |= 0x04u;
            }
            userData.userByte0 = PduInfoPtr->SduDataPtr[4];
            userData.userByte1 = PduInfoPtr->SduDataPtr[5];
            userData.userDataLength = 2u;

            /* deal with CRC format */
            if (type == CANTSYN_OFS_NOT_CRC_FD_TYPE)
            {
                userData.userByte2 = PduInfoPtr->SduDataPtr[1];
                userData.userDataLength++;
            }

            /* set new global time */
            (void)StbM_SetOffset(timeBaseId, &globalTimePtr, &userData);
        }
    }
}

/**
 * Check if GLOBAL_TIME_BASE bit is set or not
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters(IN):  domainIdx: the index of global time domain
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: TRUE: bit is set
 *               FALSE: bit is not set
 *
 */

static boolean CanTSyn_CheckGlobalTimeBaseBit(uint8 domainIdx)
{
    Std_ReturnType ret;
    boolean result = TRUE;
    StbM_TimeBaseStatusType syncTimeBaseStatus;
    StbM_TimeBaseStatusType offsetTimeBaseStatus;

    /* get current time base status */
    ret = StbM_GetTimeBaseStatus(
        CANTSYN_DOMAIN(domainIdx).CanTSynSynchronizedTimeBaseId,
        &syncTimeBaseStatus,
        &offsetTimeBaseStatus);
    if (ret == E_OK)
    {
        /* check if it is a synchronized time base */
        if (CANTSYN_DOMAIN(domainIdx).CanTSynGlobalTimeDomainId <= 15u)
        {
            if ((syncTimeBaseStatus & CANTSYN_GLOBAL_TIME_BASE_MASK) == 0u)
            {
                result = FALSE;
            }
        }
        else /* it is an offset time base */
        {
            if ((offsetTimeBaseStatus & CANTSYN_GLOBAL_TIME_BASE_MASK) == 0u)
            {
                result = FALSE;
            }
        }
    }
    else
    {
        result = FALSE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
