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
 **  @file               : DoIP_Internal_DET.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : DET in DoIP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_DET_H_
#define DOIP_INTERNAL_DET_H_

/* =================================================== inclusions =================================================== */

#include "DoIP.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */

#if (STD_ON == DOIP_DEV_ERROR_DETECT)

#include "Det.h"

/**
 * @brief       Report error to DET.
 * @param[in]   apiId: API service ID.
 * @param[in]   errorId: error code.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE void DOIP_DET_REPORT(uint8 apiId, uint8 errorId)
/* PRQA S 3406,1505 -- */
{
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, (apiId), (errorId));
}

/**
 * @brief       Report Det for DoIP_Init.
 * @param[in]   configPtr: PB configuration.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_Init_DET(const DoIP_ConfigType* configPtr)
/* PRQA S 3406,1505 -- */
{
    boolean ret = FALSE;

    if (NULL_PTR == configPtr)
    {
        DOIP_DET_REPORT(DOIP_SID_INIT, DOIP_E_PARAM_POINTER);
    }
    else
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_GetVersionInfo.
 * @param[in]   versioninfo: Pointer to where to store the version information of this module.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_GetVersionInfo_DET(const Std_VersionInfoType* versioninfo)
/* PRQA S 3406,1505 -- */
{
    boolean ret = FALSE;

    if (NULL_PTR == versioninfo)
    {
        DOIP_DET_REPORT(DOIP_SID_GETVERSIONINFO, DOIP_E_PARAM_POINTER);
    }
    else
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_ActivationLineSwitch.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_ActivationLineSwitch_DET(void)
/* PRQA S 3406,1505 -- */
{
    boolean ret = FALSE;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00285*/
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_UNINIT);
    }
    else
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_LocalIpAddrAssignmentChg.
 * @param[in]   soConId: socket connection index specifying the socket connection where the IP address assigment has
 * changed
 * @param[in]   state: state of IP address assignment.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_LocalIpAddrAssignmentChg_DET(SoAd_SoConIdType soConId, TcpIp_IpAddrStateType state)
/* PRQA S 3406,1505 -- */
{
    boolean ret = FALSE;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00195*/
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_UNINIT);
    }
    else if (
        (soConId == (SoAd_SoConIdType)DOIP_INVALID_UINT16)
        || (state > TCPIP_IPADDR_STATE_UNASSIGNED)) /* PRQA S 1476 */ /* VL_DoIP_TestAlwayFalse */ /*SWS_DoIP_00196*/
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_INVALID_PARAMETER);
    }
    else
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_SoAdIfTxConfirmation.
 * @param[in]   soadTxPduId: ID of the I-PDU that has been transmitted.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_SoAdIfTxConfirmation_DET(PduIdType soadTxPduId)
