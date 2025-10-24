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
 **  @file               : Dcm_OBD0x02.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD service 0x02 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_OBD_0X2)
#include "Dem_Dcm.h"
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief the length of requested pid and record
 */
#define DCM_OBD_PIDRECORD_REQLEN 2u
/**
 * @brief the length of responsed availability pid
 */
#define DCM_OBD_PIDAVAIL_RSPLEN 6u
/**
 * @brief the length of responsed pid
 */
#define DCM_OBD_PID_RSPLEN 2u
/**
 * @brief the length of responsed pid and DTC
 */
#define DCM_OBD_PIDDTC_RSPLEN 4u
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Called by Dcm_OBD0x02 to read pid
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x02_ReadPid(Dcm_MsgContextType* pMsgContext);

/**
 * @brief         read pid data
 * @param[in]     pid         : The requested pid
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[inout]  offset      : Current offset
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_OBD0x02_ReadPidData(Dcm_MsgContextType* pMsgContext, uint8 pid, uint8* offset);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for OBD 0x02
 */
Std_ReturnType Dcm_OBD0x02(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result;
    DCM_UNUSED(ErrorCode);
    DCM_UNUSED(OpStatus);

    /* The request length shall be even */
    if (0u != (pMsgContext->reqDataLen % DCM_OBD_PIDRECORD_REQLEN))
    {
        result = E_NOT_OK;
    }
    else
    {
        result = Dcm_OBD0x02_ReadPid(pMsgContext);
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType Dcm_OBD0x02_ReadPid(Dcm_MsgContextType* pMsgContext)
{
    Std_ReturnType result = E_OK;
    uint8          mixPid = 0u;
    uint8          offset = 0u;

    for (uint8 index = 0u; index < (pMsgContext->reqDataLen / DCM_OBD_PIDRECORD_REQLEN); index++)
    {
        const uint8* reqData = pMsgContext->reqData;
        uint8        pid     = reqData[index * DCM_OBD_PIDRECORD_REQLEN];
        /* ignore requests regarding record-numbers that are not 0 */
        if (0u == reqData[(index * DCM_OBD_PIDRECORD_REQLEN) + 1u])
        {
            boolean isResponse = FALSE;
            uint8*  resData    = pMsgContext->resData;
            uint8   availPidIndex;
            if (TRUE == Dcm_OBD_isAvailability(pid, &availPidIndex))
            {
                mixPid |= DCM_OBD_AVAILABLE_BIT;
                for (uint8 iloop = 0u; (iloop < DCM_OBDID_AVAILDATALEN) && (isResponse == FALSE); iloop++)
                {
                    if (Dcm_SupportedPidTable[iloop + DCM_GET_AVAILINDEX(availPidIndex)] != 0u)
                    {
                        isResponse = TRUE;
                    }
                }
                if (isResponse)
                {
                    resData[offset]      = pid;
                    resData[offset + 1u] = 0u;
                    DcmInternal_Memcpy(
                        &resData[offset + 2u],
                        &Dcm_SupportedPidTable[DCM_GET_AVAILINDEX(availPidIndex)],
                        DCM_OBDID_AVAILDATALEN);
                    offset += DCM_OBD_PIDAVAIL_RSPLEN;
                }
            }
            else
            {
                mixPid |= DCM_OBD_NONAVAILABLE_BIT;
                result = Dcm_OBD0x02_ReadPidData(pMsgContext, pid, &offset);
            }
            if (DCM_OBD_MIXED_AVAILABLE_BIT == mixPid)
            {
                result = E_NOT_OK;
                break;
            }
        }
    }

    if (E_OK == result)
    {
        if (0u == offset)
        {
            result = E_NOT_OK;
        }
        else
        {
            pMsgContext->resDataLen = offset;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_OBD0x02_ReadPidData(Dcm_MsgContextType* pMsgContext, uint8 pid, uint8* offset)
{
    Std_ReturnType result = E_OK;
    uint8          pidIndex;
    uint8*         resData = pMsgContext->resData;

    if (E_OK == Dcm_OBD_FindPid(pid, 0x02u, &pidIndex))
    {
        resData[*offset]      = pid;
        resData[*offset + 1u] = 0u;
        if (2u == pid)
        {
            uint32 DTC;
            result = Dem_DcmGetDTCOfOBDFreezeFrame(0u, &DTC, DEM_DTC_FORMAT_OBD);
            if (E_OK == result)
            {
                DcmInternal_TransformArray_u16(
                    &resData[*offset + DCM_OBD_PID_RSPLEN],
                    (uint16)(DTC & DCM_OBD_DTC_MASK),
                    DCM_OPAQUE);
            }
            else
            {
                DcmInternal_Memset(&resData[*offset + DCM_OBD_PID_RSPLEN], 0u, DCM_OBD_DTC_LENGTH);
                result = E_OK;
            }
            *offset = *offset + DCM_OBD_PIDDTC_RSPLEN;
        }
        else
        {
            const Dcm_DspPidType* DcmDspPidPtr = &Dcm_DspPid[pidIndex];
            DcmInternal_Memset(&resData[*offset + DCM_OBD_PID_RSPLEN], 0u, DcmDspPidPtr->PidSize);
            for (uint8 subIndex = 0u; (subIndex < DcmDspPidPtr->PidDataNum) && (E_NOT_OK != result); subIndex++)
            {
                const Dcm_DspPidDataType* PidData         = &DcmDspPidPtr->PidData[subIndex];
                uint8                     DestBufferIndex = *offset + PidData->PidByteOffset + DCM_OBD_PID_RSPLEN;
                uint16                    BufSize         = (uint16)(pMsgContext->resMaxDataLen - DestBufferIndex);
                result =
                    Dem_DcmReadDataOfOBDFreezeFrame(pid, PidData->PidDataPosition, &resData[DestBufferIndex], &BufSize);
            }
            if (E_OK == result)
            {
                *offset = *offset + DcmDspPidPtr->PidSize + DCM_OBD_PID_RSPLEN;
            }
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif
