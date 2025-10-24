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
 **  @file               : Dcm_OBD.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD public function implementation
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Dcm_Internal.h"
#if ((STD_ON == DCM_OBD_0X6) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#include "Dem_Dcm.h"
#endif
/* ===================================================== macros ===================================================== */
/**
 * @brief mask for identifying availability id
 */
#define DCM_OBD_AVAILABILITY_MASK 0x20u
/**
 * @brief the length of tid and tid available data
 */
#define DCM_TID_LEN 5u
/**
 * @brief the length of pid and pid available data
 */
#define DCM_PID_LEN 5u
/**
 * @brief the length of mid and mid available data
 */
#define DCM_MID_LEN 5u
/**
 * @brief the length of mid data
 */
#define DCM_MID_DATALEN 9u
/**
 * @brief the length of vehInfo and vehInfo available data
 */
#define DCM_VEHINFO_LEN 5u
/* ========================================== internal function declarations ======================================== */
#if ((STD_ON == DCM_OBD_0X9) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         Called by Dcm_OBD0x09 to find vehInfo index
 * @param[in]     infoType     : input infoType
 * @param[out]    vehInfoIndex : the configured vehInfo index
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x09_FindVehInfo(uint8 infoType, uint8* vehInfoIndex);
#endif
#if ((STD_ON == DCM_OBD_0X1) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         read availability Pid
 * @param[in]     Pid           : input pid
 * @param[in]     availPidIndex : the index of availability pid
 * @param[inout]  mixPid        : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                                Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize       : When the function is called this parameter contains the maximum number of data bytes
 *                                that can be written to the buffer. The function returns the actual number of written
 *                                data bytes in this parameter.
 * @param[out]    DestBuffer    : The output data
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x01_ReadAvailabilityPid(
    uint8           Pid,
    uint8*          mixPid,
    uint8           availPidIndex,
    Dcm_MsgLenType* BufSize,
    uint8*          DestBuffer);

