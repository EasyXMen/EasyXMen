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
**  FILENAME    : LinIf_Cbk.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Callback declaration supplied by LinSM                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef LINIF_CBK_H
#define LINIF_CBK_H

#include "LinIf_Cfg.h"
#include "Lin_GeneralTypes.h"
#include "ComStack_Types.h"
#include "EcuM_Types.h"
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
#include "LinTrcv.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LINIF_CBK_H_VENDOR_ID                   (uint16)62
#define LINIF_CBK_H_MODULE_ID                   (uint16)141
#define LINIF_CBK_H_AR_RELEASE_MAJOR_VERSION    (uint8)4
#define LINIF_CBK_H_AR_RELEASE_MINOR_VERSION    (uint8)5
#define LINIF_CBK_H_AR_RELEASE_REVISION_VERSION (uint8)0

#define LINIF_CBK_H_SW_MAJOR_VERSION            (uint8)1
#define LINIF_CBK_H_SW_MINOR_VERSION            (uint8)0
#define LINIF_CBK_H_SW_PATCH_VERSION            (uint8)0

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (LINIF_WAKEUP_SUPPORT == STD_ON)

/******************************************************************************/
/*
 * Brief               The LIN Driver or LIN Transceiver Driver will call this
 *                     function to report the wake up source after the
 *                     successful wakeup detection during CheckWakeup or after
 *                     power on by bus.
 * ServiceId           0x61
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WakeupSource: Source device, which initiated the wakeup
 *                                   event: LIN controller or LIN transceiver.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_WakeupConfirmation(EcuM_WakeupSourceType WakeupSource);

#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)

/******************************************************************************/
/*
 * Brief               The LIN Driver will call this function to report a
 *                     received LIN header. This function is only applicable for
 *                     LIN slave nodes
 * ServiceId           0x78
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Channels.
 * Param-Name[in]      Channel: Identification of the LIN channel.
 *                     Nad: Configured NAD to set as new slave NAD.
 * Param-Name[in/out]  PduPtr: Pointer to PDU providing the received PID and
 *                     pointer to the SDU data buffer as in parameter. Upon
 *                     return, the length, checksum type and frame response type
 *                     are received as out parameter. If the frame response type
 *                     is LIN_FRAMERESPONSE_TX, then the SDU data buffer
 *                     contains the transmission data.
 * Param-Name[out]     None
 * Return              E_OK: Request has been accepted.
 *                     E_NOT_OK: Request has not been accepted, development or
 *                               production error occurred.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
LinIf_HeaderIndication(NetworkHandleType Channel, P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/******************************************************************************/
/*
 * Brief               The LIN Driver will call this function to report a
 *                     successfully received response and provides the reception
 *                     data to the LIN Interface. This function is only
 *                     applicable for LIN slave nodes.
 * ServiceId           0x79
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Channels.
 * Param-Name[in]      Channel: Identification of the LIN channel.
 *                     Lin_SduPtr: pointer to a shadow buffer or memory mapped
 *                     LIN Hardware receive buffer where the current SDU is
 *                     stored. This pointer is only valid if the response is
 *                     received.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
LinIf_RxIndication(NetworkHandleType Channel, P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/******************************************************************************/
/*
 * Brief               The LIN Driver will call this function to report a
 *                     successfully transmitted response.
 *                     This function is only applicable for LIN slave nodes.
 * ServiceId           0x7a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Channels.
 * Param-Name[in]      Channel: Identification of the LIN channel.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_TxConfirmation(NetworkHandleType Channel);

/******************************************************************************/
/*
 * Brief               The LIN Driver will call this function to report a
 *                     detected error event during header or response processing
 *                     This function is only applicable for LIN slave nodes
 * ServiceId           0x7b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Channels.
 * Param-Name[in]      Channel: Identification of the LIN channel.
 *                     ErrorStatus: Type of detected error.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_LinErrorIndication(NetworkHandleType Channel, Lin_SlaveErrorType ErrorStatus);

#endif

#endif /* LINIF_CBK_H */
