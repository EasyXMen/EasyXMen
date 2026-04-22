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
 **  @file               : DoIP_Internal_TCP_Common.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Implement the common part of TCP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_TCP_COMMON_H_
#define DOIP_INTERNAL_TCP_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

#define DOIP_LA_LEN 2u /**< Length of logic address */

/* ra */

#define DOIP_RA_REQ_MSG_PAYLOAD_LEN          7u  /**< Length of routine activation request payload */
#define DOIP_RA_REQ_MSG_PAYLOAD_LEN_WITH_OEM 11u /**< Length of routine activation with oem request payload */

/* alive check */

#define DOIP_ALIVE_CHECK_RSP_MSG_LEN 2u /**< Length of alive check response message */

/* diagnostic */

#define DOIP_MIN_DIAG_MSG_LEN 5u /**< Minimum length of a diagnostic message */

/* ========================================= external function declarations ========================================= */

/* cfg */

/**
 * @brief       Gets the PB configuration for the specified connection.
 * @param[in]   id: Traverse index.
 * @return      DoIP_PBConnectionCfgType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 5016 ++ */ /* VL_DoIP_5016 */
const DoIP_PBConnectionCfgType* DOIP_TCP_PBCON_CFG(uint8 id);
/* PRQA S 5016 -- */

/**
 * @brief       Get target address.
 * @param[in]   sa: Source address.
 * @param[out]  ta: Tareget address.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTaFromChannel(uint16 sa, uint16* ta);

/**
 * @brief       Get PduRRxPdu.
 * @param[in]   sa: Source address.
 * @param[in]   ta: Tareget address.
 * @param[out]  pdurRxPdu: PduRxPdu.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetPduRRxPdu(uint16 sa, uint16 ta, const DoIP_PBPduRRxPduType** pdurRxPdu);

/* connect's status */

/**
 * @brief       Get index of DoIP_TcpConnStatus.
 * @param[in]   soConId: socket.
 * @param[out]  index: index of DoIP_TcpConnStatus.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySoConId(SoAd_SoConIdType soConId, uint8* index);

#ifdef __cplusplus
}
#endif
#endif
