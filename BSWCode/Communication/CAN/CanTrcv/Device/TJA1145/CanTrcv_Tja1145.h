/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : CanTrcv_Tja1145.h                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Private interfaces declared by CanTrcv driver Tja1145       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef CANTRCV_Tja1145_H_
#define CANTRCV_Tja1145_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTrcv.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define CANTRCV_TJA1145_SPI_BUFFER_SIZE 60u

/* ************ Register layout *************/
/* Main control register registers */
#define CANTRCV_TJA1145_MC  0x01u /* Mode Control Register    */
#define CANTRCV_TJA1145_MS  0x03u /* Main Status Register     */
#define CANTRCV_TJA1145_SEE 0x04u /* System Event Enable      */
#define CANTRCV_TJA1145_MC0 0x06u /* Memory control 0 (GP)    */
#define CANTRCV_TJA1145_MC1 0x07u /* Memory control 1 (GP)    */
#define CANTRCV_TJA1145_MC2 0x08u /* Memory control 2 (GP)    */
#define CANTRCV_TJA1145_MC4 0x09u /* Memory control 4 (GP)    */
#define CANTRCV_TJA1145_LC  0x0Au /* Lock control             */

/* Transceiver Control registers */
#define CANTRCV_TJA1145_CC  0x20u /* CAN Control              */
#define CANTRCV_TJA1145_TS  0x22u /* Transceiver Status       */
#define CANTRCV_TJA1145_TEE 0x23u /* Transceiver Event Enable */

/* Partial Networking Configuration Registers */
#define CANTRCV_TJA1145_DR  0x26u /* Data Rate Register       */
#define CANTRCV_TJA1145_ID0 0x27u /* Identifier Register 0    */
#define CANTRCV_TJA1145_ID1 0x28u /* Identifier Register 1    */
#define CANTRCV_TJA1145_ID2 0x29u /* Identifier Register 2    */
#define CANTRCV_TJA1145_ID3 0x2au /* Identifier Register 3    */
#define CANTRCV_TJA1145_MK0 0x2bu /* Mask Register 0          */
#define CANTRCV_TJA1145_MK1 0x2cu /* Mask Register 1          */
#define CANTRCV_TJA1145_MK2 0x2du /* Mask Register 2          */
#define CANTRCV_TJA1145_MK3 0x2eu /* Mask Register 3          */
#define CANTRCV_TJA1145_FC  0x2fu /* Frame Control Register   */
#define CANTRCV_TJA1145_DB0 0x68u /* Data Byte Register 0     */
#define CANTRCV_TJA1145_DB1 0x69u /* Data Byte Register 1     */
#define CANTRCV_TJA1145_DB2 0x6au /* Data Byte Register 2     */
#define CANTRCV_TJA1145_DB3 0x6bu /* Data Byte Register 3     */
#define CANTRCV_TJA1145_DB4 0x6cu /* Data Byte Register 4     */
#define CANTRCV_TJA1145_DB5 0x6du /* Data Byte Register 5     */
#define CANTRCV_TJA1145_DB6 0x6eu /* Data Byte Register 6     */
#define CANTRCV_TJA1145_DB7 0x6fu /* Data Byte Register 7     */

/* WAKE Pin registers */
#define CANTRCV_TJA1145_WPS 0x4bu /* WAKE pin Status Register */
#define CANTRCV_TJA1145_WPE 0x4cu /* WAKE pin Enable          */

/* Event capture registers */
#define CANTRCV_TJA1145_ECS 0x60u /* Event Capture Status     */
#define CANTRCV_TJA1145_SES 0x61u /* System Event Status      */
#define CANTRCV_TJA1145_TES 0x63u /* Transceiver Event Status */
#define CANTRCV_TJA1145_WES 0x64u /* WAKE pin Event Status    */

/* Identification Register */
#define CANTRCV_TJA1145_ISR 0x7eu /* Identification Status Register = 0x70 */

/* Mc Register */
#define CANTRCV_TJA1145_MC_MC_MASK    0x07u /* Mode Control Mask              */
#define CANTRCV_TJA1145_MC_MC_SLEEP   0x01u /* Sleep Mode                     */
#define CANTRCV_TJA1145_MC_MC_STANDBY 0x04u /* Standby Mode                   */
#define CANTRCV_TJA1145_MC_MC_NORMAL  0x07u /* Normal Mode                    */

