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
**  FILENAME    : EthTrcv.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef ETHTRCV_H_
#define ETHTRCV_H_
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthTrcv_Types.h"
#include "EthTrcv_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ETHTRCV_VENDOR_ID                  62U
#define ETHTRCV_MODULE_ID                  73U
#define ETHTRCV_INSTANCE                   0U
#define ETHTRCV_H_AR_RELEASE_MAJOR_VERSION 4u
#define ETHTRCV_H_AR_RELEASE_MINOR_VERSION 5u
#define ETHTRCV_H_AR_RELEASE_PATCH_VERSION 0u

/* API service IDs */
#define ETHTRCV_SID_INIT                        (0x01U)
#define ETHTRCV_SID_SET_TRANSCEIVER_MODE        (0x03U)
#define ETHTRCV_SID_GET_TRANSCEIVER_MODE        (0x04U)
#define ETHTRCV_SID_SET_TRANSCEIVER_WAKEUP_MODE (0x0DU)
#define ETHTRCV_SID_GET_TRANSCEIVER_WAKEUP_MODE (0x0EU)
#define ETHTRCV_SID_CHECK_WAKEUP                (0x0FU)
#define ETHTRCV_SID_START_AUTO_NEG              (0x05U)
#define ETHTRCV_SID_GET_LINK_STATE              (0x06U)
#define ETHTRCV_SID_GET_BAUD_RATE               (0x07U)
#define ETHTRCV_SID_GET_DUPLEX_MODE             (0x08U)
#define ETHTRCV_SID_GET_VERSION_INFO            (0x0BU)
#define ETHTRCV_SID_MAIN_FUNCTION               (0x0CU)
#define ETHTRCV_SID_READMIIINDICATION           (0x09U)
#define ETHTRCV_SID_WRITEMIIINDICATION          (0x0AU)
#define ETHTRCV_SID_SETPHYTESTMODE              (0x11U)
#define ETHTRCV_SID_SETPHYLOOPBACKMODE          (0x12U)
#define ETHTRCV_SID_GETPHYSIGNALQUALITY         (0x10U)
#define ETHTRCV_SID_SETPHYTXMODE                (0x13U)
#define ETHTRCV_SID_RUNCABLEDIAGNOSTIC          (0x16U)
#define ETHTRCV_SID_GETCABLEDIAGNOSTICSRESULT   (0x14U)
#define ETHTRCV_SID_GETPHYIDENTIFIER            (0x15U)
#define ETHTRCV_SID_TRANSCEIVERLINKSTATEREQUEST (0x17U) /*not defined in R19-11*/

/* ETHTRCV DET errors */
#define ETHTRCV_E_NO_ERROR             (0x00U)
#define ETHTRCV_E_INV_TRCV_IDX         (0x01U)
#define ETHTRCV_E_UNINIT               (0x02U)
#define ETHTRCV_E_PARAM_POINTER        (0x03U)
#define ETHTRCV_E_NOT_SUPPORTED        (0x04U)

#define ETHTRCV_DEFINE_MIIRXINDICATION STD_ON

/* ETHTRCV configuration data macro */
#define ETHTRCV_CFG_DATA(trcvIdx) (EthTrcv_TrcvPbCfg[trcvIdx])

#if (ETHTRCV_WAKEUP_SUPPORT != ETHTRCV_WAKEUP_NOT_SUPPORTED)
#define ETHTRCV_CFG_WAKEUP_MAP(trcvIdx, reasonIdx) (EthTrcv_TrcvPbCfg[trcvIdx].EthTrcvWakeupMapCfgPtr[reasonIdx])
#endif

/* Diagnostic Event Manager */
#if (ETHTRCV_USED_DEM_MODULE == STD_ON)
#define ETHTRCV_DEM_REPORT(TrcvIdx, EventId)                                                   \
    if (ETHTRCV_NO_DEM_EVENT != ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvAccessErrorEventId)           \
    {                                                                                          \
        Dem_ReportErrorStatus(ETHTRCV_CFG_DATA(TrcvIdx).EthTrcvAccessErrorEventId, (EventId)); \
    }
#else
#define ETHTRCV_DEM_REPORT(TrcvIdx, EventId)
#endif

extern VAR(Eth_ModeType, ETHTRCV_VAR_NOINIT) EthIf_ReqTrcvMode[ETHTRCV_TRCVS_NUM];
/* declaration of EthTrcv configuration data */
extern CONST(EthTrcv_ConfigType, ETHTRCV_CONST) EthTrcv_CfgData[ETHTRCV_TRCVS_NUM];
extern CONST(EthTrcv_GlobalType, ETHTRCV_CONST) EthTrcv_GlobalCfg;

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

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
extern FUNC(void, ETHTRCV_CODE) EthTrcv_Init(P2CONST(EthTrcv_ConfigType, ETHTRCV_CONST, ETHTRCV_CONST) CfgPtr);

