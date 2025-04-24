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
**  FILENAME    : CanIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for CANIF                                    **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V2.0.0    20200217  zhengfei.li   Initial version
 *  (Upgrade according to the R19_11 standards)
 *  V2.0.1    20210408  zhengfei.li   Optimize wake-up source detection,
 *  Modify the bug of API CanIf_CheckTrcvWakeFlag when the DET is STD_OFF.
 *  V2.1.0    20211217  zhengfei.li   PB/PC configuration parameter split
 *  V2.1.1    20230526  tong.zhao     fix Bug CPT-459
 *    Modify the bug of invalid address access in API CanIf_RxIndicationHandle,
 *    when meta is not support and ReadRxPdu is support.
 *  V2.1.2    20231009  tong.zhao     fix Bug CPT-7169
 *    1> Add condication for CanIf_MetaDataToCanId and CanIf_CanIdToMetaData.
 *    2> Change Version  Check.
 *  V2.1.3    20231007  tong.zhao     CPP-419
      1> Adaptation of different AutoSar versions of can driver, currently supporting
 *       versions 4.2.2, 4.3.1, 4.4.0
 *  V2.1.4    20231031  xiaojian.liang Add feauture CanIfRxPduDataLengthCheck.
 *  V2.1.5    20231116  tong.zhao     CPT-7511
 *    1> Modify the bug of Can_IdType may be defined as uint16 in only Standard frame,
 *       at AUTOSAR Driver version 422.
 *  V2.1.6    20240401  tong.zhao     fix Bug CPT-8618
 *    In order to ensure that ControllerId and TransciverId in CanIf does not rely on CanDrv and CanTransciver index
 *  V2.2.0    20240524  tong.zhao     CPD-33700         Muliticore adapt
 *  V2.2.1    20240606  tong.zhao     CPT-9165
 *    Modify exclusivearea in Api CanIf_TransmitHandle.
 *  V2.2.2    20240628  tong.zhao     Optimize flash
 *  V2.2.3    20240705  tong.zhao     Add binary search method for software filtering.
 *            20240705  tong.zhao     Compatible with 16-bit addressable CPU
 */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanIf<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 2992 MISRA Rule 14.3 .<br>
    Reason:variant "controllerNotSleep" may be different in other Configuration projects.

    \li PRQA S 2996 MISRA Rule 2.2 .<br>
    Reason:The assessment has no effect, but is reserved for better readability.

    \li PRQA S 2880 MISRA Rule 2.1.<br>
    Reason:Storage mapping design is required.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:Macro definition, design needs, namelength set to 63 in C99.
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CanIf_Internal.h"
#if (STD_ON == CANIF_PUBLIC_ICOM_SUPPORT)
#include "CanSM_Cbk.h"
#endif
#include "Det.h"
#include "CanIf_Cbk.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
/*check version information with CanIf*/
#if (                                                                                 \
    (CANIF_AR_RELEASE_MAJOR_VERSION != CANIF_CFG_H_AR_RELEASE_MAJOR_VERSION)          \
    || (CANIF_AR_RELEASE_MINOR_VERSION != CANIF_CFG_H_AR_RELEASE_MINOR_VERSION)       \
    || (CANIF_AR_RELEASE_REVISION_VERSION != CANIF_CFG_H_AR_RELEASE_REVISION_VERSION) \
    || (2u != CANIF_CFG_H_SW_MAJOR_VERSION) || (2u != CANIF_CFG_H_SW_MINOR_VERSION)   \
    || (2u != CANIF_CFG_H_SW_PATCH_VERSION))
#error " CanIf cfg file version mismatching with CanIf"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/*Service ID*/
#define CANIF_CHECKWAKEUP_ID  (0x11u)
#define CANIF_TRANSMIT_ID     (0x05u)
#define CANIF_RXINDICATION_ID (0x14u)
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
#define CANIF_INIT_ID                        (0x01u)
#define CANIF_SETCONTROLLER_MODE_ID          (0x03u)
#define CANIF_GETCONTROLLER_MODE_ID          (0x04u)
#define CANIF_READRXPDUDATA_ID               (0x06u)
#define CANIF_READTXNOTIFSTATUS_ID           (0x07u)
#define CANIF_READRXNOTIFSTATUS_ID           (0x08u)
#define CANIF_SETPDUMODE_ID                  (0x09u)
#define CANIF_GETPDUMODE_ID                  (0x0Au)
#define CANIF_GETVERSIONINFO_ID              (0x0Bu)
#define CANIF_SETDYNAMICTXID_ID              (0x0Cu)
#define CANIF_SETTRCVMODE_ID                 (0x0Du)
#define CANIF_GETTRCVMODE_ID                 (0x0Eu)
#define CANIF_GETTRCVWAKEUPREASON_ID         (0x0Fu)
#define CANIF_SETTRCVWAKEUPMODE_ID           (0x10u)
#define CANIF_CHECKVALIDATION_ID             (0x12u)
#define CANIF_GETTXCONFIRMATIONSTATE         (0x19u)
#define CANIF_CLEARTRCVWUFFLAG_ID            (0x1Eu)
#define CANIF_CHECKTRCVWAKEFLAG_ID           (0x1Fu)
#define CANIF_SETBAUDRATE_ID                 (0x27u)
#define CANIF_SETICOMCONFIGURATION_ID        (0x25u)
#define CANIF_TRIGGERTRANSMIT_ID             (0x41u)
#define CANIF_TXCONFIRMATION_ID              (0x13u)
#define CANIF_CONTROLLERBUSOFF_ID            (0x16u)
#define CANIF_CONFIRMPNAVAILABILITY_ID       (0x1Au)
#define CANIF_CLEARTRCVWUFFLAGINDICATION_ID  (0x20u)
#define CANIF_CHECKTRCVWAKEFLAGINDICATION_ID (0x21u)
#define CANIF_CONTROLLERMODEINDICATION_ID    (0x17u)
#define CANIF_TRCVMODEINDICATION_ID          (0x22u)
#define CANIF_CURRENTICOMCONFIGURATION_ID    (0x26u)

/* Error detection */
#define CANIF_E_PARAM_CANID          (10u)
#define CANIF_E_PARAM_HOH            (12u)
#define CANIF_E_PARAM_LPDU           (13u)
#define CANIF_E_PARAM_CONTROLLERID   (15u)
#define CANIF_E_PARAM_WAKEUPSOURCE   (16u)
#define CANIF_E_PARAM_TRCV           (17u)
#define CANIF_E_PARAM_TRCVMODE       (18u)
#define CANIF_E_PARAM_TRCVWAKEUPMODE (19u)
#define CANIF_E_PARAM_CTRLMODE       (21u)
#define CANIF_E_PARAM_PDU_MODE       (22u)
#define CANIF_E_PARAM_POINTER        (20u)
#define CANIF_E_UNINIT               (30u)
#define CANIF_E_INVALID_TXPDUID      (50u)
#define CANIF_E_INVALID_RXPDUID      (60u)
#define CANIF_E_INIT_FAILED          (80u)
#endif /*STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT*/
#define CANIF_E_INVALID_DLC                   (61u)
#define CANIF_E_DATA_LENGTH_MISMATCH          (62u)
#define CANIF_E_STOPPED                       (70u)
#define CANIF_E_NOT_SLEEP                     (71u)

#define CanIf_Det_ReportError(apiId, errorId) (void)Det_ReportError(CANIF_MODULE_ID, 0u, (apiId), (errorId))
#define CanIf_Det_ReportRuntimeError(apiId, errorId) \
    (void)Det_ReportRuntimeError(CANIF_MODULE_ID, 0u, (apiId), (errorId))

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/* Canif init status, at first define it as CANIF_UNINIT */
#define CANIF_START_SEC_VAR_INIT_8
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(CanIf_InitStatusType, CANIF_VAR_POWER_ON_INIT) CanIf_InitStatus = CANIF_UNINIT;
#define CANIF_STOP_SEC_VAR_INIT_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_INIT_PTR
#include "CanIf_MemMap.h"
P2CONST(CanIf_ConfigType, CANIF_VAR, CANIF_CONST) CanIf_ConfigStd = NULL_PTR;
#define CANIF_STOP_SEC_VAR_INIT_PTR
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_TxConfirmationState[CANIF_CANCONTROLLER_NUMBER];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
#endif

#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(CanIf_PduModeType, CANIF_VAR) CanIf_PduMode[CANIF_CANCONTROLLER_NUMBER];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(Can_ControllerStateType, CANIF_VAR) CanIf_ControllerMode[CANIF_CANCONTROLLER_NUMBER];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
#define CANIF_START_SEC_VAR_NO_INIT_BOOLEAN
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(boolean, CANIF_VAR) CanIf_PnTxFilter[CANIF_CANCONTROLLER_NUMBER];
#define CANIF_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "CanIf_MemMap.h"
#endif

#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
#define CANIF_START_SEC_VAR_NO_INIT_BOOLEAN
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(boolean, CANIF_VAR) CanIf_FirstCallRxInd[CANIF_CANCONTROLLER_NUMBER];
#define CANIF_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "CanIf_MemMap.h"
#endif
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static FUNC(void, CANIF_CODE) CanIf_FreeBufInStopped(uint8 Controller);

static FUNC(void, CANIF_CODE) CanIf_InitAllBuffer(VAR(void, AUTOMATIC));

static FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllStarted(uint8 Controller);

static FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllSleep(uint8 Controller);

static FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllStopped(uint8 Controller);

static FUNC(void, CANIF_CODE) CanIf_PduPack(
    PduIdType CanTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr,
    P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) canPdu);

static FUNC(Std_ReturnType, CANIF_CODE)
    CanIf_CanIdRangCheck(Can_HwHandleType hrhId, CanIf_IdType canId, P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) pduIdIndex);

static FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxPduHrhSearchByLinear(
    Can_HwHandleType hrhIndex,
    CanIf_IdType CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex);

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
static FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxPduHrhSearchByBinary(
    Can_HwHandleType hrhIndex,
    CanIf_IdType CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex);
#endif /* CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY */

static FUNC(Std_ReturnType, CANIF_CODE)
    CanIf_GetPduHrh(PduIdType pduIdIndex, CanIf_IdType CanId, P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex);

static FUNC(void, CANIF_CODE) CanIf_RxIndicationHandle(
    PduIdType pduIdIndex,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    CanIf_IdType canId,
#endif
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr);

static FUNC(Std_ReturnType, CANIF_CODE)
    CanIf_TransmitHandle(PduIdType CanIfTxSduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr);

static void CanIf_GetCanIfCtrlId(uint8* CanIfCtrlId, uint8 CanControllerId);
#if (CANIF_TRCV_NUMBER > 0u)
static void CanIf_GetCanIfTrcvId(uint8* CanIfTrcvId, uint8 CanTransciverId);
#endif /* CANIF_TRCV_NUMBER > 0u */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/*************************************************************************/
/*
 * Brief               This service Initializes internal and external interfaces of the CAN Interface for the further
 *                     processing.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to configuration parameter set, used e.g. for post build parameters
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
CanIf_Init(P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_CONST_PBCFG) ConfigPtr)
{
    uint8 canLoop;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* for post build,report error if NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        CanIf_Det_ReportError(CANIF_INIT_ID, CANIF_E_PARAM_POINTER);
    }
#if defined CANIF_RXPDU_MAX
    else if (ConfigPtr->CanIfRxPduNumber > CANIF_RXPDU_MAX)
    {
        CanIf_Det_ReportError(CANIF_INIT_ID, CANIF_E_INIT_FAILED);
    }
#endif /* defined CANIF_RXPDU_MAX */
#if defined CANIF_TXPDU_MAX
    else if (ConfigPtr->CanIfTxPduNumber > CANIF_TXPDU_MAX)
    {
        CanIf_Det_ReportError(CANIF_INIT_ID, CANIF_E_INIT_FAILED);
    }
#endif /* defined CANIF_TXPDU_MAX */
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        /* CanIf Module has not been initialized */
        if (CANIF_INITED != CanIf_InitStatus)
        {
            /* save the configuration to global variable */
            CanIf_ConfigStd = ConfigPtr;
            /* Set CanIf_Init Status to CANIF_INITED*/
            CanIf_InitStatus = CANIF_INITED;
            /* set controller mode to STOPPED */
            for (canLoop = 0u; canLoop < CANIF_CANCONTROLLER_NUMBER; canLoop++)
            {
                CanIf_ControllerMode[canLoop] = CAN_CS_STOPPED;
                CanIf_PduMode[canLoop] = CANIF_OFFLINE;
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
                /* Init the first call rxindication event flag */
                CanIf_FirstCallRxInd[canLoop] = FALSE;
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
                CanIf_TxConfirmationState[canLoop] = CANIF_NO_NOTIFICATION;
#endif
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
                if (CanIf_CtrlCfgData[canLoop].CanIfPnFilter)
                {
                    CanIf_PnTxFilter[canLoop] = TRUE;
                }
                else
                {
                    CanIf_PnTxFilter[canLoop] = FALSE;
                }
#endif
            }
            CanIf_InitAllBuffer();
        }
    }
}
/*************************************************************************/
/*
 * Brief               De-initializes the CanIf module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
CanIf_DeInit(VAR(void, AUTOMATIC))
{
    if (CANIF_INITED == CanIf_InitStatus)
    {
        CanIf_ConfigStd = NULL_PTR;
        CanIf_InitStatus = CANIF_UNINIT;
    }
}
/*************************************************************************/
/*
 * Brief               This service calls the corresponding CAN Driver service
 *                     for changing of the CAN controller mode.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (Not for the same controller)
 * Param-Name[in]      ControllerId: Abstracted CanIf ControllerId which is assigned
 *                     to a CAN controller, which is requested for mode transition.
 *                     ControllerMode: Requested mode transition
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Controller mode request has been accepted
 *                     E_NOT_OK: Controller mode request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanIf_SetControllerMode(uint8 ControllerId, Can_ControllerStateType ControllerMode)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETCONTROLLER_MODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if (detNoErr && (ControllerId >= CANIF_CANCONTROLLER_NUMBER))
    {
        CanIf_Det_ReportError(CANIF_SETCONTROLLER_MODE_ID, CANIF_E_PARAM_CONTROLLERID);
        detNoErr = FALSE;
    }
    if (detNoErr && ((ControllerMode > CAN_CS_SLEEP) || (ControllerMode <= CAN_CS_UNINIT)))
    {
        CanIf_Det_ReportError(CANIF_SETCONTROLLER_MODE_ID, CANIF_E_PARAM_CTRLMODE);
        detNoErr = FALSE;
    }
    if (detNoErr)
#endif /* STD_ON == CANIF_DEV_ERROR_DETECT */
    {
        /* check the ControllerMode to be set*/
        switch (ControllerMode)
        {
        case CAN_CS_STARTED:
            result = CanIf_SetControllStarted(ControllerId);
            break;
        case CAN_CS_SLEEP:
            result = CanIf_SetControllSleep(ControllerId);
            break;
        case CAN_CS_STOPPED:
            result = CanIf_SetControllStopped(ControllerId);
            break;
        /* invalid controller mode */
        default:
            result = E_NOT_OK;
            break;
        }
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               This service reports about the current status of the requested CAN controller.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ControllerId: Abstracted CanIf ControllerId which is assigned to a
 *                     CAN controller, which is requested for current operation mode.
 * Param-Name[out]     ControllerModePtr: Pointer to a memory location, where the current
 *                     mode of the CAN controller will be stored.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Controller mode request has been accepted.
 *                     E_NOT_OK: Controller mode request has not been accepted.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_GetControllerMode(
    uint8 ControllerId,
    P2VAR(Can_ControllerStateType, AUTOMATIC, CANIF_APPL_DATA) ControllerModePtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_GETCONTROLLER_MODE_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /* check whether controller is over range */
    if (detNoErr && (ControllerId >= CANIF_CANCONTROLLER_NUMBER))
    {
        CanIf_Det_ReportError(CANIF_GETCONTROLLER_MODE_ID, CANIF_E_PARAM_CONTROLLERID);
        detNoErr = FALSE;
    }
    /* check whether controller is over range */
    if (detNoErr && (NULL_PTR == ControllerModePtr))
    {
        CanIf_Det_ReportError(CANIF_GETCONTROLLER_MODE_ID, CANIF_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (detNoErr)
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        /* get current mode */
        *ControllerModePtr = CanIf_ControllerMode[ControllerId];
        result = E_OK;
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               This service initiates a request for transmission of the CAN L-PDU
 *                     specified by the CanTxSduId and CAN related data in the L-SDU structure.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanIfTxSduId: L-SDU handle to be transmitted.This handle specifies the
 *                     corresponding CAN LSDU ID and implicitly the CAN Driver instance as well
 *                     as the corresponding CAN controller device.
 *                     CanIfTxInfoPtr: Pointer to a structure with CAN L-SDU related data:
 *                     DLC and pointer to CAN L-SDU buffer including the MetaData of dynamic L-PDUs.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Transmit request has been accepted
 *                     E_NOT_OK: Transmit request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_Transmit(PduIdType CanIfTxSduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanIfTxSduId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_TRANSMIT_ID, CANIF_E_UNINIT);
    }
    /* check NULL_PTR */
    else if (NULL_PTR == PduInfoPtr)
    {
        CanIf_Det_ReportError(CANIF_TRANSMIT_ID, CANIF_E_PARAM_POINTER);
    }
    else if (CanIfTxSduId >= CanIf_ConfigStd->CanIfTxPduNumber)
    {
        CanIf_Det_ReportError(CANIF_TRANSMIT_ID, CANIF_E_INVALID_TXPDUID);
    }
    else if ((txPduConfigPtr->TxMetaDataEnable) && (NULL_PTR == PduInfoPtr->MetaDataPtr))
    {
        CanIf_Det_ReportError(CANIF_TRANSMIT_ID, CANIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        boolean pduLengthValid = TRUE;
#if ((STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT) || (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT))
        if (
#if (STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT)
            (CANIF_TX_STANDARD_CAN == txPduConfigPtr->CanIfTxPduCanIdType)
#endif /* STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT */
#if ((STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT) && (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT))
            ||
#endif /* STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT && STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT */
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
            (CANIF_TX_EXTENDED_CAN == txPduConfigPtr->CanIfTxPduCanIdType)
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT */
        )
        {
            if (PduInfoPtr->SduLength > 8u)
            {
                pduLengthValid = FALSE;
            }
        }
#endif /* STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT || STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT */
#if (                                                                                        \
    ((STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT) || (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)) \
    && ((STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT) || (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)))
        else
#endif /* STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT || STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT && STD_ON == \
          CANIF_TX_STANDARD_FD_CAN_SUPPORT || STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
        {
#if ((STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT) || (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT))
            if (PduInfoPtr->SduLength > 64u)
            {
                pduLengthValid = FALSE;
            }
#endif /* STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT || STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
        }
        if (pduLengthValid)
        {
            result = CanIf_TransmitHandle(CanIfTxSduId, PduInfoPtr);
        }
        else
        {
            CanIf_Det_ReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_DATA_LENGTH_MISMATCH);
        }
    }
    return result;
}
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/*************************************************************************/
/*
 * Brief               This service provides the CAN DLC and the received data of the requested CanIfRxSduId to the
 *                     calling upper layer.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfRxSduId: Receive L-SDU handle specifying the corresponding CAN L-SDU ID and implicitly the
 *                     CAN Driver instance as well as the corresponding CAN controller device.
 * Param-Name[out]     CanIfRxInfoPtr: Pointer to a structure with CAN L-SDU related data:DLC and pointer to CAN L-SDU
 *                     buffer including the MetaData of dynamic L-PDUs.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType E_OK: Request for L-SDU data has been accepted
 *                     E_NOT_OK: No valid data has been received
 * PreCondition        CANIF_PUBLIC_READ_RX_PDU_DATA_API is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_ReadRxPduData(PduIdType CanIfRxSduId, P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) CanIfRxInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_READRXPDUDATA_ID, CANIF_E_UNINIT);
    }
    /* check NULL PTR*/
    else if ((NULL_PTR == CanIfRxInfoPtr) || (NULL_PTR == CanIfRxInfoPtr->SduDataPtr))
    {
        CanIf_Det_ReportError(CANIF_READRXPDUDATA_ID, CANIF_E_PARAM_POINTER);
    }
    else if (CanIfRxSduId >= CanIf_ConfigStd->CanIfRxPduNumber)
    {
        CanIf_Det_ReportError(CANIF_READRXPDUDATA_ID, CANIF_E_INVALID_RXPDUID);
    }
    else
#endif /*STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT*/
#if (0u < CANIF_RXBUFFER_NUMBER)
    {
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(CanIfRxSduId);
        Can_HwHandleType hrhIndex = rxPduConfigPtr->CanIfRxPduHrhId;
        uint8 controllerId = CANIF_HRH(hrhIndex).CanIfHrhCanCtrlId;
        if ((CAN_CS_STARTED == CanIf_ControllerMode[controllerId]) && ((CANIF_OFFLINE != CanIf_PduMode[controllerId])))
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_RxBufferNumType rxPduBufferIndex = rxPduConfigPtr->CanIf_RxBufferIndex;
            if (rxPduBufferIndex < CANIF_RXBUFFER_NUMBER)
            {
                CanIf_RxPduBufferType* rxPduBuffer = &CanIf_RxBuffer[rxPduBufferIndex];
                /*copy rx buffer data to CanIfRxInfoPtr*/
                CanIfRxInfoPtr->SduLength = (PduLengthType)rxPduBuffer->length;
                (void)ILib_memcpy(CanIfRxInfoPtr->SduDataPtr, rxPduBuffer->data, CanIfRxInfoPtr->SduLength);
#if (STD_ON == CANIF_META_DATA_SUPPORT)
                if (rxPduConfigPtr->RxMetaDataEnable)
                {
                    CanIf_CanIdToMetaData(rxPduBuffer->metaData, CanIfRxInfoPtr->MetaDataPtr);
                }
#endif
                result = E_OK;
            }
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
    }
#endif /* 0u < CANIF_RXBUFFER_NUMBER */
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API */
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief               This service returns the confirmation status (confirmation occurred or
 *                     not) of a specific static or dynamic CAN Tx L-PDU, requested by the CanIfTxSduId.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfTxSduId: L-SDU handle to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType:Current confirmation status of the corresponding CAN Tx L-PDU.
 * PreCondition        CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId)
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_READTXNOTIFSTATUS_ID, CANIF_E_UNINIT);
    }
    else if (
        (CanIfTxSduId >= CanIf_ConfigStd->CanIfTxPduNumber)
        || (FALSE == CANIF_TXPDU(CanIfTxSduId).CanIfTxPduReadNotifyStatus))
    {
        CanIf_Det_ReportError(CANIF_READTXNOTIFSTATUS_ID, CANIF_E_INVALID_TXPDUID);
    }
    else
