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
 **  @file               : LinIf_Slave.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinIf slave implementation header file
 **
 ***********************************************************************************************************************/

#ifndef LINIF_SLAVE_H_
#define LINIF_SLAVE_H_

/* =================================================== inclusions =================================================== */
#include "LinIf.h"
#include "LinIf_Internal.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= external function declarations ========================================= */

#if (STD_ON == LINIF_SLAVE_SUPPORT)
/**
 * @brief               Initializes the LinIf slave channel.
 * @param[in]           applicationId: The partition id of LinIf module
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveInit(ApplicationType applicationId);

/**
 * @brief               Transmit request process in slave node.
 * @param[in]           applicationId: The partition id of LinIf module
 * @return              E_OK: Transmit request send success
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_SlaveTransmit(uint16 frameIdx);

/**
 * @brief               Process slave node wakeup request.
 * @param[in]           ch: The channel id of linif
 * @return              E_OK: Slave node wakeup process success
 * @return              E_NOT_OK: Slave node wakeup process faildure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_SlaveWakeUp(NetworkHandleType ch);

/**
 * @brief               Wakeup confirmation process in slave node
 * @param[in]           WakeupSource: LinIf channel wakeup source
 * @param[in]           applicationId: The partition id of linif channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveWakeupConfirmation(EcuM_WakeupSourceType WakeupSource, ApplicationType applicationId);

/**
 * @brief               Copy PID table to provide 'PdiBuffer'
 * @param[in]           ch: The channel id of linif
 * @param[inout]        PidBuffer: Saves the PID of the linif slave frame
 * @param[inout]        PidBufferLength: Saves the actual number of frames from linif nodes
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveGetPIDTable(NetworkHandleType ch, Lin_FramePidType* PidBuffer, uint8* PidBufferLength);

/**
 * @brief               Set 'PdiBuffer' pointed PID table to internal PID table.
 * @param[in]           ch: The channel id of linif
 * @param[in]           PidBuffer: Saves the PID of the linif slave frame
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveSetPIDTable(NetworkHandleType ch, const Lin_FramePidType* PidBuffer);

/**
 * @brief               Get the current configured NAD.
 * @param[in]           ch: The channel id of linif
 * @param[inout]        Nad: Saves the configure nad of the linif slave channel
 * @return              E_OK: Get configure nad success
 * @return              E_NOT_OK: Get configure nad faildure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_SlaveGetConfiguredNAD(NetworkHandleType ch, uint8* Nad);

/**
 * @brief               Sets the current configured NAD.
 * @param[in]           ch: The channel id of linif
 * @param[in]           Nad: Saves the configure nad of the linif slave channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveSetConfiguredNAD(NetworkHandleType ch, uint8 Nad);

/**
 * @brief               Header receive process in slave node.
 * @param[in]           ch: The channel id of linif
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                                data buffer as in parameter
 * @return              E_OK: Request has been accepted.
 * @return              E_NOT_OK: Request has not been accepted
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
Std_ReturnType LinIf_SlaveHeaderIndication(NetworkHandleType ch, Lin_PduType* PduPtr);

/**
 * @brief               Receive indication process in slave node.
 * @param[in]           ch: The channel id of linif
 * @param[in]           Lin_SduPtr: Pointer to pointer to a shadow buffer or memory mapped LIN
                                    Hardware receive buffer where the current SDU is stored.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveRxIndication(NetworkHandleType ch, uint8* Lin_SduPtr);

/**
 * @brief               Tx confirmation process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveTxConfirmation(NetworkHandleType ch);

/**
 * @brief               Lin error indication process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           ErrorStatus: Type of detected error
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveLinErrorIndication(NetworkHandleType ch, Lin_SlaveErrorType ErrorStatus);

/**
 * @brief               Go-to-sleep process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
void LinIf_SlaveGotoSleep(NetworkHandleType ch);

/**
 * @brief           LinIf slave main function
 * @param[in]       channel: Identification of the LinIf channel
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
void LinIf_SlaveMainHandle(NetworkHandleType ch);

#endif

#endif
