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
**************************************************************************** **
**                                                                           **
**  FILENAME    : LdCom.c                                                    **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 * V2.0.0    20200703   zhengfei.li   Initial version
 *                                    (Upgrade according to the R19_11 standards)
 * V2.0.1    20211108   zhengfei.li   Optimize macro names
 * V2.0.2    20230425   tong.zhao
 *    add NULL pointer check for API parameter((JIRA: CPT-267))
 * V2.0.3    20231123   tong.zhao
 *    Code Execution Optimization.(JIRA: CPD-33590)
 * V2.0.4    20240229   tong.zhao
 *    QAC check issue fix
 * V2.0.5    20240419   tong.zhao
 *    Modify the logic error that occurs when PduIdType is uint8.
 * V2.0.6    20240614   tong.zhao
 *    Modify callback funcation logic.
 ******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LdCom<br>
  RuleSorce:puhua-rule2024-2.rcf

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 3673 MISRA Rule 8.13 .<br>
    Reason:Following the AUTOSAR standard,the external interfaces defined.
 */

/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "LdCom.h"
#include "LdCom_Cbk.h"
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
/******************************************************************************
**                      Imported Compiler Switch Check                       **
******************************************************************************/
#define LDCOM_C_AR_MAJOR_VERSION 4u
#define LDCOM_C_AR_MINOR_VERSION 5u
#define LDCOM_C_AR_PATCH_VERSION 0u
#define LDCOM_C_SW_MAJOR_VERSION 2u
#define LDCOM_C_SW_MINOR_VERSION 0u
#define LDCOM_C_SW_PATCH_VERSION 6u
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                                                                                \
    (LDCOM_C_AR_MAJOR_VERSION != LDCOM_H_AR_MAJOR_VERSION) || (LDCOM_C_AR_MINOR_VERSION != LDCOM_H_AR_MINOR_VERSION) \
    || (LDCOM_C_AR_PATCH_VERSION != LDCOM_H_AR_PATCH_VERSION)                                                        \
    || (LDCOM_C_SW_MAJOR_VERSION != LDCOM_H_SW_MAJOR_VERSION)                                                        \
    || (LDCOM_C_SW_MINOR_VERSION != LDCOM_H_SW_MINOR_VERSION)                                                        \
    || (LDCOM_C_SW_PATCH_VERSION != LDCOM_H_SW_PATCH_VERSION))

#error "LdCom.c version mismatching with LdCom.h"
#endif
/******************************************************************************
**                      Private Macro Definitions                            **
******************************************************************************/
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
static inline void LDCOM_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
/******************************************************************************
**                      Private Type Definitions                             *
******************************************************************************/

/******************************************************************************
**                      Private Constant Definitions                         **
******************************************************************************/

/******************************************************************************
**                      Private Variable Definitions                         **
******************************************************************************/
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
/*Module initialize status, TRUE initialized, FALSE not initialized*/
#define LDCOM_START_SEC_VAR_INIT_BOOLEAN
#include "LdCom_MemMap.h"
static VAR(boolean, LDCOM_VAR_POWER_ON_INIT) LdCom_InitStatus = FALSE;
#define LDCOM_STOP_SEC_VAR_INIT_BOOLEAN
#include "LdCom_MemMap.h"
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */

/*configuration pointer*/
#define LDCOM_START_SEC_VAR_INIT_UNSPECIFIED
#include "LdCom_MemMap.h"
static P2CONST(LdCom_ConfigType, LDCOM_VAR, LDCOM_CONST) LdCom_CfgPtr = NULL_PTR;
#define LDCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "LdCom_MemMap.h"
/******************************************************************************
**                      Private Function Definitions                         **
******************************************************************************/

