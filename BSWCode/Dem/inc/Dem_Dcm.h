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
 **  @file               : Dem_Dcm.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : API declaration of Dem used by Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_DCM_H
#define DEM_DCM_H

/* =================================================== inclusions =================================================== */
#include "Dem_Lcfg.h"
#include "Rte_Dcm_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief         Sets the DTC Filter based on a given extended data record on the primary fault memory. The
 *                server selects all DTCs that have a matching extended data record.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     DTCFormat: Defines the DTC format of the requested data. Valid selections are DEM_DTC_FORMAT_UDS
 *                           and DEM_DTC_FORMAT_OBD_3BYTE.
 * @param[in]     ExtendedDataRecordNumber: the extended data record number the filter is set for. Valid values are
 *                                          within the range of 0x01 and 0xFD.
 * @return        Std_ReturnType
 * @retval        E_OK: DTC filter for requested extended data record successfully set.
 * @retval        E_NOT_OK: Indicates an invalid extended data record number was selected.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61111
 */
extern Std_ReturnType Dem_SetDTCFilterByExtendedDataRecordNumber(
    uint8             ClientId,
    Dem_DTCFormatType DTCFormat,
    uint8             ExtendedDataRecordNumber);

/**
 * @brief         Sets the DTC Filter based on a given DTC readiness group on the primary fault memory. The server
 *                selects all DTCs that have this DTC readiness group configured.
 * @param[in]     ClientID: Unique client id, assigned to the instance of the calling module
 * @param[in]     DTCFormat: Defines the DTC format of the requested data. Valid selections are DEM_DTC_FORMAT_UDS
 *                           and DEM_DTC_FORMAT_OBD_3BYTE.
 * @param[in]     ReadinessGroupNumber: Specifies the DTC readiness group number defined by SAE J1979-DA.
 * @return        Std_ReturnType
 * @retval        E_OK: DTC filter for requested DTC readiness group successfully set.
 * @retval        E_NOT_OK: Indicates an invalid DTC readiness group was selected.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61112
 */
extern Std_ReturnType Dem_SetDTCFilterByReadinessGroup(
    uint8                          ClientID,
    Dem_DTCFormatType              DTCFormat,
    Dem_EventOBDReadinessGroupType ReadinessGroupNumber);

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_SPARK)
/**
 * @brief         Service is used for requesting IUMPR data according to InfoType $08. This interface is derived
 *                from the prototype <Module>_GetInfotypeValueData() defined by the Dcm. Therefore
 *                Dcm_OpStatusType and Std_ReturnType are contained. API is needed in OBD-relevant ECUs only.
 *                API Availability: This API will be available only if ({ecuc(Dem/DemGeneral.DemOBDSupport)} !=
 *                DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     OpStatus: Only DCM_INITIAL will appear, because this API behaves synchronous.
 * @param[out]    Iumprdata08: Buffer containing the number of data elements (as defined in ISO-15031-5) and
 *                             contents of InfoType $08. The buffer is provided by the Dcm.
 * @param[inout]  Iumprdata08BufferSize: The maximum number of data bytes that can be written to the Iumprdata08
 *                                       Buffer. When the function returns, the value is updated with the actual
 *                                       number of data bytes that are written to the Iumprdata08BufferSize.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61099
 */
extern Std_ReturnType
    Dem_DcmGetInfoTypeValue08(Dcm_OpStatusType OpStatus, uint8* Iumprdata08, uint8* Iumprdata08BufferSize);
#endif

#if (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_COMPRESSION)
/**
 * @brief         Service is used for requesting IUMPR data according to InfoType $0B. This interface is derived
 *                from the prototype <Module>_GetInfotypeValueData() defined by the Dcm. Therefore
 *                Dcm_OpStatusType and Std_ReturnType are contained. API is needed in OBD-relevant ECUs only. API
 *                Availability: This API will be available only if ({ecuc(Dem/DemGeneral.DemOBDSupport)} !=
 *                DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     OpStatus: Only DCM_INITIAL will appear, because this API behaves synchronous.
 * @param[out]    Iumprdata0B: Buffer containing the number of data elements (as defined in ISO-15031-5) and
 *                             contents of InfoType $0B. The buffer is provided by the Dcm.
 * @param[inout]  Iumprdata0BBufferSize: The maximum number of data bytes that can be written to the Iumprdata0B
 *                                       Buffer. When the function returns, the value is updated with the actual
 *                                       number of data bytes that are written to the Iumprdata0BBufferSize.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61091
 */