#if (ETHTRCV_SET_TRCV_MODE == STD_ON)
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
extern FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetTransceiverMode(uint8 TrcvIdx, Eth_ModeType TrcvMode);
#endif /* ETHTRCV_SET_TRCV_MODE */

#if (ETHTRCV_GET_TRCV_MODE == STD_ON)
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
extern FUNC(Std_ReturnType, ETHTRCV_CODE)
    EthTrcv_GetTransceiverMode(uint8 TrcvIdx, P2VAR(Eth_ModeType, AUTOMATIC, ETHTRCV_APPL_DATA) TrcvModePtr);
#endif /* ETHTRCV_GET_TRCV_MODE */

#if (ETHTRCV_START_AUTO_NEG == STD_ON)
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
extern FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_StartAutoNegotiation(uint8 TrcvIdx);
#endif /* ETHTRCV_START_AUTO_NEG */

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
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_TransceiverLinkStateRequest(uint8 TrcvIdx, EthTrcv_LinkStateType LinkState);

#if (ETHTRCV_GET_LINK_STATE == STD_ON)
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
extern FUNC(Std_ReturnType, ETHTRCV_CODE)
    EthTrcv_GetLinkState(uint8 TrcvIdx, P2VAR(EthTrcv_LinkStateType, AUTOMATIC, AUTOMATIC) LinkStatePtr);
#endif /* ETHTRCV_GET_LINK_STATE */

#if (ETHTRCV_GET_BAUD_RATE == STD_ON)
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
extern FUNC(Std_ReturnType, ETHTRCV_CODE)
    EthTrcv_GetBaudRate(uint8 TrcvIdx, P2VAR(EthTrcv_BaudRateType, AUTOMATIC, ETHTRCV_APPL_DATA) BaudRatePtr);
#endif /* ETHTRCV_GET_BAUD_RATE */

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
#if (ETHTRCV_GET_DUPLEX_MODE == STD_ON)
extern FUNC(Std_ReturnType, ETHTRCV_CODE)
    EthTrcv_GetDuplexMode(uint8 TrcvIdx, P2VAR(EthTrcv_DuplexModeType, AUTOMATIC, AUTOMATIC) DuplexModePtr);
#endif /* ETHTRCV_GET_DUPLEX_MODE */

#if (ETHTRCV_WAKEUP_NOT_SUPPORTED != ETHTRCV_WAKEUP_SUPPORT)
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
extern FUNC(Std_ReturnType, ETHTRCV_CODE)
    EthTrcv_SetTransceiverWakeupMode(uint8 TrcvIdx, EthTrcv_WakeupModeType TrcvWakeupMode);

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
extern FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_GetTransceiverWakeupMode(
    uint8 TrcvIdx,
    P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHTRCV_APPL_DATA) TrcvWakeupModePtr);

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
extern FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_CheckWakeup(uint8 TrcvIdx);
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
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetPhyTestMode(uint8 TrcvIdx, EthTrcv_PhyTestModeType Mode);
#endif /* STD_ON == ETHTRCV_SET_PHYTESTMODE_API*/

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
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetPhyLoopbackMode(uint8 TrcvIdx, EthTrcv_PhyLoopbackModeType Mode);

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
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_GetPhySignalQuality(uint8 TrcvIdx, uint32* SignalQualityPtr);

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
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_SetPhyTxMode(uint8 TrcvIdx, EthTrcv_PhyTxModeType Mode);

#if (STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API)
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
FUNC(Std_ReturnType, ETHTRCV_CODE) EthTrcv_RunCableDiagnostic(uint8 TrcvIdx);
#endif /* STD_ON == ETHTRCV_ENABLE_CABLEDIAGNOSTIC_API*/

#if (STD_ON == ETHTRCV_GET_CABLEDIAGNOSTICS_RESULT_API)
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
FUNC(Std_ReturnType, ETHTRCV_CODE)
EthTrcv_GetCableDiagnosticsResult(uint8 TrcvIdx, EthTrcv_CableDiagResultType* ResultPtr);
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
EthTrcv_GetPhyIdentifier(uint8 TrcvIdx, uint32* OrgUniqueIdPtr, uint8* ModelNrPtr, uint8* RevisionNrPtr);
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
extern FUNC(void, ETHTRCV_CODE) EthTrcv_MainFunction(void);

#if (ETHTRCV_VERSION_INFO_API == STD_ON)
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
extern FUNC(void, ETHTRCV_CODE)
    EthTrcv_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTRCV_APPL_DATA) VersionInfoPtr);
#endif /* ETHTRCV_VERSION_INFO_API == STD_ON */

#endif /* ETHTRCV_H_ */
