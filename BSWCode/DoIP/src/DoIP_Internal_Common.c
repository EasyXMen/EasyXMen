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
 **  @file               : DoIP_Internal_Common.c
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Public part in DoIP
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_Common.h"
#include "DoIP_Internal_TCP_Diagnostic.h"
#include "DoIP_Internal_TCP_Non_Diagnostic.h"

/* ============================================ internal data definitions =========================================== */

#define DOIP_START_SEC_VAR_INIT_32
#include "DoIP_MemMap.h"
/**
 * @brief Status of DoIP
 */
DOIP_InitStatusType DoIP_InitStatus = DOIP_STATUS_UNINITED; /* PRQA S 1531 */ /* VL_DoIP_NotDefineInUseFile */
#define DOIP_STOP_SEC_VAR_INIT_32
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_BOOLEAN
#include "DoIP_MemMap.h"
/**
 * @brief GID synchronization Status
 */
boolean DoIP_GidSynStatus; /* PRQA S 1533 */ /* VL_DoIP_OnlyRefeByOneFunc */
#define DOIP_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "DoIP_MemMap.h"

/* ========================================== external function definitions ========================================= */

#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"

/**
 * @brief Returns the smaller value in the parameter.
 */
PduLengthType DOIP_MIN(PduLengthType x, PduLengthType y)
{
    return (((x) < (y)) ? (x) : (y));
}

/**
 * @brief Check version field.
 */
boolean DOIP_GENERICHEADER_CHECK(const uint8* header, PduLengthType sduLen)
{
    /* PRQA S 3120,2824,4404 ++ */  /* VL_QAC_MagicNum,VL_DoIP_ArithNullptr, VL_DoIP_4404*/
    uint8 temp = header[0] ^ 0xffu; // Avoid compiling the project warning, independent calculation

    return (
        ((header[0] == 1u) || (header[0] == 2u) || (header[0] == 0xffu)) && (temp == header[1])
        && (sduLen >= DOIP_HEADER_LEN));
    /* PRQA S 3120,2824,4404 -- */
}

/**
 * @brief Transfer byte to short.
 */
uint16 DOIP_U8_2_U16(const uint8* array)
{
    /* PRQA S 3120,2824 ++ */ /* VL_QAC_MagicNum,VL_DoIP_ArithNullptr */
    return ((uint16)((uint16)array[0] << 8u) + array[1]);
    /* PRQA S 3120,2824 -- */
}

/**
 * @brief Transfer byte to int.
 */
uint32 DOIP_U8_2_U32(const uint8* array)
{
    /* PRQA S 3120,2824 ++*/ /* VL_QAC_MagicNum,VL_DoIP_ArithNullptr */
    return (((uint32)array[0] << 24u) + ((uint32)array[1] << 16u) + ((uint32)array[2] << 8u) + ((uint32)array[3]));
    /* PRQA S 3120,2824 -- */
}

/**
 * @brief Transfer short to byte.
 */
void DoIP_u16_2_u8(uint8* array, uint16 data)
{
    /* PRQA S 3120,2824 ++*/ /* VL_QAC_MagicNum,VL_DoIP_ArithNullptr */
    array[0] = (uint8)(data >> 8u);
    array[1] = (uint8)data;
    /* PRQA S 3120,2824 -- */
}

/**
 * @brief Transfer int to byte.
 */
void DoIP_u32_2_u8(uint8* array, uint32 data)
{
    /* PRQA S 3120,2824 ++*/ /* VL_QAC_MagicNum,VL_DoIP_ArithNullptr */
    array[0] = (uint8)(data >> 24u);
    array[1] = (uint8)(data >> 16u);
    array[2] = (uint8)(data >> 8u);
    array[3] = (uint8)data;
    /* PRQA S 3120,2824 --*/
}

/**
 * @brief Copy data.
 */
void DoIP_MemCpy(uint8* dest, const uint8* src, PduLengthType len) /* PRQA S 1532*/ /* VL_QAC_OneFunRef */
{
    while (len > 0u)
    {
        len--; /* PRQA S 1338*/                 /* VL_DoIP_1338 */
        dest[len] = src[len]; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
    }
}

/**
 * @brief Close connection.
 */
void DoIP_CloseSoCon(SoAd_SoConIdType soConId, boolean isAbort) /* PRQA S 4603*/ /* VL_DoIP_4603 */
{
    (void)SoAd_CloseSoCon(soConId, isAbort);

#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)

    (void)SoAd_OpenSoCon(soConId);

#endif
}

/**
 * @brief Open Or close connection.
 */
boolean DoIP_SwitchConnection(uint16 soadTxPduRef, boolean assignment, boolean isOpen)
{
    Std_ReturnType   ret;
    SoAd_SoConIdType soConId;

    if (E_OK == SoAd_GetSoConId(soadTxPduRef, &soConId)) /* PRQA S 4461 */ /* VL_DoIP_4461 */
    {
        if (isOpen == TRUE)
        {
            /*need to be TCPIP_IPADDR_ASSIGNMENT_ALL in AUTOSAR4.4.but SoAd 4.2.2 donot have this definiation*/
            if (TRUE == assignment)
            {
                (void)SoAd_RequestIpAddrAssignment(soConId, TCPIP_IPADDR_ASSIGNMENT_ALL, NULL_PTR, 0u, NULL_PTR);
            }

            ret = SoAd_OpenSoCon(soConId);
        }
        else
        {
            ret = SoAd_CloseSoCon(soConId, TRUE);

            if (TRUE == assignment)
            {
                (void)SoAd_ReleaseIpAddrAssignment(soConId);
            }
        }
    }
    else
    {
        ret = E_NOT_OK;
    }

    return (ret == E_OK); /* PRQA S 4404*/ /* VL_DoIP_4404 */
}

#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