/******************************************************************************
**                      Global Constant Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Variable Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Function Definitions                          **
******************************************************************************/
/**
 * This service initializes internal and external interfaces and variables
    of the AUTOSAR LdCom module for the further processing
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): config, Pointer to the AUTOSAR LdCom module's
    configuration data.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
FUNC(void, LDCOM_CODE)
LdCom_Init(P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_DATA) config)
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (NULL_PTR == config)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_INIT, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
        LdCom_CfgPtr = config;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        LdCom_InitStatus = TRUE;
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    }
    return;
}

/**
 * With a call to LdCom_DeInit the AUTOSAR LdCom module is put into an not
    initialized state
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, LDCOM_CODE)
LdCom_DeInit(void)
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_DEINIT, LDCOM_E_UNINIT);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
        LdCom_CfgPtr = NULL_PTR;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        LdCom_InitStatus = FALSE;
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    }
    return;
}

#if (STD_ON == LDCOM_VERSION_INFO_API)
/**
 * Returns the version information of this module.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo, Pointer to where to store the version
    information of this module
 * Return value: NA
 */
FUNC(void, LDCOM_CODE)
LdCom_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo)
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_GETVERSIONINFO, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == LDCOM_DEV_ERROR_DETECT*/
    {
        versioninfo->moduleID = LDCOM_MODULE_ID;
        versioninfo->sw_major_version = LDCOM_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = LDCOM_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = LDCOM_H_SW_PATCH_VERSION;
        versioninfo->vendorID = LDCOM_VENDOR_ID;
    }
    return;
}
#endif /* STD_ON == LDCOM_VERSION_INFO_API */

/**
 * Initiate a transmission of a signal using either IF- or TP-API set.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for same HandleId, otherwise Reentrant.
 * Parameters(IN): Id,Id of the signal to be sent
        PduInfoPtr,Length and pointer to the buffer of the Signal.
 * Parameters(INOUT): NA
 * Parameters(OUT): Std_ReturnType , E_OK - request is
        accepted by the destination module
                        E_NOT_OK - request is not accepted by
        the destination module;
 * Return value: NA
 */
FUNC(Std_ReturnType, LDCOM_CODE)
LdCom_Transmit(PduIdType Id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TRANSMIT, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= Id)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TRANSMIT, LDCOM_E_INVALID_SIGNAL_ID);
    }
    else if (NULL_PTR == PduInfoPtr)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TRANSMIT, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
        ret = PduR_LdComTransmit(LdCom_CfgPtr->LdComIPduPtr[Id].DestModulePduId, PduInfoPtr);
    }
    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

/* Call-back functions and notifications */

/**
 * This function is called to acquire the transmit data of an I-PDU
    segment (N-PDU).
 * Service ID: 0x43
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): id,Identification of the transmitted I-PDU
                    info,Provides the destination buffer (SduDataPtr) and the
        number of bytes to be copied (SduLength).
                retry,This parameter is used to acknowledge transmitted data or
        to retransmit data after transmission problems.
 * Parameters(INOUT): NA
 * Parameters(OUT): availableDataPtr, Indicates the remaining number of bytes
        that are available in the upper layer module's Tx buffer
 * Return value: BufReq_ReturnType,BUFREQ_OK: Data has been copied to the
        transmit buffer completely as requested
                BUFREQ_E_BUSY: Request could not be fulfilled, because the
        required amount of Tx data is not available.
 */
#define LDCOM_START_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(BufReq_ReturnType, LDCOM_COPYTXDATA_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
LdCom_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) availableDataPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_COPYTXDATA, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= id)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_COPYTXDATA, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_COPYTXDATA, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
        const LdComTxCopyTxDataFuncPtrType copyTxDataFuncPtr = LdCom_CfgPtr->LdComIPduPtr[id].LdComTxCopyTxDataFuncPtr;
        if (NULL_PTR != copyTxDataFuncPtr)
        {
            ret = copyTxDataFuncPtr(info, retry, availableDataPtr);
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(info);
        LDCOM_NOUSED(retry);
        LDCOM_NOUSED(availableDataPtr);
#endif /* STD_ON == LDCOM_SUPPORT_SEND_TP */
    }
    return ret;
}
#define LDCOM_STOP_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * This function is called after the I-PDU has been transmitted on its network,
    the result indicates whether the transmission was successful or not.
 * Service ID: 0x48
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Id,Identification of the transmitted I-PDU.
        result,Result of the transmission of the I-PDU
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#define LDCOM_START_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(void, LDCOM_TPTXCONFIRMATION_CODE)
LdCom_TpTxConfirmation(PduIdType id, Std_ReturnType result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TPTXCONFIRMATION, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= id)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TPTXCONFIRMATION, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
        const LdComTpTxConfirmationFuncPtrType tpTxConfirFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[id].LdComTpTxConfirmationFuncPtr;
        if (NULL_PTR != tpTxConfirFuncPtr)
        {
            tpTxConfirFuncPtr(result);
        }

