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
**  FILENAME    : XcpOnCan.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Transport Layer:XCP on CAN                                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Internal.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (XCP_ON_CAN_ENABLE == STD_ON)
#define XCP_START_SEC_CANIFRXINDICATION_CALLBACK_CODE
#include "Xcp_MemMap.h"
/******************************************************************************/
/*
 * @brief               <Xcp callback Function>
 *
 * <Indication of a received PDU from a lower layer communication interface module.> .
 * Service ID   :       <(0x42)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant for different PduIds. Non reentrant for the same PduId.>
 * @param[in]           <RxPduId>/<PduInfoPtr>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CANIFRXINDICATION_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Xcp_CanIfRxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if ((boolean)TRUE != Xcp_RxIndCheckDetErr(RxPduId, PduInfoPtr))
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
#define XCP_STOP_SEC_CANIFRXINDICATION_CALLBACK_CODE
#include "Xcp_MemMap.h"

#define XCP_START_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
#include "Xcp_MemMap.h"
/******************************************************************************/
/*
 * @brief               <Xcp callback Function>
 *
 * <The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PD.>
 * . Service ID   :       <(0x40)> Sync/Async   :       <Synchronous> Reentrancy           <Reentrant for different
 * PduIds. Non reentrant for the same PduId.>
 * @param[in]           <TxPduId>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CANIFTXCONFIRMATION_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Xcp_CanIfTxConfirmation(PduIdType TxPduId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if ((boolean)FALSE == Xcp_TxIndCheckDetErr())
#endif /*XCP_DEV_ERROR_DETECT == STD_ON*/
    {
        Xcp_TxConfirmation(TxPduId);
    }
    return;
}
#define XCP_STOP_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
#include "Xcp_MemMap.h"
#endif /*XCP_ON_CAN_ENABLE == STD_ON*/
