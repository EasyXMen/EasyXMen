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
 **  @file               : XcpOnEth.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Transport Layer:XCP on Ethernet
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* PRQA S 0553 EOF */ /* VL_Xcp_0553 */
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
/* ========================================== external function definitions ========================================= */
#define XCP_START_SEC_CODE_FAST
#include "Xcp_MemMap.h"
/**
 * Processes the received Ethernet message. This function checks for errors in
 * the received PDU and verifies the packet counter to detect missing packets.
 * It then extracts the length and data from the PDU and calls the
 * `Xcp_RxIndication` function to handle the received data. This function is
 * intended for internal use.
 */
void Xcp_SoAdRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    uint16 counter = 0u;
    uint16 len     = 0u;

#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if ((boolean)TRUE != Xcp_RxIndCheckDetErr(RxPduId, PduInfoPtr))
#else
    XCP_AVOID_WARNNING(RxPduId);
#endif /* XCP_DEV_ERROR_DETECT == STD_ON */
    {
        Xcp_CopyU1BufferToU2(&(PduInfoPtr->SduDataPtr[XCP_CONST_2]), &counter, LOW_BYTE_FIRST);
        /*XCP Header(Ethernet) length is 4 bytes
         * byte0/byte1:LEN:LEN is the number of bytes in the original XCP Packet
         * byte2/byte3:CTR:The CTR value in the XCP Header allows detection of missing Packets.
         * */
        if (counter != Xcp_ChannelCommonData.EthRxCounter)
        {
            /*Missing packets detect*/
            /*TODO*/
        }
        Xcp_ChannelCommonData.EthRxCounter = counter + 1u;
        Xcp_CopyU1BufferToU2(&(PduInfoPtr->SduDataPtr[0u]), &len, LOW_BYTE_FIRST);
        Xcp_RxIndication(len, &(PduInfoPtr->SduDataPtr[4u]));
    }
    return;
}

/**
 * Confirms the transmission of an Ethernet message. This function checks for
 * errors in the transmission process. If error detection is enabled and no
 * errors are detected, it calls the `Xcp_TxConfirmation` function to handle
 * the transmission confirmation. This function is intended for internal use.
 */
void Xcp_SoAdTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    if (E_OK == result)
    {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
        if ((boolean)FALSE == Xcp_TxIndCheckDetErr())
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/
        {
            Xcp_TxConfirmation(TxPduId);
        }
    }
    return;
}
#define XCP_STOP_SEC_CODE_FAST
#include "Xcp_MemMap.h"
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/

/* ========================================== internal function definitions ========================================= */