/* Ts Register */
#define CANTRCV_TJA1145_TS_CFS    0x01u /* CAN Failures Status            */
#define CANTRCV_TJA1145_TS_CBSS   0x08u /* CAN Bus Silence Status         */
#define CANTRCV_TJA1145_TS_COSCS  0x10u /* CAN Oscillator Status          */
#define CANTRCV_TJA1145_TS_CPNS   0x20u /* CAN Partial Networking Status  */
#define CANTRCV_TJA1145_TS_CPNERR 0x40u /* CAN Partial Networking Error   */

/* Tee Register */
#define CANTRCV_TJA1145_TEE_CWE  0x01u /* CAN wake-up capture enable     */
#define CANTRCV_TJA1145_TEE_CFE  0x02u /* CAN failure capture enable     */
#define CANTRCV_TJA1145_TEE_CBSE 0x10u /* CAN bus silence capture enable */

/* CC Register */
#define CANTRCV_TJA1145_CC_CMC_MASK    0x03u /* CAN Mode control mask          */
#define CANTRCV_TJA1145_CC_CMC_OFFLINE 0x00u /* CMC = Off line                  */
#define CANTRCV_TJA1145_CC_CMC_NORMAL  0x01u /* CMC = Normal                   */
#define CANTRCV_TJA1145_CC_CMC_ACTIVE  0x02u /* CMC = Normal                   */
#define CANTRCV_TJA1145_CC_CMC_LISTEN  0x03u /* CMC = Listen                   */
#define CANTRCV_TJA1145_CC_CPNC        0x10u /* CAN Partial Networking Control */
#define CANTRCV_TJA1145_CC_PNCOK       0x20u /* Partial Networking Configure OK  */
#define CANTRCV_TJA1145_CC_CFDC        0x40u /* CAN FD tolerance enabled       */

/* Wpe register */
#define CANTRCV_TJA1145_WPE_WPFE 0x01u /* Wake pin falling edge capture  */
#define CANTRCV_TJA1145_WPE_WPRE 0x02u /* Wake pin rising edge capture   */

/* Ecs Register */
#define CANTRCV_TJA1145_ECS_SYSE 0x01u /* System event status            */
#define CANTRCV_TJA1145_ECS_TRXE 0x04u /* Transceiver event status       */
#define CANTRCV_TJA1145_ECS_WPE  0x08u /* Wake pin event status          */

/* Ses Register */
#define CANTRCV_TJA1145_SES_SPIF 0x02u /* SPI failure event              */
#define CANTRCV_TJA1145_SES_OTW  0x04u /* Over temperature warning event  */
#define CANTRCV_TJA1145_SES_PO   0x10u /* Power-on event                 */

/* Tes Register */
#define CANTRCV_TJA1145_TES_CW    0x01u /* CAN Wake-up event              */
#define CANTRCV_TJA1145_TES_CF    0x02u /* CAN failure event              */
#define CANTRCV_TJA1145_TES_CBS   0x10u /* CAN bus silence event          */
#define CANTRCV_TJA1145_TES_PNFDE 0x20u /* PN Frame Detection Error event */

/* Wes Register */
#define CANTRCV_TJA1145_WES_WPF 0x01u /* WAKE pin falling edge event    */
#define CANTRCV_TJA1145_WES_WPR 0x02u /* WAKE pin rising edge event     */