/**
 * @brief         read Pid Data
 * @param[in]     pidIndex   : input pid configuration index
 * @param[inout]  mixPid     : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability Id,
 *                             bit 2 is set for non-availability ones.
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes that
 *                             can be written to the buffer. The function returns the actual number of written data
 *                             bytes in this parameter.
 * @param[out]    DestBuffer : The output data
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x01_ReadPidData(uint8 pidIndex, uint8* mixPid, Dcm_MsgLenType* BufSize, uint8* DestBuffer);

/**
 * @brief         process Pid Data
 * @param[in]     DcmDspPidPtr : input pid configuration
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    DestBuffer   : The output data
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x01_ProcessPidData(const Dcm_DspPidType* DcmDspPidPtr, Dcm_MsgLenType* BufSize, uint8* DestBuffer);
#endif

#if ((STD_ON == DCM_OBD_0X6) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         read availability Mid
 * @param[in]     mid           : input mid
 * @param[inout]  mixMid        : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                                Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize       : When the function is called this parameter contains the maximum number of data bytes
 *                                that can be written to the buffer. The function returns the actual number of written
 *                                data bytes in this parameter.
 * @param[out]    DestBuffer    : The output data
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x06_ReadAvailabilityMid(uint8 mid, uint8* mixMid, Dcm_MsgLenType* BufSize, uint8* DestBuffer);

/**
 * @brief         read Mid Data
 * @param[in]     mid          : input mid
 * @param[in]     numberOfTIDs : number of TIDs
 * @param[inout]  mixMid       : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                               Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    DestBuffer   : The output data
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x06_ReadMidData(uint8 mid, uint8* mixMid, uint8 numberOfTIDs, Dcm_MsgLenType* BufSize, uint8* DestBuffer);

/**
 * @brief         process Mid Data
 * @param[in]     mid          : input mid
 * @param[in]     numberOfTIDs : number of TIDs
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    DestBuffer   : The output data
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x06_ProcessMidData(uint8 mid, uint8 numberOfTIDs, Dcm_MsgLenType* BufSize, uint8* DestBuffer);
#endif

#if ((STD_ON == DCM_OBD_0X8) || ((STD_ON == DCM_UDS_0X31) && (STD_ON == DCM_OBD_RID_MIRROR)))
/**
 * @brief         read availability Tid
 * @param[in]     tid           : input tid
 * @param[in]     availTidIndex : the index of availability tid
 * @param[inout]  MixTid        : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                                Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize       : When the function is called this parameter contains the maximum number of data bytes
 *                                that can be written to the buffer. The function returns the actual number of written
 *                                data bytes in this parameter.
 * @param[out]    DestBuffer    : The output data
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x08_ReadAvailabilityTid(
    uint8           tid,
    uint8*          MixTid,
    uint8           availTidIndex,
    Dcm_MsgLenType* BufSize,
    uint8*          DestBuffer);

/**
 * @brief         read Tid Data
 * @param[in]     tidIndex     : input tid configuration index
 * @param[in]     InBuffer     : the input data
 * @param[in]     InBufferSize : the size of input data
 * @param[inout]  MixTid       : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                               Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    DestBuffer   : The output data
 * @param[out]    ErrorCode    : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x08_ReadTidData(
    uint8                         tidIndex,
    uint8*                        MixTid,
    Dcm_MsgLenType*               BufSize,
    uint8*                        DestBuffer,
    uint8*                        InBuffer,
    Dcm_MsgLenType                InBufferSize,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
#if ((STD_ON == DCM_OBD_0X9) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
/**
 * @brief         read availability vehInfo
 * @param[in]     infoType           : input infoType
 * @param[in]     availInfoTypeIndex : input availability infoType index
 * @param[inout]  MixInfoType        : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for
 *                                     availability Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize            : When the function is called this parameter contains the maximum number of data
 *                                     bytes that can be written to the buffer. The function returns the actual number
 *                                     of written data bytes in this parameter.
 * @param[out]    DestBuffer         : The output data
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x09_ReadAvailabilityVehInfo(
    uint8           infoType,
    uint8           availInfoTypeIndex,
    uint8*          MixInfoType,
    Dcm_MsgLenType* BufSize,
    uint8*          DestBuffer);

/**
 * @brief         read vehInfo Data
 * @param[in]     OpStatus     : current operation status
 * @param[in]     vehInfoIndex : input vehInfo index
 * @param[inout]  MixInfoType  : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                               Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    DestBuffer   : The output data
 * @param[out]    ErrorCode    : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was pending
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x09_ReadVehInfoData(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         vehInfoIndex,
    uint8*                        MixInfoType,
    Dcm_MsgLenType*               BufSize,
    uint8*                        DestBuffer,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process vehInfo Data
 * @param[in]     OpStatus    : current operation status
 * @param[in]     vehInfo     : input vehInfo configuration
 * @param[inout]  MixInfoType : Start as 0 for OBD request or NULL_PTR for UDS request, bit 1 is set for availability
 *                              Id, bit 2 is set for non-availability ones.
 * @param[inout]  BufSize     : When the function is called this parameter contains the maximum number of data bytes
 *                              that can be written to the buffer. The function returns the actual number of written
 *                              data bytes in this parameter.
 * @param[out]    DestBuffer  : The output data
 * @param[out]    ErrorCode   : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was pending
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x09_ProcessVehInfoData(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspVehInfoType*     vehInfo,
    uint8*                        MixInfoType,
    Dcm_MsgLenType*               BufSize,
    uint8*                        DestBuffer,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */
#if ((STD_ON == DCM_OBD_0X3) || (STD_ON == DCM_OBD_0X7) || (STD_ON == DCM_OBD_0XA))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * get filterd DTC from Dem with input required source
 */