#endif
    {
        result = CanIf_ReadTxNotifStatusHandle(CanIfTxSduId);
    }
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API */
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief               This service returns the indication status (indication occurred or not) of a
 *                     specific CAN Rx L-PDU, requested by the CanIfRxSduId.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfRxSduId: Receive L-SDU handle specifying the corresponding CAN L-SDU ID and implicitly
 *                     the CAN Driver instance as well as the corresponding CAN controller device.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType:Current indication status of the corresponding CAN Rx L-PDU.
 * PreCondition        CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId)
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_READRXNOTIFSTATUS_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
#if (0u < CANIF_RXPDU_MAX)
        && ((CanIfRxSduId >= CANIF_RXPDU_MAX) || (CanIfRxSduId >= CanIf_ConfigStd->CanIfRxPduNumber))
#endif
    )
    {
        CanIf_Det_ReportError(CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (FALSE == CANIF_RXPDU(CanIfRxSduId).CanIfRxPduReadNotifyStatus))
    {
        CanIf_Det_ReportError(CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /*TRUE == CANIF_PUBLIC_DEV_ERROR_DETECT*/
    {
        result = CanIf_ReadRxNotifStatusHandle(CanIfRxSduId);
    }
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API */
/*************************************************************************/
/*
 * Brief               This service sets the requested mode at the L-PDUs of a predefined logical PDU channel.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ControllerId: All PDUs of the own ECU connected to the corresponding CanIf ControllerId,
 *                     which is assigned to a physical CAN controller are addressed.
 *                     PduModeRequest: Requested PDU mode change
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Request for mode transition has been accepted.
 *                     E_NOT_OK: Request for mode transition has not been accepted.
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETPDUMODE_ID, CANIF_E_UNINIT);
    }
    else if (ControllerId >= CANIF_CANCONTROLLER_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_SETPDUMODE_ID, CANIF_E_PARAM_CONTROLLERID);
    }
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
    else if (PduModeRequest > CANIF_ONLINE))
        {
            CanIf_Det_ReportError(CANIF_SETPDUMODE_ID, CANIF_E_PARAM_PDU_MODE);
        }
#else
    else if ((PduModeRequest > CANIF_ONLINE) || (PduModeRequest == CANIF_TX_OFFLINE_ACTIVE))
    {
        CanIf_Det_ReportError(CANIF_SETPDUMODE_ID, CANIF_E_PARAM_PDU_MODE);
    }
#endif /* STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT */
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        if (CAN_CS_STARTED == CanIf_ControllerMode[ControllerId])
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            switch (PduModeRequest)
            {
            case CANIF_OFFLINE:
                CanIf_PduMode[ControllerId] = CANIF_OFFLINE;
                result = E_OK;
                break;
            case CANIF_TX_OFFLINE:
                CanIf_PduMode[ControllerId] = CANIF_TX_OFFLINE;
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
                if (CanIf_CtrlCfgData[ControllerId].CanIfPnFilter)
                {
                    CanIf_PnTxFilter[ControllerId] = TRUE;
                }
#endif /* STD_ON == CANIF_PUBLIC_PN_SUPPORT */
                result = E_OK;
                break;
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
            case CANIF_TX_OFFLINE_ACTIVE:
                CanIf_PduMode[ControllerId] = CANIF_TX_OFFLINE_ACTIVE;
                result = E_OK;
                break;
#endif /* STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT */
            case CANIF_ONLINE:
                CanIf_PduMode[ControllerId] = CANIF_ONLINE;
                result = E_OK;
                break;
            default:
                /* invalid pdu mode*/
                result = E_NOT_OK;
                break;
            }
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               This service reports the current mode of a requested PDU channel.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (Not for the same channel)
 * Param-Name[in]      ControllerId: All PDUs of the own ECU connected to the corresponding CanIf
 *                     ControllerId, which is assigned to a physical CAN controller are addressed.
 * Param-Name[out]     PduModePtr: Pointer to a memory location, where the current mode of the
 *                     logical PDU channel will be stored.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: PDU mode request has been accepted
 *                     E_NOT_OK: PDU mode request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_GetPduMode(uint8 ControllerId, P2VAR(CanIf_PduModeType, AUTOMATIC, CANIF_APPL_DATA) PduModePtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_GETPDUMODE_ID, CANIF_E_UNINIT);
    }
    else if (ControllerId >= CANIF_CANCONTROLLER_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_GETPDUMODE_ID, CANIF_E_PARAM_CONTROLLERID);
    }
    /* check NULL PTR*/
    else if (NULL_PTR == PduModePtr)
    {
        CanIf_Det_ReportError(CANIF_GETPDUMODE_ID, CANIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        /* get current pdu mode */
        *PduModePtr = CanIf_PduMode[ControllerId];
        result = E_OK;
    }
    return result;
}
#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
/*************************************************************************/
/*
 * Brief               This service reconfigures the corresponding CAN identifier of the requested CAN L-PDU.
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfTxSduId: L-SDU handle to be transmitted.This handle specifies the corresponding
 *                     CAN LSDU ID and implicitly the CAN Driver instance as well as the corresponding CAN controller
 * device. CanId: Standard/Extended CAN ID of CAN L-SDU that shall be transmitted as FD or conventional CAN frame.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_SetDynamicTxId(PduIdType CanIfTxSduId, Can_IdType CanId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    boolean canIdValid = TRUE;
    const CanIf_TxPduConfigType* txPduCfgPtr = &CANIF_TXPDU(CanIfTxSduId);
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETDYNAMICTXID_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /* check CanTxPduId valid */
    if (detNoErr
        && ((CanIfTxSduId >= CanIf_ConfigStd->CanIfTxPduNumber) || (CANID_DYNAMIC != txPduCfgPtr->CanIfTxPduType)))
    {
        CanIf_Det_ReportError(CANIF_SETDYNAMICTXID_ID, CANIF_E_INVALID_TXPDUID);
        detNoErr = FALSE;
    }
    if (detNoErr)
    {
#if ((STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT) || (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT))
        /*  dynamic Txpdu CanId Extended */
        if (
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
            (CANIF_TX_EXTENDED_CAN == txPduCfgPtr->CanIfTxPduCanIdType)
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT */
#if ((STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT) && (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT))
            ||
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT && STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
            (CANIF_TX_EXTENDED_FD_CAN == txPduCfgPtr->CanIfTxPduCanIdType)
#endif /* STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
        )
        {
            if (CanId > CANIF_EXTENDED_CANID_MAX)
            {
                canIdValid = FALSE;
            }
        }
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT || STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
#if (                                                                                           \
    ((STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT) || (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)) \
    && ((STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT) || (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)))
        else
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT || STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT && STD_ON == \
          CANIF_TX_STANDARD_CAN_SUPPORT || STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT */
#if ((STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT) || (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT))
        {
            /* Standard Can Id, check whether valid */
            if (CanId > CANIF_STANDARD_CANID_MAX)
            {
                canIdValid = FALSE;
            }
        }
#endif /* STD_ON == CANIF_TX_STANDARD_CAN_SUPPORT || STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT */
        if (!canIdValid)
        {
            (void)Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SETDYNAMICTXID_ID, CANIF_E_PARAM_CANID);
            detNoErr = FALSE;
        }
    }
    if (detNoErr)
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        CanIf_SetDynamicTxIdHandle(CanIfTxSduId, CanId);
    }
}
#endif /* STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API */
#if (CANIF_TRCV_NUMBER > 0u)
/*************************************************************************/
/*
 * Brief               This service changes the operation mode of the tansceiver TransceiverId,
 *                     via calling the corresponding CAN Transceiver Driver service.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN
 *                     transceiver, which is requested for mode transition
 *                     TransceiverMode: Requested mode transition
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Transceiver mode request has been accepted.
 *                     E_NOT_OK: Transceiver mode request has not been accepted.
 * PreCondition        CANIF_TRCV_NUMBER > 0
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETTRCVMODE_ID, CANIF_E_UNINIT);
    }
    else if (TransceiverId >= CANIF_TRCV_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_SETTRCVMODE_ID, CANIF_E_PARAM_TRCV);
    }
    else if (TransceiverMode > CANTRCV_TRCVMODE_STANDBY)
    {
        CanIf_Det_ReportError(CANIF_SETTRCVMODE_ID, CANIF_E_PARAM_TRCVMODE);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TrcvCfgType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        result =
            Can_TrcvApi[trcvConfigPtr->CanTrcvDriverId].CanTrcvSetOpModeApi(trcvConfigPtr->CanTrcvId, TransceiverMode);
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               This function invokes CanTrcv_GetOpMode and updates the parameter
 *                     TransceiverModePtr with the value OpMode provided by CanTrcv.
 * ServiceId           0x0e
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a
 *                     CAN transceiver, which is requested for current operation mode.
 * Param-Name[out]     TransceiverModePtr: Requested mode of requested network the Transceiver is connected to.
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Transceiver mode request has been accepted.
 *                     E_NOT_OK: Transceiver mode request has not been accepted.
 * PreCondition        CANIF_TRCV_NUMBER > 0
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_GetTrcvMode(                                                                            /* PRQA S 1503 */
                  P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_DATA) TransceiverModePtr, /* PRQA S 3432 */
                  uint8 TransceiverId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_GETTRCVMODE_ID, CANIF_E_UNINIT);
    }
    else if (TransceiverId >= CANIF_TRCV_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_GETTRCVMODE_ID, CANIF_E_PARAM_TRCV);
    }
    /* check NULL PTR*/
    else if (NULL_PTR == TransceiverModePtr)
    {
        CanIf_Det_ReportError(CANIF_GETTRCVMODE_ID, CANIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TrcvCfgType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        result = Can_TrcvApi[trcvConfigPtr->CanTrcvDriverId].CanTrcvGetOpModeApi(
            trcvConfigPtr->CanTrcvId,
            TransceiverModePtr);
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               This service returns the reason for the wake up of the transceiver
 *                     TransceiverId, via calling the corresponding CAN Transceiver Driver service.
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN
 *                     transceiver, which is requested for wake up reason.
 * Param-Name[out]     TrcvWuReasonPtr: provided pointer to where the requested transceiver wake
 *                     up reason shall be returned
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Transceiver wake up reason request has been accepted.
 *                     E_NOT_OK: Transceiver wake up reason request has not been accepted.
 * PreCondition        CANIF_TRCV_NUMBER > 0
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_GetTrcvWakeupReason(
    uint8 TransceiverId,
    P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_DATA) TrcvWuReasonPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_GETTRCVWAKEUPREASON_ID, CANIF_E_UNINIT);
    }
    else if (TransceiverId >= CANIF_TRCV_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_GETTRCVWAKEUPREASON_ID, CANIF_E_PARAM_TRCV);
    }
    /* check NULL PTR*/
    else if (NULL_PTR == TrcvWuReasonPtr)
    {
        CanIf_Det_ReportError(CANIF_GETTRCVWAKEUPREASON_ID, CANIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TrcvCfgType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        result = Can_TrcvApi[trcvConfigPtr->CanTrcvDriverId].CanTrcvGetBusWuReasonApi(
            trcvConfigPtr->CanTrcvId,
            TrcvWuReasonPtr);
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               This function shall call CanTrcv_SetTrcvWakeupMode.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a
 *                     CAN transceiver, which is requested for wake up notification mode transition.
 *                     TrcvWakeupMode: Requested transceiver wake up notification mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Will be returned, if the wake up notifications state has been changed to the requested
 *                     mode.
 *                     E_NOT_OK: Will be returned, if the wake up notifications state change has failed or the parameter
 *                     is out of the allowed range. The previous state has not been changed.
 * PreCondition        CANIF_TRCV_NUMBER > 0
 * CallBy              API Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check whether module been initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETTRCVWAKEUPMODE_ID, CANIF_E_UNINIT);
    }
    else if (TransceiverId >= CANIF_TRCV_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_SETTRCVWAKEUPMODE_ID, CANIF_E_PARAM_TRCV);
    }
    else if (TrcvWakeupMode > CANTRCV_WUMODE_CLEAR)
    {
        CanIf_Det_ReportError(CANIF_SETTRCVWAKEUPMODE_ID, CANIF_E_PARAM_TRCVWAKEUPMODE);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TrcvCfgType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        result = Can_TrcvApi[trcvConfigPtr->CanTrcvDriverId].CanTrcvSetWakeupModeApi(
            trcvConfigPtr->CanTrcvId,
            TrcvWakeupMode);
    }
    return result;
}
#endif /* CANIF_TRCV_NUMBER > 0u */
#if (STD_ON == CANIF_WAKE_UP_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service checks, whether an underlying CAN driver or a CAN
 *                     transceiver driver already signals a wakeup event.
 * ServiceId           0x11
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeupSource: Source device, which initiated the wake up event:CAN controller or CAN transceiver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Will be returned, if the check wake up request has been accepted
 *                     E_NOT_OK: Will be returned, if the check wake up request has not been accepted
 * PreCondition        CANIF_WAKE_UP_SUPPORT is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* CanIf not initialized */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CHECKWAKEUP_ID, CANIF_E_UNINIT);
    }
    /*ECUM_WKSOURCE_POWER: bit 0,ECUM_WKSOURCE_RESET: bit 1,ECUM_WKSOURCE_INTERNAL_RESET:bit 2,
     * ECUM_WKSOURCE_INTERNAL_WDG:bit 3,ECUM_WKSOURCE_EXTERNAL_WDG:bit 4*/
    else if (WakeupSource > CANIF_WAKEUPSOURCE_MAX)
    {
        CanIf_Det_ReportError(CANIF_CHECKWAKEUP_ID, CANIF_E_PARAM_WAKEUPSOURCE);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
/*all controller and Trcv need to check*/
#if ((STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT) || (STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT))
        uint8 index;
        EcuM_WakeupSourceType wakeupSource;
        boolean controllerNotSleep = FALSE;
#endif /* STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT || STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT */
/*check all can controllers to find the wake-up source*/
#if ((STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT) && (CANIF_CANCONTROLLER_NUMBER > 0u))
        uint8 canDriver;
        const CanIf_ControllerCfgType* controllerPtr;
        for (index = 0u; index < CANIF_CANCONTROLLER_NUMBER; index++)
        {
            controllerPtr = &CanIf_CtrlCfgData[index];
            if (controllerPtr->CanIfCtrlWakeupSupport)
            {
                wakeupSource = controllerPtr->CanIfWakeUpSource;
                if (0u != (WakeupSource & wakeupSource))
                {
                    if (CAN_CS_SLEEP != CanIf_ControllerMode[index])
                    {
                        controllerNotSleep = TRUE;
                        break;
                    }
                    else
                    {
                        canDriver = controllerPtr->CanDriverId;
                        if (NULL_PTR != Can_DriverApi[canDriver].CanCheckWakeupApi)
                        {
                            if (CAN_OK == Can_DriverApi[canDriver].CanCheckWakeupApi(controllerPtr->CanCtrlId))
                            {
                                result = E_OK;
                                break;
                            }
                        }
                    }
                }
            }
        }
#endif /* STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT && CANIF_CANCONTROLLER_NUMBER > 0u */
/*check all cantrcvs to find the wake-up source*/
#if ((STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT) && (CANIF_TRCVDRIVER_NUMBER > 0u))
        uint8 trcvDriver;
        const CanIf_TrcvCfgType* trcvConfigPtr;
        if (!controllerNotSleep)
        {
            for (index = 0u; (index < CANIF_TRCV_NUMBER) && (result == E_NOT_OK); index++)
            {
                trcvConfigPtr = &CanIf_TrcvCfgData[index];
                if (trcvConfigPtr->CanIfTrcvWakeupSupport)
                {
                    wakeupSource = trcvConfigPtr->CanIfWakeUpSource;
                    if (0u != (WakeupSource & wakeupSource))
                    {
                        trcvDriver = trcvConfigPtr->CanTrcvDriverId;
                        if (NULL_PTR != Can_TrcvApi[trcvDriver].CanTrcvCheckWakeupApi)
                        {
                            result = Can_TrcvApi[trcvDriver].CanTrcvCheckWakeupApi(trcvConfigPtr->CanTrcvId);
                        }
                    }
                }
            }
        }
#endif /* STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT && CANIF_TRCVDRIVER_NUMBER > 0u */
#if ((STD_ON == CANIF_CANTRCV_WAKE_UP_SUPPORT) || (STD_ON == CANIF_CANDRV_WAKE_UP_SUPPORT))
        /* PRQA S 2992,2996,2880 ++ */ /* MISRA Rule 14.3,2.2,2.1 */
        if (controllerNotSleep)
        {
            CanIf_Det_ReportRuntimeError(CANIF_CHECKWAKEUP_ID, CANIF_E_NOT_SLEEP);
        }
        /* PRQA S 2992,2996,2880 -- */ /* MISRA Rule 14.3,2.2,2.1 */
#endif
    }
    return result;
}
#endif /* STD_ON == CANIF_WAKE_UP_SUPPORT */
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service is performed to validate a previous wakeup event.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeupSource: Source device which initiated the wake-up event and
 *                     which has to be validated: CAN controller or CAN transceiver
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Will be returned, if the check validation request has been accepted.
 *                     E_NOT_OK: Will be returned, if the check validation request has not been accepted.
 * PreCondition        CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CHECKVALIDATION_ID, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    /*ECUM_WKSOURCE_POWER: bit 0,ECUM_WKSOURCE_RESET: bit 1,ECUM_WKSOURCE_INTERNAL_RESET:bit 2,
     * ECUM_WKSOURCE_INTERNAL_WDG:bit 3,ECUM_WKSOURCE_EXTERNAL_WDG:bit 4*/
    if ((TRUE == detNoErr) && (WakeupSource > CANIF_WAKEUPSOURCE_MAX))
    {
        CanIf_Det_ReportError(CANIF_CHECKVALIDATION_ID, CANIF_E_PARAM_WAKEUPSOURCE);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 controllerIndex;
        EcuM_WakeupSourceType wakeupSource;
        const CanIf_ControllerCfgType* ctrlCfgPtr = &CanIf_CtrlCfgData[0];
        /*check all can controllers to find the wake-up source*/
        for (controllerIndex = 0u; controllerIndex < CANIF_CANCONTROLLER_NUMBER; controllerIndex++)
        {
            wakeupSource = ctrlCfgPtr->CanIfWakeUpSource;
            /* check wake-up source */
            if (0u != (WakeupSource & wakeupSource))
            {
                /* do check up Validation action */
                if ((CAN_CS_STARTED == CanIf_ControllerMode[controllerIndex])
                    && (CanIf_FirstCallRxInd[controllerIndex]))
                {
                    if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserValidateWakeupEventName)
                    {
                        /* call upper(EcuM) layer function */
                        CanIf_DispatchConfigData.CanIfDispatchUserValidateWakeupEventName(WakeupSource & wakeupSource);
                    }
                    result = E_OK;
                    break;
                }
            }
            ctrlCfgPtr++;
        }
    }
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT */
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service reports, if any TX confirmation has been done for the whole
 *                     CAN controller since the last CAN controller start.
 * ServiceId           0x19
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (Not for the same controller)
 * Param-Name[in]      ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType:Combined TX confirmation status for all TX PDUs of the CAN controller
 * PreCondition        CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT is TRUE
 * CallByAPI           Up layer(CanSM)
 */
