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
**  FILENAME    : LinIf_Internal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Types.h"
#include "LinTp_Internal.h"
#include "LinIf_Internal.h"
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinIf_Slave.h"
#endif
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/******************************************************************************/
/*
 * Brief: Gets the configuration pointer by receive 'NAD'
 * Param-Name[in]: ch: LinIf channel
 *                 Nad: received 'NAD'
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: const LinTp_RxNSduType*
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE)
LinTp_GetRxNSduByNad(NetworkHandleType ch, uint8 Nad)
{
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) rxNSdu = &LINTP_GET_RXNSDU(0u);
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_CONST) retRxNSdu = NULL_PTR;
    uint16 idx = LINTP_GET_RXNSDU_NUM;
    uint8 runtimeNad;
    boolean findFlag = FALSE;
    Std_ReturnType ret;

    while (idx > 0u)
    {
        if (rxNSdu->LinTpRxNSduChannelRef == LINIF_GET_COMM_NETWORK(ch))
        {
            if (((rxNSdu->LinTpRxNSduNad == LINTP_FUNCTIONAL_REQ_NAD)
                 || (rxNSdu->LinTpRxNSduNad == LINTP_BROADCAST_REQ_NAD))
                && (rxNSdu->LinTpRxNSduNad == Nad))
            {
                findFlag = TRUE;
            }
            else
            {
#if (STD_ON == LINIF_SLAVE_SUPPORT)
                if (LINIF_SLAVE == LINIF_GET_NODETYPE(ch))
                {
                    ret = LinIf_SlaveGetConfiguredNAD(ch, &runtimeNad);
                    if ((E_OK == ret) && (runtimeNad == Nad))
                    {
                        findFlag = TRUE;
                    }
                }
                else
#endif /* STD_ON == LINIF_SLAVE_SUPPORT */
                {
                    if (rxNSdu->LinTpRxNSduNad == Nad)
                    {
                        findFlag = TRUE;
                    }
                }
            }

            if (TRUE == findFlag)
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

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#endif
