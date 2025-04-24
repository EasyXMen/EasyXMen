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
**  FILENAME    : CanTrcv_TLE9255W.c                                          **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation by CanTrcv_TLE9255W module  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTrcv.h"
#include "CanTrcv_TLE9255W.h"
#include "Det.h"
#if (Timer_1us16bit == CANTRCV_TIMER_TYPE)
#include "Tm.h"
#else
#include "Mcal_Compiler.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CANTRCV_PN(Transceiver)                    CanTrcv_ConfigPtr->CanTrcvChannel[Transceiver].CanTrcvPartialNetwork
#define CANTRCV_SPI_CHANNEL(Transceiver)           CanTrcv_ConfigPtr->CanTrcvChannel[Transceiver].CanTrcvSpiChannel
#define CANTRCV_SPI_SEQUENCE(Transceiver)          CanTrcv_ConfigPtr->CanTrcvChannel[Transceiver].CanTrcvSpiSequence
#define CANTRCV_CHANNEL(Transceiver)               CanTrcv_ConfigPtr->CanTrcvChannel[Transceiver]
#define CANTRCV_TLE9255W_MKAD(AccessMode, Address) ((AccessMode) << 7u) | (Address)
#define CANTRCV_TLE9255W_SPI_BUFFER_SIZE           58u

#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
#define CANTRCV_DET_REPORT(ApiId, ErrorId) \
    ((void)Det_ReportError(CANTRCV_MODULE_ID, CANTRCV_INSTANCE_ID, (ApiId), (ErrorId)))
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */

/* SPI Registers */
/* buffer Index */
#define MC_REG_ADDR_INDEX           0u
#define MC_REG_DATA_INDEX           1u
#define HC_REG_ADDR_INDEX           2u
#define HC_REG_DATA_INDEX           3u
#define TTC_REG_ADDR_INDEX          4u
#define TTC_REG_DATA_INDEX          5u
#define SC_REG_ADDR_INDEX           6u
#define SC_REG_DATA_INDEX           7u
#define SWK1_REG_ADDR_INDEX         8u
#define SWK1_REG_DATA_INDEX         9u
#define SWK2_REG_ADDR_INDEX         10u
#define SWK2_REG_DATA_INDEX         11u
#define SWK_ID3_REG_ADDR_INDEX      12u
#define SWK_ID3_REG_DATA_INDEX      13u
#define SWK_ID2_REG_ADDR_INDEX      14u
#define SWK_ID2_REG_DATA_INDEX      15u
#define SWK_ID1_REG_ADDR_INDEX      16u
#define SWK_ID1_REG_DATA_INDEX      17u
#define SWK_ID0_REG_ADDR_INDEX      18u
#define SWK_ID0_REG_DATA_INDEX      19u
#define SWK_MASK_ID3_REG_ADDR_INDEX 20u
#define SWK_MASK_ID3_REG_DATA_INDEX 21u
#define SWK_MASK_ID2_REG_ADDR_INDEX 22u
#define SWK_MASK_ID2_REG_DATA_INDEX 23u
#define SWK_MASK_ID1_REG_ADDR_INDEX 24u
#define SWK_MASK_ID1_REG_DATA_INDEX 25u
#define SWK_MASK_ID0_REG_ADDR_INDEX 26u
#define SWK_MASK_ID0_REG_DATA_INDEX 27u
#define SWK_DLC_REG_ADDR_INDEX      28u
#define SWK_DLC_REG_DATA_INDEX      29u
#define SWK_DATA7_REG_ADDR_INDEX    30u
#define SWK_DATA7_REG_DATA_INDEX    31u
#define SWK_DATA6_REG_ADDR_INDEX    32u
#define SWK_DATA6_REG_DATA_INDEX    33u
#define SWK_DATA5_REG_ADDR_INDEX    34u
#define SWK_DATA5_REG_DATA_INDEX    35u
#define SWK_DATA4_REG_ADDR_INDEX    36u
#define SWK_DATA4_REG_DATA_INDEX    37u
#define SWK_DATA3_REG_ADDR_INDEX    38u
#define SWK_DATA3_REG_DATA_INDEX    39u
#define SWK_DATA2_REG_ADDR_INDEX    40u
#define SWK_DATA2_REG_DATA_INDEX    41u
#define SWK_DATA1_REG_ADDR_INDEX    42u
#define SWK_DATA1_REG_DATA_INDEX    43u
#define SWK_DATA0_REG_ADDR_INDEX    44u
#define SWK_DATA0_REG_DATA_INDEX    45u
#define TS_REG_ADDR_INDEX           46u
#define TS_REG_DATA_INDEX           47u
#define TUVS_REG_ADDR_INDEX         48u
#define TUVS_REG_DATA_INDEX         49u
#define ERRS_REG_ADDR_INDEX         50u
#define ERRS_REG_DATA_INDEX         51u
#define WS_REG_ADDR_INDEX           52u
#define WS_REG_DATA_INDEX           53u
#define SWKS_REG_ADDR_INDEX         54u
#define SWKS_REG_DATA_INDEX         55u
#define SWKES_REG_ADDR_INDEX        56u
#define SWKES_REG_DATA_INDEX        57u