/*************************************************************************/
FUNC(CanIf_NotifStatusType, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_GetTxConfirmationState(uint8 ControllerId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_GETTXCONFIRMATIONSTATE, CANIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (ControllerId >= CANIF_CANCONTROLLER_NUMBER))
    {
        CanIf_Det_ReportError(CANIF_GETTXCONFIRMATIONSTATE, CANIF_E_PARAM_CONTROLLERID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
        /* get the  TX confirmation state of the corresponding controller */
        result = CanIf_TxConfirmationState[ControllerId];
        /* clear the state */
        CanIf_TxConfirmationState[ControllerId] = CANIF_NO_NOTIFICATION;
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT */
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
/*************************************************************************/
/*
 * Brief               Requests the CanIf module to clear the WUF flag of the designated CAN transceiver.
 * ServiceId           0x1e
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different CAN transceivers
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to the designated CAN transceiver.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Request has been accepted
 *                     E_NOT_OK: Request has not been accepted
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           Up layer(CanSM)
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_ClearTrcvWufFlag(uint8 TransceiverId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CLEARTRCVWUFFLAG_ID, CANIF_E_UNINIT);
    }
    else if (TransceiverId >= CANIF_TRCV_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_CLEARTRCVWUFFLAG_ID, CANIF_E_PARAM_TRCV);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TrcvCfgType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        result = Can_TrcvApi[trcvConfigPtr->CanTrcvDriverId].CanTrcvClearTrcvWufFlagApi(trcvConfigPtr->CanTrcvId);
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               Requests the CanIf module to check the Wake flag of the designated CAN transceiver.
 * ServiceId           0x1f
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different CAN transceivers
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to the designated CAN transceiver.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Request has been accepted
 *                     E_NOT_OK: Request has not been accepted
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           Up layer(CanSM)
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_CheckTrcvWakeFlag(uint8 TransceiverId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CHECKTRCVWAKEFLAG_ID, CANIF_E_UNINIT);
    }
    else if (TransceiverId >= CANIF_TRCV_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_CHECKTRCVWAKEFLAG_ID, CANIF_E_PARAM_TRCV);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TrcvCfgType* trcvConfigPtr = &CanIf_TrcvCfgData[TransceiverId];
        result = Can_TrcvApi[trcvConfigPtr->CanTrcvDriverId].CanTrcvCheckWakeFlagApi(trcvConfigPtr->CanTrcvId);
    }
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_PN_SUPPORT */
#if (STD_ON == CANIF_SET_BAUDRATE_API)
/*************************************************************************/
/*
 * Brief               This service shall set the baud rate configuration of the CAN controller.
 *                     Depending on necessary baud rate modifications the controller might have to reset.
 * ServiceId           0x27
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different ControllerIds. Non reentrant for the same ControllerId.
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller, whose baud rate
 * shall be set. BaudRateConfigID: references a baud rate configuration by ID (see CanControllerBaudRateConfigID)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Service request accepted, setting of (new) baud rate started
 *                     E_NOT_OK: Service request not accepted
 * PreCondition        CANIF_SET_BAUDRATE_API is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETBAUDRATE_ID, CANIF_E_UNINIT);
    }
    else if (ControllerId >= CANIF_CANCONTROLLER_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_SETBAUDRATE_ID, CANIF_E_PARAM_CONTROLLERID);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_ControllerCfgType* controllerPtr = &CanIf_CtrlCfgData[ControllerId];
        result =
            Can_DriverApi[controllerPtr->CanDriverId].CanSetBaudrateApi(controllerPtr->CanCtrlId, BaudRateConfigID);
    }
    return result;
}
#endif /* STD_ON == CANIF_SET_BAUDRATE_API */
#if (STD_ON == CANIF_PUBLIC_ICOM_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service shall change the Icom Configuration of a CAN controller to the requested one.
 * ServiceId           0x25
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant only for different controller Ids
 * Param-Name[in]      ControllerId: Abstracted CanIf Controller Id which is assigned to a CAN controller.
 *                     ConfigurationId: Requested Configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: Request accepted
 *                     E_NOT_OK: Request denied
 * PreCondition        CANIF_PUBLIC_ICOM_SUPPORT is TRUE
 * CallByAPI           Up layer
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_SetIcomConfiguration(uint8 ControllerId, IcomConfigIdType ConfigurationId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_SETICOMCONFIGURATION_ID, CANIF_E_UNINIT);
    }
    else if (ControllerId >= CANIF_CANCONTROLLER_NUMBER)
    {
        CanIf_Det_ReportError(CANIF_SETICOMCONFIGURATION_ID, CANIF_E_PARAM_CONTROLLERID);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_ControllerCfgType* controllerPtr = &CanIf_CtrlCfgData[ControllerId];
        result = Can_DriverApi[controllerPtr->CanDriverId].CanSetIcomConfigurationApi(
            controllerPtr->CanCtrlId,
            ConfigurationId);
    }
    return result;
}
#endif /* STD_ON == CANIF_PUBLIC_ICOM_SUPPORT */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/*******************************************************************************
**                      CallBack API                                          **
*******************************************************************************/
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
/*************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module) shall check
 *                     whether the available data fits into the buffer size reported by PduInfoPtr-
 *                     >SduLength. If it fits, it shall copy its data into the buffer provided by
 *                     PduInfoPtr->SduDataPtr and update the length of the actual copied data in
 *                     PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the SDU that is requested to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU data
 *                     shall be copied, and the available buffer size in SduLengh.On return, the
 *                     service will indicate the length of the copied SDU data in SduLength.
 * Return              Std_ReturnType
 *                     E_OK: SDU has been copied and SduLength indicates the number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must not be used since it may
 *                     contain a NULL pointer or point to invalid data.
 * PreCondition        CANIF_TRIGGER_TRANSMIT_SUPPORT is TRUE
 * CallByAPI           Lo layer
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFTRIGGERTRANSMIT_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType, CANIF_CODE)
CanIf_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_TRIGGERTRANSMIT_ID, CANIF_E_UNINIT);
    }
    else if (TxPduId >= CanIf_ConfigStd->CanIfTxPduNumber)
    {
        CanIf_Det_ReportError(CANIF_TRIGGERTRANSMIT_ID, CANIF_E_INVALID_TXPDUID);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(TxPduId);
        if (txPduConfigPtr->CanIfTxPduTriggerTransmit)
        {
            if (NULL_PTR != CanIf_UpTriggerTransmitArray[txPduConfigPtr->CanIfUpTxTrigApiIndex])
            {
                result = CanIf_UpTriggerTransmitArray[txPduConfigPtr->CanIfUpTxTrigApiIndex](
                    txPduConfigPtr->CanIfUpPduId,
                    PduInfoPtr);
            }
        }
    }
    return result;
}
#define CANIF_STOP_SEC_CANIFTRIGGERTRANSMIT_CALLBACK_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT */
/*************************************************************************/
/*
 * Brief               This service confirms a previously successfully processed transmission of a CAN TxPDU.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTxPduId: L-PDU handle of CAN L-PDU successfully transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_TxConfirmation(PduIdType CanTxPduId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_TXCONFIRMATION_ID, CANIF_E_UNINIT);
    }
    /* check parameter valid */
    else if (CanTxPduId >= CanIf_ConfigStd->CanIfTxPduNumber)
    {
        CanIf_Det_ReportError(CANIF_TXCONFIRMATION_ID, CANIF_E_PARAM_LPDU);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanTxPduId);
#if ((STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT) || (STD_ON == CANIF_PUBLIC_PN_SUPPORT))
        Can_HwHandleType hthIndex = txPduConfigPtr->CanIfTxPduHthId;
        Can_HwHandleType controllerId = CANIF_HTH(hthIndex).CanIfHthCanCtrlId;
#endif
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
        CanIf_TransmitBufferedPdu(CanTxPduId);
#endif
/* enable notify status */
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
        CanIf_SetTxNotifStatusHandle(CanTxPduId);
#endif /* STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API */
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
        if (CAN_CS_STARTED == CanIf_ControllerMode[controllerId])
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_TxConfirmationState[controllerId] = CANIF_TX_RX_NOTIFICATION;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
#endif /* STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT */
        if (NULL_PTR != CanIf_UpTxConfirmationArray[txPduConfigPtr->CanIfUpTxConfApiIndex])
        {
            /* call upper layer function,feedback to upper layer*/
            CanIf_UpTxConfirmationArray[txPduConfigPtr->CanIfUpTxConfApiIndex](txPduConfigPtr->CanIfUpPduId);
        }
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
        if (CanIf_CtrlCfgData[controllerId].CanIfPnFilter)
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_PnTxFilter[controllerId] = FALSE;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
#endif /* STD_ON == CANIF_PUBLIC_PN_SUPPORT */
    }
}
#define CANIF_STOP_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
/*************************************************************************/
/*
 * Brief               This service indicates a successful reception of a received CAN Rx LPDU
 *                     to the CanIf after passing all filters and validation checks.
 * ServiceId           0x14
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Mailbox: Identifies the HRH and its corresponding CAN Controller
 *                     PduInfoPtr: Pointer to the received L-PDU
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFRXINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_RxIndication(
    P2CONST(Can_HwType, AUTOMATIC, CANIF_APPL_DATA) Mailbox,
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    /* check initiation */
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_RXINDICATION_ID, CANIF_E_UNINIT);
    }
    else if ((NULL_PTR == Mailbox) || (NULL_PTR == PduInfoPtr))
    {
        CanIf_Det_ReportError(CANIF_RXINDICATION_ID, CANIF_E_PARAM_POINTER);
    }