#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(result);
#endif /* STD_ON == LDCOM_SUPPORT_SEND_TP */
    }
    return;
}
#define LDCOM_STOP_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * TThis function is called at the start of receiving an N-SDU.
 * Service ID: 0x46
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): id,Identification of the I-PDU
                    info,Pointer to a PduInfoType structure containing
        the payload data and payload length of the first frame or single
        frame of a transport protocol I-PDU reception.
                    TpSduLength,Total length of the N-SDU to be received.
 * Parameters(INOUT): NA
 * Parameters(OUT): bufferSizePtr, Available receive buffer in
        the receiving module.
 * Return value: BufReq_ReturnType,BUFREQ_OK:Connection has been accepted.
                BUFREQ_E_NOT_OK:Connection has been rejected
                BUFREQ_E_OVFL: No buffer of the required length can be provided.
 */
#define LDCOM_START_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(BufReq_ReturnType, LDCOM_STARTOFRECEPTION_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
LdCom_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_STARTOFRECEPTION, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= id)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_STARTOFRECEPTION, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else if (NULL_PTR == bufferSizePtr)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_STARTOFRECEPTION, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
        const LdComRxStartOfReceptionFuncPtrType rxStartOfReceptionFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[id].LdComRxStartOfReceptionFuncPtr;
        if (NULL_PTR != rxStartOfReceptionFuncPtr)
        {
            ret = rxStartOfReceptionFuncPtr(info, TpSduLength, bufferSizePtr);
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(info);
        LDCOM_NOUSED(TpSduLength);
        LDCOM_NOUSED(bufferSizePtr);
#endif /* STD_ON == LDCOM_SUPPORT_RECEIVER_TP */
    }
    return ret;
}
#define LDCOM_STOP_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * This function is called to provide the received data of an I-PDU segment
    (N-PDU) to the upper layer.
 * Service ID: 0x44
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): id,Identification of the transmitted I-PDU
                    info,Provides the destination buffer (SduDataPtr) and the
        number of bytes to be copied (SduLength).
 * Parameters(INOUT): NA
 * Parameters(OUT): bufferSizePtr, Available receive buffer after data has
        been copied .
 * Return value: BufReq_ReturnType,BUFREQ_OK: Data copied successfully.
                BUFREQ_E_NOT_OK:  Data was not copied because an error occurred.
 */
#define LDCOM_START_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(BufReq_ReturnType, LDCOM_COPYRXDATA_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
LdCom_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_COPYRXDATA, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= id)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_COPYRXDATA, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_COPYRXDATA, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
        const LdComRxCopyRxDataFuncPtrType rxCopyRxDataFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[id].LdComRxCopyRxDataFuncPtr;
        if (NULL_PTR != rxCopyRxDataFuncPtr)
        {
            ret = rxCopyRxDataFuncPtr(info, bufferSizePtr);
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(info);
        LDCOM_NOUSED(bufferSizePtr);
#endif /* STD_ON == LDCOM_SUPPORT_RECEIVER_TP */
    }
    return ret;
}
#define LDCOM_STOP_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * Called after an I-PDU has been received via the TP API.
 * Service ID: 0x45
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): Id,Identification of the received I-PDU.
        result,Result of the reception.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#define LDCOM_START_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(void, LDCOM_TPRXINDICATION_CODE)
