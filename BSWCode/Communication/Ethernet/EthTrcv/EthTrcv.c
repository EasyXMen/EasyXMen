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
**  FILENAME    : EthTrcv.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V2.0.0       2020-08-18  wanglili        R19_11 initial version.
 *  V2.0.1       2021-02-25  wanglili
 *      1.UnitTest bug update.
 *  V2.0.2       2021-05-13  wanglili
 *      1.Modification after integration test
 *  V2.0.3       2021-06-17  wanglili
 *      1.Add configuration item EthTrcvSetPhyTxModeApi
 *  V2.0.4       2023-9-1   shaoqiangLiang
 *    Modify  to configure registers
 *  V2.0.5       2023-10-07  zhaotong
 *    Delete 422 and 440 version code,only R19 code reserve
 *  V2.0.6       2024-04-25 fupeng.yu
 *    add the judgment of module initialization fo EthTrcv_MainFunction<XX>
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EthTrcv<br>
  RuleSorce:puhua-rule2024-2.rcf 2.3.5

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:ETHTRCV_TRCVS_NUM follows configuration changes.

    \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:That a variable is used by more than one function is a matter of configuration (EthTrcv_ModuleState).

 */
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define ETHTRCV_VENDOR_ID_C                62U
#define ETHTRCV_C_AR_RELEASE_MAJOR_VERSION 4u
#define ETHTRCV_C_AR_RELEASE_MINOR_VERSION 5u
#define ETHTRCV_C_AR_RELEASE_PATCH_VERSION 0u
#define ETHTRCV_C_SW_MAJOR_VERSION         2u
#define ETHTRCV_C_SW_MINOR_VERSION         0u
#define ETHTRCV_C_SW_PATCH_VERSION         5u
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthTrcv_Internal.h"
#include "EthTrcv.h"
#include "EthIf_Cbk.h"
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
#include "EcuM_Cbk.h"
#endif
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (ETHTRCV_C_AR_RELEASE_MAJOR_VERSION != ETHTRCV_H_AR_RELEASE_MAJOR_VERSION)
#error "EthTrcv.c : Mismatch in Specification Major Version"
#endif
#if (ETHTRCV_C_AR_RELEASE_MINOR_VERSION != ETHTRCV_H_AR_RELEASE_MINOR_VERSION)
#error "EthTrcv.c : Mismatch in Specification _MINOR Version"
#endif
#if (ETHTRCV_C_AR_RELEASE_PATCH_VERSION != ETHTRCV_H_AR_RELEASE_PATCH_VERSION)
#error "EthTrcv.c : Mismatch in Specification PATCH Version"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#if (ETHTRCV_SET_TRCV_MODE == STD_ON)
static FUNC(void, ETHTRCV_CODE) EthTrcv_SetTrcvModeActive(uint8 TrcvIdx, Std_ReturnType* retVal);

static FUNC(void, ETHTRCV_CODE) EthTrcv_SetTrcvModeDown(uint8 TrcvIdx, Std_ReturnType* retVal);
#endif /* ETHTRCV_SET_TRCV_MODE == STD_ON */

static FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_LinkStateRequest(uint8 TrcvIdx, EthTrcv_LinkStateType LinkState);
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define ETHTRCV_START_SEC_VAR_INIT_8
#include "EthTrcv_MemMap.h"
/* PRQA S 3218 ++ */ /* MISRA Rule 8.9 */
static VAR(EthTrcv_StateType, ETHTRCV_VAR_ZERO_INIT) EthTrcv_ModuleState = ETHTRCV_STATE_UNINIT;
/* PRQA S 3218 -- */ /* MISRA Rule 8.9 */
#define ETHTRCV_STOP_SEC_VAR_INIT_8
#include "EthTrcv_MemMap.h"

#define ETHTRCV_START_SEC_VAR_NO_INIT_8
#include "EthTrcv_MemMap.h"
static VAR(Eth_ModeType, ETHTRCV_VAR_NOINIT) EthTrcv_TrcvMode[ETHTRCV_TRCVS_NUM];
static VAR(EthTrcv_LinkStateType, ETHTRCV_VAR_NOINIT) EthTrcv_LinkState[ETHTRCV_TRCVS_NUM];

#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
static VAR(EthTrcv_WakeupModeType, ETHTRCV_VAR_NOINIT) EthTrcv_WakeupMode[ETHTRCV_TRCVS_NUM];
#endif /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/

VAR(Eth_ModeType, ETHTRCV_VAR_NOINIT) EthIf_ReqTrcvMode[ETHTRCV_TRCVS_NUM];
#define ETHTRCV_STOP_SEC_VAR_NO_INIT_8
#include "EthTrcv_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#define ETHTRCV_START_SEC_VAR_INIT_PTR
#include "EthTrcv_MemMap.h"
static P2CONST(EthTrcv_ConfigType, ETHTRCV_CONST, ETHTRCV_CONST) EthTrcv_TrcvPbCfg = NULL_PTR;
#define ETHTRCV_STOP_SEC_VAR_INIT_PTR
#include "EthTrcv_MemMap.h"

