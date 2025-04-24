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
**  FILENAME    : CanTrcv_Tja1145.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation by CanTrcv driver Tja1145   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V2.0.0       2020/9/7   Xinrun.Wang     create
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTrcv_Tja1145.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CANTRCV_CHANNEL(Transceiver)    (CanTrcv_Config.CanTrcvChannel[(Transceiver)])
#define CANTRCV_SPI_ACCESS(Transceiver) (CanTrcv_Config.CanTrcvChannel[(Transceiver)].CanTrcvAccess.CanTrcvSpiAccess)

/* Build an address */
#define CANTRCV_TJA1145_MKADR(addr, pro) (uint8)((((addr) << 1) & 0xFEu) | (pro))

/* asychornous transmit */
#define Spi_Async_Transmit(seq) seq = 0;

/* Flags to handle any kind of requests */
#define CANTRCV_TJA1145_REQ_NONE                                                    \
    0x00000000u                                             /* No request pending   \
                                                                                  \ \
/* operation modes requests */
#define CANTRCV_TJA1145_REQ_OPMODEMASK          0x0000000Fu /* Mask to query operation mode requests  */
#define CANTRCV_TJA1145_REQ_OPMODENORMAL        0x00000001u /* Normal operation mode                  */
#define CANTRCV_TJA1145_REQ_OPMODESTANDBY       0x00000002u /* Standby mode                           */
#define CANTRCV_TJA1145_REQ_OPMODESLEEP         0x00000004u /* Sleep mode                             */
#define CANTRCV_TJA1145_REQ_OPMODENORMALPN      0x00000008u /* Special mode with Pin enabled in Normal */
#define CANTRCV_TJA1145_REQ_CLRWUFFLAG          0x00000010u /* Request to execute ClearTrcvWufFlag    */
#define CANTRCV_TJA1145_REQ_CHKWAKFLAG          0x00000020u /* Request to execute CheckWakeFlag       */
#define CANTRCV_TJA1145_REQ_CBBYBUSFLAG         0x00000040u /* Request to execute CB_WakeupByBus      */
#define CANTRCV_TJA1145_REQ_CONFIRMPNAVAIBILITY 0x00001000u /* Request to confirm Pin avaibility       */

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef struct CanTrcv_Tja1145_Prob_T
{
    /* Holds the last read statusFlags */
    CanTrcv_Tja1145_StatusFlagsType statusFlags;
    /* Request state of transceiver driver */
    uint32 requestState;
    /* Stores the last requested SPI command */
    uint8 CanTrcvSpiReqType;
} CanTrcv_Tja1145_Prob_Type;

typedef struct CanTrcv_Tja1145_StatusFlags_T
{
    uint8 Ts;  /* Transceiver status register */
    uint8 Ecs; /* Event capture status register */
    uint8 Ses; /* System event capture status register */
    uint8 Tes; /* Transceiver event capture status register */
    uint8 Wes; /* Wake pin event capture status register */
} CanTrcv_Tja1145_StatusFlagsType;

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/* SPI Input Buffer */
#define CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"
VAR(Spi_DataType, CANTRCV_VAR_NOINIT)
CanTrcv_Tja1145_SpiInBuffer[CANTRCV_MAX_CHANNELS][CANTRCV_TJA1145_SPI_BUFFER_SIZE];
#define CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"

/* SPI Output Buffer */
#define CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"
VAR(Spi_DataType, CANTRCV_VAR_NOINIT)
CanTrcv_Tja1145_SpiOutBuffer[CANTRCV_MAX_CHANNELS][CANTRCV_TJA1145_SPI_BUFFER_SIZE];
#define CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"

/* whether enable Can Fd Frame */
#define CANTRCV_START_SEC_CONST_BOOLEAN
#include "CanTrcv_MemMap.h"
CONST(boolean, CANTRCV_CONST) CanTrcv_Tja1145_Can_FD_Tolerance_Enabled = FALSE;
#define CANTRCV_STOP_SEC_CONST_BOOLEAN
#include "CanTrcv_MemMap.h"

/* general structure containing general infos */
#define CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"
VAR(CanTrcv_Tja1145_Prob_Type, CANTRCV_VAR_NOINIT) CanTrcv_Tja1145_Prob[CANTRCV_MAX_CHANNELS];
#define CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"

/* whether to use pin local wakeup */
#define CANTRCV_START_SEC_CONST_BOOLEAN
#include "CanTrcv_MemMap.h"
CONST(boolean, CANTRCV_CONST) CanTrcv_Tja1145_WakeupByPinUsed[CANTRCV_MAX_CHANNELS] = {TRUE, TRUE};
#define CANTRCV_STOP_SEC_CONST_BOOLEAN
#include "CanTrcv_MemMap.h"

/* whether to tolerance FD */
#define CANTRCV_START_SEC_CONST_BOOLEAN
#include "CanTrcv_MemMap.h"
CONST(boolean, CANTRCV_CONST) CanTrcv_Tja1145_CanFDToleranceEnabled = FALSE;
#define CANTRCV_STOP_SEC_CONST_BOOLEAN
#include "CanTrcv_MemMap.h"

/* Ecum Wakeup Source for pin wakeup */
#define CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"
VAR(EcuM_WakeupSourceType, CANTRCV_VAR) CanTrcv_PinWakeupSourceRef[CANTRCV_MAX_CHANNELS];
#define CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"

/* CanTrcv driver devcide id */
#define CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"
VAR(uint8, CANTRCV_VAR) CanTrcv_Tja1145_DeviceId[CANTRCV_MAX_CHANNELS];
#define CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"

/* CanTrcv driver work mode */
#define CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"
VAR(uint8, CANTRCV_VAR) CanTrcv_Tja1145_WorkMode[CANTRCV_MAX_CHANNELS];
#define CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTrcv_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/**
 * This function prepares data for SPI write buffer.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 SpiChannel: driver channel of SPI module.
 *                 bufferOffset: offset index of SPI write buffer.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly written
 *               E_NOT_OK: fail to write
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_PrepareTxBlock(uint8 Transceiver, Spi_ChannelType SpiChannel, uint8 bufferOffset)
{
    Std_ReturnType result;
    result = Spi_SetupEB(
        SpiChannel,
        &CanTrcv_Tja1145_SpiOutBuffer[Transceiver][bufferOffset],
        &CanTrcv_Tja1145_SpiInBuffer[Transceiver][bufferOffset],
        2);
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function transmits data for SPI driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): SpiSequence: transmit sequence of SPI driver.
 *                 isSync: transmit mode of SPI driver.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly transmitted
 *               E_NOT_OK: fail to transmit
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_SpiTransmit(Spi_SequenceType SpiSequence, boolean isSync)
{
    Std_ReturnType result;
    if (isSync == FALSE)
    {
        /* Asynchronous transmission */
        result = Spi_Async_Transmit(Spi_Sequence);
        if ((result == SPI_SEQ_PENDING) || (result == SPI_SEQ_OK))
        {
            result = E_OK;
        }
    }
    else
    {
        /* Using an application specific implementation */
        result = Spi_SyncTransmit(Spi_Sequence);
    }
    if (E_OK != result)
    {
        /* Correct return value */
        result = E_NOT_OK;
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function set the driver to requested mode.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 OpMode: the requested opmode.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly set
 *               E_NOT_OK: fail to set
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode)
{
    Std_ReturnType result;
    switch (OpMode)
    {
    case CANTRCV_TRCVMODE_SLEEP:
        CanTrcv_Tja1145_Prob[Transceiver].requestState &= (uint32)(~CANTRCV_TJA1145_REQ_OPMODEMASK);
        CanTrcv_Tja1145_Prob[Transceiver].requestState |= CANTRCV_TJA1145_REQ_OPMODESLEEP;
        result = CanTrcv_Tja1145_Driver_SetSleepMode(Transceiver);
        break;
    case CANTRCV_TRCVMODE_STANDBY:
        CanTrcv_Tja1145_Prob[Transceiver].requestState &= (uint32)(~CANTRCV_TJA1145_REQ_OPMODEMASK);
        CanTrcv_Tja1145_Prob[Transceiver].requestState |= CANTRCV_TJA1145_REQ_OPMODESTANDBY;
        result = CanTrcv_Tja1145_Driver_SetStandbyMode(Transceiver);
        break;
    case CANTRCV_TRCVMODE_NORMAL:
        CanTrcv_Tja1145_Prob[Transceiver].requestState &= (uint32)(~CANTRCV_TJA1145_REQ_OPMODEMASK);
        CanTrcv_Tja1145_Prob[Transceiver].requestState |= CANTRCV_TJA1145_REQ_OPMODENORMAL;
        result = CanTrcv_Tja1145_Driver_SetNormalMode(Transceiver);
        break;
    default:
        result = E_NOT_OK;
        break;
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function set the driver to normal mode.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly set
 *               E_NOT_OK: fail to set
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetNormalMode(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    /*switch to normal mode*/
    do
    {
        /* Write mode control bits to mode control register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = CANTRCV_TJA1145_MC_MC_NORMAL;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        result += Tm_BusyWait1us16bit(CANTRCV_WAIT_TIME);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        CanTrcv_Tja1145_Work_Mode[Transceiver] =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][MC_REG_DATA_NUM] & CANTRCV_TJA1145_MC_MC_MASK);
    } while ((CANTRCV_TJA1145_MC_MC_NORMAL != CanTrcv_Tja1145_Work_Mode[Transceiver]) && (E_OK == result));

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function set the driver to standby mode.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly set
 *               E_NOT_OK: fail to set
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetStandbyMode(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
        /* Write clear CAN wake-up flag bit to transceiver event status register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = CANTRCV_TJA1145_TES_CW;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    if (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver])
    {
        /* Write WPRE-WPFE flag bit to WAKE pin event capture enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = CANTRCV_TJA1145_WPE_WPRE;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    else
    {
        /* Write WPRE-WPFE flag bit to WAKE pin event capture enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = (uint8)(0x00);
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    /*switch to standby mode*/
    do
    {
        /* Write mode control bits to mode control register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = CANTRCV_TJA1145_MC_MC_STANDBY;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        result += Tm_BusyWait1us16bit(CANTRCV_WAIT_TIME);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        CanTrcv_Tja1145_Work_Mode[Transceiver] =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][MC_REG_DATA_NUM] & CANTRCV_TJA1145_MC_MC_MASK);
    } while ((CANTRCV_TJA1145_MC_MC_STANDBY != CanTrcv_Tja1145_Work_Mode[Transceiver]) && (E_OK == result));
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function set the driver to sleep mode.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non ReentrantNon Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly set
 *               E_NOT_OK: fail to set
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetSleepMode(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 tempValue = 0x00u;

    CanTrcv_Tja1145_Driver_DeInit(Transceiver);

    /*switch to sleep mode*/
    do
    {
        /* Write mode control bits to mode control register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = CANTRCV_TJA1145_MC_MC_SLEEP;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        result += Tm_BusyWait1us16bit(CANTRCV_WAIT_TIME);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ECS, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ECS_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        tempValue = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][ECS_REG_DATA_NUM]);
        if (tempValue != 0u)
        {
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_ADDR_NUM] =
                CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ECS, CANTRCV_TJA1145_WRITE_REG);
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_DATA_NUM] = tempValue;
            result +=
                CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ECS_REG_ADDR_NUM);
            result += CanTrcv_Tja1145_SpiTransmit(
                CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
                CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        }
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        tempValue = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][SES_REG_DATA_NUM]);
        if (tempValue != 0u)
        {
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
                CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_WRITE_REG);
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = tempValue;
            result +=
                CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
            result += CanTrcv_Tja1145_SpiTransmit(
                CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
                CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        }
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        tempValue = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][TES_REG_DATA_NUM]);
        if (tempValue != 0u)
        {
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
                CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_WRITE_REG);
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = tempValue;
            result +=
                CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
            result += CanTrcv_Tja1145_SpiTransmit(
                CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
                CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        }
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        tempValue = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][WES_REG_DATA_NUM]);
        if (tempValue != 0u)
        {
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_ADDR_NUM] =
                CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_WRITE_REG);
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_DATA_NUM] = tempValue;
            result +=
                CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WES_REG_ADDR_NUM);
            result += CanTrcv_Tja1145_SpiTransmit(
                CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
                CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        }
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TS, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TS_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        tempValue = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][TS_REG_DATA_NUM]);
        if (tempValue != 0u)
        {
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_ADDR_NUM] =
                CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TS, CANTRCV_TJA1145_WRITE_REG);
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_DATA_NUM] = tempValue;
            result +=
                CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TS_REG_ADDR_NUM);
            result += CanTrcv_Tja1145_SpiTransmit(
                CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
                CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        }
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPS, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        tempValue = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][WPE_REG_DATA_NUM]);
        if (tempValue != 0u)
        {
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
                CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPS, CANTRCV_TJA1145_WRITE_REG);
            CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = tempValue;
            result +=
                CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
            result += CanTrcv_Tja1145_SpiTransmit(
                CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
                CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        }

        /* Read mode control status bits from mode control register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        CanTrcv_Tja1145_Work_Mode[Transceiver] =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][MC_REG_DATA_NUM] & CANTRCV_TJA1145_MC_MC_MASK);
    } while ((CANTRCV_TJA1145_MC_MC_SLEEP != CanTrcv_Tja1145_Work_Mode[Transceiver]) && (E_OK == result));

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function initializes the TJA1145 transceiver driver module.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly initialized
 *               E_NOT_OK: fail to initialize
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Init(uint8 Transceiver)
{
    Std_ReturnType result;
    uint8 trcvStatus = 0x00;
    uint8 mainStatus = 0x00;
    uint8 deviceId = 0x00;
    CanTrcv_ChannelType channel = CANTRCV_CHANNEL(Transceiver);

    /* initialize memory for general structure and other types */
    CanTrcv_Tja1145_DeviceId[Transceiver] = 0x00;
    CanTrcv_Tja1145_WorkMode[Transceiver] = 0x00;
    CanTrcv_Tja1145_Prob[Transceiver].requestState = CANTRCV_TJA1145_REQ_NONE;
    CanTrcv_Tja1145_Prob[Transceiver].CanTrcvSpiReqType = CANTRCV_TJA1145_REQ_NONE;
    CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ts = 0x00;
    CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs = 0x00;
    CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ses = 0x00;
    CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Tes = 0x00;
    CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Wes = 0x00;
    CanTrcv_PinWakeupSourceRef[0] = 0;
    CanTrcv_PinWakeupSourceRef[1] = 0;

    /* Read wake-up status bits from transceiver driver registers */
    result = CanTrcv_Tja1145_Driver_ReadStatus(Transceiver);

    if (E_OK == result)
    {
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ts =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][TS_REG_DATA_NUM] & 0xFB);
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][ECS_REG_DATA_NUM] & 0x0D);
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ses =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][SES_REG_DATA_NUM] & 0x16);
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Tes =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][TES_REG_DATA_NUM] & 0x33);
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Wes =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][WES_REG_DATA_NUM] & 0x03);

        /* Check if wake-up event is CAN bus wake-up */
        if ((((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs & CANTRCV_TJA1145_ECS_TRXE) != 0x00u)
             && ((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Tes & CANTRCV_TJA1145_TES_CW) != 0x00u))
            && (TRUE == channel.CanTrcvWakeupByBusUsed))
        {
            /* A wake-up occurred. Now the reason is wake-up By Bus */
            CanTrcv_Prob[Transceiver].wakeUpReason = CANTRCV_WU_BY_BUS;
            /* Report result of wake-up event to upper layer */
            EcuM_SetWakeupEvent(channel.CanTrcvWakeupSourceRef);
        }
        /* Check if wake-up event is WAKE pin wake-up */
        else if (
            ((((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs & CANTRCV_TJA1145_ECS_WPE) != 0x00u))
             && ((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Wes & CANTRCV_TJA1145_WES_WPR) != 0x00u))
            && (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver]))
        {
            /* A wake-up occurred. Now the reason is wake-up By Pin */
            CanTrcv_Prob[Transceiver].wakeUpReason = CANTRCV_WU_BY_PIN;
            /* Report result of wake-up event to upper layer */
            EcuM_SetWakeupEvent(CanTrcv_PinWakeupSourceRef[Transceiver]);
        }
        /* Check if wake-up event is power on wake-up */
        else if (
            (((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs & CANTRCV_TJA1145_ECS_SYSE) != 0x00u)
             && ((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ses & CANTRCV_TJA1145_SES_PO) != 0x00u))
            && (TRUE == channel.CanTrcvPartialNetwork.CanTrcvPowerOnFlag))
        {
            /* If POR flag is set, transceiver needs to be reconfigured */
            CanTrcv_Prob[Transceiver].wakeUpReason = CANTRCV_WU_POWER_ON;
            /* Report result of wake-up event to upper layer */
            EcuM_SetWakeupEvent(CanTrcv_ChannelConfig[Transceiver].CanTrcvPorWakeupSourceRef);
        }