/* Actual Addresses */
#define MODE_CTRL_ADDR         0x01u
#define HW_CTRL_ADDR           0x02u
#define TXD_TO_CTRL_ADDR       0x03u
#define SUPPLY_CTRL_ADDR       0x04u
#define SWK_CTRL_1_ADDR        0x05u
#define SWK_CTRL_2_ADDR        0x06u
#define SWK_ID3_CTRL_ADDR      0x07u
#define SWK_ID2_CTRL_ADDR      0x08u
#define SWK_ID1_CTRL_ADDR      0x09u
#define SWK_ID0_CTRL_ADDR      0x0Au
#define SWK_MASK_ID3_CTRL_ADDR 0x0Bu
#define SWK_MASK_ID2_CTRL_ADDR 0x0Cu
#define SWK_MASK_ID1_CTRL_ADDR 0x0Du
#define SWK_MASK_ID0_CTRL_ADDR 0x0Eu
#define SWK_DLC_CTRL_ADDR      0x0Fu
#define SWK_DATA7_CTRL_ADDR    0x10u
#define SWK_DATA6_CTRL_ADDR    0x11u
#define SWK_DATA5_CTRL_ADDR    0x12u
#define SWK_DATA4_CTRL_ADDR    0x13u
#define SWK_DATA3_CTRL_ADDR    0x14u
#define SWK_DATA2_CTRL_ADDR    0x15u
#define SWK_DATA1_CTRL_ADDR    0x16u
#define SWK_DATA0_CTRL_ADDR    0x17u
#define TRANS_STAT_ADDR        0x18u
#define TRANS_UV_STAT_ADDR     0x19u
#define ERR_STAT_ADDR          0x1Au
#define WAKE_STAT_ADDR         0x1Bu
#define SWK_STAT_ADDR          0x1Cu
#define SWK_ECNT_STAT_ADDR     0x1Du