/* PRQA S 3406,1505 -- */
{
    boolean ret = FALSE;

    if (DoIP_InitStatus == DOIP_STATUS_UNINITED) /*SWS_DoIP_00249*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADIFTXCONFIRMATION, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdTxPdu_MAX_ID <= soadTxPduId) /*SWS_DoIP_00250*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADIFTXCONFIRMATION, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_SoAdTpCopyTxData.
 * @param[in]   soadTxPduId: Identification of the transmitted I-PDU.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @param[in]   retry: This parameter is used to acknowledge transmitted data or toretransmit data after transmission
 * problems.
 * @param[out]  availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer module's
 * Tx buffer.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: No error.
 * @retval      BUFREQ_E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE BufReq_ReturnType DoIP_SoAdTpCopyTxData_DET(
    PduIdType            soadTxPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retry,
    const PduLengthType* availableDataPtr)
/* PRQA S 3406,1505 -- */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00175*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdTxPdu_MAX_ID <= soadTxPduId) /*SWS_DoIP_00176*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else if ((NULL_PTR == pduInfoPtr) || (NULL_PTR == availableDataPtr)) /*SWS_DoIP_00177*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_PARAM_POINTER);
    }
    else if (NULL_PTR != retry) /*SWS_DoIP_00178*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_INVALID_PARAMETER);
    }
    else
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_IfTransmit.
 * @param[in]   pdurTxPduId: DoIP unique identifier of the PDU to be transmitted by the PduR
 * @param[in]   pduInfoPtr: Tx Pdu information structure which contains the length of the DoIPTxMessage.
 * @return      Std_ReturnType
 * @retval      E_OK: No error.
 * @retval      E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_IfTransmit_DET(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 3406,1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus)
    {
        DOIP_DET_REPORT(DOIP_SID_IFTRANSMIT, DOIP_E_UNINIT);
    }
    else if (NULL_PTR == pduInfoPtr)
    {
        DOIP_DET_REPORT(DOIP_SID_IFTRANSMIT, DOIP_E_PARAM_POINTER);
    }
    else if (DoIPConf_DoIPPduRTxPdu_MAX_ID <= pdurTxPduId)
    {
        DOIP_DET_REPORT(DOIP_SID_IFTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_IfCancelTransmit.
 * @param[in]   pdurTxPduId: Identification of the I-PDU to be cancelled.
 * @return      Std_ReturnType
 * @retval      E_OK: No error.
 * @retval      E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_IfCancelTransmit_DET(PduIdType pdurTxPduId)
/* PRQA S 3406,1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus)
    {
        DOIP_DET_REPORT(DOIP_SID_IFCANCELTRANSMIT, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPPduRTxPdu_MAX_ID <= pdurTxPduId)
    {
        DOIP_DET_REPORT(DOIP_SID_IFCANCELTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_SoAdTpCopyRxData.
 * @param[in]   soadRxPduId: Identification of the received I-PDU.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[in]   bufferSizePtr: Available receive buffer after data has been copied.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: No error.
 * @retval      BUFREQ_E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE BufReq_ReturnType
    DoIP_SoAdTpCopyRxData_DET(PduIdType soadRxPduId, const PduInfoType* pduInfoPtr, const PduLengthType* bufferSizePtr)
/* PRQA S 3406,1505 -- */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00183*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdRxPdu_MAX_ID <= soadRxPduId) /*SWS_DoIP_00036*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else if ((NULL_PTR == pduInfoPtr) || (NULL_PTR == bufferSizePtr)) /*SWS_DoIP_00184*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_PARAM_POINTER);
    }
    else
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_SoAdIfRxIndication.
 * @param[in]   soadRxPduId: Identification of the received I-PDU.
 * @param[in]   pduInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 * containing the PDU, and the MetaData related to this PDU.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_SoAdIfRxIndication_DET(PduIdType soadRxPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 3406,1505 -- */
{
    boolean DetStatus = FALSE;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00246*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADIFRXINDICATION, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdRxPdu_MAX_ID <= soadRxPduId) /*SWS_DoIP_00247*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADIFRXINDICATION, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else if (NULL_PTR == pduInfoPtr) /*SWS_DoIP_00248*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADIFRXINDICATION, DOIP_E_PARAM_POINTER);
    }
    else
    {
        DetStatus = TRUE;
    }

    return DetStatus;
}

/**
 * @brief       Report Det for DoIP_SoAdTpStartOfReception.
 * @param[in]   soadRxPduId: Identification of the I-PDU.
 * @param[in]   sduLen: Total length of the N-SDU to be received.
 * @param[in]   bufferSizePtr: Available receive buffer in the receiving module. This parameter will be used to compute
 * the Block Size (BS) in the transport protocol module.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: No error.
 * @retval      BUFREQ_E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE BufReq_ReturnType
    DoIP_SoAdTpStartOfReception_DET(PduIdType soadRxPduId, PduLengthType sduLen, const PduLengthType* bufferSizePtr)
/* PRQA S 3406,1505 -- */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00186*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdRxPdu_MAX_ID <= soadRxPduId) /*SWS_DoIP_00187*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else if (NULL_PTR == bufferSizePtr) /*SWS_DoIP_00188*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_PARAM_POINTER);
    }
    else if (0u != sduLen) /*SWS_DoIP_00189*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_INVALID_PARAMETER);
    }
    else
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_TpTransmit.
 * @param[in]   pdurTxPduId: DoIP unique identifier of the PDU to be transmitted by the PduR
 * @param[in]   pduInfoPtr: Tx Pdu information structure which contains the length of the DoIPTxMessage.
 * @return      Std_ReturnType
 * @retval      E_OK: No error.
 * @retval      E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_TpTransmit_DET(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 3406,1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00162*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPTRANSMIT, DOIP_E_UNINIT);
    }
    else if (NULL_PTR == pduInfoPtr) /*SWS_DoIP_00164*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPTRANSMIT, DOIP_E_PARAM_POINTER);
    }
    else if (DoIPConf_DoIPPduRTxPdu_MAX_ID <= pdurTxPduId) /*SWS_DoIP_00163*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_SoAdTpTxConfirmation.
 * @param[in]   soadTxPduId: Identification of the transmitted I-PDU.
 * @param[in]   result: E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_SoAdTpTxConfirmation_DET(PduIdType soadTxPduId, Std_ReturnType result)
/* PRQA S 3406,1505 -- */
{
    boolean det_Status = FALSE;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00180*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdTxPdu_MAX_ID <= soadTxPduId) /*SWS_DoIP_00181*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else if ((E_OK != result) && (E_NOT_OK != result)) /*SWS_DoIP_00182*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_INVALID_PARAMETER);
    }
    else
    {
        det_Status = TRUE;
    }

    return det_Status;
}

/**
 * @brief       Report Det for DoIP_SoConModeChg.
 * @param[in]   soConId: socket connection index specifying the socket connection with the mode change.
 * @param[in]   mode: new mode.
 * @return      boolean
 * @retval      TRUE:No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_SoConModeChg_DET(SoAd_SoConIdType soConId, SoAd_SoConModeType mode)
/* PRQA S 3406,1505 -- */
{
    boolean det_status = FALSE;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00193*/
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_UNINIT);
    }
    else if (/*SWS_DoIP_00194*/
             ((SoAd_SoConIdType)DOIP_INVALID_UINT16 == soConId)
             || ((SOAD_SOCON_ONLINE != mode) && (SOAD_SOCON_RECONNECT != mode) && (SOAD_SOCON_OFFLINE != mode)))
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_INVALID_PARAMETER);
    }
    else
    {
        det_status = TRUE;
    }

    return det_status;
}