#if (STD_ON == CANTRCV_BUS_ERR_FLAG)
        /* Check if wake-up event is system error wake-up */
        else if (
            ((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ts & CANTRCV_TJA1145_TS_CPNERR) != 0x00u)
            && (TRUE == channel.CanTrcvPartialNetwork.CanTrcvBusErrFlag))
        {
            /* If SYSERR flag is set, transceiver needs to be reconfigured */
            CanTrcv_Prob[Transceiver].wakeUpReason = CANTRCV_WU_BY_SYSERR;
            /* Report result of wake-up event to upper layer */
            EcuM_SetWakeupEvent(CanTrcv_ChannelConfig[Transceiver].CanTrcvSysErrWakeupSourceRef);
        }
#endif /* STD_ON == CANTRCV_BUS_ERR_FLAG */
    }

    if (E_OK == result)
    {
        /* clear all status register */
        result += CanTrcv_Tja1145_Driver_ClearStatus(Transceiver, CANTRCV_TJA1145_RT_CLR_ALL);
        /* Initialize for TJA1145 transceiver driver */
        result += CanTrcv_Tja1145_Driver_Initialize(Transceiver);
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
        /* Set parameter for TJA1145 transceiver driver */
        result += CanTrcv_Tja1145_Driver_PnWakeUpSet(Transceiver);
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */
        /* Enable CAN bus wake-up event for TJA1145 transceiver driver */
        result += CanTrcv_Tja1145_Driver_EnableCanBusWakeUp(Transceiver);
        /* Enable CAN local wake-up event for TJA1145 transceiver driver */
        result += CanTrcv_Tja1145_Driver_EnableLocalWakeUp(Transceiver);
        /* Active CAN wake-up mode for TJA1145 transceiver driver */
        result += CanTrcv_Tja1145_Driver_SetCanTrcvMode(Transceiver);
        /* Verify if channel specific initialization was success full */
        result += CanTrcv_Tja1145_Driver_VerifyData(Transceiver);
        /* Get transceiver status form register */
        result += CanTrcv_Tja1145_Driver_ReadTrcvStatus(Transceiver, &trcvStatus);
        /* Get main status form register */
        result += CanTrcv_Tja1145_Driver_GetMainStatus(Transceiver, &mainStatus);
        /* Get device identifier form register */
        result += CanTrcv_Tja1145_Driver_GetDeviceID(Transceiver, &deviceId);
        CanTrcv_Tja1145_DeviceId[Transceiver] = deviceId;
    }

    if (E_OK == result)
    {
        result += CanTrcv_Tja1145_SetOpMode(Transceiver, channel.CanTrcvInitState);
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function initializes CanTrcv driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly initialized
 *               E_NOT_OK: fail to initialize
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_Initialize(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 seeValue = 0x00u;
    uint8 sesValue = CANTRCV_TJA1145_SES_SPIF | CANTRCV_TJA1145_SES_PO;
    uint8 canCtrlValue = 0x00u;

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    if (CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport == TRUE)
    {
        canCtrlValue |= (uint8)(CANTRCV_TJA1145_CC_CPNC);
        canCtrlValue |= (uint8)(CANTRCV_TJA1145_CC_PNCOK);
    }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

    if (CanTrcv_Tja1145_Can_FD_Tolerance_Enabled == TRUE)
    {
        canCtrlValue |= (uint8)(CANTRCV_TJA1145_CC_CFDC);
    }
    canCtrlValue |= CANTRCV_TJA1145_CC_CMC_NORMAL;

    /* Write Can Control value to register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][CC_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_CC, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][CC_REG_DATA_NUM] = canCtrlValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, CC_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write configuration value to System Event Status and Clear PO Event flag*/
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = sesValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write configuration value to System event enable register*/
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SEE_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SEE, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SEE_REG_DATA_NUM] = seeValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SEE_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* CAN bus silence detection disabled, CAN failure detection disabled, CAN wake-up detection disabled */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TEE_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TEE, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TEE_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TEE_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* rising-edge detection on WAKE pin disabled,falling-edge detection on WAKE pin disabled */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function de-initializes CanTrcv driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly deinitialized
 *               E_NOT_OK: fail to deinitialize
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_DeInit(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    result += CanTrcv_Tja1145_Driver_ClearPreSleepAllStatus(Transceiver);
    result += CanTrcv_Tja1145_Driver_PnWakeUpSet(Transceiver);
    result += CanTrcv_Tja1145_Driver_SetPreSleepCondition(Transceiver);
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function handles wakeup events for CanTrcv driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: wakeup events successfuly handled
 *               E_NOT_OK: Error has occured or fail to access HW
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_HandleWakeupEvents(uint8 Transceiver)
{
    Std_ReturnType result;
    boolean isBusWakeup = FALSE;
    boolean isPinWakeup = FALSE;

    result = CanTrcv_Tja1145_Driver_GetOpMode(Transceiver, &(CanTrcv_Prob[Transceiver].curOpMode));

    if (E_OK == result)
    {
        if (CANTRCV_TRCVMODE_NORMAL != CanTrcv_Prob[Transceiver].curOpMode)
        {
            /* check if transceiver driver wake-up event is valid */
            result += CanTrcv_Tja1145_Driver_IsBusWakeup(CanTrcvIndex, &isBusWakeup);
            result += CanTrcv_Tja1145_Driver_IsPinWakeup(CanTrcvIndex, &isPinWakeup);

            if ((E_OK == result) && ((TRUE == isBusWakeup) || (TRUE == isPinWakeup)))
            {
                /* Set transceiver driver wake-up event output flag is valid */
                CanTrcv_Tja1145_Prob[CanTrcvIndex].hasWakeupEvent = TRUE;
                result = CanTrcv_Tja1145_CB_WakeupByBus(CanTrcvIndex);
            }
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears CanTrcv driver pin wakeup event flags.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: det successfuly cleared
 *               E_NOT_OK: Error has occured or fail to access HW
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearPinWakeupFlag(uint8 Transceiver)
{
    Std_ReturnType result = E_NOT_OK;

    if (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver])
    {
        /* Write clear CAN wake-up flag bit to transceiver event status register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] =
            (uint8)(CANTRCV_TJA1145_WES_WPR | CANTRCV_TJA1145_WES_WPF);
        result = CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears CanTrcv driver bus wakeup event flags.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: det successfuly cleared
 *               E_NOT_OK: Error has occured or fail to access HW
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearBusWakeupFlag(uint8 Transceiver)
{
    Std_ReturnType result = E_NOT_OK;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
        /* Write clear CAN wake-up flag bit to transceiver event status register */
        CanTrcv_Tja1145_SpiOutBuffer[CanTrcvIndex][TES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[CanTrcvIndex][TES_REG_DATA_NUM] = CANTRCV_TJA1145_TES_CW;
        result = CanTrcv_Tja1145_PrepareTxBlock(CanTrcvIndex, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function reads CanTrcv driver all status.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly read
 *               E_NOT_OK: Error has occured or fail to access HW
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ReadStatus(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    /* Read status value from Transceiver status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TS, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_DATA_NUM] = 0x00u;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read status value from Event capture status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ECS, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ECS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read status value from System event status register and clear PO Event flag */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read status value from Transceiver event status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read status value from WAKE pin event status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears CanTrcv driver status registers based on given request type.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
                   requestType: type of request
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: det successfuly cleared
 *               E_NOT_OK: Error has occured or fail to access HW
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_ClearStatus(uint8 Transceiver, CanTrcv_Tja1145_Driver_ClearStatusRequestType requestType)
{
    Std_ReturnType result = E_OK;
    CanTrcv_Tja1145_StatusFlagsType clearFlags;

    /* initialize clear flags */
    clearFlags.Ecs = 0x00;
    clearFlags.Ses = 0x00;
    clearFlags.Tes = 0x00;
    clearFlags.Wes = 0x00;

    switch (requestType)
    {
    /* only clear CAN failure flag bit */
    case CANTRCV_TJA1145_RT_RD_STATUS:
        clearFlags.Tes = CANTRCV_TJA1145_TES_CF;
        break;
    /* only clear wake-up flag bits */
    case CANTRCV_TJA1145_RT_CLR_WUF:
        clearFlags.Tes = (uint8)(CANTRCV_TJA1145_TES_CF);
        clearFlags.Tes |= (uint8)(CANTRCV_TJA1145_TES_CW);
        clearFlags.Wes = (uint8)(CANTRCV_TJA1145_WES_WPR);
        break;
    /* clear all flag bits */
    case CANTRCV_TJA1145_RT_CLR_ALL:
        clearFlags.Ecs = (uint8)(CANTRCV_TJA1145_ECS_SYSE | CANTRCV_TJA1145_ECS_TRXE);
        clearFlags.Ecs |= (uint8)(CANTRCV_TJA1145_ECS_WPE);
        clearFlags.Ses = (uint8)(CANTRCV_TJA1145_SES_SPIF | CANTRCV_TJA1145_SES_OTW | CANTRCV_TJA1145_SES_PO);
        clearFlags.Tes = (uint8)(CANTRCV_TJA1145_TES_CF | CANTRCV_TJA1145_TES_CBS);
        clearFlags.Tes |= (uint8)(CANTRCV_TJA1145_TES_CW);
        clearFlags.Tes |= (uint8)(CANTRCV_TJA1145_TES_PNFDE);
        clearFlags.Wes = (uint8)(CANTRCV_TJA1145_WES_WPR | CANTRCV_TJA1145_WES_WPF);
        break;
    /* only clear partial networking frame detection error bits */
    case CANTRCV_TJA1145_RT_CLR_PNFDE:
        clearFlags.Tes = CANTRCV_TJA1145_TES_PNFDE;
        break;
    default:
        result = E_NOT_OK;
        break;
    }

    /* Write clear flag bits to Event capture status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ECS, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_DATA_NUM] = clearFlags.Ecs;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ECS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to System event status register and clear PO Event flag */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = clearFlags.Ses;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to Transceiver event status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = clearFlags.Tes;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to WAKE pin event status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_DATA_NUM] = clearFlags.Wes;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets the current driver operation mode.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): OpMode: Pointer to operation mode of the bus the API is applied to.
 * Return value: E_OK: the operation mode was detected.
 *               E_NOT_OK: the operation mode was not detected.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode)
{
    Std_ReturnType result = E_OK;

    /* Read MC status flag bit from mode control register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MC, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MC_REG_DATA_NUM] = 0x00u;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MC_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    CanTrcv_Tja1145_WorkMode[Transceiver] =
        (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][1] & CANTRCV_TJA1145_MC_MC_MASK);

    if (E_OK == result)
    {
        if (CANTRCV_TJA1145_MC_MC_NORMAL == (CanTrcv_Tja1145_WorkMode[Transceiver] & CANTRCV_TJA1145_MC_MC_NORMAL))
        {
            *OpMode = CANTRCV_TRCVMODE_NORMAL;
        }
        else if (
            CANTRCV_TJA1145_MC_MC_STANDBY == (CanTrcv_Tja1145_WorkMode[Transceiver] & CANTRCV_TJA1145_MC_MC_STANDBY))
        {
            *OpMode = CANTRCV_TRCVMODE_STANDBY;
        }
        else if (CANTRCV_TJA1145_MC_MC_SLEEP == (CanTrcv_Tja1145_WorkMode[Transceiver] & CANTRCV_TJA1145_MC_MC_SLEEP))
        {
            *OpMode = CANTRCV_TRCVMODE_SLEEP;
        }
        else
        {
            result = E_NOT_OK;
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
/**
 * This function configures the driver PN parameters for the TJA1145 transceiver driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_PnWakeUpSet(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
    {
        result += CanTrcv_Tja1145_Driver_SetPnRate(Transceiver);
        result += CanTrcv_Tja1145_Driver_SetPnIdentifier(Transceiver);
        result += CanTrcv_Tja1145_Driver_SetPnIdMask(Transceiver);
        result += CanTrcv_Tja1145_Driver_SetFrameControl(Transceiver);
        result += CanTrcv_Tja1145_Driver_SetDataMask(Transceiver);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function configures the driver PN parameters for the TJA1145 transceiver driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPnRate(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DR_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DR, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DR_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvBaudrate;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DR_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function configures the driver PN parameters for the TJA1145 transceiver driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPnIdentifier(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint32 Le_dw_can_id_value = 0x00000000;
    uint8 Le_u_can_id_value[4] = {0x00, 0x00, 0x00, 0x00};
    Le_dw_can_id_value = CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameCanId;

    if (FALSE == CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnCanIdIsExtended)
    {
        Le_u_can_id_value[1] = (uint8)((Le_dw_can_id_value & 0x7C0) >> 6);
        Le_u_can_id_value[0] = (uint8)((Le_dw_can_id_value & 0x3f) << 2);
        /*bits ID05 to ID00 of the standard frame format*/
        /* Write CAN wake up frame Id to identifier register 2 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID2_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID2, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID2_REG_DATA_NUM] = Le_u_can_id_value[0];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID2_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits ID10 to ID06 of the standard frame format*/
        /* Write CAN wake up frame Id to identifier register 3 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID3_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID3, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID3_REG_DATA_NUM] = Le_u_can_id_value[1];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID3_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    else
    {
        Le_u_can_id_value[3] = (uint8)((Le_dw_can_id_value & 0x8f000000) >> 24);
        Le_u_can_id_value[2] = (uint8)((Le_dw_can_id_value & 0xff0000) >> 16);
        Le_u_can_id_value[1] = (uint8)((Le_dw_can_id_value & 0xff00) >> 8);
        Le_u_can_id_value[0] = (uint8)((Le_dw_can_id_value & 0xff) >> 0);
        /*bits ID07 to ID00 of the extended frame format*/
        /* Write CAN wake up frame Id to identifier register 0  */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID0_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID0, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID0_REG_DATA_NUM] = Le_u_can_id_value[0];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID0_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits ID15 to ID08 of the extended frame format*/
        /* Write CAN wake up frame Id to identifier register 1  */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID1_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID1, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID1_REG_DATA_NUM] = Le_u_can_id_value[1];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID1_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits ID23 to ID16 of the extended frame format*/
        /* Write CAN wake up frame Id to identifier register 2  */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID2_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID2, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID2_REG_DATA_NUM] = Le_u_can_id_value[2];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID2_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits ID28 to ID24 of the extended frame format*/
        /* Write CAN wake up frame Id to identifier register 3  */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID3_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID3, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID3_REG_DATA_NUM] = Le_u_can_id_value[3];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID3_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function configures the driver PN parameters for the TJA1145 transceiver driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPnIdMask(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint32 Le_dw_can_id_mask = 0x00000000;
    uint8 Le_u_can_id_mask[4] = {0x00, 0x00, 0x00, 0x00};
    Le_dw_can_id_mask = CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameCanIdMask;

    if (CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnCanIdIsExtended == FALSE)
    {
        Le_u_can_id_mask[1] = (uint8)((Le_dw_can_id_mask & 0x7C0) >> 6);
        Le_u_can_id_mask[0] = (uint8)((Le_dw_can_id_mask & 0x3f) << 2);
        /*bits ID05 to ID00 of the standard frame mask*/
        /* Write CAN wake up frame Id mask code to mask register 2 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK2_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK2, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK2_REG_DATA_NUM] = Le_u_can_id_mask[0];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK2_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits ID10 to ID06 of the standard frame mask*/
        /* Write CAN wake up frame Id mask code to mask register 3 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK3_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK3, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK3_REG_DATA_NUM] = Le_u_can_id_mask[1];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK3_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    else
    {
        Le_u_can_id_mask[3] = (uint8)((Le_dw_can_id_mask & 0x8f000000) >> 24);
        Le_u_can_id_mask[2] = (uint8)((Le_dw_can_id_mask & 0xff0000) >> 16);
        Le_u_can_id_mask[1] = (uint8)((Le_dw_can_id_mask & 0xff00) >> 8);
        Le_u_can_id_mask[0] = (uint8)((Le_dw_can_id_mask & 0xff) >> 0);
        /*bits MK07 to MK00 of the standard frame mask*/
        /* Write CAN wake up frame Id mask to mask register 0 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK0_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK0, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK0_REG_DATA_NUM] = Le_u_can_id_mask[0];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK0_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits MK15 to MK8 of the standard frame mask*/
        /* Write CAN wake up frame Id mask to mask register 1 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK1_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK1, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK1_REG_DATA_NUM] = Le_u_can_id_mask[1];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK1_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits MK23 to MK16 of the standard frame mask*/
        /* Write CAN wake up frame Id mask to mask register 0 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK2_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK2, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK2_REG_DATA_NUM] = Le_u_can_id_mask[2];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK2_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        /*bits MK28 to MK24 of the standard frame mask*/
        /* Write CAN wake up frame Id mask to mask register 0 */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK3_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK3, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK3_REG_DATA_NUM] = Le_u_can_id_mask[3];
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK3_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function configures the driver PN parameters for the TJA1145 transceiver driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetFrameControl(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 canFrameCtrlValue = 0x00;

    if (CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnCanIdIsExtended == TRUE)
    {
        canFrameCtrlValue |= (uint8)(0x80);
    }
    else
    {
        canFrameCtrlValue &= ~(uint8)(0x80);
    }
    if (CanTrcv_Tja1145_PnWakeupEventSelect == CANTRCV_TJA1145_WAKEUP_EVENT_IS_ID)
    {
        canFrameCtrlValue &= ~(uint8)(0x40);
    }
    else
    {
        canFrameCtrlValue |= (uint8)(0x40);
    }
    canFrameCtrlValue |= (uint8)(CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDlc);
    /*standard frame format (11-bit),data length code and data field are evaluated at wake-up,DLC 8*/
    /* Write CAN wake up DLC to frame control register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][FC_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_FC, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][FC_REG_DATA_NUM] = canFrameCtrlValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, FC_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function configures the driver PN parameters for the TJA1145 transceiver driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetDataMask(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    /* Write CAN wake up data mask to data byte register 0 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB0_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB0, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB0_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[0].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB0_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 1 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB1_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB1, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB1_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[1].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB1_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 2 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB2_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB2, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB2_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[2].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB2_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 3 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB3_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB3, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB3_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[3].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB3_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 4 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB4_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB4, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB4_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[4].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB4_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 5 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB5_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB5, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB5_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[5].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB5_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 6 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB6_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB6, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB6_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[6].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB6_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write CAN wake up data mask to data byte register 7 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB7_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB7, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB7_REG_DATA_NUM] =
        CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPnFrameDataMaskSpec[7].CanTrcvPnFrameDataMask;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB7_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function checks whether POR flag is set.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): isPorSet: whether the POR flag is set.
 * Return value: E_OK: successfuly checked.
 *               E_NOT_OK: fail to check.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_CheckPorFlag(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isPorSet)
{
    Std_ReturnType result;
    *isPorSet = FALSE;

    /* Read status value from Event capture status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ECS, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_DATA_NUM] = 0x00;
    result = CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ECS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    /* Read status value from System Error status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    if (result == E_OK)
    {
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][ECS_REG_DATA_NUM] & 0x0D);
        CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ses =
            (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][SES_REG_DATA_NUM] & 0x16);

        /* Check if wake-up event is power on wake-up */
        if ((((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ecs & CANTRCV_TJA1145_ECS_SYSE) != 0x00u)
             && ((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ses & CANTRCV_TJA1145_SES_PO) != 0x00u))
            && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvPowerOnFlag))
        {
            *isPorSet = TRUE;
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#if (STD_ON == CANTRCV_BUS_ERR_FLAG)
/**
 * This function checks whether SYSERR flag is set.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): isSyserrSet: whether the SYSERR flag is set.
 * Return value: E_OK: successfuly checked.
 *               E_NOT_OK: fail to check.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_CheckSyserrFlag(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isSyserrSet)
{
    Std_ReturnType result;
    *isSyserrSet = FALSE;
    result = CanTrcv_Tja1145_Driver_ReadTrcvStatus(Transceiver, &(CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ts));

    /* Check if wake-up event is system error wake-up */
    if (((CanTrcv_Tja1145_Prob[Transceiver].statusFlags.Ts & CANTRCV_TJA1145_TS_CPNERR) != 0x00u)
        && (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvBusErrFlag))
    {
        *isSyserrSet = TRUE;
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
#endif /* STD_ON == CANTRCV_BUS_ERR_FLAG */

/**
 * This function reads the transceiver configuration/status data of driver and returns.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvSysData: Configuration/Status data of the transceiver.
 * Return value: E_OK: the transceiver status is successfully read.
 *               E_NOT_OK: the transceiver status data is not available or a development error occurs.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_GetTrcvSystemData(uint8 Transceiver, P2CONST(uint32, AUTOMATIC, CANTRCV_CONST) TrcvSysData)
{
    Std_ReturnType result = E_NOT_OK;

    /* check if SPI is busy */
    if (CANTRCV_TJA1145_REQ_NONE == CanTrcv_Tja1145_Prob[Transceiver].requestState)
    {
        /* Read the value of the diag register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MS_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MS, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MS_REG_DATA_NUM] = 0x00u;
        result = CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MS_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TS, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_DATA_NUM] = 0x00u;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TS_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPS, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_DATA_NUM] = 0x00u;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

        if (result == E_OK)
        {
            *TrcvSysData =
                (((uint32)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][MS_REG_DATA_NUM]) << 0x00u)
                 | ((uint32)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][TS_REG_DATA_NUM]) << 0x08u)
                 | ((uint32)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][WES_REG_DATA_NUM]) << 0x10u));
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

/**
 * This function enables driver wake up by bus detection.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_EnableCanBusWakeUp(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 teeValue = 0x00;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
        teeValue = (uint8)(CANTRCV_TJA1145_TEE_CWE);
        /* CAN bus silence detection disabled, CAN failure detection disabled, CAN wake-up detection enabled */
        /* Write CWE flag bit to transceiver event enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TEE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TEE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TEE_REG_DATA_NUM] = teeValue;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TEE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function enables driver wake up by pin detection.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly configured.
 *               E_NOT_OK: fail to configure.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_EnableLocalWakeUp(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 wpeValue;

    if (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver])
    {
        /* WPRE = on,WPFE = off */
        wpeValue = (uint8)(CANTRCV_TJA1145_WPE_WPRE);
        /* rising-edge detection on WAKE pin enabled, falling-edge detection on WAKE pin disabled */
        /* Write WPR flag bits to WAKE pin enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = wpeValue;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    else
    {
        /* WPRE = on, WPFE = off */
        wpeValue = 0x00u;
        /* rising-edge detection on WAKE pin enabled,falling-edge detection on WAKE pin disabled */
        /* Write WPR flag bits to WAKE pin enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = wpeValue;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function sets Can working mode of driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly set.
 *               E_NOT_OK: fail to set.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetCanTrcvMode(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 canControlValue = 0x00;

    if (TRUE == CanTrcv_Tja1145_Can_FD_Tolerance_Enabled)
    {
        canControlValue |= CANTRCV_TJA1145_CC_CFDC;
    }
#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
    {
        canControlValue |= CANTRCV_TJA1145_CC_PNCOK;
        canControlValue |= CANTRCV_TJA1145_CC_CPNC;
    }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */
    canControlValue |= CANTRCV_TJA1145_CC_CMC_NORMAL;

    /*partial networking registers configured successfully,enable CAN selective wake-up*/
    /* Write CMC flag bit to CAN control register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][CC_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_CC, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][CC_REG_DATA_NUM] = canControlValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, CC_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function verifies data for Tja1145 transceiver driver parameter.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly verified.
 *               E_NOT_OK: fail to verify.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_VerifyData(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;

    /* Read value from Data Rate Register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DR_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DR, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DR_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DR_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Identifier Register 2 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID2_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID2, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID2_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID2_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Identifier Register 3 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID3_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ID3, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ID3_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ID3_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Mask Register 2 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK2_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK2, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK2_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK2_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Mask Register 3 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK3_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MK3, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MK3_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MK3_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Frame Control Register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][FC_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_FC, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][FC_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, FC_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 0 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB0_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB0, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB0_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB0_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 1 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB1_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB1, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB1_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB1_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 2 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB2_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB2, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB2_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB2_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 3 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB3_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB3, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB3_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB3_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 4 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB4_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB4, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB4_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB4_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 5 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB5_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB5, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB5_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB5_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 6 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB6_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB6, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB6_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB6_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Read value from Data Byte Register 7 */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB7_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_DB7, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][DB7_REG_DATA_NUM] = 0x00;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, DB7_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets transceiver status form register.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 trcvStatus: the status of transceiver
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly gotten.
 *               E_NOT_OK: fail to get.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_ReadTrcvStatus(uint8 Transceiver, P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) trcvStatus)
{
    Std_ReturnType result = E_OK;

    /* Read status flag bits from transceiver status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TS, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_DATA_NUM] = 0x00u;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    *trcvStatus = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][TS_REG_DATA_NUM] & 0xFB);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets main status form register.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 mainStatus: the main status of transceiver
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly gotten.
 *               E_NOT_OK: fail to get.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_GetMainStatus(uint8 Transceiver, P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) mainStatus)
{
    Std_ReturnType result = E_OK;

    /* Read status flag bits from main status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_MS, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][MS_REG_DATA_NUM] = 0x00u;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, MS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    *mainStatus = (uint8)(CanTrcv_Tja1145_SpiInBuffer[Transceiver][MS_REG_DATA_NUM] & 0xE0);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function gets device id of the driver.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 *                 deviceId: the id of device.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly gotten.
 *               E_NOT_OK: fail to get.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_GetDeviceID(uint8 Transceiver, P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) deviceId)
{
    Std_ReturnType result = E_OK;

    /* Read IDS flag bits from identification status register */
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ISR_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ISR, CANTRCV_TJA1145_READ_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ISR_REG_DATA_NUM] = 0x00u;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ISR_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    *deviceId = CanTrcv_Tja1145_SpiInBuffer[Transceiver][ISR_REG_DATA_NUM];

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function clears all status before put to sleep.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly cleared.
 *               E_NOT_OK: fail to clear.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearPreSleepAllStatus(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    CanTrcv_Tja1145_StatusFlagsType clearFlags;
    uint8 wakeStatusValue = 0x00;
    clearFlags.Ecs = 0x00;
    clearFlags.Ses = 0x00;
    clearFlags.Tes = 0x00;
    clearFlags.Wes = 0x00;
    clearFlags.Ts = 0x00;

    /* Write clear flag bits to Event capture status register */
    clearFlags.Ecs = (uint8)(CANTRCV_TJA1145_ECS_WPE | CANTRCV_TJA1145_ECS_TRXE | CANTRCV_TJA1145_ECS_SYSE);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_ECS, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][ECS_REG_DATA_NUM] = clearFlags.Ecs;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, ECS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to System event status register and clear PO Event flag */
    clearFlags.Ses = (uint8)(CANTRCV_TJA1145_SES_PO | CANTRCV_TJA1145_SES_OTW | CANTRCV_TJA1145_SES_SPIF);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_SES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][SES_REG_DATA_NUM] = clearFlags.Ses;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, SES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to Transceiver event status register */
    clearFlags.Tes =
        (uint8)(CANTRCV_TJA1145_TES_PNFDE | CANTRCV_TJA1145_TES_CBS | CANTRCV_TJA1145_TES_CF | CANTRCV_TJA1145_TES_CW);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = clearFlags.Tes;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to WAKE pin event status register */
    clearFlags.Wes = (uint8)(CANTRCV_TJA1145_WES_WPF | CANTRCV_TJA1145_WES_WPR);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WES_REG_DATA_NUM] = clearFlags.Wes;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WES_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    /* Write clear flag bits to Transceiver status register */
    clearFlags.Ts = (uint8)(CANTRCV_TJA1145_TS_CFS | CANTRCV_TJA1145_TS_CBSS | CANTRCV_TJA1145_TS_COSCS
                            | CANTRCV_TJA1145_TS_CPNS | CANTRCV_TJA1145_TS_CPNERR);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TS, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TS_REG_DATA_NUM] = clearFlags.Ts;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TS_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    wakeStatusValue = 0x02;
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPS, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = wakeStatusValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function set specific condition before set to sleep.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly set.
 *               E_NOT_OK: fail to set.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPreSleepCondition(uint8 Transceiver)
{
    Std_ReturnType result = E_OK;
    uint8 canControlValue = 0x00;
    uint8 teeRegValue = 0x00;
    uint8 wpeRegValue = 0x00;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
        /* Write CWE-CFE-CBSE flag bit to Transceiver event capture enable register */
        teeRegValue = (uint8)(CANTRCV_TJA1145_TEE_CWE);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TEE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TEE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TEE_REG_DATA_NUM] = teeRegValue;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TEE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    if (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver])
    {
        wpeRegValue = (uint8)(CANTRCV_TJA1145_WPE_WPRE);
        /* Write WPRE-WPFE flag bit to WAKE pin event capture enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = wpeRegValue;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }
    else
    {
        wpeRegValue = 0x00u;
        /* Write WPRE-WPFE flag bit to WAKE pin event capture enable register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WPE, CANTRCV_TJA1145_WRITE_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = wpeRegValue;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);
    }

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
    /* Write CFDC-PVCOK-CPNC-CMC flag bit to CAN control register */
    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvHwPnSupport)
    {
        canControlValue |= (uint8)(CANTRCV_TJA1145_CC_CPNC);
        canControlValue |= (uint8)(CANTRCV_TJA1145_CC_PNCOK);
    }
#endif /* STD_ON == CANTRCV_HWPN_SUPPORT */

    if (TRUE == CanTrcv_Tja1145_CanFDToleranceEnabled)
    {
        canControlValue |= (uint8)(CANTRCV_TJA1145_CC_CFDC);
    }
    canControlValue |= CANTRCV_TJA1145_CC_CMC_OFFLINE;
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][CC_REG_ADDR_NUM] =
        CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_CC, CANTRCV_TJA1145_WRITE_REG);
    CanTrcv_Tja1145_SpiOutBuffer[Transceiver][CC_REG_DATA_NUM] = canControlValue;
    result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, CC_REG_ADDR_NUM);
    result += CanTrcv_Tja1145_SpiTransmit(
        CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
        CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function checks whether there is a bus wakeup event
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): isBusWakeup: whether there is a bus wakeup
 * Return value: E_OK: successfuly checked.
 *               E_NOT_OK: fail to check.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_IsBusWakeup(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isBusWakeup)
{
    Std_ReturnType result = E_OK;
    *isBusWakeup = FALSE;

    if (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
    {
        /* Read CAN wake-up event detect bit(CW) from transceiver event status register */
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_TES, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][TES_REG_DATA_NUM] = 0x00;
        result += CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, TES_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

        /* check if wake-up event is CAN bus wake-up */
        if (CANTRCV_TJA1145_TES_CW
            == (CanTrcv_Tja1145_SpiInBuffer[Transceiver][TES_REG_DATA_NUM] & CANTRCV_TJA1145_TES_CW))
        {
            /* Set current wake-up reason is CAN bus wake-up */
            CanTrcv_Prob[Transceiver].wakeUpReason = CANTRCV_WU_BY_BUS;
            *isBusWakeup = TRUE;
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function checks whether there is a pin wakeup event
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): isBusWakeup: whether there is a pin wakeup
 * Return value: E_OK: successfuly checked.
 *               E_NOT_OK: fail to check.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE)
CanTrcv_Tja1145_Driver_IsPinWakeup(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isBusWakeup)
{
    Std_ReturnType result = E_NOT_OK;
    *isPinWakeup = FALSE;

    if (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver])
    {
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_ADDR_NUM] =
            CANTRCV_TJA1145_MKADR(CANTRCV_TJA1145_WES, CANTRCV_TJA1145_READ_REG);
        CanTrcv_Tja1145_SpiOutBuffer[Transceiver][WPE_REG_DATA_NUM] = 0x00;
        result = CanTrcv_Tja1145_PrepareTxBlock(Transceiver, CANTRCV_SPI_ACCESS.CanTrcvSpiChannel, WPE_REG_ADDR_NUM);
        result += CanTrcv_Tja1145_SpiTransmit(
            CANTRCV_SPI_ACCESS.CanTrcvSpiSequence,
            CANTRCV_SPI_ACCESS.CanTrcvSpiAccessSynchronous);

        /* check if wake-up event is CAN bus wake-up */
        if (CANTRCV_TJA1145_WES_WPR
            == (CanTrcv_Tja1145_SpiInBuffer[Transceiver][WPE_REG_DATA_NUM] & CANTRCV_TJA1145_WES_WPR))
        {
            /* Set current wake-up reason is CAN bus wake-up */
            CanTrcv_Prob[Transceiver].wakeUpReason = CANTRCV_WU_BY_PIN;
            *isPinWakeup = TRUE;
        }
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"

/**
 * This function deals with network wake-up event.
 * Service ID: None
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): Transceiver: CAN transceiver to which API call has to be applied.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: successfuly dealt.
 *               E_NOT_OK: fail to deal.
 *
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_CB_WakeupByBus(uint8 Transceiver)
{
    Std_ReturnType result = E_NOT_OK;

    /* check if transceiver driver wake-up mode is ENABLE and CAN bus wake-up used state is enabled */
    if (CANTRCV_WUMODE_ENABLE == CanTrcv_Prob[Transceiver].wakeUpMode)
    {
        /* Check if there are some stored wake-up events (detected during initialization,polling) */
        if ((TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupByBusUsed)
            && (CANTRCV_WU_BY_BUS == CanTrcv_Tja1145_Prob[Transceiver].wakeUpReason))
        {
            /* For SPI drivers, inform higher layer only if in power-down mode */
            if (CANTRCV_TRCVMODE_NORMAL != CanTrcv_Prob[Transceiver].curOpMode)
            {
                /* Report the wake-up to the upper layer */
                EcuM_SetWakeupEvent(CANTRCV_CHANNEL(Transceiver).CanTrcvWakeupSourceRef);
            }
        }
        else if (
            (TRUE == CanTrcv_Tja1145_WakeupByPinUsed[Transceiver])
            && (CANTRCV_WU_BY_PIN == CanTrcv_Prob[Transceiver].wakeUpReason))
        {
            /* For SPI drivers, inform higher layer only if in power-down mode */
            if (CANTRCV_TRCVMODE_NORMAL != CanTrcv_Prob[Transceiver].curOpMode)
            {
                /* Report the wake-up to the upper layer */
                EcuM_SetWakeupEvent(CanTrcv_PinWakeupSourceRef[CanTrcvIndex]);
            }
        }
#if (STD_ON == CANTRCV_BUS_ERR_FLAG)
        else if (
            (TRUE == CANTRCV_CHANNEL(Transceiver).CanTrcvPartialNetwork.CanTrcvBusErrFlag)
            && (CANTRCV_WU_BY_SYSERR == CanTrcv_Prob[Transceiver].wakeUpReason))
        {
            /* For SPI drivers, inform higher layer only if in power-down mode */
            if (CANTRCV_TRCVMODE_NORMAL != CanTrcv_Prob[Transceiver].curOpMode)
            {
                /* Report the wake-up to the upper layer */
                EcuM_SetWakeupEvent(CANTRCV_CHANNEL(Transceiver).CanTrcvSyserrWakeupSourceRef);
            }
        }
#endif /* STD_ON == CANTRCV_BUS_ERR_FLAG */

        /* clear for wake-up flag */
        result = CanTrcv_Tja1145_Driver_ClearBusWakeupFlag(Transceiver);
        result += CanTrcv_Tja1145_Driver_ClearPinWakeupFlag(Transceiver);
    }

    return result;
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"