/* Specific Register Masks */
#define MODE_CTRL_MODE                 0x0Fu
#define HW_CTRL_STTS_EN                0x80u
#define HW_CTRL_LWU_NEG                0x40u
#define HW_CTRL_LWU_POS                0x20u
#define HW_CTRL_WAKE_TOG               0x02u
#define HW_CTRL_VBAT_CON               0x01u
#define TXD_TO_CTRL_TXD_TO             0x07u
#define SUPPLY_CTRL_VIO_UV_T           0xF0u
#define SUPPLY_CTRL_VCC_UV_T           0x0Fu
#define SWK_CTRL_1_CFG_VAL             0x01u
#define SWK_CTRL_2_SWK_EN              0x80u
#define SWK_CTRL_2_BR_RATIO            0x30u
#define SWK_CTRL_2_BR                  0x07u
#define SWK_ID3_CTRL_IDE               0x20u
#define SWK_ID3_CTRL_ID28_24           0x1Fu
#define SWK_ID2_CTRL_ID23_16           0xFFu
#define SWK_ID1_CTRL_ID15_8            0xFFu
#define SWK_ID0_CTRL_ID7_0             0xFFu
#define SWK_MASK_ID3_CTRL_MASK_ID28_24 0x1Fu
#define SWK_MASK_ID2_CTRL_MASK_ID23_16 0xFFu
#define SWK_MASK_ID1_CTRL_MASK_ID15_8  0xFFu
#define SWK_MASK_ID0_CTRL_MASK_ID7_0   0xFFu
#define SWK_DLC_CTRL_DLC               0x0Fu
#define SWK_DATA7_CTRL_DATA7           0xFFu
#define SWK_DATA6_CTRL_DATA6           0xFFu
#define SWK_DATA5_CTRL_DATA5           0xFFu
#define SWK_DATA4_CTRL_DATA4           0xFFu
#define SWK_DATA3_CTRL_DATA3           0xFFu
#define SWK_DATA2_CTRL_DATA2           0xFFu
#define SWK_DATA1_CTRL_DATA1           0xFFu
#define SWK_DATA0_CTRL_DATA0           0xFFu
#define TRANS_STAT_POR                 0x80u
#define TRANS_STAT_TXD_TO              0x04u
#define TRANS_STAT_TSD                 0x02u
#define TRANS_UV_STAT_VBAT_UV          0x80u
#define TRANS_UV_STAT_VCC_LTUV         0x20u
#define TRANS_UV_STAT_VCC_STUV         0x10u
#define TRANS_UV_STAT_VIO_LTUV         0x02u
#define TRANS_UV_STAT_VIO_STUV         0x01u
#define ERR_STAT_COM_ERR               0x02u
#define ERR_STAT_CMD_ERR               0x01u
#define WAKE_STAT_LWU_DIR              0x08u
#define WAKE_STAT_LWU                  0x04u
#define WAKE_STAT_WUP                  0x02u
#define WAKE_STAT_WUF                  0x01u
#define SWK_STAT_SYSERR                0x10u
#define SWK_STAT_SYNC                  0x08u
#define SWK_STAT_CANTO                 0x04u
#define SWK_STAT_CANSIL                0x02u
#define SWK_STAT_SWK_ACTIVE            0x01u
#define SWK_ECNT_STAT_ECNT             0x3Fu

#define CANTRCV_TLE9255W_WRITE_REG     1u
#define CANTRCV_TLE9255W_READ_REG      0u

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef enum CanTrcv_TLE9255W_WorkModeType_E
{
    CANTRCV_TLE9255W_SLEEPMODE = 0x01u,
    CANTRCV_TLE9255W_STANDBYMODE = 0x02u,
    CANTRCV_TLE9255W_RECEIVEONLYMODE = 0x04u,
    CANTRCV_TLE9255W_NORMALMODE = 0x08u,
} CanTrcv_TLE9255W_WorkModeType;
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_SpiProcess(uint8 Transceiver, uint8 addressIndex, uint8 address, uint8 dataIndex, uint8 data);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(void, CANTRCV_CODE) CanTrcv_TLE9255W_ProcessSpiAddress(
    CanTrcv_TLE9255W_StatusType StatusType,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) addrIndex,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) dataIndex,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) actualAddr,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) dataMask);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_ModeChangeProcess(uint8 Transceiver, CanTrcv_TLE9255W_WorkModeType workMode);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ClearAllStatus(uint8 Transceiver);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
********************************************************************************/
/* SPI Input Buffer */
#define CANTRCV_START_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"
static VAR(Spi_DataBufferType, CANTRCV_VAR_NOINIT)
    CanTrcv_TLE9255W_SpiInBuffer[CANTRCV_MAX_CHANNELS][CANTRCV_TLE9255W_SPI_BUFFER_SIZE];
#define CANTRCV_STOP_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"

/* SPI Output Buffer */
#define CANTRCV_START_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"
static VAR(Spi_DataBufferType, CANTRCV_VAR_NOINIT)
    CanTrcv_TLE9255W_SpiOutBuffer[CANTRCV_MAX_CHANNELS][CANTRCV_TLE9255W_SPI_BUFFER_SIZE];