Std_ReturnType Dcm_OBD_GetFilteredDTC(
    uint8             ClientId,
    uint8             DTCStatusMask,
    Dem_DTCOriginType DTCOrigin,
    uint8*            DestBuffer,
    uint16*           NumberOfFilteredDTC)
{
    /* set dtc filter for OBD format DTC */
    Std_ReturnType result = Dem_SetDTCFilter(
        ClientId,
        DTCStatusMask,
        DEM_DTC_FORMAT_OBD,
        DTCOrigin,
        FALSE,
        DEM_SEVERITY_NO_SEVERITY,
        FALSE);

    /* get the number of DTC after filter */
    if (E_OK == result)
    {
        result = Dem_GetNumberOfFilteredDTC(ClientId, NumberOfFilteredDTC);
        if ((E_OK == result) && (*NumberOfFilteredDTC > DCM_INVALID_UINT8))
        {
            result = E_NOT_OK;
        }
    }

    /* get filtered DTC and set to DestBuffer */
    uint8 offset = 0u;
    while (E_OK == result)
    {
        uint32 DTC;
        uint8  DTCStatus;
        result = Dem_GetNextFilteredDTC(ClientId, &DTC, &DTCStatus);
        if (E_OK == result)
        {
            DcmInternal_TransformArray_u16(&DestBuffer[offset], (uint16)(DTC & DCM_OBD_DTC_MASK), DCM_OPAQUE);
            offset += DCM_OBD_DTC_LENGTH;
        }
    }

    /* reach end, return normally */
    if (DEM_NO_SUCH_ELEMENT == result)
    {
        result = E_OK;
    }

    /* no DTC is found */
    if (0u == offset)
    {
        *NumberOfFilteredDTC = 0u;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X1) || (STD_ON == DCM_OBD_0X2))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * get pidIndex of input pid and serviceid
 */