/**
 * @brief       Report Det for DoIP_TpCancelReceive.
 * @param[in]   pdurRxPduId: DoIP unique identifier of the PDU for which reception shall be canceled by the PduR
 * @return      Std_ReturnType
 * @retval      E_OK: No error.
 * @retval      E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_TpCancelReceive_DET(PduIdType pdurRxPduId)
/* PRQA S 3406,1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00169*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPCANCELRECEIVE, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPPduRRxPdu_MAX_ID <= pdurRxPduId) /*SWS_DoIP_00170*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPCANCELRECEIVE, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_TpCancelTransmit.
 * @param[in]   pdurTxPduId: DoIP unique identifier ofthe PDU to be transmitted by the PduR
 * @return      Std_ReturnType
 * @retval      E_OK: No error.
 * @retval      E_NOT_OK: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_TpCancelTransmit_DET(PduIdType pdurTxPduId)
/* PRQA S 3406,1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00166*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPCANCELTRANSMIT, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPPduRTxPdu_MAX_ID <= pdurTxPduId) /*SWS_DoIP_00167*/
    {
        DOIP_DET_REPORT(DOIP_SID_TPCANCELTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief       Report Det for DoIP_SoAdTpRxIndication.
 * @param[in]   soadRxPduId: Identification of the received I-PDU.
 * @param[in]   result: E_OK: The PDU was received. E_NOT_OK: Reception of the PDU failed.
 * @return      boolean
 * @retval      TRUE: No error.
 * @retval      FALSE: Error occur.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 3406,1505 ++ */ /* VL_DoIP_DefinedInHeaderFile,VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DoIP_SoAdTpRxIndication_DET(PduIdType soadRxPduId, Std_ReturnType result)
/* PRQA S 3406,1505 -- */
{
    boolean det_Status = FALSE;

    if (DOIP_STATUS_UNINITED == DoIP_InitStatus) /*SWS_DoIP_00190*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPRXINDICATION, DOIP_E_UNINIT);
    }
    else if (DoIPConf_DoIPSoAdRxPdu_MAX_ID <= soadRxPduId) /*SWS_DoIP_00191*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPRXINDICATION, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else if ((E_OK != result) && (E_NOT_OK != result)) /*SWS_DoIP_00192*/
    {
        DOIP_DET_REPORT(DOIP_SID_SOADTPRXINDICATION, DOIP_E_INVALID_PARAMETER);
    }
    else
    {
        det_Status = TRUE;
    }

    return det_Status;
}

/* PRQA S 3472 ++ */                                              /* VL_DoIP_CouldReplacedByFuncCall */
#define DOIP_INIT_DET(DoIPConfigPtr) DoIP_Init_DET(DoIPConfigPtr) /**< Report Det for DoIP_Init */
#define DOIP_GETVERSIONINFO_DET(versioninfo) \
    DoIP_GetVersionInfo_DET(versioninfo) /**< Report Det for DoIP_GetVersionInfo */
#define DOIP_ACTIVATIONLINESWITCH_DET()                                           \
    DoIP_ActivationLineSwitch_DET() /**< Report Det for DoIP_ActivationLineSwitch \
                                     */
#define DOIP_LOCALIPADDRASSIGNMENTCHG_DET(soConId, state) \
    DoIP_LocalIpAddrAssignmentChg_DET(soConId, state) /**< Report Det for DoIP_LocalIpAddrAssignmentChg */
#define DOIP_SOADIFTXCONFIRMATION_DET(soadTxPduId) \
    DoIP_SoAdIfTxConfirmation_DET(soadTxPduId) /**< Report Det for DoIP_SoAdIfTxConfirmation */
#define DOIP_SOADTPCOPYTXDATA_DET(soadTxPduId, pduInfoPtr, retry, availableDataPtr) \
    DoIP_SoAdTpCopyTxData_DET(                                                      \
        soadTxPduId,                                                                \
        pduInfoPtr,                                                                 \
        retry,                                                                      \
        availableDataPtr) /**< Report Det for DoIP_SoAdTpCopyTxData */
#define DOIP_IFTRANSMIT_DET(pdurTxPduId, pduInfoPtr) \
    DoIP_IfTransmit_DET(pdurTxPduId, pduInfoPtr) /**< Report Det for DoIP_IfTransmit */
#define DOIP_IFCANCELTRANSMIT_DET(pdurTxPduId) \
    DoIP_IfCancelTransmit_DET(pdurTxPduId) /**< Report Det for DoIP_IfCancelTransmit */
#define DOIP_SOADTPCOPYRXDATA_DET(soadRxPduId, pduInfoPtr, bufferSizePtr) \
    DoIP_SoAdTpCopyRxData_DET(soadRxPduId, pduInfoPtr, bufferSizePtr) /**< Report Det for DoIP_SoAdTpCopyRxData */
#define DOIP_SOADIFRXINDICATION_DET(soadRxPduId, pduInfoPtr) \
    DoIP_SoAdIfRxIndication_DET(soadRxPduId, pduInfoPtr) /**< Report Det for DoIP_SoAdIfRxIndication */
#define DOIP_SOADTPSTARTOFRECEPTION_DET(soadRxPduId, tpSduLen, bufferSizePtr) \
    DoIP_SoAdTpStartOfReception_DET(                                          \
        soadRxPduId,                                                          \
        tpSduLen,                                                             \
        bufferSizePtr) /**< Report Det for DoIP_SoAdTpStartOfReception */
#define DOIP_TPTRANSMIT_DET(pdurTxPduId, DoIPPduRTxInfoPtr) \
    DoIP_TpTransmit_DET(pdurTxPduId, DoIPPduRTxInfoPtr) /**< Report Det for DoIP_TpTransmit */
#define DOIP_SOADTPTXCONFIRMATION_DET(soadTxPduId, result) \
    DoIP_SoAdTpTxConfirmation_DET(soadTxPduId, result) /**< Report Det for DoIP_SoAdTpTxConfirmation */
#define DOIP_SOCONMODECFG_DET(soConId, mode) \
    DoIP_SoConModeChg_DET(soConId, mode) /**< Report Det for DoIP_SoConModeChg */
#define DOIP_TPCANCELRECEIVE_DET(pdurRxPduId) \
    DoIP_TpCancelReceive_DET(pdurRxPduId) /**< Report Det for DoIP_TpCancelReceive */
#define DOIP_TPCANCELTRANSMIT_DET(pdurTxPduId) \
    DoIP_TpCancelTransmit_DET(pdurTxPduId) /**< Report Det for DoIP_TpCancelTransmit */
#define DOIP_SOADTPRXINDICATION_DET(soadRxPduId, result) \
    DoIP_SoAdTpRxIndication_DET(soadRxPduId, result) /**< Report Det for DoIP_SoAdTpRxIndication */
/* PRQA S 3472 -- */

#else

#define DOIP_INIT_DET(DoIPConfigPtr)                      TRUE /**< Report Det for DoIP_Init */
#define DOIP_GETVERSIONINFO_DET(versioninfo)              TRUE /**< Report Det for DoIP_GetVersionInfo */
#define DOIP_ACTIVATIONLINESWITCH_DET()                   TRUE /**< Report Det for DoIP_ActivationLineSwitch */
#define DOIP_LOCALIPADDRASSIGNMENTCHG_DET(soConId, state) TRUE /**< Report Det for DoIP_LocalIpAddrAssignmentChg */
#define DOIP_SOADIFTXCONFIRMATION_DET(soadTxPduId)        TRUE /**< Report Det for DoIP_SoAdIfTxConfirmation */
#define DOIP_SOADTPCOPYTXDATA_DET(soadTxPduId, pduInfoPtr, retry, availableDataPtr) \
    BUFREQ_OK                                                  /**< Report Det for DoIP_SoAdTpCopyTxData */
#define DOIP_IFTRANSMIT_DET(pdurTxPduId, pduInfoPtr) BUFREQ_OK /**< Report Det for DoIP_IfTransmit */
#define DOIP_IFCANCELTRANSMIT_DET(pdurTxPduId)       E_OK      /**< Report Det for DoIP_IfCancelTransmit */
#define DOIP_SOADTPCOPYRXDATA_DET(soadRxPduId, pduInfoPtr, bufferSizePtr) \
    BUFREQ_OK                                                     /**< Report Det for DoIP_SoAdTpCopyRxData */
#define DOIP_SOADIFRXINDICATION_DET(soadRxPduId, pduInfoPtr) TRUE /**< Report Det for DoIP_SoAdIfRxIndication */
#define DOIP_SOADTPSTARTOFRECEPTION_DET(soadRxPduId, tpSduLen, bufferSizePtr) \
    BUFREQ_OK                                                    /**< Report Det for DoIP_SoAdTpStartOfReception */
#define DOIP_TPTRANSMIT_DET(pdurTxPduId, DoIPPduRTxInfoPtr) E_OK /**< Report Det for DoIP_TpTransmit */
#define DOIP_SOADTPTXCONFIRMATION_DET(soadTxPduId, result)  TRUE /**< Report Det for DoIP_SoAdTpTxConfirmation */
#define DOIP_SOCONMODECFG_DET(soConId, mode)                TRUE /**< Report Det for DoIP_SoConModeChg */
#define DOIP_TPCANCELRECEIVE_DET(pdurRxPduId)               E_OK /**< Report Det for DoIP_TpCancelReceive */
#define DOIP_TPCANCELTRANSMIT_DET(pdurTxPduId)              E_OK /**< Report Det for DoIP_TpCancelTransmit */
#define DOIP_SOADTPRXINDICATION_DET(soadRxPduId, result)    TRUE /**< Report Det for DoIP_SoAdTpRxIndication */

#endif

#ifdef __cplusplus
}
#endif
#endif
