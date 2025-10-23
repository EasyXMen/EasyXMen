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
 **  @file               : Dem_DcmAPIImplementation.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : API declaration of Dem used by Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_DCMDAPIIMPLEMENTATION_H
#define DEM_DCMDAPIIMPLEMENTATION_H

/* =================================================== inclusions =================================================== */
#include "Dem_Dcm.h"

/* PRQA S 3406, 1503 ++ */ /* VL_Dem_3406, VL_QAC_NoUsedApi */
/* ========================================== external function definitions ========================================= */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * @brief Sets the DTC Filter based on a given extended data record on the primary fault memory. The
 * server selects all DTCs that have a matching extended data record.
 */
Std_ReturnType Dem_SetDTCFilterByExtendedDataRecordNumber(
    uint8             ClientId,
    Dem_DTCFormatType DTCFormat,
    uint8             ExtendedDataRecordNumber)
{
    /** for 19 1A, API not yet implemented */
    DEM_PARAM_UNUSED(ClientId);
    DEM_PARAM_UNUSED(DTCFormat);
    DEM_PARAM_UNUSED(ExtendedDataRecordNumber);
    return E_NOT_OK;
}

/**
 * @brief Sets the DTC Filter based on a given DTC readiness group on the primary fault memory. The server
 * selects all DTCs that have this DTC readiness group configured.
 */
Std_ReturnType Dem_SetDTCFilterByReadinessGroup(
    uint8                          ClientID,
    Dem_DTCFormatType              DTCFormat,
    Dem_EventOBDReadinessGroupType ReadinessGroupNumber)
{
    /** for 19 56, API not yet implemented */
    DEM_PARAM_UNUSED(ClientID);
    DEM_PARAM_UNUSED(DTCFormat);
    DEM_PARAM_UNUSED(ReadinessGroupNumber);
    return E_NOT_OK;
}

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_SPARK)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
/**
 * @brief Service is used for requesting IUMPR data according to InfoType $08. This interface is derived
 * from the prototype <Module>_GetInfotypeValueData() defined by the Dcm. Therefore
 * Dcm_OpStatusType and Std_ReturnType are contained. API is needed in OBD-relevant ECUs only.
 * API Availability: This API will be available only if ({ecuc(Dem/DemGeneral.DemOBDSupport)} !=
 * DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetInfoTypeValue08(Dcm_OpStatusType OpStatus, uint8* Iumprdata08, uint8* Iumprdata08BufferSize)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(OpStatus);
    DEM_PARAM_UNUSED(Iumprdata08);
    DEM_PARAM_UNUSED(Iumprdata08BufferSize);
    return E_NOT_OK;
}
#endif

#if (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_COMPRESSION)
/**
 * @brief Service is used for requesting IUMPR data according to InfoType $0B. This interface is derived
 * from the prototype <Module>_GetInfotypeValueData() defined by the Dcm. Therefore
 * Dcm_OpStatusType and Std_ReturnType are contained. API is needed in OBD-relevant ECUs only. API
 * Availability: This API will be available only if ({ecuc(Dem/DemGeneral.DemOBDSupport)} !=
 * DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetInfoTypeValue0B(Dcm_OpStatusType OpStatus, uint8* Iumprdata0B, uint8* Iumprdata0BBufferSize)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(OpStatus);
    DEM_PARAM_UNUSED(Iumprdata0B);
    DEM_PARAM_UNUSED(Iumprdata0BBufferSize);
    return E_NOT_OK;
}
#endif

/**
 * @brief Service to report the value of PID $01 computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPID01(uint8* PID01value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID01value);
    return E_NOT_OK;
}

/**
 * @brief Service to report the value of PID $1C computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPID1C(uint8* PID1Cvalue)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID1Cvalue);
    return E_NOT_OK;
}

/**
 * @brief Service to report the value of PID $21 computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPID21(uint8* PID21value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID21value);
    return E_NOT_OK;
}

/**
 * @brief Service to report the value of PID $30 computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPID30(uint8* PID30value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID30value);
    return E_NOT_OK;
}

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/**
 * @brief Service to report the value of PID $31 computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 */
Std_ReturnType Dem_DcmReadDataOfPID31(uint8* PID31value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID31value);
    return E_NOT_OK;
}
#endif

/**
 * @brief Service to report the value of PID $41 computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPID41(uint8* PID41value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID41value);
    return E_NOT_OK;
}

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/**
 * @brief Service to report the value of PID $4D computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 */
Std_ReturnType Dem_DcmReadDataOfPID4D(uint8* PID4Dvalue)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID4Dvalue);
    return E_NOT_OK;
}
#endif

#if (DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
/**
 * @brief Service to report the value of PID $4E computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 */
Std_ReturnType Dem_DcmReadDataOfPID4E(uint8* PID4Evalue)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID4Evalue);
    return E_NOT_OK;
}
#endif

/**
 * @brief Service to report the value of PID $91 computed by the Dem. API is needed in OBD-relevant ECUs
 * only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPID91(uint8* PID91value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID91value);
    return E_NOT_OK;
}

/**
 * @brief Gets data element per PID and index of the most important freeze frame being selected for the
 * output of service $02. The function stores the data in the provided DestBuffer. API is needed in
 * OBD-relevant ECUs only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType
    Dem_DcmReadDataOfOBDFreezeFrame(uint8 PID, uint8 DataElementIndexOfPID, uint8* DestBuffer, uint16* BufSize)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PID);
    DEM_PARAM_UNUSED(DataElementIndexOfPID);
    DEM_PARAM_UNUSED(DestBuffer);
    DEM_PARAM_UNUSED(BufSize);
    return E_NOT_OK;
}

/**
 * @brief Gets DTC by freeze frame record number. API is needed in OBD-relevant ECUs only. API Availability: This API
 * will be available only if ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetDTCOfOBDFreezeFrame(uint8 FrameNumber, uint32* DTC, Dem_DTCFormatType DTCFormat)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(FrameNumber);
    DEM_PARAM_UNUSED(DTC);
    DEM_PARAM_UNUSED(DTCFormat);
    return E_NOT_OK;
}

/**
 * @brief Reports the value of a requested "availability-OBDMID" to the DCM upon a Service $06 request.
 * Derived from that the tester displays the supported tests a mechanic can select from. API is
 * needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetAvailableOBDMIDs(uint8 Obdmid, uint32* Obdmidvalue)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(Obdmid);
    DEM_PARAM_UNUSED(Obdmidvalue);
    return E_NOT_OK;
}

/**
 * @brief Gets the number of TIDs per (functional) OBDMID. This can be used by the DCM to iteratively
 * request for OBD/TID result data within a loop from 0....numberOfTIDs-1 API is needed in
 * OBD-relevant ECUs only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetNumTIDsOfOBDMID(uint8 Obdmid, uint8* numberOfTIDs)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(Obdmid);
    DEM_PARAM_UNUSED(numberOfTIDs);
    return E_NOT_OK;
}

/**
 * @brief Reports a DTR data along with TID-value, UaSID, test result with lower and upper limit. API is
 * needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetDTRData(
    uint8   Obdmid,
    uint8   TIDindex,
    uint8*  TIDvalue,
    uint8*  UaSID,
    uint16* Testvalue,
    uint16* Lowlimvalue,
    uint16* Upplimvalue)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(Obdmid);
    DEM_PARAM_UNUSED(TIDindex);
    DEM_PARAM_UNUSED(TIDvalue);
    DEM_PARAM_UNUSED(UaSID);
    DEM_PARAM_UNUSED(Testvalue);
    DEM_PARAM_UNUSED(Lowlimvalue);
    DEM_PARAM_UNUSED(Upplimvalue);
    return E_NOT_OK;
}

/**
 * @brief Service to report the value of monitor activity denominator PID computed by the Dem. API is
 * needed in OBD-relevant ECUs only. API Availability: This API will be available only if:
 * ({ecucDem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmGetInfoTypeValue79(Dcm_OpStatusType OpStatus, uint8* DataValueBuffer, uint8* DataValueBufferSize)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(OpStatus);
    DEM_PARAM_UNUSED(DataValueBuffer);
    DEM_PARAM_UNUSED(DataValueBufferSize);
    return E_NOT_OK;
}

/**
 * @brief Function to report the value of PID 0xF501 computed by the Dem. API is needed in OBD-relevant
 * ECUs only. API Availability: This API will be available only if:
 * ({ecucDem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 */
Std_ReturnType Dem_DcmReadDataOfPIDF501(uint8* PIDF501value)
{
    /** API not yet implemented */
    DEM_PARAM_UNUSED(PIDF501value);
    return E_NOT_OK;
}
/* PRQA S 3673 -- */
#endif
/* PRQA S 3406, 1503 -- */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