/* register addresses */
#define MC_REG_ADDR_NUM  0U
#define MC_REG_DATA_NUM  1U
#define CC_REG_ADDR_NUM  2U
#define CC_REG_DATA_NUM  3U
#define TS_REG_ADDR_NUM  4U
#define TS_REG_DATA_NUM  5U
#define ECS_REG_ADDR_NUM 6U
#define ECS_REG_DATA_NUM 7U
#define SES_REG_ADDR_NUM 8U
#define SES_REG_DATA_NUM 9U
#define TES_REG_ADDR_NUM 10U
#define TES_REG_DATA_NUM 11U
#define WES_REG_ADDR_NUM 12U
#define WES_REG_DATA_NUM 13U
#define SEE_REG_ADDR_NUM 14U
#define SEE_REG_DATA_NUM 15U
#define TEE_REG_ADDR_NUM 16U
#define TEE_REG_DATA_NUM 17U
#define WPE_REG_ADDR_NUM 18U
#define WPE_REG_DATA_NUM 19U
#define DR_REG_ADDR_NUM  20U
#define DR_REG_DATA_NUM  21U
#define ID0_REG_ADDR_NUM 22U
#define ID0_REG_DATA_NUM 23U
#define ID1_REG_ADDR_NUM 24U
#define ID1_REG_DATA_NUM 25U
#define ID2_REG_ADDR_NUM 26U
#define ID2_REG_DATA_NUM 27U
#define ID3_REG_ADDR_NUM 28U
#define ID3_REG_DATA_NUM 29U
#define MK0_REG_ADDR_NUM 30U
#define MK0_REG_DATA_NUM 31U
#define MK1_REG_ADDR_NUM 32U
#define MK1_REG_DATA_NUM 33U
#define MK2_REG_ADDR_NUM 34U
#define MK2_REG_DATA_NUM 35U
#define MK3_REG_ADDR_NUM 36U
#define MK3_REG_DATA_NUM 37U
#define FC_REG_ADDR_NUM  38U
#define FC_REG_DATA_NUM  39U
#define DB0_REG_ADDR_NUM 40U
#define DB0_REG_DATA_NUM 41U
#define DB1_REG_ADDR_NUM 42U
#define DB1_REG_DATA_NUM 43U
#define DB2_REG_ADDR_NUM 44U
#define DB2_REG_DATA_NUM 45U
#define DB3_REG_ADDR_NUM 46U
#define DB3_REG_DATA_NUM 47U
#define DB4_REG_ADDR_NUM 48U
#define DB4_REG_DATA_NUM 49U
#define DB5_REG_ADDR_NUM 50U
#define DB5_REG_DATA_NUM 51U
#define DB6_REG_ADDR_NUM 52U
#define DB6_REG_DATA_NUM 53U
#define DB7_REG_ADDR_NUM 54U
#define DB7_REG_DATA_NUM 55U
#define ISR_REG_ADDR_NUM 56U
#define ISR_REG_DATA_NUM 57U
#define MS_REG_ADDR_NUM  58U
#define MS_REG_DATA_NUM  59U

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef enum
{
    CANTRCV_TJA1145_RT_RD_STATUS = 0u,
    CANTRCV_TJA1145_RT_CLR_WUF,
    CANTRCV_TJA1145_RT_CLR_WUF,
    CANTRCV_TJA1145_RT_CLR_PNFDE
} CanTrcv_Tja1145_Driver_ClearStatusRequestType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_PrepareTxBlock(uint8 Transceiver, Spi_ChannelType SpiChannel, uint8 bufferOffset);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_SpiTransmit(Spi_SequenceType SpiSequence, boolean isSync);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetNormalMode(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetStandbyMode(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetSleepMode(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Init(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_Initialize(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_DeInit(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_HandleWakeupEvents(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearPinWakeupFlag(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearBusWakeupFlag(uint8 Transceiver);

/**
 * This function reads CanTrcv driver Transceiver Status to global variable.
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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ReadTrcvStatus(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ReadStatus(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearStatus(uint8 Transceiver, uint32 requestType);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_GetOpMode(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_PnWakeUpSet(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPnRate(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPnIdentifier(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPnIdMask(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetFrameControl(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetDataMask(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_CheckPorFlag(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isPorSet);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_CheckSyserrFlag(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isSyserrSet);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_GetTrcvSystemData(uint8 Transceiver, P2CONST(uint32, AUTOMATIC, CANTRCV_CONST) TrcvSysData);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_EnableCanBusWakeUp(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_EnableLocalWakeUp(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetCanTrcvMode(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_VerifyData(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_GetMainStatus(uint8 Transceiver, P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) mainStatus);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_GetDeviceID(uint8 Transceiver, P2VAR(uint8, AUTOMATIC, CANTRCV_APPL_DATA) deviceId);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_ClearPreSleepAllStatus(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_Driver_SetPreSleepCondition(uint8 Transceiver);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_IsBusWakeUp(uitn8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isBusWakeup);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Tja1145_Driver_IsPinWakeup(uint8 Transceiver, P2VAR(boolean, AUTOMATIC, AUTOMATIC) isBusWakeup);

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

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Tja1145_CB_WakeupByBus(uint8 Transceiver);

#endif /* CANTRCV_Tja1145_H_ */