#define ETHTRCV_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "EthTrcv_MemMap.h"
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
static VAR(EthTrcv_CableDiagStateType, ETHTRCV_VAR_NOINIT) EthTrcv_CableDiagState[ETHTRCV_TRCVS_NUM];
#endif /* STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API*/
#define ETHTRCV_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "EthTrcv_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ETHTRCV_START_SEC_CODE
#include "EthTrcv_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the Ethernet Transceiver Driver
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CfgPtr, Points to the implementation specific structure
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, ETHTRCV_CODE)
EthTrcv_Init(P2CONST(EthTrcv_ConfigType, ETHTRCV_CONST, ETHTRCV_CONST) CfgPtr)
{
    uint8 trcvIdx;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
    EcuM_WakeupSourceType wakeupSourceValue;
#endif /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (CfgPtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        EthTrcv_TrcvPbCfg = CfgPtr;
        for (trcvIdx = 0; trcvIdx < ETHTRCV_TRCVS_NUM; trcvIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            EthTrcv_TrcvMode[trcvIdx] = ETH_MODE_DOWN;
            EthTrcv_LinkState[trcvIdx] = ETHTRCV_LINK_STATE_DOWN;
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
            EthTrcv_CableDiagState[trcvIdx].CableDiagState = ETHTRCV_CABLEDIAG_OK;
            EthTrcv_CableDiagState[trcvIdx].PendingSetTrcvModeReq = FALSE;
            EthTrcv_CableDiagState[trcvIdx].PendingTrcvLinkStateReq = FALSE;
#endif /* STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API*/
            /*@req <SWS_EthTrcv_00040>*/
            CtrlIdx = EthTrcv_TrcvPbCfg[trcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
            MiiIdx = EthTrcv_TrcvPbCfg[trcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
            if (EthTrcv_IsTrcvAccessOk(CtrlIdx, MiiIdx) == E_OK)
            {
                ETHTRCV_DEM_REPORT(trcvIdx, DEM_EVENT_STATUS_PREPASSED);
                /*Based on configuration parameters,Init transceiver Register*/
                EthTrcv_InitCfgHandle(&EthTrcv_TrcvPbCfg[trcvIdx]);
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
                EthTrcv_WakeupMode[trcvIdx] = ETHTRCV_WUM_ENABLE;
                if ((uint8)E_OK == EthTrcv_GetWakeupReasonHandle(trcvIdx, &wakeupSourceValue))
                {
                    /*@req <SWS_EthTrcv_00115>*/
                    EcuM_SetWakeupEvent(wakeupSourceValue);
                }
#endif /* ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED */
            }
            else
            {
                ETHTRCV_DEM_REPORT(trcvIdx, DEM_EVENT_STATUS_PREFAILED)
            }
        }
        EthTrcv_ModuleState = ETHTRCV_STATE_INIT;
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_INIT, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return;
}

/******************************************************************************/
/*
 * Brief               Enables / disables the indexed transceiver
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the transceiver within the context of
 *                     the Ethernet Transceiver Driver
 *                     TrcvMode, ETH_MODE_DOWN: disable the transceiver
 *                     ETH_MODE_ACTIVE: enable the transceiver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Service accepted  E_NOT_OK: Service denied
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_SET_TRCV_MODE)
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetTransceiverMode(uint8 TrcvIdx, Eth_ModeType TrcvMode)
{
    Std_ReturnType retVal = E_NOT_OK;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if ((TrcvMode != ETH_MODE_DOWN) && (TrcvMode != ETH_MODE_ACTIVE))
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        /* check the access to the Ethernet transceiver */
        /*@req <SWS_EthTrcv_00104>*/
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        if (EthTrcv_IsTrcvAccessOk(CtrlIdx, MiiIdx) == E_OK)
        {
            ETHTRCV_DEM_REPORT(TrcvIdx, DEM_EVENT_STATUS_PREPASSED)
            /* the transceiver is already in the requested mode*/
            /*@req <SWS_EthTrcv_00094>*/
            if (TrcvMode == EthTrcv_TrcvMode[TrcvIdx])
            {
                retVal = E_OK;
            }
            /* change the transceiver mode to the requested mode*/
            else
            {
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
                if (ETHTRCV_CABLEDIAG_PENDING == EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
                {
                    /*@req <SWS_EthTrcv_00163> <SWS_EthTrcv_00118>*/
                    EthTrcv_CableDiagState[TrcvIdx].PendingSetTrcvModeReq = TRUE;
                    EthTrcv_CableDiagState[TrcvIdx].PendingSetTrcvModeReqMode = TrcvMode;
                    retVal = E_OK;
                }
                else
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
                {

                    EthIf_ReqTrcvMode[TrcvIdx] = TrcvMode;
                    if (TrcvMode == ETH_MODE_ACTIVE)
                    {
                        EthTrcv_SetTrcvModeActive(TrcvIdx, &retVal);
                    }
                    else
                    {
                        EthTrcv_SetTrcvModeDown(TrcvIdx, &retVal);
                    }
                }
            }
        }
        else
        {
            ETHTRCV_DEM_REPORT(TrcvIdx, DEM_EVENT_STATUS_PREFAILED)
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_SET_TRANSCEIVER_MODE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* STD_ON == ETHTRCV_SET_TRCV_MODE */

/******************************************************************************/
/*
 * Brief               Obtains the state of the indexed transceiver
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the transceiver within the context of
 *                     the Ethernet Transceiver Driver
 * Param-Name[out]     TrcvModePtr, ETH_MODE_DOWN: disable the transceiver
 *                     ETH_MODE_ACTIVE: enable the transceiver
 * Param-Name[in/out]  NA
 * Return              E_OK: success E_NOT_OK: transceiver could not be initialized
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_GET_TRCV_MODE)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetTransceiverMode(uint8 TrcvIdx, P2VAR(Eth_ModeType, AUTOMATIC, ETHTRCV_APPL_DATA) TrcvModePtr)
{
    Std_ReturnType retVal;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if (TrcvModePtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        retVal = EthTrcv_GetTrcvModeHandle(CtrlIdx, MiiIdx, TrcvModePtr);
    }
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GET_TRANSCEIVER_MODE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_TRCV_MODE */

/******************************************************************************/
/*
 * Brief               Restarts the negotiation of the transmission parameters
 *                     used by the indexed transceiver
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: success
 *                     E_NOT_OK: transceiver could not be initialized
 */
/******************************************************************************/
/* Caveat: The function is not required or called by an upper layer BSW software component. */
#if (ETHTRCV_START_AUTO_NEG == STD_ON)
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_StartAutoNegotiation(uint8 TrcvIdx)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
    {
        /*do nothing*/
    }
    if (ETHTRCV_E_NO_ERROR == errorId)
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
/*@req <SWS_EthTrcv_00056>*/
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
        if (ETHTRCV_CABLEDIAG_PENDING != EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
        {
            retVal = EthTrcv_StartAutoNegotiationHandle(CtrlIdx, MiiIdx);
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_START_AUTO_NEG, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* ETHTRCV_START_AUTO_NEG == STD_ON */

/******************************************************************************/
/*
 * Brief               Request the given link state for the given Ethernet
 *                     transceiver
 * ServiceId           0x17(the value not defined in R19-11)
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 *                     LinkState, The Ethernet link state of a physical
 *                     Ethernet connection
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_TransceiverLinkStateRequest(uint8 TrcvIdx, EthTrcv_LinkStateType LinkState)
{
    Std_ReturnType retVal;
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        if (LinkState == EthTrcv_LinkState[TrcvIdx])
        {
            retVal = E_OK;
        }
        else
        {
/*@req <SWS_EthTrcv_00165>*/
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
            if (ETHTRCV_CABLEDIAG_PENDING == EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
            {
                EthTrcv_CableDiagState[TrcvIdx].PendingTrcvLinkStateReq = TRUE;
                EthTrcv_CableDiagState[TrcvIdx].PendingTrcvLinkStateReqMode = LinkState;
                retVal = E_OK;
            }
            else
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
            {
                retVal = EthTrcv_LinkStateRequest(TrcvIdx, LinkState);
            }
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_TRANSCEIVERLINKSTATEREQUEST, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
/******************************************************************************/
/*
 * Brief               Obtains the link state of the indexed transceiver
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the transceiver within the context of
 *                     the Ethernet Transceiver Driver
 * Param-Name[out]     LinkStatePtr, ETHTRCV_LINK_STATE_DOWN: transceiver is
 *                     disconnected  ETHTRCV_LINK_STATE_ACTIVE: transceiver is
 *                     connected
 * Param-Name[in/out]  NA
 * Return              E_OK: success
 *                     E_NOT_OK: transceiver could not be initialized
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_GET_LINK_STATE)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetLinkState(
    uint8 TrcvIdx,
    P2VAR(EthTrcv_LinkStateType, AUTOMATIC, ETHTRCV_APPL_DATA) LinkStatePtr

)

{
    Std_ReturnType retVal;
    boolean result;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if (LinkStatePtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        if (ETH_MODE_ACTIVE == EthTrcv_TrcvMode[TrcvIdx])
        {
            /* get the PHY link state*/
            retVal = EthTrcv_GetLinkStateHandle(CtrlIdx, MiiIdx, &result);
            if (retVal == (uint8)E_OK)
            {
                if (result == (boolean)TRUE)
                /* The link state register returns link state ACTIVE */
                {
                    *LinkStatePtr = ETHTRCV_LINK_STATE_ACTIVE;
                }
                /* The link state register returns link state DOWN */
                else
                {
                    *LinkStatePtr = ETHTRCV_LINK_STATE_DOWN;
                }
            }
        }
        else
        {
            *LinkStatePtr = ETHTRCV_LINK_STATE_DOWN;
            retVal = E_OK;
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GET_LINK_STATE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_LINK_STATE */

/******************************************************************************/
/*
 * Brief               Obtains the baud rate of the indexed transceiver
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the transceiver within the context of
 *                     the Ethernet Transceiver Driver
 * Param-Name[out]     BaudRatePtr, ETHTRCV_BAUD_RATE_10MBIT: 10MBit connection
 *                     ETHTRCV_BAUD_RATE_100MBIT: 100MBit connection
 *                     ETHTRCV_BAUD_RATE_1000MBIT: 1000MBit connection
 *                     ETHTRCV_BAUD_RATE_2500MBIT: 2500MBit connection
 * Param-Name[in/out]  NA
 * Return              E_OK: success
 *                     E_NOT_OK: transceiver could not be initialized
 */
/******************************************************************************/
/* The function is not required or called by an upper layer BSW software component */
#if (STD_ON == ETHTRCV_GET_BAUD_RATE)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetBaudRate(uint8 TrcvIdx, P2VAR(EthTrcv_BaudRateType, AUTOMATIC, ETHTRCV_APPL_DATA) BaudRatePtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if (BaudRatePtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        retVal = EthTrcv_GetBaudRateHandle(CtrlIdx, MiiIdx, BaudRatePtr);
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GET_BAUD_RATE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_BAUD_RATE */

/******************************************************************************/
/*
 * Brief               Obtains the duplex mode of the indexed transceiver
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     DuplexModePtr, ETHTRCV_DUPLEX_MODE_HALF: half duplex
 *                     connections ETHTRCV_DUPLEX_MODE_FULL: full duplex connection
 * Param-Name[in/out]  NA
 * Return              E_OK: success
                       E_NOT_OK: transceiver could not be initialized
 */
/******************************************************************************/
/* The function is not required or called by an upper layer BSW software component */
#if (STD_ON == ETHTRCV_GET_DUPLEX_MODE)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetDuplexMode(uint8 TrcvIdx, P2VAR(EthTrcv_DuplexModeType, AUTOMATIC, ETHTRCV_APPL_DATA) DuplexModePtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if (DuplexModePtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        retVal = EthTrcv_GetDuplexModeHandle(CtrlIdx, MiiIdx, DuplexModePtr);
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GET_DUPLEX_MODE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_DUPLEX_MODE */

/******************************************************************************/
/*
 * Brief               Enables / disables the wake-up mode or clear the wake-up
 *                     reason of the indexed transceiver
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 *                     TrcvWakeupMode, ETHTRCV_WUM_DISABLE: disable transceiver
 *                     wake up ETHTRCV_WUM_ENABLE: enable transceiver wake up
 *                     ETHTRCV_WUM_CLEAR: clears transceiver wake up reason
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: transceiver wake up mode has been changed.
 *                     E_NOT_OK: transceiver wake up mode could not be changed
 *                     or the wake-up reason could not be cleared.
 */
/******************************************************************************/
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_SetTransceiverWakeupMode(uint8 TrcvIdx, EthTrcv_WakeupModeType TrcvWakeupMode)
{
    Std_ReturnType retVal = E_NOT_OK;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
        /*@req <SWS_EthTrcv_00120> <SWS_EthTrcv_00121> <SWS_EthTrcv_00164>*/
        if (ETHTRCV_CABLEDIAG_PENDING != EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
        {
            switch (TrcvWakeupMode)
            {
            case ETHTRCV_WUM_DISABLE:
            case ETHTRCV_WUM_ENABLE:
                EthTrcv_WakeupMode[TrcvIdx] = TrcvWakeupMode;
                retVal = E_OK;
                break;
            case ETHTRCV_WUM_CLEAR:
                retVal = EthTrcv_ClearWakeupReasonsHandle(CtrlIdx, MiiIdx);
                break;
            default:
                /*nothing*/
                break;
            }
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_SET_TRANSCEIVER_WAKEUP_MODE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}

/******************************************************************************/
/*
 * Brief               Returns the wake up mode of the indexed transceiver
 * ServiceId           0x0e
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     TrcvWakeupModePtr, ETHTRCV_WUM_DISABLE: transceiver wake
 *                     up is disabled ETHTRCV_WUM_ENABLE: transceiver wake up is
 *                     enabled
 * Param-Name[in/out]  NA
 * Return              E_OK: success
                       E_NOT_OK: transceiver wake up mode could not be obtained
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_GET_TRCV_WAKEUP_MODE)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetTransceiverWakeupMode(
    uint8 TrcvIdx,
    P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHTRCV_APPL_DATA) TrcvWakeupModePtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if (TrcvWakeupModePtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        *TrcvWakeupModePtr = EthTrcv_WakeupMode[TrcvIdx];
        retVal = E_OK;
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GET_TRANSCEIVER_WAKEUP_MODE, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_TRCV_WAKEUP_MODE */

/******************************************************************************/
/*
 * Brief               Service is called by EthIf in case a wake-up interrupt
 *                     is detected.
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: The function has been successfully executed
 *                     E_NOT_OK: The function could not be successfully executed
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_CheckWakeup(uint8 TrcvIdx)
{
    Std_ReturnType retVal = E_NOT_OK;
    EcuM_WakeupSourceType wakeupSourceValue;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        if (ETHTRCV_WUM_ENABLE == EthTrcv_WakeupMode[TrcvIdx])
        {
            /*check the wakeup reason and report to EcuM*/
            retVal = EthTrcv_GetWakeupReasonHandle(CtrlIdx, MiiIdx, &wakeupSourceValue);
            if ((uint8)E_OK == retVal)
            {
                EcuM_SetWakeupEvent(wakeupSourceValue);
            }
        }
        else
        {
            /*If the wake-up mode of the corresponding transceiver is not
            ETHTRCV_WUM_ENABLE, the function EthTrcv_CheckWakeup() shall return
            E_OK*/
            retVal = E_OK;
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_CHECK_WAKEUP, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
    return retVal;
}
#endif /* ETHTRCV_WAKEUP_NOT_SUPPORTED != ETHTRCV_WAKEUP_SUPPORT */

#if (STD_ON == ETHTRCV_SET_PHYTESTMODE_API)
/******************************************************************************/
/*
 * Brief               Activates a given test mode.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 *                     Mode, Test mode to be activated
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetPhyTestMode(uint8 TrcvIdx, EthTrcv_PhyTestModeType Mode)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
/*@req <SWS_EthTrcv_00166>*/
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
        if (ETHTRCV_CABLEDIAG_PENDING != EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
        {
            if ((uint8)E_OK == EthTrcv_CheckPhyTestModeSupport(CtrlIdx, MiiIdx, Mode))
            {
                retVal = EthTrcv_SetPhyTestModeHandle(CtrlIdx, MiiIdx, Mode);
            }
            else
            {
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
                (void)Det_ReportError(
                    ETHTRCV_MODULE_ID,
                    ETHTRCV_INSTANCE,
                    ETHTRCV_SID_SETPHYTESTMODE,
                    ETHTRCV_E_NOT_SUPPORTED);
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
            }
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_SETPHYTESTMODE, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
#endif /* STD_ON == ETHTRCV_SET_PHYTESTMODE_API */

/******************************************************************************/
/*
 * Brief               Activates a given loopback mode.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 *                     Mode, Loopback mode to be activated
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_SetPhyLoopbackMode(uint8 TrcvIdx, EthTrcv_PhyLoopbackModeType Mode)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
/*@req <SWS_EthTrcv_00167>*/
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
        if (ETHTRCV_CABLEDIAG_PENDING != EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
        {
            if ((uint8)E_OK == EthTrcv_CheckPhyLoopBackSupport(CtrlIdx, MiiIdx, Mode))
            {
                retVal = EthTrcv_SetPhyLoopBackModeHandle(CtrlIdx, MiiIdx, Mode);
            }
            else
            {
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
                (void)Det_ReportError(
                    ETHTRCV_MODULE_ID,
                    ETHTRCV_INSTANCE,
                    ETHTRCV_SID_SETPHYLOOPBACKMODE,
                    ETHTRCV_E_NOT_SUPPORTED);
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
            }
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_SETPHYLOOPBACKMODE, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}

#if (STD_ON == ETHTRCV_GET_PHYSIGNALQUALITY_API)
/******************************************************************************/
/*
 * Brief               Obtains the current signal quality of the link of the
 *                     indexed transceiver
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     SignalQualityPtr, Pointer to the memory where the signal
 *                     quality shall be stored.
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_GetPhySignalQuality(uint8 TrcvIdx, uint32* SignalQualityPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        retVal = EthTrcv_GetPhySQHandle(CtrlIdx, MiiIdx, SignalQualityPtr);
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GETPHYSIGNALQUALITY, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_PHYSIGNALQUALITY_API*/

#if (STD_ON == ETHTRCV_SET_PHYTXMODE_API)
/******************************************************************************/
/*
 * Brief               Activates a given transmission mode.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 *                     Mode, Transmission mode to be activated
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetPhyTxMode(uint8 TrcvIdx, EthTrcv_PhyTxModeType Mode)
{
    Std_ReturnType retVal = E_NOT_OK;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
/*@req <SWS_EthTrcv_00168>*/
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
        if (ETHTRCV_CABLEDIAG_PENDING != EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/
        {
            if ((uint8)E_OK == EthTrcv_CheckPhyTxModeSupport(CtrlIdx, MiiIdx, Mode))
            {
                retVal = EthTrcv_SetPhyTxModeHandle(CtrlIdx, MiiIdx, Mode);
            }
            else
            {
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
                (void)Det_ReportError(
                    ETHTRCV_MODULE_ID,
                    ETHTRCV_INSTANCE,
                    ETHTRCV_SID_SETPHYTXMODE,
                    ETHTRCV_E_NOT_SUPPORTED);
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
            }
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_SETPHYTXMODE, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
#endif

/******************************************************************************/
/*
 * Brief               Trigger the cable diagnostics for the given Ethernet
 *                     transceiver
 * ServiceId           0x16
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: The trigger has been accepted
 *                     E_NOT_OK: The trigger has not been accepted
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_RunCableDiagnostic(uint8 TrcvIdx)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        /*@req SWS_EthTrcv_00159*/
        if ((ETHTRCV_CABLEDIAG_PENDING != EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
            && (EthTrcv_TrcvMode[TrcvIdx] == ETH_MODE_ACTIVE)
            && (EthTrcv_LinkState[TrcvIdx] == ETHTRCV_LINK_STATE_DOWN))
        {
            EthTrcv_CableDiagState[TrcvIdx].CableDiagState = ETHTRCV_CABLEDIAG_PENDING;
            retVal = EthTrcv_RunCableDiagHandle(CtrlIdx, MiiIdx);
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_RUNCABLEDIAGNOSTIC, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
#endif /* STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API*/

/******************************************************************************/
/*
 * Brief               Retrieves the cable diagnostics result of a given transceiver
 * ServiceId           0x14
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx.
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     ResultPtr, Pointer to the location where the cable
 *                     diagnostics result shall be stored
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API)
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetCableDiagnosticsResult(uint8 TrcvIdx, EthTrcv_CableDiagResultType* ResultPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        if (ETHTRCV_CABLEDIAG_PENDING == EthTrcv_CableDiagState[TrcvIdx].CableDiagState)
        {
            EthTrcv_CableDiagResultType cableDiagResult;
            retVal = EthTrcv_GetCableDiagRsHandle(CtrlIdx, MiiIdx, &cableDiagResult);
            if (E_OK == retVal)
            {
                EthTrcv_CableDiagState[TrcvIdx].CableDiagState = cableDiagResult;
                *ResultPtr = cableDiagResult;
                if (ETHTRCV_CABLEDIAG_PENDING != cableDiagResult)
                {
                    if (TRUE == EthTrcv_CableDiagState[TrcvIdx].PendingSetTrcvModeReq)
                    {
                        Std_ReturnType SetTrcvModeReqValue;
                        if (ETH_MODE_ACTIVE == EthTrcv_CableDiagState[TrcvIdx].PendingSetTrcvModeReqMode)
                        {
                            EthTrcv_SetTrcvModeActive(TrcvIdx, &SetTrcvModeReqValue);
                        }
                        else
                        {
                            EthTrcv_SetTrcvModeDown(TrcvIdx, &retVal);
                        }
                        (void)SetTrcvModeReqValue;
                        EthTrcv_CableDiagState[TrcvIdx].PendingSetTrcvModeReq = FALSE;
                    }
                    if (TRUE == EthTrcv_CableDiagState[TrcvIdx].PendingTrcvLinkStateReq)
                    {
                        EthTrcv_LinkStateRequest(TrcvIdx, EthTrcv_CableDiagState[TrcvIdx].PendingTrcvLinkStateReqMode);
                        EthTrcv_CableDiagState[TrcvIdx].PendingTrcvLinkStateReq = FALSE;
                    }
                }
            }
        }
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GETCABLEDIAGNOSTICSRESULT, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API*/

#if (STD_ON == ETHTRCV_GET_PHYIDENTIFIER_API)
/******************************************************************************/
/*
 * Brief               Obtains the PHY identifier of the Ethernet Transceiver
 *                     according to IEEE 802.3-2015 chapter 22.2.4.3.1 PHY
 *                     Identifer
 * ServiceId           0x15
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different TrcvIdx. Non reentrant for the
 *                     same TrcvIdx
 * Param-Name[in]      TrcvIdx, Index of the Ethernet transceiver within the
 *                     context of the Ethernet Transceiver Driver
 * Param-Name[out]     OrgUniqueIdPtr, Pointer to the memory where the
 *                     Organizationally Unique Identifier shall be stored
 *                     ModelNrPtr, Pointer to the memory where the Manufacturer's
 *                     Model Number shall be stored
 *                     RevisionNrPtr, Pointer to the memory where the Revision
 *                     Number shall be stored
 * Param-Name[in/out]  NA
 * Return              E_OK: The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetPhyIdentifier(uint8 TrcvIdx, uint32* OrgUniqueIdPtr, uint8* ModelNrPtr, uint8* RevisionNrPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
    {
        errorId = ETHTRCV_E_UNINIT;
    }
    else if (TrcvIdx >= ETHTRCV_TRCVS_NUM)
    {
        errorId = ETHTRCV_E_INV_TRCV_IDX;
    }
    else if ((NULL_PTR == OrgUniqueIdPtr) || (NULL_PTR == ModelNrPtr) || (NULL_PTR == RevisionNrPtr))
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
        MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
        retVal = EthTrcv_GetPhyIDHandle(CtrlIdx, MiiIdx, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr);
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {
        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GETPHYIDENTIFIER, errorId);
    }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT*/
    return retVal;
}
#endif /* STD_ON == ETHTRCV_GET_PHYIDENTIFIER_API*/

/******************************************************************************/
/*
 * Brief               Used for polling state changes and wakeup reasons.
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          NA
 * Param-Name[in]      NA
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, ETHTRCV_CODE) EthTrcv_MainFunction(void)
{
    if (EthTrcv_ModuleState == ETHTRCV_STATE_INIT)
    {
        uint8 trcvIdx;
        Std_ReturnType retVal;
        /*for mii Register operation*/
        uint8 CtrlIdx;
        uint8 MiiIdx;
        Eth_ModeType trcvModeValue;

#if (ETHTRCV_WAKEUP_BY_POLLING == ETHTRCV_WAKEUP_SUPPORT)
        EcuM_WakeupSourceType wakeupSourceValue;
#endif
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
        uint8 errorId = ETHTRCV_E_NO_ERROR;
        if (EthTrcv_ModuleState == ETHTRCV_STATE_UNINIT)
        {
            errorId = ETHTRCV_E_UNINIT;
        }
        else
#endif
        {
            for (trcvIdx = 0; trcvIdx < ETHTRCV_TRCVS_NUM; trcvIdx++) /*PRQA S 2877*/ /* MISRA Dir 4.1 */
            {
                CtrlIdx = EthTrcv_TrcvPbCfg[trcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
                MiiIdx = EthTrcv_TrcvPbCfg[trcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;
                retVal = EthTrcv_GetTrcvModeHandle(CtrlIdx, MiiIdx, &trcvModeValue);
                if (((uint8)E_OK == retVal) && (EthTrcv_TrcvMode[trcvIdx] != trcvModeValue))
                {
                    EthTrcv_TrcvMode[trcvIdx] = trcvModeValue;
                    /*@req <SWS_EthTrcv_00043>*/
                    EthIf_TrcvModeIndication(EthTrcv_TrcvPbCfg[trcvIdx].EthTrcvRefCtrlIdxInEthIf, trcvModeValue);
                }
/*  Wakeup by polling is enabled */
#if (ETHTRCV_WAKEUP_BY_POLLING == ETHTRCV_WAKEUP_SUPPORT)
                retVal = EthTrcv_GetWakeupReasonHandle(trcvIdx, &wakeupSourceValue);
                if ((uint8)E_OK == retVal)
                {
                    EcuM_SetWakeupEvent(wakeupSourceValue);
                }
#endif /* ETHTRCV_WAKEUP_BY_POLLING == ETHTRCV_WAKEUP_SUPPORT */
            }
        }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
        if (errorId != ETHTRCV_E_NO_ERROR)
        {
            (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_MAIN_FUNCTION, errorId);
        }
#endif /* STD_ON == ETHTRCV_DEV_ERROR_DETECT  */
    }
}

/******************************************************************************/
/*
 * Brief               Returns the version information of this module
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      NA
 * Param-Name[out]     VersionInfoPtr, Version information of this module
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
#if (STD_ON == ETHTRCV_VERSION_INFO_API)
FUNC(void, ETHTRCV_CODE) EthTrcv_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTRCV_APPL_DATA) VersionInfoPtr)
{
#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    uint8 errorId = ETHTRCV_E_NO_ERROR;
    if (VersionInfoPtr == NULL_PTR)
    {
        errorId = ETHTRCV_E_PARAM_POINTER;
    }
    else
#endif
    {
        VersionInfoPtr->vendorID = ETHTRCV_VENDOR_ID;
        VersionInfoPtr->moduleID = ETHTRCV_MODULE_ID;
        VersionInfoPtr->sw_major_version = ETHTRCV_C_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = ETHTRCV_C_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = ETHTRCV_C_SW_PATCH_VERSION;
    }

#if (STD_ON == ETHTRCV_DEV_ERROR_DETECT)
    if (errorId != ETHTRCV_E_NO_ERROR)
    {

        (void)Det_ReportError(ETHTRCV_MODULE_ID, ETHTRCV_INSTANCE, ETHTRCV_SID_GET_VERSION_INFO, errorId);
    }
#endif /*STD_ON == ETHTRCV_DEV_ERROR_DETECT */
}
#endif /* STD_ON == ETHTRCV_VERSION_INFO_API */

#if (STD_ON == ETHTRCV_DEFINE_MIIRXINDICATION)
/******************************************************************************/
/*
 * Brief               Called when information has been read out via MII interface.
 *                     Triggered by previous Eth_ReadMii call. Can directly be
 *                     called within Eth_ReadMii.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same CtrlIdx, reentrant for different
 * Param-Name[in]      CtrlIdx, Index of the controller within the context of the
 *                     Ethernet Driver
 *                     TrcvIdx, Index of the transceiver on the MII
 *                     RegIdx, Index of the transceiver register on the MII
 *                     RegVal, Value contained in the indexed register
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, ETHTRCV_CODE)
EthTrcv_ReadMiiIndication(uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx, uint8 RegVal)
{
    (void)CtrlIdx;
    (void)TrcvIdx;
    (void)RegIdx;
    (void)RegVal;
    return;
}

/******************************************************************************/
/*
 * Brief               Called when information has been written via MII interface.
 *                     Triggered by previous Eth_WriteMii call. Can directly be
 *                     called within Eth_WriteMii.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same CtrlIdx, reentrant for different
 * Param-Name[in]      CtrlIdx, Index of the controller within the context of the
 *                     Ethernet Driver
 *                     TrcvIdx, Index of the transceiver on the MII
 *                     RegIdx, Index of the transceiver register on the MII
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, ETHTRCV_CODE)
EthTrcv_WriteMiiIndication(uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx)
{
    (void)CtrlIdx;
    (void)TrcvIdx;
    (void)RegIdx;
    return;
}
#endif /*STD_ON == ETHTRCV_DEFINE_MIIRXINDICATION*/

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if (ETHTRCV_SET_TRCV_MODE == STD_ON)
static FUNC(void, ETHTRCV_CODE) EthTrcv_SetTrcvModeActive(uint8 TrcvIdx, Std_ReturnType* retVal)
{
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;
    CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
    MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;

#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
    EcuM_WakeupSourceType wakeupSourceValue;
#endif /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/

/*set the PHY mode to normal*/
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
    if (ETHTRCV_WUM_ENABLE == EthTrcv_WakeupMode[TrcvIdx])
    {
        /*@req <SWS_EthTrcv_00118>*/
        /*Active wake up the PHY*/
        EthTrcv_LocalWakeupHandle(TrcvIdx);
        if ((boolean)FALSE == EthTrcv_PhyModeIsSleep(CtrlIdx，MiiIdx))
        {
            /*check the wakeup reason (EcuM wakeup source)*/
            if ((uint8)E_OK == EthTrcv_GetWakeupReasonHandle(CtrlIdx, MiiIdx, &wakeupSourceValue))
            {
                EcuM_SetWakeupEvent(wakeupSourceValue);
            }
            /*call the configuration API WakeUpCallout*/
            if (NULL_PTR != ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvWakeUpCallout)
            {
                ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvWakeUpCallout(TrcvIdx);
            }
            /*request change the PHY mode to normal mode*/
            EthTrcv_SetTrcvModeHandle(CtrlIdx, MiiIdx, ECR_NORMAL_MODE);
            /*@req <SWS_EthTrcv_00112>*/
#if (ETHTRCV_USED_ICU_MODULE == STD_ON)
            if (ICU_CHANNEL_ID_INVALID != ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvRefChannelIdx)
            {
                Icu_DisableNotification(ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvRefChannelIdx);
            }
#endif /*ETHTRCV_USED_ICU_MODULE == STD_ON*/
            *retVal = E_OK;
        }
    }
    else
    {
        EthTrcv_SetTrcvModeHandle(CtrlIdx, MiiIdx, ECR_NORMAL_MODE);
        *retVal = E_OK;
    }
#else  /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/
    EthTrcv_SetTrcvModeHandle(CtrlIdx, MiiIdx, ECR_NORMAL_MODE);
    *retVal = E_OK;
#endif /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/
}

static FUNC(void, ETHTRCV_CODE) EthTrcv_SetTrcvModeDown(uint8 TrcvIdx, Std_ReturnType* retVal)
{
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;
    CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
    MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;

/*set the PHY mode to sleep or standby*/
#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
    /*@req <SWS_EthTrcv_00117>*/
    if (ETHTRCV_WUM_ENABLE == EthTrcv_WakeupMode[TrcvIdx])
    {
        /*set the PHY mode to sleep mode*/
        EthTrcv_SetTrcvModeHandle(CtrlIdx, MiiIdx, ECR_SLEEP_REQUEST_MODE);
/*@req <SWS_EthTrcv_00111>*/
#if (ETHTRCV_USED_ICU_MODULE == STD_ON)
        if (ICU_CHANNEL_ID_INVALID != ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvRefChannelIdx)
        {
            Icu_EnableNotification(ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvRefChannelIdx);
        }
#endif /*ETHTRCV_USED_ICU_MODULE == STD_ON*/
    }
    else
    {
        /*set the PHY mode to standby mode*/
        EthTrcv_SetTrcvModeHandle(CtrlIdx, MiiIdx, ECR_STANDBY_MODE);
    }
#else  /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/
    /*set the PHY mode to standby mode*/
    EthTrcv_SetTrcvModeHandle(CtrlIdx, MiiIdx, ECR_STANDBY_MODE);
#endif /*ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED*/
    *retVal = E_OK;
}
#endif

static FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_LinkStateRequest(uint8 TrcvIdx, EthTrcv_LinkStateType LinkState)
{
    Std_ReturnType retVal = E_NOT_OK;
    /*for mii Register operation*/
    uint8 CtrlIdx;
    uint8 MiiIdx;
    CtrlIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvCtrlIdx;
    MiiIdx = EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvMgmtInterface->EthTrcvMiiInterface->EthTrcvMiiIdx;

    if (ETHTRCV_LINK_STATE_ACTIVE == LinkState)
    {
        /*@req <SWS_EthTrcv_00151>*/
        if ((TRCV_CONN_NEG_AUTO == EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvConnNeg)
            || (TRCV_CONN_NEG_MASTER == EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvConnNeg))
        {
            retVal = EthTrcv_LinkStateActiveHandle(CtrlIdx, MiiIdx);
        }
    }
    else
    {
        /*@req <SWS_EthTrcv_00152>*/
        if ((TRCV_CONN_NEG_AUTO == EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvConnNeg)
            || (TRCV_CONN_NEG_MASTER == EthTrcv_TrcvPbCfg[TrcvIdx].EthTrcvConnNeg))
        {
            retVal = EthTrcv_LinkStateDownHandle(CtrlIdx, MiiIdx);
        }
    }
    if ((uint8)E_OK == retVal)
    {
        EthTrcv_LinkState[TrcvIdx] = LinkState;
    }
    return retVal;
}

#define ETHTRCV_STOP_SEC_CODE
#include "EthTrcv_MemMap.h"