LdCom_TpRxIndication(PduIdType id, Std_ReturnType result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TPRXINDICATION, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= id)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TPRXINDICATION, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
        const LdComTpRxIndicationFuncPtrType tpRxIndicationFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[id].LdComTpRxIndicationFuncPtr;
        if (NULL_PTR != tpRxIndicationFuncPtr)
        {
            tpRxIndicationFuncPtr(result);
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(result);
#endif /* STD_ON == LDCOM_SUPPORT_RECEIVER_TP */
    }
    return;
}
#define LDCOM_STOP_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * Contains the length (SduLength) of the received I-PDU and a pointer to
    a buffer (SduDataPtr) containing the I-PDU.
 * Service ID: 0x42
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId
 * Parameters(IN): RxPduId,ID of the received I-PDU
        PduInfoPtr,Contains the length (SduLength) of the received I-PDU and a
    pointer to a buffer (SduDataPtr) containing the I-PDU
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#define LDCOM_START_SEC_LDCOMRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(void, LDCOM_RXINDICATION_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
LdCom_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_RXINDICATION, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= RxPduId)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_RXINDICATION, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else if (NULL_PTR == PduInfoPtr)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_RXINDICATION, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_IF)
        const LdComRxIndicationFuncPtrType rxIndicationFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[RxPduId].LdComRxIndicationFuncPtr;
        if (NULL_PTR != rxIndicationFuncPtr)
        {
            rxIndicationFuncPtr(PduInfoPtr);
        }
#else
        LDCOM_NOUSED(RxPduId);
        LDCOM_NOUSED(PduInfoPtr);
#endif /* STD_ON == LDCOM_SUPPORT_RECEIVER_IF */
    }
    return;
}
#define LDCOM_STOP_SEC_LDCOMRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * The lower layer communication interface module confirms the transmission of
    an IPDU.
 * Service ID: 0x40
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId
 * Parameters(IN): TxPduId,ID of the I-PDU that has been transmitted.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#define LDCOM_START_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(void, LDCOM_TXCONFIRMATION_CODE)
LdCom_TxConfirmation(PduIdType TxPduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TXCONFIRMATION, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= TxPduId)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TXCONFIRMATION, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_SEND_IF)
        const LdComTxConfirmationFuncPtrType txConfirFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[TxPduId].LdComTxConfirmationFuncPtr;
        if (NULL_PTR != txConfirFuncPtr)
        {
            txConfirFuncPtr();
        }
#else
        LDCOM_NOUSED(TxPduId);
#endif /* STD_ON == LDCOM_SUPPORT_SEND_IF */
    }
    return;
}
#define LDCOM_STOP_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

/**
 * Contains the length (SduLength) of the received I-PDU and a pointer to
    a buffer (SduDataPtr) containing the I-PDU.
 * Service ID: 0x41
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId
 * Parameters(IN): TxPduId,ID of the SDU that is requested to be transmitted.
        PduInfoPtr,Contains a pointer to a buffer (SduDataPtr) to where the SDU
    data shall be copied, and the available buffer size in SduLengh.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E_OK:SDU has been copied and SduLength
        indicates the number of copied bytes.
                    E_NOT_OK: No SDU data has been copied.
  SWS_LDCOM_00047
 */
#define LDCOM_START_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
#include "LdCom_MemMap.h"
FUNC(Std_ReturnType, LDCOM_TRIGGERTRANSMIT_CODE)
/* PRQA S 1532,3673 ++ */ /* MISRA Rule 8.7,8.13 */
LdCom_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532,3673 -- */ /* MISRA Rule 8.7,8.13 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (!LdCom_InitStatus)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, LDCOM_E_UNINIT);
    }
    else if (LDCOM_IPDU_NUMBER_MAX <= TxPduId)
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        LDCOM_DET_REPORT(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, LDCOM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == LDCOM_DEV_ERROR_DETECT */
    {
#if (STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT)
        const LdComTxTriggerTransmitFuncPtrType txTriggerTransmitFuncPtr =
            LdCom_CfgPtr->LdComIPduPtr[TxPduId].LdComTxTriggerTransmitFuncPtr;
        if (NULL_PTR != txTriggerTransmitFuncPtr)
        {
            ret = txTriggerTransmitFuncPtr(PduInfoPtr);
        }
#else
        LDCOM_NOUSED(TxPduId);
        LDCOM_NOUSED(PduInfoPtr);
#endif /* STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT */
    }
    return ret;
}
#define LDCOM_STOP_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
#include "LdCom_MemMap.h"