#define CANTRCV_STOP_SEC_VAR_NO_INIT_8
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/**
 * This function enable the CanTrcv HW wakeup functionalities.
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_EnableWakeup(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    if ((PN_ENABLED == CanTrcv_PNActivation) && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed))
    {
        result = CanTrcv_TLE9255W_PNWKSetup(Transceiver);
    }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

    if ((TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByPinUsed))
    {
        result += CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            HC_REG_ADDR_INDEX,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, HW_CTRL_ADDR),
            HC_REG_DATA_INDEX,
            0xE1);
    }
    else if (
        (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
        && (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByPinUsed))
    {
        result += CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            HC_REG_ADDR_INDEX,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, HW_CTRL_ADDR),
            HC_REG_DATA_INDEX,
            0x81);
    }
    else if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByPinUsed)
    {
        result += CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            HC_REG_ADDR_INDEX,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, HW_CTRL_ADDR),
            HC_REG_DATA_INDEX,
            0x81);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function disables wakeup functionality
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_DisableWakeup(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    result = CanTrcv_TLE9255W_DisablePNWK(Transceiver);
#endif

    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        HC_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, HW_CTRL_ADDR),
        HC_REG_DATA_INDEX,
        0x81);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function reads from status registers
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
                   StatusType: request type
 * Parameters(INOUT): NA
 * Parameters(OUT): FlagState: the requested flag state
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_TLE9255W_ReadStatus(
    uint8 Transceiver,
    CanTrcv_TLE9255W_StatusType StatusType,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState)
{
    Std_ReturnType result = E_NOT_OK;
    uint8 addrIndex;
    uint8 dataIndex;
    uint8 actualAddr;
    uint8 dataMask;

    if (CANTRCV_TLE9255W_WK != StatusType)
    {
        CanTrcv_TLE9255W_ProcessSpiAddress(StatusType, &addrIndex, &dataIndex, &actualAddr, &dataMask);

        result = CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            addrIndex,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, actualAddr),
            dataIndex,
            0);
        if (dataMask == (CanTrcv_TLE9255W_SpiInBuffer[Transceiver][dataIndex] & dataMask))
        {
            *FlagState = CANTRCV_FLAG_SET;
        }
        else
        {
            *FlagState = CANTRCV_FLAG_CLEARED;
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears specific status register
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
                   StatusType: request type
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_TLE9255W_ClearStatus(uint8 Transceiver, CanTrcv_TLE9255W_StatusType StatusType)
{
    Std_ReturnType result = E_NOT_OK;
    uint8 addrIndex;
    uint8 dataIndex;
    uint8 actualAddr;
    uint8 dataMask;

    if ((CANTRCV_TLE9255W_CANSIL != StatusType) && (CANTRCV_TLE9255W_SYSERR != StatusType))
    {
        CanTrcv_TLE9255W_ProcessSpiAddress(StatusType, &addrIndex, &dataIndex, &actualAddr, &dataMask);

        result = CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            addrIndex,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, actualAddr),
            dataIndex,
            dataMask);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function changes HW to Sleep Mode
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetSleepMode(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    if (PN_ENABLED == CanTrcv_PNActivation)
    {
        result = CanTrcv_TLE9255W_PNWKSetup(Transceiver);
    }
#endif

    if (E_OK == result)
    {
        result = CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_WK);
    }

    if (E_OK == result)
    {
        result = CanTrcv_TLE9255W_ModeChangeProcess(Transceiver, CANTRCV_TLE9255W_SLEEPMODE);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function changes HW to Standby Mode
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetStandbyMode(uint8 Transceiver)
{
    Std_ReturnType result;

    result = CanTrcv_TLE9255W_ClearStatus(Transceiver, CANTRCV_TLE9255W_WK);

    if (E_OK == result)
    {
        CanTrcv_TLE9255W_ModeChangeProcess(Transceiver, CANTRCV_TLE9255W_STANDBYMODE);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function changes HW to Normal Mode
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetNormalMode(uint8 Transceiver)
{
    Std_ReturnType result;

    result = CanTrcv_TLE9255W_ModeChangeProcess(Transceiver, CANTRCV_TLE9255W_NORMALMODE);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets the current operation mode of HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): OpMode: current aquired opperation mode
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_TLE9255W_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode)
{
    Std_ReturnType result;
    uint8 workMode;

    result = CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        MC_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, MODE_CTRL_ADDR),
        MC_REG_DATA_INDEX,
        0);

    workMode = CanTrcv_TLE9255W_SpiInBuffer[Transceiver][MC_REG_DATA_INDEX] & MODE_CTRL_MODE;
    if (CANTRCV_TLE9255W_SLEEPMODE == workMode)
    {
        *OpMode = CANTRCV_TRCVMODE_SLEEP;
    }
    else if (CANTRCV_TLE9255W_STANDBYMODE == workMode)
    {
        *OpMode = CANTRCV_TRCVMODE_STANDBY;
    }
    else if (CANTRCV_TLE9255W_NORMALMODE == workMode)
    {
        *OpMode = CANTRCV_TRCVMODE_NORMAL;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function deinitialize the HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_DeInit(uint8 Transceiver)
{
    Std_ReturnType result;

    result = CanTrcv_TLE9255W_ClearAllStatus(Transceiver);
    result += CanTrcv_TLE9255W_SetStandbyMode(Transceiver);
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    result += CanTrcv_TLE9255W_DisablePNWK(Transceiver);
#endif
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        HC_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, HW_CTRL_ADDR),
        HC_REG_DATA_INDEX,
        0x81);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
/**
 * This function disables Partial network wakeup
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_DisablePNWK(uint8 Transceiver)
{
    Std_ReturnType result;

    /* Disable SWK_EN in SWK_CTRL_2 */
    result = CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        SWK2_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_CTRL_2_ADDR),
        SWK2_REG_DATA_INDEX,
        0x04u);
    /* Clear CFG_VAL in SWK_CTRL_1 */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        SWK1_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_CTRL_1_ADDR),
        SWK1_REG_DATA_INDEX,
        0x00u);
    /* clear CANTO in SWK_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        SWKS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_STAT_ADDR),
        SWKS_REG_DATA_INDEX,
        SWK_STAT_CANTO);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets transceiver system data from HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvSysData: the required system data
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_TLE9255W_GetTrcvSystemData(uint8 Transceiver, P2VAR(uint32, AUTOMATIC, CANTRCV_APPL_DATA) TrcvSysData)
{
    Std_ReturnType result;

    /* Read from TRANS_STAT */
    result = CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        TS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, TRANS_STAT_ADDR),
        TS_REG_DATA_INDEX,
        0);

    /* Read from TRANS_UV_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        TUVS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, TRANS_UV_STAT_ADDR),
        TUVS_REG_DATA_INDEX,
        0);

    /* Read from SWK_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        SWKS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, SWK_STAT_ADDR),
        SWKS_REG_DATA_INDEX,
        0);

    /* Read from SWK_ECNT_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        SWKES_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, SWK_ECNT_STAT_ADDR),
        SWKES_REG_DATA_INDEX,
        0);

    *TrcvSysData = (uint32)CanTrcv_TLE9255W_SpiInBuffer[Transceiver][TS_REG_DATA_INDEX];
    *TrcvSysData = ((uint32)CanTrcv_TLE9255W_SpiInBuffer[Transceiver][TUVS_REG_DATA_INDEX]) << 8;
    *TrcvSysData = ((uint32)CanTrcv_TLE9255W_SpiInBuffer[Transceiver][SWKS_REG_DATA_INDEX]) << 16;
    *TrcvSysData = ((uint32)CanTrcv_TLE9255W_SpiInBuffer[Transceiver][SWKES_REG_DATA_INDEX]) << 24;

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function setup PN wakeup functionalities
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_PNWKSetup(uint8 Transceiver)
{
    uint8 baudrate;
    uint8 dataIndex;
    Std_ReturnType result = E_OK;
    CanTrcv_TrcvFlagStateType isSyserrSet;
    CanTrcv_TrcvFlagStateType isCfgConfirm;
    CanTrcv_TrcvFlagStateType isSWKEnabled;

    if (NULL_PTR != CANTRCV_PN(Transceiver))
    {
        if (125u == CANTRCV_PN(Transceiver)->CanTrcvBaudRate)
        {
            baudrate = 0x02;
        }
        else if (250u == CANTRCV_PN(Transceiver)->CanTrcvBaudRate)
        {
            baudrate = 0x03;
        }
        else if (500u == CANTRCV_PN(Transceiver)->CanTrcvBaudRate)
        {
            baudrate = 0x04;
        }
        else if (1024u == CANTRCV_PN(Transceiver)->CanTrcvBaudRate)
        {
            baudrate = 0x05;
        }
        else
        {
#if (STD_ON == CANTRCV_DEV_ERROR_DETECT)
            CANTRCV_DET_REPORT(CANTRCV_INIT_ID, CANTRCV_E_BAUDRATE_NOT_SUPPORTED);
#endif /* STD_ON == CANTRCV_DEV_ERROR_DETECT */
            result = E_NOT_OK;
        }

        if (E_OK == result)
        {
            /* Set Baudrate and SWK_EN at SWK_CTRL_2 */
            result = CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK2_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_CTRL_2_ADDR),
                SWK2_REG_DATA_INDEX,
                baudrate | SWK_CTRL_2_SWK_EN);

            /* Set WUF CAN Id */
            /* normal */
            if (FALSE == CANTRCV_PN(Transceiver)->CanTrcvPnCanIdIsExtended)
            {
                /* Id0-5 */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID2_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID2_CTRL_ADDR),
                    SWK_ID2_REG_DATA_INDEX,
                    ((uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanId << 2u)));

                /* Id6-10 */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID3_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID3_CTRL_ADDR),
                    SWK_ID3_REG_DATA_INDEX,
                    ((uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanId >> 6u)));
            }
            /* extended */
            else
            {
                /* IDE */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID3_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID3_CTRL_ADDR),
                    SWK_ID3_REG_DATA_INDEX,
                    SWK_ID3_CTRL_IDE);
                /* Id 0-7 */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID0_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID0_CTRL_ADDR),
                    SWK_ID0_REG_DATA_INDEX,
                    (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanId));
                /* Id 8-15 */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID1_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID1_CTRL_ADDR),
                    SWK_ID1_REG_DATA_INDEX,
                    (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanId >> 8u));
                /* Id 16-23 */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID2_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID2_CTRL_ADDR),
                    SWK_ID2_REG_DATA_INDEX,
                    (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanId >> 16u));
                /* Id 24-28 */
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_ID3_REG_ADDR_INDEX,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_ID3_CTRL_ADDR),
                    SWK_ID3_REG_DATA_INDEX,
                    SWK_ID3_CTRL_ID28_24 & (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanId >> 24u));
            }

            /* Set WUF CAN Id Mask */
            /* Id Mask 0-7 */
            result += CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK_MASK_ID0_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_MASK_ID0_CTRL_ADDR),
                SWK_MASK_ID0_REG_DATA_INDEX,
                (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanIdMask));
            /* Id Mask 8-15 */
            result += CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK_MASK_ID1_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_MASK_ID1_CTRL_ADDR),
                SWK_MASK_ID1_REG_DATA_INDEX,
                (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanIdMask >> 8u));
            /* Id Mask 16-23 */
            result += CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK_MASK_ID2_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_MASK_ID2_CTRL_ADDR),
                SWK_MASK_ID2_REG_DATA_INDEX,
                (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanIdMask >> 16u));
            /* Id Mask 24-28 */
            result += CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK_MASK_ID3_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_MASK_ID3_CTRL_ADDR),
                SWK_MASK_ID3_REG_DATA_INDEX,
                SWK_MASK_ID3_CTRL_MASK_ID28_24 & (uint8)(CANTRCV_PN(Transceiver)->CanTrcvPnFrameCanIdMask >> 24u));

            /* Set DLC */
            result += CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK_DLC_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_DLC_CTRL_ADDR),
                SWK_DLC_REG_DATA_INDEX,
                SWK_DLC_CTRL_DLC & CANTRCV_PN(Transceiver)->CanTrcvPnFrameDlc);

            /* Set Data */
            for (dataIndex = 0u; dataIndex < CANTRCV_PN(Transceiver)->CanTrcvPnFrameDlc; dataIndex++)
            {
                result += CanTrcv_TLE9255W_SpiProcess(
                    Transceiver,
                    SWK_DATA0_REG_ADDR_INDEX - 2 * dataIndex,
                    CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_DATA0_CTRL_ADDR - dataIndex),
                    SWK_DATA0_REG_DATA_INDEX - 2 * dataIndex,
                    CANTRCV_PN(Transceiver)->CanTrcvPnFrameDataMaskSpec[dataIndex].CanTrcvPnFrameDataMask);
            }

            /* Confirm the SWK Configuration */
            result += CanTrcv_TLE9255W_SpiProcess(
                Transceiver,
                SWK1_REG_ADDR_INDEX,
                CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_CTRL_1_ADDR),
                SWK1_REG_DATA_INDEX,
                SWK_CTRL_1_CFG_VAL);

            /* check SYSERR */
            result += CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_SYSERR, &isSyserrSet);
            result += CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_CFG_VAL, &isCfgConfirm);
            result += CanTrcv_TLE9255W_ReadStatus(Transceiver, CANTRCV_TLE9255W_SWK_EN, &isSWKEnabled);
            if ((CANTRCV_FLAG_SET == isSyserrSet) || (CANTRCV_FLAG_CLEARED == isCfgConfirm)
                || (CANTRCV_FLAG_CLEARED == isSWKEnabled))
            {
                /* Disable PNWK */
                (void)CanTrcv_TLE9255W_DisablePNWK(Transceiver);
                result = E_NOT_OK;
            }
        }
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /*  (STD_ON == CANTRCV_HWPN_SUPPORT) */

