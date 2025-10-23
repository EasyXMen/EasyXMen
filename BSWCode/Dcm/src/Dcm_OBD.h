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
/*
 ************************************************************************************************************************
 **
 **  @file               : Dcm_OBD.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD Functions Declaration and Type Definitions
 **
 ***********************************************************************************************************************/
#ifndef DCM_OBD_H_
#define DCM_OBD_H_

/* =================================================== inclusions =================================================== */
#include "Dcm_Dsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#if ((STD_ON == DCM_OBD_0X2) || (STD_ON == DCM_OBD_0X3) || (STD_ON == DCM_OBD_0X7) || (STD_ON == DCM_OBD_0XA))

#define DCM_OBD_DTC_MASK 0x0000FFFFu /**<OBD DTC mask */
#endif
#define DCM_OBD_AVAILABLE_BIT 0x01u /**<bit mask for availability id */

#define DCM_OBD_NONAVAILABLE_BIT 0x02u /**<bit mask for non availability id */

#define DCM_OBD_MIXED_AVAILABLE_BIT 0x03u /**<bit mask for mixed availability id and non-availability id */

#define DCM_OBD_DTC_LENGTH 2u /**<the length of OBD DTC */

#define DCM_OBDID_AVAILDATALEN 4u /**<the length of available data */
/* ========================================= external function declarations ========================================= */
#if (STD_ON == DCM_OBD_0X1)
/**
 * @brief        The service interpreter for OBD 0x01
 * @param[in]    OpStatus    : Indicates the current operation status
 * @param[inout] pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                             pMsgContext shall point behind the SID.
 * @param[out]   ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x01(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_OBD_0X1) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         Called by OBD 0x01 and UDS 0x22 to read the PidData
 * @param[in]     OpStatus   : Indicates the current operation status
 * @param[in]     Pid        : Target PID
 * @param[out]    DestBuffer : address to output Pid data
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes that
 *                             can be written to the buffer. The function returns the actual number of written data
 *                             bytes in this parameter.
 * @param[inout]  mixPid     : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability Id,
 *                             bit 2 is set for non-availability ones.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x01_ReadPid(
    Dcm_ExtendedOpStatusType OpStatus,
    uint8                    Pid,
    uint8*                   DestBuffer,
    Dcm_MsgLenType*          BufSize,
    uint8*                   mixPid);
#endif

#if (STD_ON == DCM_OBD_0X2)
/**
 * @brief         The service interpreter for OBD 0x02
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x02(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_OBD_0X3)
/**
 * @brief         The service interpreter for OBD 0x02
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x03(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_OBD_0X4)
/**
 * @brief         The service interpreter for OBD 0x04
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x04(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_OBD_0X6)
/**
 * @brief         The service interpreter for OBD 0x06
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x06(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_OBD_0X6) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         Called by OBD 0x06 and UDS 0x22 to read the midData
 * @param[in]     mid        : Target MID
 * @param[out]    DestBuffer : address to output Pid data
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes that
 *                             can be written to the buffer. The function returns the actual number of written data
 *                             bytes in this parameter.
 * @param[inout]  mixMid     : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability Id,
 *                             bit 2 is set for non-availability ones.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x06_ReadMid(uint8 mid, uint8* DestBuffer, Dcm_MsgLenType* BufSize, uint8* mixMid);
#endif

#if (STD_ON == DCM_OBD_0X7)
/**
 * @brief         The service interpreter for OBD 0x07
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x07(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_OBD_0X8)
/**
 * @brief         The service interpreter for OBD 0x08
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x08(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_OBD_0X8) || ((STD_ON == DCM_UDS_0X31) && (STD_ON == DCM_OBD_RID_MIRROR)))
/**
 * @brief         Called by OBD 0x08 and UDS 0x31 to read the tidData
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     tid          : Target TID
 * @param[in]     InBuffer     : the input data
 * @param[in]     InBufferSize : the size of input data
 * @param[out]    DestBuffer   : address to output Pid data
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x08_ReadTid(
    uint8                         tid,
    uint8*                        InBuffer,
    Dcm_MsgLenType                InBufferSize,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         Called by OBD 0x08 and UDS 0x31 to find the configured tid index
 * @param[in]     tid      : Target TID
 * @param[out]    tidIndex : the configured tid index
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x08_FindTid(uint8 tid, uint8* tidIndex);
#endif

#if (STD_ON == DCM_OBD_0X9)
/**
 * @brief         The service interpreter for OBD 0x09
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x09(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_OBD_0X9) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         Called by OBD 0x09 and UDS 0x22 to read the vehInfo data
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[in]     infoType    : Target infoType
 * @param[out]    DestBuffer  : address to output Pid data
 * @param[inout]  BufSize     : When the function is called this parameter contains the maximum number of data bytes
 *                              that can be written to the buffer. The function returns the actual number of written
 *                              data bytes in this parameter.
 * @param[inout]  MixInfoType : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                              Id, bit 2 is set for non-availability ones.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x09_ReadVehInfo(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         infoType,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    uint8*                        MixInfoType,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_OBD_0XA)
/**
 * @brief         The service interpreter for OBD 0x0A
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD0x0A(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_OBD_0X3) || (STD_ON == DCM_OBD_0X7) || (STD_ON == DCM_OBD_0XA))
/**
 * @brief         Called by OBD 0x03, 0x07, 0x0A to get filtered DTC
 * @param[in]     ClientId            : Dem ClientId
 * @param[in]     DTCStatusMask       : Status-byte mask for DTC status-byte filtering
 * @param[in]     DTCOrigin           : used to select the source memory the DTCs shall be read from
 * @param[out]    DestBuffer          : address to output DTC
 * @param[out]    NumberOfFilteredDTC : the number of output DTC
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD_GetFilteredDTC(
    uint8             ClientId,
    uint8             DTCStatusMask,
    Dem_DTCOriginType DTCOrigin,
    uint8*            DestBuffer,
    uint16*           NumberOfFilteredDTC);
#endif

#if ((STD_ON == DCM_OBD_0X1) || (STD_ON == DCM_OBD_0X2))
/**
 * @brief         Called by OBD 0x01, 0x02 to find the pid in the configuration
 * @param[in]     Pid       : Target Pid
 * @param[in]     ServiceId : Actual OBD service ID
 * @param[out]    PidIndex  : Pid index in the configuration
 * @return        Std_ReturnType
 * @retval        E_OK     : Pid index found
 * @retval        E_NOT_OK : Pid is not configured
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_OBD_FindPid(uint8 Pid, uint8 ServiceId, uint8* PidIndex);
#endif

/**
 * @brief         Called by OBD 0x01, 0x02, 0x06, 0x08, 0x09 to check is this Id an availabilityId and find which
 *                availableIdIndex it belongs to
 * @param[in]     Id           : OBD ID (pid/mid/testId)
 * @param[out]    AvailIdIndex : the availableIndex this id belongs to (if it is an availability id)
 * @return        Std_ReturnType
 * @retval        TRUE  : it is an availability Id, and availIdIndex is found
 * @retval        FALSE : it is not an availability Id
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
boolean Dcm_OBD_isAvailability(uint8 Id, uint8* AvailIdIndex);

#ifdef __cplusplus
}
#endif

#endif
