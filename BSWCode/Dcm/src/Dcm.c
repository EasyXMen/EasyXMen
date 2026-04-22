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
 **  @file               : Dcm.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Public Functions Implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Dem.h"
#include "Dcm_Internal.h"
#if (STD_ON == DCM_MULTICORE_ENABLED)
#include "Os.h"
#endif
/* ============================================ internal data definitions =========================================== */
#ifdef DCM_VIN_REF
/**
 * @brief Vehicle Info Data Buffer
 */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
DCM_LOCAL uint8 Dcm_Vin[DCM_VIN_SIZE];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif

#ifdef DCM_VIN_REF
/**
 * @brief whether vehicle info data has been retrived or not
 */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
DCM_LOCAL boolean Dcm_VinObtained; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * initiailize global variables
 */
/* PRQA S 1503 ++ */ /* VL_Dcm_1503 */
void Dcm_Init(const Dcm_ConfigType* ConfigPtr)
/* PRQA S 1503 -- */
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        DCM_DET_REPORT(DCM_INIT_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        Dcm_CfgPtr = ConfigPtr;
        DslInternal_ProtocolInit();
        DcmInternal_ProgCtrlInit();
        DslInternal_SessionInit();
        DslInternal_SecurityInit();
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        Dcm_QueuedIndex                                 = 0u;
        Dcm_QueuedRequestCtrl[0].QueuedRequestOldBuffer = NULL_PTR;
        Dcm_QueuedRequestCtrl[1].QueuedRequestOldBuffer = NULL_PTR;
        DslInternal_InitChannelQueuedRequestCtrl(0u);
        DslInternal_InitChannelQueuedRequestCtrl(1u);
#endif
#if (STD_ON == DCM_UDS_0X86)
        Dcm_UDS0x86_Init();
#endif
#if (STD_ON == DCM_UDS_0X29)
        Dcm_UDS0x29_Init();
#endif
#if (STD_ON == DCM_UDS_0X2E)
#ifdef DCM_SECURE_CODING_DID_NUM
        Dcm_UDS0x2E_Init();
#endif
#endif
#ifdef DCM_VIN_REF
        (void)Dcm_GetVin(Dcm_Vin);
#endif
    }
}

/**
 * periodically deal with asynchronous functionalities
 */
/* PRQA S 1503 ++ */                          /* VL_Dcm_1503 */
void Dcm_MainFunction(void) /* PRQA S 6070 */ /* VL_MTR_Dcm_STCAL */
/* PRQA S 1503 -- */
{
    if (NULL_PTR != Dcm_CfgPtr)
    {
#if ((STD_ON == DCM_UDS_0X27) && (STD_ON == DCM_SECURITY_READOUT))
        /* only process when there is a pending get/set attemptCounter operation */
        if (Dcm_PendingGetAttemptCounter)
        {
            Dcm_UDS0x27_GetAttemptCounter();
        }
        if (Dcm_PendingSetAttemptCounter)
        {
            Dcm_UDS0x27_SetAttemptCounterHandle();
        }
#endif
        /* iterate over timers to check for timeout and decrement timer */
        DcmInternal_CheckTimer();
        /* process asynchronous TxConfirmation */
        DcmInternal_ProcessTxConfirmation();
        /* process asynchronous RxIndication */
        DcmInternal_ProcessRxIndication();
        /* process asynchronous diagnostic request or queued request */
        DcmInternal_ProcessRequest();
        if (Dcm_PendingSetProg)
        {
            Std_ReturnType result = DcmInternal_SetProgConditions(DCM_PENDING);
            if ((E_OK == result) || (E_NOT_OK == result))
            {
                Dcm_PendingSetProg = FALSE;
            }
        }
        /* UDS service mainfunction for periodic session/security/modeRule change detection and periodic transmission */
#if (                        \
    (STD_ON == DCM_UDS_0X28) \
    && ((DCM_COMCONTROL_ALL_NUM > 0) || (DCM_COMCONTROL_SPE_NUM > 0) || (DCM_COMCONTROL_SUB_NUM > 0)))
        Dcm_UDS0x28_MainFunction();
#endif
#if ((STD_ON == DCM_UDS_0X2A) && (STD_ON == DCM_DYN_DID) && (DCM_PERIODIC_CONNECTION_NUM > 0u))
        Dcm_UDS0x2A_MainFunction();
#endif
#if (STD_ON == DCM_UDS_0X29)
#ifdef DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT
        Dcm_UDS0x29_MainFunction();
#endif
#endif
#if ((STD_ON == DCM_UDS_0X85) && (STD_ON == DCM_UDS_0X85_MODERULE))
        Dcm_UDS0x85_MainFunction();
#endif
    }
}

#if (STD_ON == DCM_MULTICORE_ENABLED)
/**
 * process transmission on targeted application
 */
