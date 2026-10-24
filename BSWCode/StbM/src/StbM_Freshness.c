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
 **********************************************************************************************************************
 **
 **  @file               : StbM_Freshness.c
 **  @author             : xiaojian.liang
 **  @date               : 2026/01/20
 **  @vendor             : isoft
 **  @description        : Synchronized TimeBase Manager Freshness Source File
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "StbM_Freshness.h"

/* PRQA S 0553 EOF */ /* VL_StbM_0553 */
#if STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE
/* ===================================================== macros ===================================================== */
#define STBM_OCTET_BIT (8U)

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_FreshnessTruncData(
    const uint8*  freshnessValue,
    const uint32* freshnessValueLength,
    uint8*        truncatedFreshnessValue,
    uint32*       truncatedFreshnessValueLength);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
STBM_LOCAL void StbM_FreshnessTruncData(
    const uint8*  freshnessValue,
    const uint32* freshnessValueLength,
    uint8*        truncatedFreshnessValue,
    uint32*       truncatedFreshnessValueLength)
{
    /* SWS_SecOC_00220 All unused bits in StbMFreshnessValue are already set to 0. */
    if (*freshnessValueLength < *truncatedFreshnessValueLength)
    {
        *truncatedFreshnessValueLength = *freshnessValueLength;
    }

    /*           initial value of freshnessValueByteIndex is 1 (start is 0)
     *                                           |
     * StbMFreshnessValue 0xB3BEA0  10110011 10111110 101-----  StbMFreshnessValueLength 19
     *                                        0111110 101-----  StbMTruncatedFreshnessValueLength 10
     *                                       |
     *                                   bitOffset is 1 (start is 1)
     * StbMTruncatedFreshnessValue = StbMFreshnessValue[freshnessValueByteIndex:] << bitOffset
     *                    0x7D40    01111101 01------
     *                                           |
     *                                     unused bits are 0
     */
    uint8  bitOffset = (uint8)((*freshnessValueLength - *truncatedFreshnessValueLength) % STBM_OCTET_BIT);
    uint32 freshnessValueByteLength = (*freshnessValueLength + (STBM_OCTET_BIT - 1u)) / STBM_OCTET_BIT;
    uint32 truncatedFreshnessValueByteLength =
        (*truncatedFreshnessValueLength + (STBM_OCTET_BIT - 1u)) / STBM_OCTET_BIT;

    /* truncatedFreshnessValue start byte index of freshnessValue */
    uint32 freshnessValueByteIndex          = (*freshnessValueLength - *truncatedFreshnessValueLength) / STBM_OCTET_BIT;
    uint32 truncatedFreshnessValueByteIndex = 0u;

    /* if *truncatedFreshnessValueLength is 0, do not truncate freshnessValue */
    while (truncatedFreshnessValueByteIndex < truncatedFreshnessValueByteLength)
    {
        uint8 temp = (uint8)(freshnessValue[freshnessValueByteIndex] << bitOffset);
        if ((freshnessValueByteIndex + 1u) < freshnessValueByteLength)
        {
            temp |= (uint8)(freshnessValue[freshnessValueByteIndex + 1u] >> (STBM_OCTET_BIT - bitOffset));
        }
        truncatedFreshnessValue[truncatedFreshnessValueByteIndex] = temp;
        freshnessValueByteIndex++;
        truncatedFreshnessValueByteIndex++;
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6060, 2889 ++ */ /* VL_MTR_StbM_STM19, VL_QAC_MultiReturn */
const StbM_FreshnessValueType* StbM_FreshnessGetConfig(uint16 freshnessValueId)
{
    const StbM_FreshnessValueType* freshnessValue = StbM_FreshnessValueInformation.FreshnessValue;
    for (uint16 index = 0u; index < STBM_FRESHNESS_VALUE_NUM; index++)
    {
        if (freshnessValue[index].FreshnessValueId == freshnessValueId)
        {
            return &freshnessValue[index];
        }
    }
    return NULL_PTR;
}
/* PRQA S 6060, 2889 -- */
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_FreshnessGetTxData(uint16 freshnessValueId, uint8* freshnessValue, uint32* freshnessValueLength)
{
    Std_ReturnType stdReturn = E_NOT_OK;

    if (StbM_FreshnessValueInformation.GetTxFreshnessFct != NULL_PTR)
    {
        stdReturn =
            StbM_FreshnessValueInformation.GetTxFreshnessFct(freshnessValueId, freshnessValue, freshnessValueLength);
    }
    return stdReturn;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_FreshnessGetTxTruncData(
    uint16  freshnessValueId,
    uint8*  freshnessValue,
    uint32* freshnessValueLength,
    uint8*  truncatedFreshnessValue,
    uint32* truncatedFreshnessValueLength)
{
    Std_ReturnType stdReturn = E_NOT_OK;

    if (StbM_FreshnessGetConfig(freshnessValueId)->ProvideTxTruncatedFreshnessValue)
    {
        if (StbM_FreshnessValueInformation.GetTxFreshnessTruncDataFct != NULL_PTR)
        {
            stdReturn = StbM_FreshnessValueInformation.GetTxFreshnessTruncDataFct(
                freshnessValueId,
                freshnessValue,
                freshnessValueLength,
                truncatedFreshnessValue,
                truncatedFreshnessValueLength);
        }
    }
    else
    {
        if (StbM_FreshnessValueInformation.GetTxFreshnessFct != NULL_PTR)
        {
            /* SWS_SecOC_00220 All unused bits in StbMFreshnessValue are already set to 0. */
            stdReturn = StbM_FreshnessValueInformation.GetTxFreshnessFct(
                freshnessValueId,
                freshnessValue,
                freshnessValueLength);
            if (stdReturn == E_OK)
            {
                StbM_FreshnessTruncData(
                    freshnessValue,
                    freshnessValueLength,
                    truncatedFreshnessValue,
                    truncatedFreshnessValueLength);
            }
        }
    }
    return stdReturn;
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
void StbM_FreshnessSPduTxConfirmation(uint16 freshnessValueId)
{
    if (StbM_FreshnessValueInformation.SPduTxConfirmationFct != NULL_PTR)
    {
        StbM_FreshnessValueInformation.SPduTxConfirmationFct(freshnessValueId);
    }
}
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
/* PRQA S 6040 ++ */ /* VL_MTR_StbM_STPAR */
Std_ReturnType StbM_FreshnessGetRxData(
    uint16       freshnessValueId,
    const uint8* truncatedFreshnessValue,
    uint32       truncatedFreshnessValueLength,
    uint16       authVerifyAttempts,
    uint8*       freshnessValue,
    uint32*      freshnessValueLength)
{
    Std_ReturnType stdReturn = E_NOT_OK;

    if (StbM_FreshnessValueInformation.GetRxFreshnessFct != NULL_PTR)
    {
        stdReturn = StbM_FreshnessValueInformation.GetRxFreshnessFct(
            freshnessValueId,
            truncatedFreshnessValue,
            truncatedFreshnessValueLength,
            authVerifyAttempts,
            freshnessValue,
            freshnessValueLength);
    }
    return stdReturn;
}
/* PRQA S 6040 -- */
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif /* STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE */

/* ========================================== internal function definitions ========================================= */