#if (CANIF_CANID_UINT16 == CANIF_CAN_IDTYPE)
    else if (0x0000u != (Mailbox->CanId & 0xb800u)) /*Standard frame*/
#else
    else if (
        (0x80000000u <= Mailbox->CanId) ? (0x80000000u != (Mailbox->CanId & 0xa0000000u))  /*Extended Frame*/
                                        : (0x00000000u != (Mailbox->CanId & 0xbffff800u))) /*Standard frame*/
#endif /* CANIF_CANID_UINT16 == CANIF_CAN_IDTYPE */
    {
        CanIf_Det_ReportError(CANIF_RXINDICATION_ID, CANIF_E_PARAM_CANID);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 controllerId;
        CanIf_GetCanIfCtrlId(&controllerId, Mailbox->ControllerId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (controllerId >= CANIF_CANCONTROLLER_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_RXINDICATION_ID, CANIF_E_PARAM_CONTROLLERID);
        }
#if (STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER)
        else if (
            (Mailbox->Hoh >= CanIf_ConfigStd->CanIfHrhNumber)
            || (CANIF_HRH(Mailbox->Hoh).CanIfHrhCanCtrlId != controllerId))
#else
        else if (
            (Mailbox->Hoh >= CanIf_CanHoh2Hrh[CANIF_CONTROLLER_DRV_ID(controllerId)].CanIfCanHohLen)
            || (CanIf_CanHoh2Hrh[CANIF_CONTROLLER_DRV_ID(controllerId)].CanIfCanHoh2HrhRef[Mailbox->Hoh]
                == CANIF_UNUSED_UINT16))
#endif /* STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER */
        {
            CanIf_Det_ReportError(CANIF_RXINDICATION_ID, CANIF_E_PARAM_HOH);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            CanIf_IdType canId = (CanIf_IdType)Mailbox->CanId;
#if (CANIF_CANID_UINT16 == CANIF_CAN_IDTYPE)
            if ((canId & CANIF_CANID16_CANFD_MASK) != 0u)
            {
                /*Standard Fd frame*/
                canId = (canId & ~CANIF_CANID16_CANFD_MASK) | CANIF_CANID32_CANFD_MASK;
            }
#endif /* CANIF_CANID_UINT16 == CANIF_CAN_IDTYPE */
#if (STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER)
            Can_HwHandleType canIfHrhId = Mailbox->Hoh;
#else
            Can_HwHandleType canIfHrhId = (Can_HwHandleType)CanIf_CanHoh2Hrh[CANIF_CONTROLLER_DRV_ID(controllerId)]
                                              .CanIfCanHoh2HrhRef[Mailbox->Hoh];
#endif /* STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER */
            PduIdType rxPduIndex;
            if (E_OK == CanIf_CanIdRangCheck(canIfHrhId, canId, &rxPduIndex))
            {
                /* DLC check enabled */
#if (STD_ON == CANIF_PRIVATE_DLC_CHECK)
                if (CANIF_RXPDU(rxPduIndex).CanIfRxPduDataLengthCheck
                    && (PduInfoPtr->SduLength < (CANIF_RXPDU(rxPduIndex).CanIfRxPduDlc)))
                {
                    CanIf_Det_ReportRuntimeError(CANIF_RXINDICATION_ID, CANIF_E_INVALID_DLC);
                }
                else
#endif /* STD_ON == CANIF_PRIVATE_DLC_CHECK */
                {
                    CanIf_RxIndicationHandle(
                        rxPduIndex,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
                        canId,
#endif
                        PduInfoPtr);
                }
                SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM)
                if (CANIF_RXPDU(rxPduIndex).CanIfRxPduForNM)
                {
                    /* set first call rxindication event flag */
                    CanIf_FirstCallRxInd[controllerId] = TRUE;
                }
#else
                /* set first call rxindication event flag */
                CanIf_FirstCallRxInd[controllerId] = TRUE;
#endif /* STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM */
#endif /* STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT */

#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
                if (TRUE == CanIf_CtrlCfgData[controllerId].CanIfPnFilter)
                {
                    CanIf_PnTxFilter[controllerId] = FALSE;
                }
#endif /* STD_ON == CANIF_PUBLIC_PN_SUPPORT */
                SchM_Exit_CanIf_ExclusiveArea_Channel();
            }
        }
    }
}
#define CANIF_STOP_SEC_CANIFRXINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
/*************************************************************************/
/*
 * Brief               This service indicates a Controller BusOff event referring to
 *                     the corresponding CAN Controller with the abstract CanIf ControllerId.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a CAN
 *                     controller, where a BusOff occured.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFCONTROLLERBUSOFF_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_ControllerBusOff(uint8 ControllerId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CONTROLLERBUSOFF_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 controllerId;
        CanIf_GetCanIfCtrlId(&controllerId, ControllerId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (controllerId == CANIF_CANCONTROLLER_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_CONTROLLERBUSOFF_ID, CANIF_E_PARAM_CONTROLLERID);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
            CanIf_TxConfirmationState[controllerId] = CANIF_NO_NOTIFICATION;
#endif
            /* set controller to STOP status */
            CanIf_ControllerMode[controllerId] = CAN_CS_STOPPED;
            /*set the PDU channel mode of the corresponding channel to CANIF_TX_OFFLINE*/
            CanIf_PduMode[controllerId] = CANIF_TX_OFFLINE;