void Dcm_SatelliteMainFunction(void)
{
    ApplicationType CurrentApplication = GetApplicationID();
    /* iterate over protocolrows to find message pending to be sent due to different application */
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[index];
        if ((DCM_MSG_SENDING == protocolCtrlPtr->State)
            && (CurrentApplication == Dcm_DslMainConnection[protocolCtrlPtr->ConnectionId].ApplicationId))
        {
            DslInternal_ProcessTransmit(index);
        }
    }
}
#endif

/**
 * retrive version info from macros
 */
void Dcm_GetVersionInfo(Std_VersionInfoType* versionInfo) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == versionInfo)
    {
        DCM_DET_REPORT(DCM_GETVERSIONINFO_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versionInfo->vendorID         = DCM_VENDOR_ID;
        versionInfo->moduleID         = DCM_MODULE_ID;
        versionInfo->sw_major_version = DCM_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = DCM_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = DCM_SW_PATCH_VERSION;
    }
}

/**
 * called by dem to notify DTC status change (potentially triggering roe)
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType
    Dcm_DemTriggerOnDTCStatus(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
    DCM_UNUSED(DTC);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_DEMTRIGGERONDTCSTATUS_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
#if (STD_ON == DCM_UDS_0X86)
#if (STD_ON == DCM_UDS_0X14)
        /* do not trigger during processing clearing DTC */
        if (FALSE == Dcm_ProcessingClearingDiagInfo)
#endif
        {
            DspInternal_handleDTCRoe(DTCStatusOld, DTCStatusNew);
        }
#else
        DCM_UNUSED(DTCStatusNew);
        DCM_UNUSED(DTCStatusOld);
#endif
    }
    return E_OK;
}

/**
 * get vehicle info data (called by DoIP)
 */
/* PRQA S 3673++ */ /* VL_QAC_3673 */
Std_ReturnType Dcm_GetVin(uint8* Data)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_GETVIN_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
#ifdef DCM_VIN_REF
        if (FALSE == Dcm_VinObtained)
        {
            DcmInternal_Memset(Data, 0u, DCM_VIN_SIZE);
            /* iterate over configured did's signals, execute initial read */
            for (uint16 sigIndex = Dcm_DspDid[DCM_VIN_REF].DidSignalIndexStart;
                 sigIndex <= Dcm_DspDid[DCM_VIN_REF].DidSignalIndexEnd;
                 sigIndex++)
            {
                Dcm_NegativeResponseCodeType errorCode;
                result = Dcm_DspData[sigIndex].DspDataReadFnc(
                    DCM_INITIAL,
                    &Data[Dcm_DspDidSignal[sigIndex].DidByteOffset],
                    &errorCode);
                if (result != E_OK)
                {
                    break;
                }
            }
            if (result == E_OK)
            {
                Dcm_VinObtained = TRUE;
            }
        }
        else
        {
            DcmInternal_Memcpy(Data, Dcm_Vin, DCM_VIN_SIZE);
            result = E_OK;
        }
#else
        DCM_UNUSED(Data);
#endif
    }
    return result;
}
/* PRQA S 3673-- */ /* VL_QAC_3673 */

/**
 * get current security level
 */
/* PRQA S 1503++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Dcm_GetSecurityLevel(Dcm_SecLevelType* SecLevel)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_GETSECURITYLEVEL_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        *SecLevel = Dcm_CurrentSecurity;
    }
    return E_OK;
}
/* PRQA S 1503-- */ /* VL_QAC_NoUsedApi */

/**
 * get current session level
 */
/* PRQA S 1503++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Dcm_GetSesCtrlType(Dcm_SesCtrlType* SesCtrlType)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_GETSESCTRLTYPE_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        *SesCtrlType = Dcm_SessionCtrl.CurrentSession;
    }
    return E_OK;
}
/* PRQA S 1503-- */ /* VL_QAC_NoUsedApi */

/**
 * set session to default session
 */
/* PRQA S 1503++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Dcm_ResetToDefaultSession(void)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_RESETTODEFAULTSESSION_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        DslInternal_ResetToDefaultSession(TRUE);
    }
    return E_OK;
}
/* PRQA S 1503-- */ /* VL_QAC_NoUsedApi */

#if (STD_ON == DCM_USE_BNDM)
/**
 * callbacks for BndM asynchronous jobs finish notification
 */
void Dcm_BndMWriteBlockFinish(BndM_BlockIdType BlockId, BndM_ResultType result)
{
    for (uint16 index = 0u; index < DCM_DID_NUM; index++)
    {
        if (BlockId == Dcm_DspDid[index].DidBndMBlockIdRef)
        {
            Dcm_BndMResult[Dcm_DspDid[index].DidBndMIndex] = (Std_ReturnType)result;
        }
    }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