Std_ReturnType Dcm_OBD_FindPid(uint8 Pid, uint8 ServiceId, uint8* PidIndex)
{
    Std_ReturnType result = E_NOT_OK;

#if (DCM_PID_NUM > 0)
    for (uint8 index = 0u; index < DCM_PID_NUM; index++)
    {
        /* find match id and service id */
        const Dcm_DspPidType* dspPid = &Dcm_DspPid[index];
        if ((TRUE == Dcm_CfgPtr->PidUsed[index]) && (dspPid->PidIdentifier == Pid)
            && ((DCM_SERVICE_01_02 == dspPid->PidService)
                || ((0x01u == ServiceId) && (DCM_SERVICE_01 == dspPid->PidService))
                || ((0x02u == ServiceId) && (DCM_SERVICE_02 == dspPid->PidService))))
        {
            *PidIndex = index;
            result    = E_OK;
            break;
        }
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * check whether the input id is an availability one
 */
boolean Dcm_OBD_isAvailability(uint8 Id, uint8* AvailIdIndex) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    boolean result;
    /* check if id is 0x20/0x40/0x60/... as an availability id */
    if ((Id % DCM_OBD_AVAILABILITY_MASK) == 0u)
    {
        if (NULL_PTR != AvailIdIndex)
        {
            /* get the index of the supported table */
            *AvailIdIndex = Id / DCM_OBD_AVAILABILITY_MASK;
        }
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if ((STD_ON == DCM_OBD_0X8) || ((STD_ON == DCM_UDS_0X31) && (STD_ON == DCM_OBD_RID_MIRROR)))
/**
 * Retrive Tid data
 */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_OBD0x08_ReadTid(
    uint8                         tid,
    uint8*                        InBuffer,
    Dcm_MsgLenType                InBufferSize,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
#ifdef DCM_TID_NUM
    Std_ReturnType result = E_OK;
    uint8          availTidIndex;
    uint8          MixTid = 0u;

    if (TRUE == Dcm_OBD_isAvailability(tid, &availTidIndex))
    {
        result = Dcm_OBD0x08_ReadAvailabilityTid(tid, &MixTid, availTidIndex, BufSize, DestBuffer);
    }
    else
    {
        uint8 tidIndex;
        result = Dcm_OBD0x08_FindTid(tid, &tidIndex);
        if (E_OK == result)
        {
            result = Dcm_OBD0x08_ReadTidData(tidIndex, &MixTid, BufSize, DestBuffer, InBuffer, InBufferSize, ErrorCode);
        }
    }
    return result;
#else
    return E_NOT_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * find tid index based on input tid
 */
Std_ReturnType Dcm_OBD0x08_FindTid(uint8 tid, uint8* tidIndex)
{
    Std_ReturnType result = E_NOT_OK;

#ifdef DCM_TID_NUM
    /* find matched testId configuration index */
    for (uint8 index = 0u; index < DCM_TID_NUM; index++)
    {
        if (Dcm_DspRequestControl[index].TestId == tid)
        {
            *tidIndex = index;
            result    = E_OK;
            break;
        }
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X1) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * retrive pid data
 */
Std_ReturnType Dcm_OBD0x01_ReadPid(
    Dcm_ExtendedOpStatusType OpStatus,
    uint8                    Pid,
    uint8*                   DestBuffer,
    Dcm_MsgLenType*          BufSize,
    uint8*                   mixPid)
{
    uint8          availPidIndex;
    Std_ReturnType result = E_OK;
    DCM_UNUSED(OpStatus);

    if (TRUE == Dcm_OBD_isAvailability(Pid, &availPidIndex))
    {
        result = Dcm_OBD0x01_ReadAvailabilityPid(Pid, mixPid, availPidIndex, BufSize, DestBuffer);
    }
    else
    {
#if (DCM_PID_NUM > 0)
        uint8 pidIndex = 0u;
        if (E_OK == Dcm_OBD_FindPid(Pid, 0x01u, &pidIndex))
        {
            result = Dcm_OBD0x01_ReadPidData(pidIndex, mixPid, BufSize, DestBuffer);
        }
        else
        {
            result = E_NOT_OK;
        }
#else
        result = E_NOT_OK;
#endif
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X6) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * retrive mid data
 */
Std_ReturnType Dcm_OBD0x06_ReadMid(uint8 mid, uint8* DestBuffer, Dcm_MsgLenType* BufSize, uint8* mixMid)
{
    Std_ReturnType result;
    uint8          availMidIndex;

    if (TRUE == Dcm_OBD_isAvailability(mid, &availMidIndex))
    {
        result = Dcm_OBD0x06_ReadAvailabilityMid(mid, mixMid, BufSize, DestBuffer);
    }
    else
    {
        /* get number of reporting TID */
        uint8 numberOfTIDs;
        result = Dem_DcmGetNumTIDsOfOBDMID(mid, &numberOfTIDs);
        if (E_OK == result)
        {
            result = Dcm_OBD0x06_ReadMidData(mid, mixMid, numberOfTIDs, BufSize, DestBuffer);
        }
    }
    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X9) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * Retrive vehInfo Data
 */
Std_ReturnType Dcm_OBD0x09_ReadVehInfo(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         infoType,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    uint8*                        MixInfoType,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint8          availInfoTypeIndex;

    if (TRUE == Dcm_OBD_isAvailability(infoType, &availInfoTypeIndex))
    {
        result = Dcm_OBD0x09_ReadAvailabilityVehInfo(infoType, availInfoTypeIndex, MixInfoType, BufSize, DestBuffer);
    }
    else
    {
        uint8 vehInfoIndex;
        result = Dcm_OBD0x09_FindVehInfo(infoType, &vehInfoIndex);
        if (E_OK == result)
        {
            result = Dcm_OBD0x09_ReadVehInfoData(OpStatus, vehInfoIndex, MixInfoType, BufSize, DestBuffer, ErrorCode);
        }
    }
    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#if ((STD_ON == DCM_OBD_0X9) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * Retrive vehInfo Data
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x09_FindVehInfo(uint8 infoType, uint8* vehInfoIndex)
{
    Std_ReturnType result = E_NOT_OK;

#ifdef DCM_TID_NUM
    /* iterate to find matched vehInfo and configuration index */
    for (uint8 index = 0u; index < DCM_TID_NUM; index++)
    {
        if (Dcm_DspVehInfo[index].VehInfoInfoType == infoType)
        {
            *vehInfoIndex = index;
            result        = E_OK;
            break;
        }
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X1) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_OBD0x01_ReadAvailabilityPid(
    uint8           Pid,
    uint8*          mixPid,
    uint8           availPidIndex,
    Dcm_MsgLenType* BufSize,
    uint8*          DestBuffer)
{
    Std_ReturnType result = E_OK;

    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != mixPid) && (0u != (*mixPid & DCM_OBD_NONAVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != mixPid)
        {
            (*mixPid) |= DCM_OBD_AVAILABLE_BIT;
        }
        if (*BufSize >= DCM_PID_LEN)
        {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            if (NULL_PTR != DestBuffer)
#endif
            {
                /* get supported info from table */
                DestBuffer[0u]     = Pid;
                boolean isResponse = FALSE;
                for (uint8 index = 0u; (index < DCM_OBDID_AVAILDATALEN) && (isResponse == FALSE); index++)
                {
                    if (Dcm_SupportedPidTable[index + DCM_GET_AVAILINDEX(availPidIndex)] != 0u)
                    {
                        isResponse = TRUE;
                    }
                }
                if (isResponse)
                {
                    DcmInternal_Memcpy(
                        &DestBuffer[1u],
                        &Dcm_SupportedPidTable[DCM_GET_AVAILINDEX(availPidIndex)],
                        DCM_OBDID_AVAILDATALEN);
                    *BufSize = DCM_PID_LEN;
                }
                else
                {
                    *BufSize = 0u;
                }
            }
        }
        else
        {
            *BufSize = 0u;
            result   = E_NOT_OK;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X1) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x01_ReadPidData(uint8 pidIndex, uint8* mixPid, Dcm_MsgLenType* BufSize, uint8* DestBuffer)
{
    Std_ReturnType result = E_OK;

    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != mixPid) && (0u != (*mixPid & DCM_OBD_AVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != mixPid)
        {
            (*mixPid) |= DCM_OBD_NONAVAILABLE_BIT;
        }
#if (DCM_PID_NUM > 0u)
        const Dcm_DspPidType* DcmDspPidPtr = &Dcm_DspPid[pidIndex];
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (NULL_PTR != DestBuffer)
#endif
        {
            if (*BufSize >= ((Dcm_MsgLenType)DcmDspPidPtr->PidSize + 1u))
            {
                result = Dcm_OBD0x01_ProcessPidData(DcmDspPidPtr, BufSize, DestBuffer);
            }
            else
            {
                *BufSize = 0u;
                result   = E_NOT_OK;
            }
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        else if (*BufSize >= ((Dcm_MsgLenType)DcmDspPidPtr->PidSize + 1u))
        {
            *BufSize = (Dcm_MsgLenType)DcmDspPidPtr->PidSize + 1u;
        }
        else
        {
            *BufSize = 0u;
            result   = E_NOT_OK;
        }
#endif
#else
        result = E_NOT_OK;
#endif
    }

    return result;
}

DCM_LOCAL Std_ReturnType
    Dcm_OBD0x01_ProcessPidData(const Dcm_DspPidType* DcmDspPidPtr, Dcm_MsgLenType* BufSize, uint8* DestBuffer)
{
    Std_ReturnType result = E_OK;

    DestBuffer[0u] = DcmDspPidPtr->PidIdentifier;
    DcmInternal_Memset(&DestBuffer[1u], 0u, DcmDspPidPtr->PidSize);
    for (uint8 index = 0u; (index < DcmDspPidPtr->PidDataNum) && (E_NOT_OK != result); index++)
    {
        const Dcm_DspPidDataType* PidData = &(DcmDspPidPtr->PidData[index]);
        /* check configuration for service 01 */
        if (NULL_PTR != PidData->PidService01)
        {
            /* get output data from pidDataReadFnc */
            result = PidData->PidService01->PidDataReadFnc(&DestBuffer[PidData->PidByteOffset + 1u]);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            if ((E_OK != result) && (E_NOT_OK != result))
            {
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
                result = E_NOT_OK;
            }
#endif
        }
        else
        {
            result = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        *BufSize = (Dcm_MsgLenType)DcmDspPidPtr->PidSize + 1u;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X6) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType
    Dcm_OBD0x06_ReadAvailabilityMid(uint8 mid, uint8* mixMid, Dcm_MsgLenType* BufSize, uint8* DestBuffer)
{
    Std_ReturnType result = E_OK;
    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != mixMid) && (0u != (*mixMid & DCM_OBD_NONAVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != mixMid)
        {
            (*mixMid) |= DCM_OBD_AVAILABLE_BIT;
        }
        if (*BufSize >= DCM_MID_LEN)
        {
            uint32 Obdmidvalue = 0u;
            /* get availability data from Dem */
            result = Dem_DcmGetAvailableOBDMIDs(mid, &Obdmidvalue);
            if (E_OK == result)
            {
                *BufSize = 0u;
                if ((Obdmidvalue != 0u)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
                    && (NULL_PTR != DestBuffer)
#endif
                )
                {
                    DestBuffer[0u] = mid;
                    DcmInternal_TransformArray_u32(&DestBuffer[1u], Obdmidvalue, DCM_OPAQUE);
                    *BufSize = DCM_MID_LEN;
                }
            }
        }
        else
        {
            *BufSize = 0u;
            result   = E_NOT_OK;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType
    Dcm_OBD0x06_ReadMidData(uint8 mid, uint8* mixMid, uint8 numberOfTIDs, Dcm_MsgLenType* BufSize, uint8* DestBuffer)
{
    Std_ReturnType result = E_OK;
    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != mixMid) && (0u != (*mixMid & DCM_OBD_AVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != mixMid)
        {
            (*mixMid) |= DCM_OBD_NONAVAILABLE_BIT;
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (NULL_PTR != DestBuffer)
#endif
        {
            /* check for buffer size to hold TID data */
            if (*BufSize >= ((Dcm_MsgLenType)numberOfTIDs * DCM_MID_DATALEN))
            {
                result = Dcm_OBD0x06_ProcessMidData(mid, numberOfTIDs, BufSize, DestBuffer);
            }
            else
            {
                *BufSize = 0u;
                result   = E_NOT_OK;
            }
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        else
        {
            *BufSize = (Dcm_MsgLenType)numberOfTIDs * DCM_MID_DATALEN;
        }
#endif
    }

    return result;
}

DCM_LOCAL Std_ReturnType
    Dcm_OBD0x06_ProcessMidData(uint8 mid, uint8 numberOfTIDs, Dcm_MsgLenType* BufSize, uint8* DestBuffer)
{
    Std_ReturnType result = E_OK;
    /* iterate over TID to get DTR Data */
    for (uint8 index = 0u; (index < numberOfTIDs) && (E_OK == result); index++)
    {
        uint8  TIDvalue;
        uint8  UaSID;
        uint16 Testvalue;
        uint16 Lowlimvalue;
        uint16 Upplimvalue;
        result = Dem_DcmGetDTRData(mid, index, &TIDvalue, &UaSID, &Testvalue, &Lowlimvalue, &Upplimvalue);
        if (E_OK == result)
        {
            DestBuffer[0u] = mid;
            DestBuffer[1u] = TIDvalue;
            DestBuffer[2u] = UaSID;
            DcmInternal_TransformArray_u16(&DestBuffer[3u], Testvalue, DCM_OPAQUE);
            DcmInternal_TransformArray_u16(&DestBuffer[5u], Lowlimvalue, DCM_OPAQUE);
            DcmInternal_TransformArray_u16(&DestBuffer[7u], Upplimvalue, DCM_OPAQUE);
        }
    }
    if (E_OK == result)
    {
        *BufSize = (Dcm_MsgLenType)numberOfTIDs * DCM_MID_DATALEN;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X8) || ((STD_ON == DCM_UDS_0X31) && (STD_ON == DCM_OBD_RID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_OBD0x08_ReadAvailabilityTid(
    uint8           tid,
    uint8*          MixTid,
    uint8           availTidIndex,
    Dcm_MsgLenType* BufSize,
    uint8*          DestBuffer)
{
#ifdef DCM_TID_NUM
    boolean        isResponse = FALSE;
    Std_ReturnType result     = E_OK;

    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != MixTid) && (0u != (*MixTid & DCM_OBD_NONAVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != MixTid)
        {
            (*MixTid) |= DCM_OBD_AVAILABLE_BIT;
        }
        /* get supported info from table */
        if (*BufSize >= DCM_TID_LEN)
        {
            for (uint8 iloop = 0u; (iloop < DCM_OBDID_AVAILDATALEN) && (isResponse == FALSE); iloop++)
            {
                if (Dcm_SupportedTidTable[iloop + DCM_GET_AVAILINDEX(availTidIndex)] != 0u)
                {
                    isResponse = TRUE;
                }
            }
            if (isResponse)
            {
                DestBuffer[0u] = tid;
                DcmInternal_Memcpy(
                    &DestBuffer[1u],
                    &Dcm_SupportedTidTable[DCM_GET_AVAILINDEX(availTidIndex)],
                    DCM_OBDID_AVAILDATALEN);
                *BufSize = DCM_TID_LEN;
            }
            else
            {
                *BufSize = 0u;
            }
        }
        else
        {
            *BufSize = 0u;
            result   = E_NOT_OK;
        }
    }

    return result;
#else
    return E_NOT_OK;
#endif
}

DCM_LOCAL Std_ReturnType Dcm_OBD0x08_ReadTidData(
    uint8                         tidIndex,
    uint8*                        MixTid,
    Dcm_MsgLenType*               BufSize,
    uint8*                        DestBuffer,
    uint8*                        InBuffer,
    Dcm_MsgLenType                InBufferSize,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
#ifdef DCM_TID_NUM
    Std_ReturnType result = E_OK;

    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != MixTid) && (0u != (*MixTid & DCM_OBD_AVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != MixTid)
        {
            (*MixTid) |= DCM_OBD_NONAVAILABLE_BIT;
        }
        const Dcm_DspRequestControlType* requestControl = &Dcm_DspRequestControl[tidIndex];
        /* only process if input data size matches with configuration */
        if (InBufferSize == requestControl->InBufferSize)
        {
            if (*BufSize >= ((Dcm_MsgLenType)requestControl->OutBufferSize + 1u))
            {
                /* enough buffer provided, provide output data by callout fncs */
                DestBuffer[0u] = requestControl->TestId;
                DcmInternal_Memset(&DestBuffer[1u], 0u, requestControl->OutBufferSize);
                result   = requestControl->RequestControlFnc(&DestBuffer[1u], InBuffer);
                *BufSize = (Dcm_MsgLenType)requestControl->OutBufferSize + 1u;
                if (E_NOT_OK == result)
                {
                    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                }
            }
            else
            {
                *BufSize = 0u;
                result   = E_NOT_OK;
            }
        }
        else
        {
            result = E_NOT_OK;
        }
    }

    return result;
#else
    return E_NOT_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if ((STD_ON == DCM_OBD_0X9) || ((STD_ON == DCM_UDS_0X22) && (STD_ON == DCM_OBD_DID_MIRROR)))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_OBD0x09_ReadAvailabilityVehInfo(
    uint8           infoType,
    uint8           availInfoTypeIndex,
    uint8*          MixInfoType,
    Dcm_MsgLenType* BufSize,
    uint8*          DestBuffer)
{
    boolean        isResponse = FALSE;
    Std_ReturnType result     = E_OK;

    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != MixInfoType) && (0u != (*MixInfoType & DCM_OBD_NONAVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != MixInfoType)
        {
            (*MixInfoType) |= DCM_OBD_AVAILABLE_BIT;
        }
        if (*BufSize >= DCM_VEHINFO_LEN)
        {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            if (NULL_PTR != DestBuffer)
#endif
            {
                for (uint8 index = 0u; (index < DCM_OBDID_AVAILDATALEN) && (isResponse == FALSE); index++)
                {
                    if (Dcm_SupportedPidTable[index + DCM_GET_AVAILINDEX(availInfoTypeIndex)] != 0u)
                    {
                        isResponse = TRUE;
                    }
                }
                if (isResponse)
                {
                    /* get supported info from table */
                    DestBuffer[0u] = infoType;
                    DcmInternal_Memcpy(
                        &DestBuffer[1u],
                        &Dcm_SupportedInfoTypeTable[DCM_GET_AVAILINDEX(availInfoTypeIndex)],
                        DCM_OBDID_AVAILDATALEN);
                    *BufSize = DCM_VEHINFO_LEN;
                }
                else
                {
                    *BufSize = 0u;
                }
            }
        }
        else
        {
            *BufSize = 0u;
            result   = E_NOT_OK;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_OBD0x09_ReadVehInfoData(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         vehInfoIndex,
    uint8*                        MixInfoType,
    Dcm_MsgLenType*               BufSize,
    uint8*                        DestBuffer,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    /* mixed availability and regular tid, not allowed */
    if ((NULL_PTR != MixInfoType) && (0u != (*MixInfoType & DCM_OBD_AVAILABLE_BIT)))
    {
        result = E_NOT_OK;
    }
    else
    {
        if (NULL_PTR != MixInfoType)
        {
            (*MixInfoType) |= DCM_OBD_NONAVAILABLE_BIT;
        }
        const Dcm_DspVehInfoType* vehInfo = &Dcm_DspVehInfo[vehInfoIndex];
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (NULL_PTR != DestBuffer)
#endif
        {
            DestBuffer[0u] = vehInfo->VehInfoInfoType;
            result = Dcm_OBD0x09_ProcessVehInfoData(OpStatus, vehInfo, MixInfoType, BufSize, DestBuffer, ErrorCode);
        }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        else
        {
            Dcm_MsgLenType tempBufSize = 0u;
            for (uint8 index = 0u; index < vehInfo->VehInfoDataNum; index++)
            {
                const Dcm_DspVehInfoDataType* VehInfoData = &vehInfo->VehInfoData[index];
                tempBufSize += VehInfoData->VehInfoDataSize;
            }
            *BufSize = tempBufSize + 1u;
        }
#endif
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_OBD0x09_ProcessVehInfoData(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspVehInfoType*     vehInfo,
    uint8*                        MixInfoType,
    Dcm_MsgLenType*               BufSize,
    uint8*                        DestBuffer,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType finalResult = E_OK;
    Dcm_MsgLenType offset      = 0u;
    Dcm_MsgLenType tempBufSize = *BufSize;

    /* iterate over vehInfoData and calls callouts to get data */
    for (uint8 index = 0u; (index < vehInfo->VehInfoDataNum) && (E_NOT_OK != finalResult); index++)
    {
        const Dcm_DspVehInfoDataType* VehInfoData = &vehInfo->VehInfoData[index];
        if (*BufSize < VehInfoData->VehInfoDataSize)
        {
            *BufSize    = 0u;
            finalResult = E_NOT_OK;
            break;
        }
        Std_ReturnType result =
            VehInfoData->VehInfoDataReadFnc(OpStatus, &DestBuffer[offset + 2u], (uint8*)&tempBufSize);

        switch (result)
        {
        case E_OK:
        {
            /* use size provided by callout or from configuration depending on VehInfoNODIProvResp */
            offset += (TRUE == vehInfo->VehInfoNODIProvResp) ? tempBufSize : VehInfoData->VehInfoDataSize;
            tempBufSize = (TRUE == vehInfo->VehInfoNODIProvResp) ? (*BufSize - tempBufSize)
                                                                 : (*BufSize - VehInfoData->VehInfoDataSize);
            break;
        }
        case DCM_E_PENDING:
        {
            finalResult = DCM_E_PENDING;
            break;
        }
        case E_NOT_OK:
        {
            finalResult = E_NOT_OK;
            if (NULL_PTR != ErrorCode)
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            }
            break;
        }
        default:
        {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            finalResult = E_NOT_OK;
#endif
            break;
        }
        }
    }

    if (E_OK == finalResult)
    {
        *BufSize = offset;
        if (NULL_PTR != MixInfoType)
        {
            DestBuffer[1u] = (uint8)offset;
        }
    }

    return finalResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
