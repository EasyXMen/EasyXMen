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
 **  @file               : LinIf_Internal.c
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinTp internal header for internal API definitions
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Types.h"
#include "LinTp_Internal.h"
#include "LinIf_Internal.h"
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinIf_Slave.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */

/* ============================================ internal data definition ============================================ */

/* ============================================ external data definition ============================================ */

/* ========================================== external function definition ========================================== */

/* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr */

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Gets the configuration pointer by receive 'NAD'
 */
/* PRQA S 1532,6030 ++ */ /* VL_QAC_OneFunRef,VL_MTR_LinTp_STMIF */
const LinTp_RxNSduType* LinTp_GetRxNSduByNad(NetworkHandleType ch, uint8 Nad)
{
    const LinTp_RxNSduType* rxNSdu    = LinTp_GetRxNSduCfg(0u);
    const LinTp_RxNSduType* retRxNSdu = NULL_PTR;
    uint16                  idx       = LinTp_GetRxNSduNum();
    boolean                 findFlag  = FALSE;

    while (idx > 0u)
    {
        if (rxNSdu->RxNSduChannelRef == LinIf_GetComMNetwork(ch))
        {
            if (((rxNSdu->RxNSduNad == LINTP_FUNCTIONAL_REQ_NAD) || (rxNSdu->RxNSduNad == LINTP_BROADCAST_REQ_NAD))
                && (rxNSdu->RxNSduNad == Nad))
            {
                findFlag = TRUE;
            }
            else
            {
#if (STD_ON == LINIF_SLAVE_SUPPORT)
                if (LINIF_SLAVE == LinIf_GetNodeType(ch))
                {
                    uint8          runtimeNad = 0u;
                    Std_ReturnType ret        = LinIf_SlaveGetConfiguredNAD(ch, &runtimeNad);
                    if ((E_OK == ret) && (runtimeNad == Nad))
                    {
                        findFlag = TRUE;
                    }
                }
                else
#endif
                {
                    if (rxNSdu->RxNSduNad == Nad)
                    {
                        findFlag = TRUE;
                    }
                }
            }

            if (findFlag)
            {
                retRxNSdu = rxNSdu;
                break;
            }
        }

        idx--;
        rxNSdu++;
    }
    return retRxNSdu;
}
/* PRQA S 1532,6030 -- */

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824 -- */

#endif