extern Std_ReturnType
    Dem_DcmGetInfoTypeValue0B(Dcm_OpStatusType OpStatus, uint8* Iumprdata0B, uint8* Iumprdata0BBufferSize);
#endif

/**
 * @brief         Service to report the value of PID $01 computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[out]    PID01value: Buffer containing the contents of PID $01 computed by the Dem. The buffer is
 *                            provided by the Dcm with the appropriate size, i.e. during configuration, the Dcm
 *                            identifies the required size from the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61092
 */
extern Std_ReturnType Dem_DcmReadDataOfPID01(uint8* PID01value);

/**
 * @brief         Service to report the value of PID $1C computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[out]    PID1Cvalue: Buffer containing the contents of PID $1C computed by the Dem. The value of PID$1C
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61093
 */
extern Std_ReturnType Dem_DcmReadDataOfPID1C(uint8* PID1Cvalue);

/**
 * @brief         Service to report the value of PID $21 computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[out]    PID21value: Buffer containing the contents of PID $21 computed by the Dem. The value of PID$21
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61094
 */
extern Std_ReturnType Dem_DcmReadDataOfPID21(uint8* PID21value);

/**
 * @brief         Service to report the value of PID $30 computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[out]    PID30value: Buffer containing the contents of PID $30 computed by the Dem. The value of PID$30
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61096
 */
extern Std_ReturnType Dem_DcmReadDataOfPID30(uint8* PID30value);

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/**
 * @brief         Service to report the value of PID $31 computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 * @param[out]    PID31value: Buffer containing the contents of PID $31 computed by the Dem. The value of PID$31
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61097
 */
extern Std_ReturnType Dem_DcmReadDataOfPID31(uint8* PID31value);
#endif

/**
 * @brief         Service to report the value of PID $41 computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 * @param[out]    PID41value: Buffer containing the contents of PID $41 computed by the Dem. The value of PID$41
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61095
 */
extern Std_ReturnType Dem_DcmReadDataOfPID41(uint8* PID41value);

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/**
 * @brief         Service to report the value of PID $4D computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 * @param[out]    PID4Dvalue: Buffer containing the contents of PID $4D computed by the Dem. The value of PID$4D
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61098
 */
extern Std_ReturnType Dem_DcmReadDataOfPID4D(uint8* PID4Dvalue);
#endif

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/**
 * @brief         Service to report the value of PID $4E computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 * @param[out]    PID4Evalue: Buffer containing the contents of PID $4E computed by the Dem. The value of PID$4E
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61089
 */
extern Std_ReturnType Dem_DcmReadDataOfPID4E(uint8* PID4Evalue);
#endif

/**
 * @brief         Service to report the value of PID $91 computed by the Dem. API is needed in OBD-relevant ECUs
 *                only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 * @param[out]    PID91value: Buffer containing the contents of PID $91 computed by the Dem. The value of PID$91
 *                            is configuration within DemOBDCompliancy. The buffer is provided by the Dcm with the
 *                            appropriate size, i.e. during configuration, the Dcm identifies the required size from
 *                            the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61090
 */
extern Std_ReturnType Dem_DcmReadDataOfPID91(uint8* PID91value);

/**
 * @brief         Gets data element per PID and index of the most important freeze frame being selected for the
 *                output of service $02. The function stores the data in the provided DestBuffer. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     PID: This parameter is an identifier for a PID as defined in ISO15031-5.
 * @param[in]     DataElementIndexOfPID: Data element index of this PID according to the Dcm configuration of
 *                                       service $02. It is zero-based and consecutive, and ordered by the data
 *                                       element positions (configured in Dcm, refer to SWS_Dem_00597).
 * @param[inout]  DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the data
 *                            element of the PID shall be written to. The format is raw hexadecimal values and
 *                            contains no header-information.
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data bytes
 *                         that can be written to the buffer. The function returns the actual number of written
 *                         data bytes in this parameter.
 * @return        Std_ReturnType
 * @retval        E_OK: Freeze frame data was successfully reported
 * @retval        E_NOT_OK: Freeze frame data was not successfully reported
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61088
 */
extern Std_ReturnType
    Dem_DcmReadDataOfOBDFreezeFrame(uint8 PID, uint8 DataElementIndexOfPID, uint8* DestBuffer, uint16* BufSize);

/**
 * @brief         Gets DTC by freeze frame record number. API is needed in OBD-relevant ECUs only. API
 *                Availability: This API will be available only if ({ecuc(Dem/DemGeneral.DemOBDSupport)} !=
 *                DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     FrameNumber: Unique identifier for a freeze frame record as defined in ISO 15031-5. The value
 *                             0x00 indicates the complete OBD freeze frame. Other values are reserved for future
 *                             functionality
 * @param[in]     DTCFormat: Output format of the DTC value.
 * @param[out]    DTC: Diagnostic Trouble Code in ODB format. If the return value of the function is other than
 *                      E_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: No DTC available
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61085
 */
