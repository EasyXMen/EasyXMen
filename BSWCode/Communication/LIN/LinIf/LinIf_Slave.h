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
********************************************************************************
**                                                                            **
**  FILENAME    : LinIf_Slave.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef LINIF_SLAVE_H
#define LINIF_SLAVE_H

#include "LinIf.h"
#include "LinIf_Internal.h"

#if (STD_ON == LINIF_SLAVE_SUPPORT)
/*******************************************************************************
**                      Macros Definitions                                    **
*******************************************************************************/

/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Initializes the LinIf slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveInit(void);

/******************************************************************************/
/*
 * Brief               Transmit request process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      frameIdx: Frame Index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveTransmit(uint16 frameIdx);

/******************************************************************************/
/*
 * Brief               Wakeup process in slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveWakeUp(NetworkHandleType ch);

/******************************************************************************/
/*
 * Brief: Wakeup confirmation process in slave node
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveWakeupConfirmation(EcuM_WakeupSourceType WakeupSource);

/* PRQA S 3432 ++ */ /* VL_LinIf_MacrosFunctionUsed */

/******************************************************************************/
/*
 * Brief               Copy PID table to provide 'PdiBuffer'
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     PidBuffer: Pointer to existing buffer to which the
 *                                current assigned PID values are copied to.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveGetPIDTable(NetworkHandleType ch, P2VAR(Lin_FramePidType, AUTOMATIC, LINIF_APPL_DATA) PidBuffer);

/******************************************************************************/
/*
 * Brief               Set 'PdiBuffer' pointed PID table to internal PID table.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     PidBuffer: Pointer to buffer which contains the PID
 *                                values to configure.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveSetPIDTable(NetworkHandleType ch, const Lin_FramePidType* PidBuffer);

/******************************************************************************/
/*
 * Brief               Get the current configured NAD.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     Nad: Configured NAD of slave.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SlaveGetConfiguredNAD(NetworkHandleType ch, P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Nad);

/******************************************************************************/
/*
 * Brief               Sets the current configured NAD.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Nad: Configured NAD to set as new slave NAD.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveSetConfiguredNAD(NetworkHandleType ch, uint8 Nad);

/******************************************************************************/
/*
 * Brief               Header receive process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     Nad: Configured NAD of slave.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SlaveHeaderIndication(NetworkHandleType ch, P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr);

/******************************************************************************/
/*
 * Brief               Receive indication process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveRxIndication(NetworkHandleType ch, P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr);

/******************************************************************************/
/*
 * Brief               Tx confirmation process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveTxConfirmation(NetworkHandleType ch);

/* PRQA S 3432 -- */

/******************************************************************************/
/*
 * Brief               Lin error indication process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel.
 *                     ErrorStatus: Type of detected error.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveLinErrorIndication(NetworkHandleType ch, Lin_SlaveErrorType ErrorStatus);

/******************************************************************************/
/*
 * Brief               Go-to-sleep process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveGotoSleep(NetworkHandleType ch);

/******************************************************************************/
/*
 * Brief: LinIf slave main function
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
void LinIf_SlaveMainHandle(NetworkHandleType ch);

#endif

#endif