#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode)
{
    Std_ReturnType result = E_NOT_OK;

    switch (OpMode)
    {
    case CANTRCV_TRCVMODE_NORMAL:
        result = CanTrcv_TLE9255W_SetNormalMode(Transceiver);
        break;
    case CANTRCV_TRCVMODE_SLEEP:
        result = CanTrcv_TLE9255W_SetSleepMode(Transceiver);
        break;
    case CANTRCV_TRCVMODE_STANDBY:
        result = CanTrcv_TLE9255W_SetStandbyMode(Transceiver);
        break;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/**
 * This function process mode change for HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 *                 workMode: the requested workMode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_ModeChangeProcess(uint8 Transceiver, CanTrcv_TLE9255W_WorkModeType workMode)
{
#if (None == CANTRCV_TIMER_TYPE)
    uint16 waitTime;
#endif
    Std_ReturnType result;

    do
    {
        result = CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            MC_REG_ADDR_INDEX,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, MODE_CTRL_ADDR),
            MC_REG_DATA_INDEX,
            workMode);
#if (Timer_1us16bit == CANTRCV_TIMER_TYPE)
        result += Tm_BusyWait1us16bit(CANTRCV_WAIT_TIME);
#else
        for (waitTime = 0u; waitTime < CANTRCV_WAIT_TIME; waitTime++)
        {
            NOP();
        }
#endif
        result += CanTrcv_TLE9255W_SpiProcess(
            Transceiver,
            MC_REG_ADDR_INDEX,
            CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_READ_REG, MODE_CTRL_ADDR),
            MC_REG_DATA_INDEX,
            0);
    } while (workMode != (CanTrcv_TLE9255W_SpiInBuffer[Transceiver][MC_REG_DATA_INDEX] & MODE_CTRL_MODE));

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function process SPI requests
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 *                 addressIndex: the index of requsted address
 *                 address: the actul address
 *                 dataIndex: the index of requsted data
 * Parameters(INOUT): data: the actual data
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_SpiProcess(uint8 Transceiver, uint8 addressIndex, uint8 address, uint8 dataIndex, uint8 data)
{
    Std_ReturnType result;

    CanTrcv_TLE9255W_SpiOutBuffer[Transceiver][addressIndex] = address;
    CanTrcv_TLE9255W_SpiOutBuffer[Transceiver][dataIndex] = data;

    result = Spi_SetupEB(
        CANTRCV_SPI_CHANNEL(Transceiver),
        &CanTrcv_TLE9255W_SpiOutBuffer[Transceiver][addressIndex],
        &CanTrcv_TLE9255W_SpiInBuffer[Transceiver][addressIndex],
        2);

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvSpiAccessSynchronous)
    {
        /* Asynchronous transmission */
        /* result = Spi_AsyncTransmit(CANTRCV_SPI_SEQUENCE(Transceiver)); */
    }
    else
    {
        /* Using an application specific implementation */
        result = Spi_SyncTransmit(CANTRCV_SPI_SEQUENCE(Transceiver));
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function outputs requsted address and data infos based on requested StatusType
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): StatusType: the requsted status type
 * Parameters(INOUT):
 * Parameters(OUT): addrIndex: the index of requsted address
 *                  dataIndex: the index of requsted data
 *                  actualAddr: the actul address
 *                  dataMask: the specific required data mask
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(void, CANTRCV_CODE) CanTrcv_TLE9255W_ProcessSpiAddress(
    CanTrcv_TLE9255W_StatusType StatusType,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) addrIndex,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) dataIndex,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) actualAddr,
    P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) dataMask)
{
    switch (StatusType)
    {
    case CANTRCV_TLE9255W_POR:
        *addrIndex = TS_REG_ADDR_INDEX;
        *dataIndex = TS_REG_DATA_INDEX;
        *actualAddr = TRANS_STAT_ADDR;
        *dataMask = TRANS_STAT_POR;
        break;
    case CANTRCV_TLE9255W_SYSERR:
        *addrIndex = SWKS_REG_ADDR_INDEX;
        *dataIndex = SWKS_REG_DATA_INDEX;
        *actualAddr = SWK_STAT_ADDR;
        *dataMask = SWK_STAT_SYSERR;
        break;
    case CANTRCV_TLE9255W_WK:
        *addrIndex = WS_REG_ADDR_INDEX;
        *dataIndex = WS_REG_DATA_INDEX;
        *actualAddr = WAKE_STAT_ADDR;
        *dataMask = WAKE_STAT_LWU | WAKE_STAT_WUP | WAKE_STAT_WUF;
        break;
    case CANTRCV_TLE9255W_LWU:
        *addrIndex = WS_REG_ADDR_INDEX;
        *dataIndex = WS_REG_DATA_INDEX;
        *actualAddr = WAKE_STAT_ADDR;
        *dataMask = WAKE_STAT_LWU;
        break;
    case CANTRCV_TLE9255W_WUF:
        *addrIndex = WS_REG_ADDR_INDEX;
        *dataIndex = WS_REG_DATA_INDEX;
        *actualAddr = WAKE_STAT_ADDR;
        *dataMask = WAKE_STAT_WUF;
        break;
    case CANTRCV_TLE9255W_WUP:
        *addrIndex = WS_REG_ADDR_INDEX;
        *dataIndex = WS_REG_DATA_INDEX;
        *actualAddr = WAKE_STAT_ADDR;
        *dataMask = WAKE_STAT_WUP;
        break;
    case CANTRCV_TLE9255W_CANTO:
        *addrIndex = SWKS_REG_ADDR_INDEX;
        *dataIndex = SWKS_REG_DATA_INDEX;
        *actualAddr = SWK_STAT_ADDR;
        *dataMask = SWK_STAT_CANTO;
        break;
    case CANTRCV_TLE9255W_CANSIL:
        *addrIndex = SWKS_REG_ADDR_INDEX;
        *dataIndex = SWKS_REG_DATA_INDEX;
        *actualAddr = SWK_STAT_ADDR;
        *dataMask = SWK_STAT_CANSIL;
        break;
    case CANTRCV_TLE9255W_CFG_VAL:
        *addrIndex = SWK1_REG_ADDR_INDEX;
        *dataIndex = SWK1_REG_DATA_INDEX;
        *actualAddr = SWK_CTRL_1_ADDR;
        *dataMask = SWK_CTRL_1_CFG_VAL;
        break;
    case CANTRCV_TLE9255W_SWK_EN:
        *addrIndex = SWK2_REG_ADDR_INDEX;
        *dataIndex = SWK2_REG_DATA_INDEX;
        *actualAddr = SWK_CTRL_2_ADDR;
        *dataMask = SWK_CTRL_2_SWK_EN;
        break;
    case CANTRCV_TLE9255W_TXD_TO:
        *addrIndex = TS_REG_ADDR_INDEX;
        *dataIndex = TS_REG_DATA_INDEX;
        *actualAddr = TRANS_STAT_ADDR;
        *dataMask = TRANS_STAT_TXD_TO;
        break;
    default:
        break;
    }
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears all status in HW
 * Service ID: None
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: the specific transceiver the function applies to
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: the request to HW succeeded
 *               E_NOT_OK: he request to HW failed
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
static FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ClearAllStatus(uint8 Transceiver)
{
    Std_ReturnType result;

    /* clear TRANS_STAT */
    result = CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        TS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, TRANS_STAT_ADDR),
        TS_REG_DATA_INDEX,
        0x86);

    /* clear TRANS_UV_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        TUVS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, TRANS_UV_STAT_ADDR),
        TUVS_REG_DATA_INDEX,
        0xB3);

    /* clear ERR_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        ERRS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, ERR_STAT_ADDR),
        ERRS_REG_DATA_INDEX,
        0x03);

    /* clear WAKE_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        WS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, WAKE_STAT_ADDR),
        WS_REG_DATA_INDEX,
        0x07);

    /* clear SWK_STAT */
    result += CanTrcv_TLE9255W_SpiProcess(
        Transceiver,
        SWKS_REG_ADDR_INDEX,
        CANTRCV_TLE9255W_MKAD(CANTRCV_TLE9255W_WRITE_REG, SWK_STAT_ADDR),
        SWKS_REG_DATA_INDEX,
        SWK_STAT_CANTO);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