extern Std_ReturnType Dem_DcmGetDTCOfOBDFreezeFrame(uint8 FrameNumber, uint32* DTC, Dem_DTCFormatType DTCFormat);

/**
 * @brief         Reports the value of a requested "availability-OBDMID" to the DCM upon a Service $06 request.
 *                Derived from that the tester displays the supported tests a mechanic can select from. API is
 *                needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     Obdmid: Availablity OBDMID ($00,$20, $40...)
 * @param[out]    Obdmidvalue: Bit coded information on the support of OBDMIDs.
 * @return        Std_ReturnType
 * @retval        E_OK: Report of DTR result successful
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61084
 */
extern Std_ReturnType Dem_DcmGetAvailableOBDMIDs(uint8 Obdmid, uint32* Obdmidvalue);

/**
 * @brief         Gets the number of TIDs per (functional) OBDMID. This can be used by the DCM to iteratively
 *                request for OBD/TID result data within a loop from 0....numberOfTIDs-1 API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     Obdmid: OBDMID subject of the request to identify the number of assigned TIDs
 * @param[out]    numberOfTIDs: Number of assigned TIDs for the requested OBDMID. Used as loop value for the DCM
 *                              to retrieve all OBD/TID result data.
 * @return        Std_ReturnType
 * @retval        E_OK: Get number of TIDs successful
 * @retval        E_NOT_OK: Get number of TIDs failed
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61086
 */
extern Std_ReturnType Dem_DcmGetNumTIDsOfOBDMID(uint8 Obdmid, uint8* numberOfTIDs);

/**
 * @brief         Reports a DTR data along with TID-value, UaSID, test result with lower and upper limit. API is
 *                needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     Obdmid: Identification of a DTR element by assigned DTRId.
 * @param[in]     TIDindex: Index of the TID within the DEM. Runs from 0 to "numberOfTIDs" obtained in the call
 *                          to Dem_DcmGetNumTIDsOfOBDMID()
 * @param[out]    TIDvalue: TID to be put on the tester reponse
 * @param[out]    UaSID: UaSID to be put on the tester reponse
 * @param[out]    Testvalue: Latest test result
 * @param[out]    Lowlimvalue: Lower limit value associated to the latest test result
 * @param[out]    Upplimvalue: Upper limit value associated to the latest test result
 * @return        Std_ReturnType
 * @retval        E_OK: Report of DTR result successful
 * @retval        E_NOT_OK: Report of DTR result failed
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61087
 */
extern Std_ReturnType Dem_DcmGetDTRData(
    uint8   Obdmid,
    uint8   TIDindex,
    uint8*  TIDvalue,
    uint8*  UaSID,
    uint16* Testvalue,
    uint16* Lowlimvalue,
    uint16* Upplimvalue);

/**
 * @brief         Service to report the value of monitor activity denominator PID computed by the Dem. API is
 *                needed in OBD-relevant ECUs only. API Availability: This API will be available only if:
 *                ({ecucDem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     OpStatus: OpStatus "Only DCM_INITIAL will appear, because this API behaves synchronous."
 * @param[out]    DataValueBuffer: Buffer containing the contents of the monitor activity denominator. The buffer
 *                                 is provided by the caller with the appropriate size.
 * @param[inout]  DataValueBufferSize: The maximum number of data bytes that can be written to the
 *                                     DataValueBuffer. When the function returns, the value is updated with the
 *                                     actual number of data bytes that are written to the Data ValueBuffer.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61080
 */
extern Std_ReturnType
    Dem_DcmGetInfoTypeValue79(Dcm_OpStatusType OpStatus, uint8* DataValueBuffer, uint8* DataValueBufferSize);

/**
 * @brief         Function to report the value of PID 0xF501 computed by the Dem. API is needed in OBD-relevant
 *                ECUs only. API Availability: This API will be available only if:
 *                ({ecucDem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[out]    PIDF501value: Buffer containing the contents of the PID 0xF501. The buffer is provided by the
 *                              Dcm with the appropriate size.
 * @return        Std_ReturnType
 * @retval        E_OK: Always E_OK is returned
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61082
 */
extern Std_ReturnType Dem_DcmReadDataOfPIDF501(uint8* PIDF501value);

#endif

#ifdef __cplusplus
}
#endif
#endif
