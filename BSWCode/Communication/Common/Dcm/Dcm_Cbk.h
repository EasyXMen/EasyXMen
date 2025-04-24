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
**************************************************************************** **
**                                                                           **
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef COMMUNICATION_DCM_DCM_CBK_H_
#define COMMUNICATION_DCM_DCM_CBK_H_

#include "ComStack_Types.h"
#include "Dcm_PBcfg.h"
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
#include "KeyM.h"
#include "Csm.h"
#endif
/*************************************************************************/
/*
 * Brief               <This function is called at the start of receiving an N-SDU.
                        The N-SDU might be fragmented into multiple N-PDUs (FF with one or more following CFs)
                        or might consist of a single N-PDU (SF).>
 * ServiceId           <0x46>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the I-PDU.
 *                      info: Pointer to a PduInfoType structure containing the payload data
 *                      TpSduLength: Total length of the N-SDU to be received.>
 * Param-Name[out]     <bufferSizePtr:  Available receive buffer in the receiving module.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK
 *                      BUFREQ_E_NOT_OK
 *                      BUFREQ_E_OVFL
 *                      BUFREQ_E_BUSY>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <0x44>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the received I-PDU.
 *                      info: Provides the source buffer (SduDataPtr) and the number
 *                            of bytes to be copied (SduLength).>
 * Param-Name[out]     <bufferSizePtr: Available receive buffer after data has been copied.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK
 *                      BUFREQ_E_NOT_OK>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <Called after an I-PDU has been received via the
 *                      TP API, the result indicates whether the
 *                      transmission was successful or not.>
 * ServiceId           <0x45>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id:ID of DCM I-PDU that has been received
 *                      result: The result of the diagnostic request message received>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after
 * call of Dcm_StartOfReception() and Dcm_CopyRxData()>
 * CallByAPI           <None>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) Dcm_TpRxIndication(PduIdType id, Std_ReturnType result);

/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <0x43>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the transmitted I-PDU.
 *                      info: Provides the destination buffer (SduDataPtr) and the number
 *                            of bytes to be copied (SduLength).
 *                      retry: This parameter is used to acknowledge transmitted data or to
 *                             retransmit data after transmission problems.>
 * Param-Name[out]     <availableDataPtr: Indicates the remaining number of bytes that are
 *                                        available in the upper layer module's Tx buffer.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK
 *                      BUFREQ_E_NOT_OK
 *                      BUFREQ_E_BUSY>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    P2CONST(RetryInfoType, AUTOMATIC, DCM_CONST) retry,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) availableDataPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <This function is called after the I-PDU has been transmitted
                        on its network, the result indicates whether the transmission
                        was successful or not.>
 * ServiceId           <0x48>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <id: Identification of the transmitted I-PDU.
 *                      result: Result of the transmission of the I-PDU.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(PduIdType id, Std_ReturnType result);

/*************************************************************************/
/*
 * Brief               <The lower layer communication interface module
 *                       confirms the transmission of an IPDU.>
 * ServiceId           <0x40>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM I-PDU that has been transmitted.
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) Dcm_TxConfirmation(PduIdType DcmTxPduId);

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_NO_COMMUNICATION>
 * ServiceId           <0x21>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(uint8 NetworkId);

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_SILENT_COMMUNICATION>
 * ServiceId           <0x22>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(uint8 NetworkId);

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_FULL_COMMUNICATION>
 * ServiceId           <0x23>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(uint8 NetworkId);

#if (DCM_UDS_SERVICE0X29_ENABLED == STD_ON)
/*************************************************************************/
/*
 * Brief               <Dcm_KeyMAsyncCertificateVerifyFinished>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <CertID/Result>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE)
    Dcm_KeyMAsyncCertificateVerifyFinished(KeyM_CertificateIdType CertID, KeyM_CertificateStatusType Result);

/*************************************************************************/
/*
 * Brief               <Dcm_CsmAsyncJobFinished>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <job/Result>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_CsmAsyncJobFinished(const Crypto_JobType* job, Crypto_ResultType result);
#endif /* DCM_UDS_SERVICE0X29_ENABLED == STD_ON */

#endif /* COMMUNICATION_DCM_DCM_CBK_H_ */
