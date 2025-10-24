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
 **  @file        : LinIf_Cbk.h
 **  @author      : HuRongbo
 **  @date        :
 **  @vendor      : isoft
 **  @description : Callback declaration supplied by LinSM
 **
 ***********************************************************************************************************************/

#ifndef LINIF_CBK_H_
#define LINIF_CBK_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "EcuM_Types.h"
#include "Lin_GeneralTypes.h"
#include "LinIf_Cfg.h"
#include "EcuM.h"
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#include "LinTrcv.h"
#endif
/* ===================================================== macros ===================================================== */

/* ========================================= external function declarations ========================================= */
#if (LINIF_WAKEUP_SUPPORT == STD_ON)

/**
 * @brief               The LIN Driver or LIN Transceiver Driver will call this function to report the
 *                      wake up source after the successful wakeup detection during CheckWakeup or after
 *                      power on by bus.
 * @param[in]           WakeupSource: Source device, which initiated the wakeup event:
 *                                    LIN controller or LIN transceiver.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-56887
 */
void LinIf_WakeupConfirmation(EcuM_WakeupSourceType WakeupSource);

#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)

/**
 * @brief               The LIN Driver will call this function to report a received LIN header. This function
 *                      is only applicable for LIN slave nodes
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[in]           Nad:    Configured NAD to set as new slave NAD.
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU data buffer
 *                              as in parameter. Upon return, the length, checksum type and frame response type
 *                              are received as out parameter. If the frame response type is LIN_FRAMERESPONSE_TX,
 *                              then the SDU data buffer contains the transmission data.
 * @return              E_OK: Request has been accepted.
 * @return              E_NOT_OK: Request has not been accepted, development or production error occurred.
 * @reentrant           Reentrant for different Channels.
 * @synchronous         TRUE
 * @trace               CPD-56889
 */
Std_ReturnType LinIf_HeaderIndication(NetworkHandleType Channel, Lin_PduType* PduPtr);

/**
 * @brief               The LIN Driver will call this function to report a successfully received response and
 *                      provides the reception data to the LIN Interface. This function is only applicable
 *                      for LIN slave nodes.
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[in]           Lin_SduPtr: pointer to a shadow buffer or memory mapped LIN Hardware receive buffer
 *                                  where the current SDU is stored. This pointer is only valid if the response
 *                                  is received.
 * @reentrant           Reentrant for different Channels.
 * @synchronous         TRUE
 * @trace               CPD-56890
 */
void LinIf_RxIndication(NetworkHandleType Channel, uint8* Lin_SduPtr);

/**
 * @brief               The LIN Driver will call this function to report a successfully transmitted response.
 *                      This function is only applicable for LIN slave nodes.
 * @param[in]           Channel: Identification of the LIN channel.
 * @reentrant           Reentrant for different Channels.
 * @synchronous         TRUE
 * @trace               CPD-56891
 */
void LinIf_TxConfirmation(NetworkHandleType Channel);

/**
 * @brief               The LIN Driver will call this function to report a detected error event during header
 *                      or response processing This function is only applicable for LIN slave nodes
 * @param[in]           Channel: Identification of the LIN channel.
 * @param[in]           ErrorStatus: Type of detected error.
 * @reentrant           Reentrant for different Channels.
 * @synchronous         TRUE
 * @trace               CPD-56892
 */
void LinIf_LinErrorIndication(NetworkHandleType Channel, Lin_SlaveErrorType ErrorStatus);

#endif

#endif
