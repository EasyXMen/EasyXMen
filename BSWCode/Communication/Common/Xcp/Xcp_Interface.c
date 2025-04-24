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
**  FILENAME    : Xcp_Interface.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Interface functions with other modules                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Interface.h"
#include "Xcp_Internal.h"

#if (XCP_BUILD_CHECKSUM == STD_ON)
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static uint16 XcpCrc16Ccitt(const uint16* pu1data, uint16 u2Crc);
static uint16 Xcp_GetCrc16Citt(uint8* data, uint16 crc);
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#endif

/* Build_checksum */
#if (XCP_BUILD_CHECKSUM == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_16
#include "Xcp_MemMap.h"
static VAR(uint16, XCP_VAR) Xcp_Crc16CittTable[256u];
#define XCP_STOP_SEC_VAR_CLEARED_16
#include "Xcp_MemMap.h"
#endif

#if (STD_ON == XCP_SEED_AND_UNLOCK)
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType Xcp_CreateSeed(uint8* length, uint8* seed, const uint8 resource)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 i;
    *length = resource;
    for (i = 0; i < *length; i++)
    {
        seed[i] = i;
    }

    return E_OK;
}

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
boolean Xcp_IsKeyRight(const Xcp_SeedVerifyType seedVerify, const Xcp_KeyVerifyType keyVerify, const uint8 resource)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 i;
    if ((keyVerify.lengthK == resource) && (seedVerify.lengthS == resource))
    {
        for (i = 0; i < keyVerify.lengthK; i++)
        {
            if (keyVerify.key[i] != (seedVerify.seed[i] + seedVerify.seed[i / 2u]))
            {
                break;
            }
        }
        if (resource != i)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#endif /*STD_ON == XCP_SEED_AND_UNLOCK*/

#if (STD_ON == XCP_BUILD_CHECKSUM)
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static uint16 XcpCrc16Ccitt(const uint16* pu1data, uint16 u2Crc)
{
    uint8 CrCCounter;
    uint16 t_u2_ccitt16 = (uint16)0x1021u;
    /* PRQA S 1338 ++ */ /* MISRA Rule 17.8 */
    u2Crc ^= ((uint16)(*pu1data) << 8u);
    for (CrCCounter = 0u; CrCCounter < 8u; CrCCounter++)
    {
        if ((u2Crc & (uint16)0x8000u) == (uint16)0x8000u)
        {
            u2Crc <<= 1;
            u2Crc ^= t_u2_ccitt16;
        }
        else
        {
            u2Crc <<= 1;
        }
    }
    /* PRQA S 1338 -- */ /* MISRA Rule 17.8 */
    return u2Crc;
}

/* Init the Xcp_Crc16CittTable */
void Xcp_InitCrc16CcittTable(void)
{
    uint16 i;
    for (i = 0u; i < 256u; i++)
    {
        Xcp_Crc16CittTable[i] = XcpCrc16Ccitt(&i, 0);
    }
}
/* PRQA S 3673 ++ */ /* MISRA Rule 17.8 */
static uint16 Xcp_GetCrc16Citt(uint8* data, uint16 crc)
{
    /* PRQA S 1338 ++ */ /* MISRA Rule 17.8 */
    uint16 c;
    c = crc >> 8;
    crc <<= 8;
    crc ^= Xcp_Crc16CittTable[(*data) ^ c];
    return crc;
    /* PRQA S 1338 -- */ /* MISRA Rule 17.8 */
}
/* PRQA S 3673 -- */ /* MISRA Rule 17.8 */

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, XCP_CODE) Xcp_ChecksumCompute(const uint32 checksumStartAddress, const uint32 blockSize, uint32* crcResult)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint32 i;
    uint16 t_u2Result;
    t_u2Result = (uint16)0xffffu;

    for (i = 0; i < blockSize; i++)
    {
        /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
        t_u2Result = Xcp_GetCrc16Citt((uint8*)(checksumStartAddress + i), t_u2Result);
        /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    }
    *crcResult = (uint32)t_u2Result;

    return;
}
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#endif /*STD_ON == XCP_BUILD_CHECKSUM*/

#if (STD_ON == XCP_PAG_SUPPORT)
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
void Xcp_InitCalMem(void)
{
    /*Overlay or MMU init.*/
    return;
}
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#endif /*STD_ON == XCP_PAG_SUPPORT*/