/* free tx buffer if enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
            CanIf_FreeControllerTxBuffer(controllerId);
#endif
            SchM_Exit_CanIf_ExclusiveArea_Channel();
            if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserCtrlBusOffName)
            {
                CanIf_DispatchConfigData.CanIfDispatchUserCtrlBusOffName(controllerId);
            }
        }
    }
}
#define CANIF_STOP_SEC_CANIFCONTROLLERBUSOFF_CALLBACK_CODE
#include "CanIf_MemMap.h"
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service indicates that the transceiver is running in PN
 *                     communication mode referring to the corresponding CAN transceiver
 *                     with the abstract CanIf TransceiverId.
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to
 *                     a CAN transceiver, which was checked for PN availability.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFCONFIRMPNAVAILABILITY_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_ConfirmPnAvailability(uint8 TransceiverId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CONFIRMPNAVAILABILITY_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 transceiverId;
        CanIf_GetCanIfTrcvId(&transceiverId, TransceiverId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (transceiverId == CANIF_TRCV_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_CONFIRMPNAVAILABILITY_ID, CANIF_E_PARAM_TRCV);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserConfirmPnAvailabilityName)
            {
                CanIf_DispatchConfigData.CanIfDispatchUserConfirmPnAvailabilityName(transceiverId);
            }
        }
    }
}
#define CANIF_STOP_SEC_CANIFCONFIRMPNAVAILABILITY_CALLBACK_CODE
#include "CanIf_MemMap.h"
/*************************************************************************/
/*
 * Brief               This service indicates that the transceiver has cleared the WufFlag
 *                     referring to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 * ServiceId           0x20
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                     for which this function was called.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFCLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CLEARTRCVWUFFLAGINDICATION_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 transceiverId;
        CanIf_GetCanIfTrcvId(&transceiverId, TransceiverId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (transceiverId == CANIF_TRCV_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_CLEARTRCVWUFFLAGINDICATION_ID, CANIF_E_PARAM_TRCV);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserClearTrcvWufFlagIndicationName)
            {
                CanIf_DispatchConfigData.CanIfDispatchUserClearTrcvWufFlagIndicationName(transceiverId);
            }
        }
    }
}
#define CANIF_STOP_SEC_CANIFCLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
/*************************************************************************/
/*
 * Brief               This service indicates that the check of the transceiver!/s wake-up flag
 *                     has been finished by the corresponding CAN transceiver with the abstract
 *                     CanIf TransceiverId. This indication is used to cope with the asynchronous
 *                     transceiver communication.
 * ServiceId           0x21
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to a CAN
 *                     transceiver, for which this function was called.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_PN_SUPPORT is TRUE
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFCHECKTRCVWAKEFLAGINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CHECKTRCVWAKEFLAGINDICATION_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 transceiverId;
        CanIf_GetCanIfTrcvId(&transceiverId, TransceiverId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (transceiverId == CANIF_TRCV_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_CHECKTRCVWAKEFLAGINDICATION_ID, CANIF_E_PARAM_TRCV);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserCheckTrcvWakeFlagIndicationName)
            {
                CanIf_DispatchConfigData.CanIfDispatchUserCheckTrcvWakeFlagIndicationName(transceiverId);
            }
        }
    }
}
#define CANIF_STOP_SEC_CANIFCHECKTRCVWAKEFLAGINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_PUBLIC_PN_SUPPORT */
/*************************************************************************/
/*
 * Brief               This service indicates a controller state transition referring
 *                     to the corresponding CAN controller with the abstract CanIf ControllerId.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a
 *                     CAN controller, which state has been transitioned.
 *                     ControllerMode: Mode to which the CAN controller transitioned
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Can driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFCONTROLLERMODEINDICATION_CALLBACK_CODE /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode) /* PRQA S 1532 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CONTROLLERMODEINDICATION_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 controllerId;
        CanIf_GetCanIfCtrlId(&controllerId, ControllerId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (controllerId == CANIF_CANCONTROLLER_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_CONTROLLERMODEINDICATION_ID, CANIF_E_PARAM_CONTROLLERID);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        /* store controller status */
        {
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_ControllerMode[controllerId] = ControllerMode;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
            if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserCtrlModeIndicationName)
            {
                CanIf_DispatchConfigData.CanIfDispatchUserCtrlModeIndicationName(controllerId, ControllerMode);
            }
        }
    }
}

#define CANIF_STOP_SEC_CANIFCONTROLLERMODEINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
#if (CANIF_TRCV_NUMBER > 0u)
/*************************************************************************/
/*
 * Brief               This service indicates a transceiver state transition referring
 *                     to the corresponding CAN transceiver with the abstract CanIf TransceiverId.
 * ServiceId           0x22
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId: Abstract CanIf TransceiverId, which is assigned to a
 *                     CAN transceiver, which state has been transitioned.
 *                     TransceiverMode Mode to which the CAN transceiver transitioned
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_TRCV_NUMBER > 0
 * CallByAPI           CanTrcv driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFTRCVMODEINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_TRCVMODEINDICATION_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 transceiverId;
        CanIf_GetCanIfTrcvId(&transceiverId, TransceiverId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (transceiverId == CANIF_TRCV_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_TRCVMODEINDICATION_ID, CANIF_E_PARAM_TRCV);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            if (NULL_PTR != CanIf_DispatchConfigData.CanIfDispatchUserTrcvModeIndicationName)
            {
                CanIf_DispatchConfigData.CanIfDispatchUserTrcvModeIndicationName(transceiverId, TransceiverMode);
            }
        }
    }
}
#define CANIF_STOP_SEC_CANIFTRCVMODEINDICATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
#endif /* CANIF_TRCV_NUMBER >0 */
#if (STD_ON == CANIF_PUBLIC_ICOM_SUPPORT)
/*************************************************************************/
/*
 * Brief               This service shall inform about the change of the Icom Configuration of
 *                     a CAN controller using the abstract CanIf ControllerId.
 * ServiceId           0x26
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant only for different controller Ids
 * Param-Name[in]      ControllerId: Abstract CanIf ControllerId which is assigned to a CAN
 *                     controller, which informs about the Configuration Id.
 *                     ConfigurationId: Active Configuration Id.
 *                     Error:
 *                     ICOM_SWITCH_E_OK: No Error
 *                     ICOM_SWITCH_E_FAILED: Switch to requested Configuration failed. Severe Error.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_ICOM_SUPPORT is TRUE
 * CallByAPI           Can driver
 */
/*************************************************************************/
#define CANIF_START_SEC_CANIFCURRENTICOMCONFIGURATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE)
CanIf_CurrentIcomConfiguration(uint8 ControllerId, IcomConfigIdType ConfigurationId, IcomSwitch_ErrorType Error)
{
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
    if (CANIF_INITED != CanIf_InitStatus)
    {
        CanIf_Det_ReportError(CANIF_CURRENTICOMCONFIGURATION_ID, CANIF_E_UNINIT);
    }
    else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
    {
        uint8 controllerId;
        CanIf_GetCanIfCtrlId(&controllerId, ControllerId);
#if (STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT)
        if (controllerId == CANIF_CANCONTROLLER_NUMBER)
        {
            CanIf_Det_ReportError(CANIF_CURRENTICOMCONFIGURATION_ID, CANIF_E_PARAM_CONTROLLERID);
        }
        else
#endif /* STD_ON == CANIF_PUBLIC_DEV_ERROR_DETECT */
        {
            CanSM_CurrentIcomConfiguration(controllerId, ConfigurationId, Error);
        }
    }
}
#define CANIF_STOP_SEC_CANIFCURRENTICOMCONFIGURATION_CALLBACK_CODE
#include "CanIf_MemMap.h"
#endif /* STD_ON == CANIF_PUBLIC_ICOM_SUPPORT */
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/*************************************************************************/
/*
 * Brief               Set controller state to start
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (Not for the same controller)
 * Param-Name[in]      Controller: Number of controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetControllerMode
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllStarted(uint8 Controller)
{
    Std_ReturnType result = E_NOT_OK;
    const CanIf_ControllerCfgType* controllerPtr = &CanIf_CtrlCfgData[Controller];
    /*find which Can Driver API can be invoked*/
    uint8 canDriverId = controllerPtr->CanDriverId;
    /* get Can controller Id,defined in Can Driver */
    uint8 controllerId = controllerPtr->CanCtrlId;
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
    if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_CS_STARTED))
#else  /*The default version of can driver is 4.2.2*/
    if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_T_START))
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
    {
        result = E_OK;
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               Set controller state to sleep
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (Not for the same controller)
 * Param-Name[in]      Controller: Number of controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetControllerMode
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllSleep(uint8 Controller)
{
    Std_ReturnType result = E_NOT_OK;
    const CanIf_ControllerCfgType* controllerPtr = &CanIf_CtrlCfgData[Controller];
    /*find which Can Driver API can be invoked*/
    uint8 canDriverId = controllerPtr->CanDriverId;
    /* get Can controller Id,defined in Can Driver */
    uint8 controllerId = controllerPtr->CanCtrlId;
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
    if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_CS_SLEEP))
#else  /*The default version of can driver is 4.2.2*/
    if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_T_SLEEP))
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
        /* Clear first call rxindication event flag While Sleep */
        CanIf_FirstCallRxInd[Controller] = FALSE;
#endif
        /*set the PDU channel mode of the corresponding channel to CANIF_OFFLINE*/
        CanIf_PduMode[Controller] = CANIF_OFFLINE;
/* free tx buffer if enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
        CanIf_FreeControllerTxBuffer(Controller);
#endif
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
        if (controllerPtr->CanIfPnFilter)
        {
            CanIf_PnTxFilter[Controller] = TRUE;
        }
#endif
        SchM_Exit_CanIf_ExclusiveArea_Channel();
        result = E_OK;
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               Set controller state to stop
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (Not for the same controller)
 * Param-Name[in]      Controller: Number of controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_SetControllerMode
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllStopped(uint8 Controller)
{
    Std_ReturnType result = E_NOT_OK;
    const CanIf_ControllerCfgType* controllerPtr = &CanIf_CtrlCfgData[Controller];
    /*find which Can Driver API can be invoked*/
    uint8 canDriverId = controllerPtr->CanDriverId;
    /* get Can controller Id,defined in Can Driver */
    uint8 controllerId = controllerPtr->CanCtrlId;
    /* current mode is SLEEP */
    if (CAN_CS_SLEEP == CanIf_ControllerMode[Controller])
    {
        /* wake up the controller */
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
        if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_CS_STOPPED))
#else  /*The default version of can driver is 4.2.2*/
        if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_T_WAKEUP))
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
        {
            /*set the PDU channel mode of the corresponding channel to CANIF_TX_OFFLINE*/
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_PduMode[Controller] = CANIF_TX_OFFLINE;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
            result = E_OK;
        }
    }
    else
    {
        /* set controller mode STOP */
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
        if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_CS_STOPPED))
#else  /*The default version of can driver is 4.2.2*/
        if (CAN_OK == Can_DriverApi[canDriverId].CanSetControllerModeApi(controllerId, CAN_T_STOP))
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
        {
            CanIf_FreeBufInStopped(Controller);
            /*set the PDU channel mode of the corresponding channel to CANIF_TX_OFFLINE*/
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_PduMode[Controller] = CANIF_TX_OFFLINE;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
            result = E_OK;
        }
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               Check (Basic Can) CanId and find the rxpduId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      hrhId: ID of the corresponding hardware object
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that
 *                     has been successfully received
 * Param-Name[out]     pduIdIndex:the rx pdu that receive the can message
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_RxIndication
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE)
    CanIf_CanIdRangCheck(Can_HwHandleType hrhId, CanIf_IdType canId, P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) pduIdIndex)
{
    Std_ReturnType result = E_NOT_OK;
    boolean isSearch = FALSE;
    /*The controllerId is CanIf's controller index*/
    /*The hrhId is Can's HardwareObject index*/
#if ((STD_ON == CANIF_HRH_RANGE_SUPPORT) || (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY))
    const CanIfHrhCfgType* hrhPtr = &CANIF_HRH(hrhId);
#endif /* STD_ON == CANIF_HRH_RANGE_SUPPORT || CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_LINEAR */
#if (STD_ON == CANIF_HRH_RANGE_SUPPORT)
    uint8 hrhCanIdRangeNum = hrhPtr->CanIfHrhCanIdRangeNum;
    const CanIfHrhRangeType* hrhRangePtr = &(hrhPtr->CanIfHrhCanIdRange[0]);
    /* if the hrh defined as Basic Can and sofware filter enabled */
    if ((CANIF_BASIC_CAN == hrhPtr->CanIfHrhType) && (0u < hrhCanIdRangeNum))
    {
        Can_IdType canIdMasked = canId & CANIF_EXTENDED_CANID_MAX;
        /* do HRH software filter*/
        for (Can_HwHandleType cnt = 0u; cnt < hrhCanIdRangeNum; cnt++)
        {
            if ((canIdMasked >= hrhRangePtr->CanIfHrhRangeRxPduLowerCanId)
                && (canIdMasked <= hrhRangePtr->CanIfHrhRangeRxPduUpperCanId))
            {
                /* pass the HRH range filter */
                isSearch = TRUE;
                break;
            }
            hrhRangePtr++;
        }
    }
    else /* FullCan */
#endif   /* STD_ON == CANIF_HRH_RANGE_SUPPORT */
    {
        isSearch = TRUE;
    }
    if (isSearch)
    {
        /* find the rxpduId due to the hrhId */
#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_LINEAR)
        result = CanIf_RxPduHrhSearchByLinear(hrhId, canId, pduIdIndex);
#elif (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
        if (hrhPtr->CanIfSoftFilterType == CANIF_LINEAR)
        {
            result = CanIf_RxPduHrhSearchByLinear(hrhId, canId, pduIdIndex);
        }
        else /* CANIF_BINARY */
        {
            result = CanIf_RxPduHrhSearchByBinary(hrhId, canId, pduIdIndex);
        }
#endif /* CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_LINEAR */
    }
    return result;
}
/*************************************************************************/
/*
 * Brief               Can pdu pack for transmit
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTxPduId: the id of Tx Pdu
 *                     PduInfoPtr: Pointer to a structure with CAN L-SDU related data:
 *                     DLC and pointer to CAN L-SDU buffer including the MetaData of dynamic L-PDUs.
 * Param-Name[out]     None
 * Param-Name[in/out]  canPdu: Pointer to SDU user memory, DLC and Identifier.
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_Transmit
 */
/*************************************************************************/
static FUNC(void, CANIF_CODE) CanIf_PduPack(
    PduIdType CanTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr,
    P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) canPdu)
{
    CanIf_IdType canId;
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanTxPduId);
    CanIf_TxPduCanIdMode canIdMode = txPduConfigPtr->CanIfTxPduCanIdType;
    CanIf_GetBaseCanIdHandle(
        CanTxPduId,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
        PduInfoPtr,
#endif
        &canId);
    /*set the 2 bits of highest CanId base the CanIf_TxPduCanIdMode*/
    switch (canIdMode)
    {
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
    case CANIF_TX_EXTENDED_CAN:
        canId |= (CanIf_IdType)CANIF_CANID_EXTENDED_MASK;
        break;
#endif /* STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT */
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
    case CANIF_TX_EXTENDED_FD_CAN:
        canId |= (CanIf_IdType)(CANIF_CANID32_CANFD_MASK | CANIF_CANID_EXTENDED_MASK);
        break;
#endif /* STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)
    case CANIF_TX_STANDARD_FD_CAN:
        canId |= (CanIf_IdType)CANIF_CANID32_CANFD_MASK;
        break;
#endif       /* STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT */
    default: /*CANIF_TX_STANDARD_CAN*/
        break;
    }
#if (CANIF_CANID_UINT16 == CANIF_CAN_IDTYPE)
    if ((canId & CANIF_CANID32_CANFD_MASK) != 0u)
    {
        /*Standard Fd frame,Fd Flag bit change to bit14 for 16 bit Can_IdType*/
        canPdu->id = (Can_IdType)(canId | CANIF_CANID16_CANFD_MASK);
    }
    else
    {
        canPdu->id = (Can_IdType)canId;
    }
#else
    canPdu->id = canId;
#endif /* CANIF_CANID_UINT16 == CANIF_CAN_IDTYPE */
    /* get PDU length */
    canPdu->length = (uint8)PduInfoPtr->SduLength;
    if (((PduLengthType)canPdu->length) > txPduConfigPtr->CanIfTxPduDlc)
    {
        canPdu->length = (uint8)txPduConfigPtr->CanIfTxPduDlc;
    }
    /* get PDU handle */
    canPdu->swPduHandle = CanTxPduId;
    /* get SDU */
    canPdu->sdu = PduInfoPtr->SduDataPtr;
}
/*************************************************************************/
/*
 * Brief               direct switch to Stopped Mode,and do buffer clear.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: the id of can controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_SetControllSleep,CanIf_SetControllStopped
 */
/*************************************************************************/
static FUNC(void, CANIF_CODE) CanIf_FreeBufInStopped(uint8 Controller)
{
    /* free tx buffer if enabled */
    SchM_Enter_CanIf_ExclusiveArea_Channel();
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
    CanIf_FreeControllerTxBuffer(Controller);
#endif
/* Init CanIf RxBuffer */
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
    CanIf_FreeControllerRxBuffer(Controller);
#endif
/* set the rx and tx Notify status buffer Of this controller */
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
    CanIf_FreeRxTxNotifyStatus(Controller);
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
    CanIf_TxConfirmationState[Controller] = CANIF_NO_NOTIFICATION;
#endif
    /* init dynamic CAN ID array */
    CanIf_FreeDynamicTxPduCanIds(Controller);
    SchM_Exit_CanIf_ExclusiveArea_Channel();
}
/*************************************************************************/
/*
 * Brief               init all buffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_Init
 */
/*************************************************************************/
static FUNC(void, CANIF_CODE) CanIf_InitAllBuffer(VAR(void, AUTOMATIC))
{
/* free tx buffer if enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
    CanIf_InitTxBuffer();
#endif
/* (re-)init the rx Buffer */
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
    CanIf_InitRxBuffer();
#endif
/* re-init the txNotifStatus Of this controller */
/* re-init the rxNotifStatus Of this controller */
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
    CanIf_InitRxTxNotifyStatus();
#endif
    /* init the DynamicTxPduCanId with the configured canId */
    CanIf_InitDynamicTxPduCanId();
}
/*************************************************************************/
/*
 * Brief               Linear Search the index of RxPdu Config due to the Hrh and CanId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      hrhIndex: ID of the corresponding hardware object
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that
 *                     has been successfully received
 * Param-Name[out]     rxPduIndex: the rx pdu that receive the can message
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_CanIdRangCheck
 *
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxPduHrhSearchByLinear(
    Can_HwHandleType hrhIndex,
    CanIf_IdType CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex)
{
    Std_ReturnType result = E_NOT_OK;
    PduIdType cnt, pduIdIndex;
    const CanIf_RxPduConfigType* rxPduConfigPtr;
    const CanIfHrhCfgType* hrhConfigPtr = &CANIF_HRH(hrhIndex);
    PduIdType hrhRefPduNum = hrhConfigPtr->CanIfRxPduRefNum;
    for (cnt = 0u; cnt < hrhRefPduNum; cnt++)
    {
        pduIdIndex = hrhConfigPtr->CanIfRxPduIdRef[cnt];
        rxPduConfigPtr = &CANIF_RXPDU(pduIdIndex);
        if (rxPduConfigPtr->CanIfRxPduCanIdMasked == (rxPduConfigPtr->CanIfRxPduCanIdMask & CanId))
        {
            result = CanIf_GetPduHrh(pduIdIndex, CanId, rxPduIndex);
            if (E_OK == result)
            {
                break;
            }
        }
    }
    return result;
}

#if (CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY)
/*************************************************************************/
/*
 * Brief               Binary Search the index of RxPdu Config due to the Hrh and CanId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      hrhIndex: ID of the corresponding hardware object
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that
 *                     has been successfully received
 * Param-Name[out]     rxPduIndex: the rx pdu that receive the can message
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_CanIdRangCheck
 *
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxPduHrhSearchByBinary(
    Can_HwHandleType hrhIndex,
    CanIf_IdType CanId,
    P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex)
{
    Std_ReturnType result = E_NOT_OK;
    const CanIfHrhCfgType* hrhConfigPtr = &CANIF_HRH(hrhIndex);
    CanIf_IdType rxPduCfgCanIdMasked;

    PduIdType minPduIdIndex = hrhConfigPtr->CanIfRxPduIdRef[0];
    /* Using binary search to find indicate that all Pdu associated with HRH using the same Mask value. */
    CanIf_IdType hrhCanIdMasked = CANIF_RXPDU(minPduIdIndex).CanIfRxPduCanIdMask & CanId;
    PduIdType maxPduIdIndex = minPduIdIndex + hrhConfigPtr->CanIfRxPduRefNum - 1u;
    PduIdType midPduIdIndex;
    while (minPduIdIndex <= maxPduIdIndex)
    {
        midPduIdIndex = (maxPduIdIndex + minPduIdIndex) >> 1u;
        rxPduCfgCanIdMasked = CANIF_RXPDU(midPduIdIndex).CanIfRxPduCanIdMasked;
        if (hrhCanIdMasked == rxPduCfgCanIdMasked)
        {
            result = CanIf_GetPduHrh(midPduIdIndex, CanId, rxPduIndex);
            break;
        }
        else if (hrhCanIdMasked > rxPduCfgCanIdMasked)
        {
            minPduIdIndex = midPduIdIndex + 1u;
        }
        else
        {
            maxPduIdIndex = midPduIdIndex - 1u;
        }
    }
    return result;
}
#endif /* CANIF_SOFTWARE_FILTER_TYPE == CANIF_SOFTWARE_FILTER_BINARY */

/*************************************************************************/
/*
 * Brief               Get the index of RxPdu Config due to the pduIdIndex and CanId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      pduIdIndex: ID of the pdu
 *                     CanId: Standard/Extended CAN ID of CAN L-PDU that
 *                     has been successfully received
 * Param-Name[out]     rxPduIndex: the rx pdu that receive the can message
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 * PreCondition        None
 * CallByAPI           CanIf_RxPduHrhSearchByLinear,CanIf_RxPduHrhSearchByBinary
 *
 */
/*************************************************************************/
static FUNC(Std_ReturnType, CANIF_CODE)
    CanIf_GetPduHrh(PduIdType pduIdIndex, CanIf_IdType CanId, P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) rxPduIndex)
{
    Std_ReturnType result = E_NOT_OK;
    const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(pduIdIndex);
#if (STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT)
    if ((rxPduConfigPtr->CanIfRxCanIdRangeRef == NULL_PTR)
        || ((rxPduConfigPtr->CanIfRxCanIdRangeRef != NULL_PTR)
            && ((CanId & CANIF_EXTENDED_CANID_MAX) >= rxPduConfigPtr->CanIfRxCanIdRangeRef[0].CanIfRxPduLowerCanId)
            && ((CanId & CANIF_EXTENDED_CANID_MAX) <= rxPduConfigPtr->CanIfRxCanIdRangeRef[0].CanIfRxPduUpperCanId)))
#endif /* STD_ON == CANIF_RXPDU_CANID_RANGE_SUPPORT */
    {
        switch (rxPduConfigPtr->CanIfRxPduCanIdType)
        {
#if (STD_ON == CANIF_RX_EXTENDED_CAN_SUPPORT)
        case CANIF_RX_EXTENDED_CAN:
            if (CanId >= 0x80000000u)
            {
                result = E_OK;
                *rxPduIndex = pduIdIndex;
            }
            break;
#endif /* STD_ON == CANIF_RX_EXTENDED_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_EXTENDED_FD_CAN_SUPPORT)
        case CANIF_RX_EXTENDED_FD_CAN:
            if (CanId >= 0xc0000000u)
            {
                result = E_OK;
                *rxPduIndex = pduIdIndex;
            }
            break;
#endif /* STD_ON == CANIF_RX_EXTENDED_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_EXTENDED_NO_FD_CAN_SUPPORT)
        case CANIF_RX_EXTENDED_NO_FD_CAN:
            if ((CanId >= 0x80000000u) && (CanId < 0xc0000000u))
            {
                result = E_OK;
                *rxPduIndex = pduIdIndex;
            }
            break;
#endif /* STD_ON == CANIF_RX_EXTENDED_NO_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_STANDARD_CAN_SUPPORT)
        case CANIF_RX_STANDARD_CAN:
            if (CanId < 0x80000000u)
            {
                result = E_OK;
                *rxPduIndex = pduIdIndex;
            }
            break;
#endif /* STD_ON == CANIF_RX_STANDARD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_STANDARD_FD_CAN_SUPPORT)
        case CANIF_RX_STANDARD_FD_CAN:
            if ((CanId < 0x80000000u) && (CanId >= 0x40000000u))
            {
                result = E_OK;
                *rxPduIndex = pduIdIndex;
            }
            break;
#endif /* STD_ON == CANIF_RX_STANDARD_FD_CAN_SUPPORT */
#if (STD_ON == CANIF_RX_STANDARD_NO_FD_CAN_SUPPORT)
        case CANIF_RX_STANDARD_NO_FD_CAN:
            if (CanId < 0x40000000u)
            {
                result = E_OK;
                *rxPduIndex = pduIdIndex;
            }
            break;
#endif /* STD_ON == CANIF_RX_STANDARD_NO_FD_CAN_SUPPORT */
        default:
            /*do nothing*/
            break;
        }
    }
    return result;
}

/*************************************************************************/
/*
 * Brief               do RxIndication Handle with the correct rxPuIndex
 *                     which get by hrh and CanId through CanIf_CanIdRangCheck
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      pduIdIndex: Rx Pdu id
 *                     canId: 11/29 bit CanId
 *                     PduInfoPtr: Pointer to a structure with CAN L-SDU related data:
 *                     DLC and pointer to CAN L-SDU buffer including the MetaData of dynamic L-PDUs.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_RxIndication
 */
/*************************************************************************/
static FUNC(void, CANIF_CODE) CanIf_RxIndicationHandle(
    PduIdType pduIdIndex,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    CanIf_IdType canId,
#endif
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr)
{
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
    CanIf_SetRxNotifStatusHandle(pduIdIndex);
#endif /* STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API */

    PduInfoType pduInfo = *PduInfoPtr;
    const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(pduIdIndex);
#if (                                             \
    (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API) \
    || ((STD_OFF == CANIF_RXINDICATION_AUTOSAR_PARAMETER) && (STD_ON == CANIF_META_DATA_SUPPORT)))
    boolean metaDataEnable;
    CanIf_IdType metaData;
#endif

#if ((STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER) || (STD_OFF == CANIF_META_DATA_SUPPORT))
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    metaDataEnable = (NULL_PTR != PduInfo.MetaDataPtr);
    if (metaDataEnable)
    {
        metaData = CanIf_MetaDataToCanId(PduInfo.MetaDataPtr);
    }
#endif /* STD_ON == CANIF_META_DATA_SUPPORT */
#endif /* STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API */
#else
    metaDataEnable = rxPduConfigPtr->RxMetaDataEnable;
    if (metaDataEnable)
    {
        metaData = ((CanIf_IdType)CANIF_EXTENDED_CANID_MAX & canId);
        pduInfo.MetaDataPtr = (uint8*)(&metaData);
    }
    else
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#endif /* STD_ON == CANIF_RXINDICATION_AUTOSAR_PARAMETER) || (STD_OFF == CANIF_META_DATA_SUPPORT */

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
    CanIf_RxBufferNumType rxPduBufferIndex = rxPduConfigPtr->CanIf_RxBufferIndex;
    CanIf_RxPduBufferType* rxPduBuffer;
    if (rxPduBufferIndex < CANIF_RXBUFFER_NUMBER)
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
        rxPduBuffer = &CanIf_RxBuffer[rxPduBufferIndex];
        rxPduBuffer->length = (uint8)pduInfo.SduLength;
        /* save sdu */
        (void)ILib_memcpy(rxPduBuffer->data, pduInfo.SduDataPtr, pduInfo.SduLength);
        if (TRUE == metaDataEnable)
        {
            rxPduBuffer->metaData = metaData;
        }
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
#endif /* STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API */
    /* inform upper layer */
    if (NULL_PTR != CanIf_UpRxIndicationArray[rxPduConfigPtr->CanIfUpRxIndicationApiIndex])
    {
        CanIf_UpRxIndicationArray[rxPduConfigPtr->CanIfUpRxIndicationApiIndex](rxPduConfigPtr->CanIfUpPduId, &pduInfo);
    }
}

static FUNC(Std_ReturnType, CANIF_CODE)
    CanIf_TransmitHandle(PduIdType CanIfTxSduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
    Std_ReturnType retVal = E_NOT_OK;
#else
    Can_ReturnType retVal = CAN_NOT_OK;
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanIfTxSduId);
    /* Get the hth config Index */
    Can_HwHandleType hthConfigIndex = txPduConfigPtr->CanIfTxPduHthId;
    /* Get the hth config */
    const CanIfHthCfgType* hthPtr = &CANIF_HTH(hthConfigIndex);
    /* get controller ID */
    uint8 canControllerId = hthPtr->CanIfHthCanCtrlId;
    if (CAN_CS_STARTED == CanIf_ControllerMode[canControllerId])
    {
        if (CANIF_ONLINE == CanIf_PduMode[canControllerId])
        {
            Can_PduType canPdu;
            const CanIf_ControllerCfgType* controllerPtr = &CanIf_CtrlCfgData[canControllerId];
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_PduPack(CanIfTxSduId, PduInfoPtr, &canPdu);
            /* call lower layer CAN driver API,and get return value */
            uint8 canDriver = controllerPtr->CanDriverId;
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
            if ((controllerPtr->CanIfPnFilter) && (CanIf_PnTxFilter[canControllerId]))
            {
                if (txPduConfigPtr->CanIfTxPduPnFilterPdu)
                {
                    retVal = Can_DriverApi[canDriver].CanWriteApi(hthPtr->CanObjectId, &canPdu);
                }
            }
            else
#endif
            {
                retVal = Can_DriverApi[canDriver].CanWriteApi(hthPtr->CanObjectId, &canPdu);
            }
            if (CAN_OK == retVal)
            {
                result = E_OK;
            }
/* if CAN_BUSY and buffer enabled */
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
            else if (CAN_BUSY == retVal)
            {
                result = CanIf_TxBusyHandle(&canPdu);
                if ((E_OK == result) && ((PduInfoPtr->SduLength) > (PduLengthType)(canPdu.length)))
                {
                    CanIf_Det_ReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_DATA_LENGTH_MISMATCH);
                }
            }
#endif /* STD_ON == CANIF_PUBLIC_TX_BUFFERING */
            else
            {
                result = E_NOT_OK;
            }
            SchM_Exit_CanIf_ExclusiveArea_Channel();
        }
#if (STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT)
        else if (CANIF_TX_OFFLINE_ACTIVE == CanIf_PduMode[canControllerId])
        {
            /* check notify enable */
#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
            CanIf_SetTxNotifStatusHandle(CanIfTxSduId);
#endif /* STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API */
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
            SchM_Enter_CanIf_ExclusiveArea_Channel();
            CanIf_TxConfirmationState[canControllerId] = CANIF_TX_RX_NOTIFICATION;
            SchM_Exit_CanIf_ExclusiveArea_Channel();
#endif
            if (NULL_PTR != CanIf_UpTxConfirmationArray[txPduConfigPtr->CanIfUpTxConfApiIndex])
            {
                /* call call-back function,feedback to upper layer */
                CanIf_UpTxConfirmationArray[txPduConfigPtr->CanIfUpTxConfApiIndex](txPduConfigPtr->CanIfUpPduId);
            }
            result = E_OK;
        }
#endif /* STD_ON == CANIF_TX_OFFLINE_ACTIVE_SUPPORT */
        else
        {
            CanIf_Det_ReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_STOPPED);
        }
    }
    else /* Not CAN_CS_STARTED mode */
    {
        CanIf_Det_ReportRuntimeError(CANIF_TRANSMIT_ID, CANIF_E_STOPPED);
    }
    return result;
}

static void CanIf_GetCanIfCtrlId(uint8* CanIfCtrlId, uint8 CanControllerId)
{
    uint8 cnt;
    for (cnt = 0u; cnt < CANIF_CANCONTROLLER_NUMBER; cnt++)
    {
        if (CanControllerId == CanIf_CtrlCfgData[cnt].CanCtrlId)
        {
            break;
        }
    }
    *CanIfCtrlId = cnt;
}
#if (CANIF_TRCV_NUMBER > 0u)
static void CanIf_GetCanIfTrcvId(uint8* CanIfTrcvId, uint8 CanTransciverId)
{
    uint8 cnt;
    for (cnt = 0u; cnt < CANIF_TRCV_NUMBER; cnt++)
    {
        if (CanTransciverId == CanIf_TrcvCfgData[cnt].CanTrcvId)
        {
            break;
        }
    }
    *CanIfTrcvId = cnt;
}
#endif /* CANIF_TRCV_NUMBER > 0u */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
