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
 **  @file               : XcpOnCan.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Transport Layer:XCP on CAN
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#if (XCP_ON_CAN_ENABLE == STD_ON)
#define XCP_START_SEC_CODE_FAST
#include "Xcp_MemMap.h"
/**
 * Processes the received CAN message. This function checks for errors in the
 * received PDU and, if error detection is enabled, verifies the PDU. It then
 * calls the `Xcp_RxIndication` function to handle the received data. If the
 * message length does not match the expected length, it is ignored. This
 * function is intended for internal use.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 1532 -- */
{
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (!Xcp_RxIndCheckDetErr(RxPduId, PduInfoPtr))
#else
    XCP_AVOID_WARNNING(RxPduId);
#endif /* XCP_DEV_ERROR_DETECT == STD_ON */
    {
#if (XCP_CAN_MAX_DLC_REQUIRED == STD_ON)
        if (XCP_CAN_MAX_DLC == PduInfoPtr->SduLength)
#endif /*XCP_CAN_MAX_DLC_REQUIRED == STD_ON*/
        {
            Xcp_RxIndication(PduInfoPtr->SduLength, PduInfoPtr->SduDataPtr);
        }
    }
    return;
}

/**
 * Confirms the transmission of a CAN message. This function checks for errors
 * in the transmission process. If error detection is enabled and no errors
 * are detected, it calls the `Xcp_TxConfirmation` function to handle the
 * transmission confirmation. This function is intended for internal use.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    if (E_OK == result)
    {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
        if (!Xcp_TxIndCheckDetErr())
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/
        {
            Xcp_TxConfirmation(TxPduId);
        }
    }
    return;
}
#define XCP_STOP_SEC_CODE_FAST
#include "Xcp_MemMap.h"
#endif /*XCP_ON_CAN_ENABLE == STD_ON*/

/* ========================================== internal function definitions ========================================= */
