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
**  FILENAME    : LinIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for LinIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>   <REVISION LOG>
 * V2.0.0      20200804   HuRongbo    R19_11 LinTp initial version.
 * V2.0.1      20211124   HuRongbo    Modify LinTp_SlaveAbortTxRxAndNotifyFailToUpper
 *                                    Change Dcm API to PduR
 *             20220711   HuRongbo    QAC check issue fix
 * V2.0.2      20221114   HuRongbo
 *    1> Resolve UNINIT state config data access in LinTp_GetTxNSdu()
 *       (JIRA: PRD0032020-233)
 * V2.0.3      20221114   HuRongbo
 *    1> Change LinTp_MasterRTData[] from static to macro LINTP_LOCAL
 *    2> Change private variable keyword from static to macro LINTP_LOCAL
 * V2.0.4      20221215   ZhaoTong    fix PH-PRD-QC-004-2022PRD0042022-357
 * V2.0.5      20230322   ZhaoTong
 *    1> Change the define of type LinTp_ConfigType and LinTp_ChannelConfigType
 * V2.0.6      20230915   ZhaoTong
 *    1> Bug fix CPT-6932.
 *       It's necessary to add a macro switch When referencing the Master/Slave header file
 * V2.0.7      20230920   ZhaoTong    Bug fix CPT-6946
 *    1> Add two macro for Master/Slave of TP separately.
 * V2.0.8      20240105   ZhaoTong    Muliticore adapt
 * V2.0.9      20240705   ZhaoTong    Add macro condition for the call to API BswM_LinTp_RequestMode.
 * V2.0.10     20240723   ZhaoTong    Receiving a functional transmission in slave node shall not respond to any slave
 * response header.
 * V2.0.11     20240913   ZouZhiJia    1.Bug fix CPT-10293
 *                                     2.Delete LinTp_SlaveAbortTxAndNotifyFailToUpper
 *                                     3.Delete LinTp_SlaveAbortRxAndNotifyFailToUpper
 * V2.0.12     20240825   ZhaoTong    fix bug CPT-10610
 * V2.0.13     20241029   ZhaoTong    fix bug CPT-11007
 * V2.0.14     20241122   ZhaoTong    fix bug CPT-10961
 */

/*******************************************************************************
**                      Version Information Definitions                       **
*******************************************************************************/
#define LINTP_C_SW_MAJOR_VERSION 2U
#define LINTP_C_SW_MINOR_VERSION 0U
#define LINTP_C_SW_PATCH_VERSION 14U

/*******************************************************************************
**                               Includes                                     **
*******************************************************************************/
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#if (STD_ON == LINIF_MASTER_SUPPORT)
#include "LinTp_Master.h"
#endif /*STD_ON == LINIF_MASTER_SUPPORT*/
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinTp_Slave.h"
#endif /*STD_ON == LINIF_SLAVE_SUPPORT*/
#include "LinTp_Internal.h"
#include "LinIf_Internal.h"

/*******************************************************************************
**                             Version Check                                  **
*******************************************************************************/
#if (LINTP_C_SW_MAJOR_VERSION != LINTP_H_SW_MAJOR_VERSION)
#error "LinIf.c : Mismatch in Specification Major Version"
#endif
#if (LINTP_C_SW_MINOR_VERSION != LINTP_H_SW_MINOR_VERSION)
#error "LinIf.c : Mismatch in Specification Minor Version"
#endif
#if (LINTP_C_SW_PATCH_VERSION != LINTP_H_SW_PATCH_VERSION)
#error "LinIf.c : Mismatch in Specification Patch Version"
#endif

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

static FUNC(P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE) LinTp_GetTxNSdu(PduIdType txSduId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
static FUNC(P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE) LinTp_GetRxNSdu(PduIdType rxSduId);
#endif

static FUNC(LinIf_NodeTypeType, LINIF_CODE) LinTp_GetNodeType(NetworkHandleType ComMChannel);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define LINIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "LinIf_MemMap.h"
/* The status of LINTP */
VAR(LinTp_StateType, LINIF_VAR) LinTp_Status = LINTP_UNINIT;

#define LINIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_VAR_INIT_PTR
#include "LinIf_MemMap.h"
/* Global configuration pointer of LINTP */
P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_APPL_CONST)
LinTp_ConfigPtr = NULL_PTR;

#define LINIF_STOP_SEC_VAR_INIT_PTR
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Public Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the LIN Transport Layer.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the LIN Transport Protocol
 *                                configuration
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, LINIF_CODE)
LinTp_Init(P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == ConfigPtr)
    {
        LinIf_Det_ReportError(LINTP_INIT_ID, LINIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        /*@req <SWS_LinIf_00427> */
        LinTp_ConfigPtr = ConfigPtr;

#if (STD_ON == LINTP_MASTER_SUPPORT)
        LinTp_MasterInit();
#endif

#if (STD_ON == LINTP_SLAVE_SUPPORT)
        LinTp_SlaveInit();
#endif

        /*@req <SWS_LinIf_00320> */
        /* Set the status of LINTP */
        LinTp_Status = LINTP_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               Requests the transfer of segmented data.
 * ServiceId           0x53
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      LinTpTxSduId: This parameter contains the unique
 *                                   identifier of the N-SDU to be transmitted
 *                     LinTpTxInfoPtr: A pointer to a structure with N-SDU
 *                                     related data
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_OK: The request can be started successfully
 *                     E_NOT_OK: The request can not be started
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
/* PRQA S 1503,1532 ++ */ /* MISRA Rule 2.1,8.7 */
LinTp_Transmit(PduIdType LinTpTxSduId, P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxInfoPtr)
/* PRQA S 1503,1532 -- */ /* MISRA Rule 2.1,8.7 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST)
    tx = LinTp_GetTxNSdu(LinTpTxSduId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    boolean detNoErr = TRUE;
    if ((LINTP_UNINIT == LinTp_Status) || (LINIF_UNINIT == LinIf_Status))
    {
        /*@req <SWS_LinIf_00535>,<SWS_LinIf_00687> */
        LinIf_Det_ReportError(LINTP_TRANSMIT_ID, LINIF_E_UNINIT);
        detNoErr = FALSE;
    }
    if (NULL_PTR == LinTpTxInfoPtr)
    {
        /*@req <SWS_LinIf_00574>*/
        LinIf_Det_ReportError(LINTP_TRANSMIT_ID, LINIF_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (NULL_PTR == tx)
    {
        /*@req <SWS_LinIf_00576>*/
        LinIf_Det_ReportError(LINTP_TRANSMIT_ID, LINIF_E_PARAMETER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
        LinIf_NodeTypeType nodeType = LinTp_GetNodeType(tx->LinTpTxNSduChannelRef);

        if (LINIF_MASTER == nodeType)
        {
#if (STD_ON == LINTP_MASTER_SUPPORT)
            ret = LinTp_MasterTransmit(tx, LinTpTxInfoPtr);
#endif /* STD_ON == LINTP_MASTER_SUPPORT */
        }
        else
        {
#if (STD_ON == LINTP_SLAVE_SUPPORT)
            ret = LinTp_SlaveTransmit(tx, LinTpTxInfoPtr);
#endif /* STD_ON == LINTP_SLAVE_SUPPORT */
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Shutdowns the LIN TP
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Shutdown(void) /* PRQA S 1503 */
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if ((LINTP_UNINIT == LinTp_Status) || (LINIF_UNINIT == LinIf_Status))
    {
        /*@req <SWS_LinIf_00535>,<SWS_LinIf_00687> */
        LinIf_Det_ReportError(LINTP_SHUTDOWN_ID, LINIF_E_UNINIT);
    }
    else
#endif
    {
#if (STD_ON == LINTP_MASTER_SUPPORT)
        LinTp_MasterShutdown();
#endif /* STD_ON == LINTP_MASTER_SUPPORT */

#if (STD_ON == LINTP_SLAVE_SUPPORT)
        LinTp_SlaveShutdown();
#endif /* STD_ON == LINTP_SLAVE_SUPPORT */

        /*@req <SWS_LinIf_00484>*/
        /* Reset the status of LINTP */
        LinTp_Status = LINTP_UNINIT;
    }
}

/******************************************************************************/
/*
 * Brief               A dummy method introduced for interface compatibility
 * ServiceId           0x4b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id:        Identifier of the received N-SDU on which the
 *                                reception parameter has to be changed.
 *                     parameter: The selected parameter that the request shall
 *                                change (STmin).
 *                     value:     The new value of the parameter.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              E_NOT_OK:  request is not accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinTp_ChangeParameter(/* PRQA S 1503 */
                      PduIdType id,
                      TPParameterType parameter, /* PRQA S 3206 */
                      uint16 value               /* PRQA S 3206 */
)
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    const LinTp_RxNSduType* rx = LinTp_GetRxNSdu(id);
    if ((LINTP_UNINIT == LinTp_Status) || (LINIF_UNINIT == LinIf_Status))
    {
        /*@req <SWS_LinIf_00535>,<SWS_LinIf_00687> */
        LinIf_Det_ReportError(LINTP_CHANGEPARAMETER_ID, LINIF_E_UNINIT);
    }
    if (NULL_PTR == rx)
    {
        /*@req <SWS_LinIf_00578>*/
        LinIf_Det_ReportError(LINTP_CHANGEPARAMETER_ID, LINIF_E_PARAMETER);
    }
#endif
    LINTP_NOUSED(id);
    LINTP_NOUSED(parameter);
    LINTP_NOUSED(value);
    /*@req <SWS_LinIf_00592> */
    return E_NOT_OK;
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               Gets configured TxNSdu pointer by parameter 'txSduId'
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txSduId: tx sdu id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Configured TxNSdu address(Not found return NULL_PTR)
 */
/******************************************************************************/
static FUNC(P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE) LinTp_GetTxNSdu(PduIdType txSduId)
{
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) retTxNSdu = NULL_PTR;
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) tx;
    uint16 idx;

    if (LINTP_INIT == LinTp_Status)
    {
        tx = &LINTP_GET_TXNSDU(0u);

        idx = LINTP_GET_TXNSDU_NUM;
        while (idx > 0u)
        {
            if (tx->LinTpTxNSduId == txSduId)
            {
                retTxNSdu = tx;
            }
            tx++;
            idx--;
        }
    }

    return retTxNSdu;
}

/******************************************************************************/
/*
 * Brief               Gets configured RxNSdu pointer by parameter 'rxSduId'
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      rxSduId: rx sdu id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Configured RxNSdu address(Not found return NULL_PTR)
 */
/******************************************************************************/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
static FUNC(P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST), LINIF_CODE) LinTp_GetRxNSdu(PduIdType rxSduId)
{
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) retRxNSdu = NULL_PTR;

    if (LINTP_INIT == LinTp_Status)
    {
        P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST)
        rx = &LINTP_GET_RXNSDU(0u);
        uint16 idx = LINTP_GET_RXNSDU_NUM;

        while (idx > 0u)
        {
            if (rx->LinTpRxNSduId == rxSduId)
            {
                retRxNSdu = rx;
            }
            rx++;
            idx--;
        }
    }

    return retRxNSdu;
}
#endif

/******************************************************************************/
/*
 * Brief: Gets the configuration pointer by transmit 'NSduId'
 * Param-Name[in]: id
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: const LinTp_TxNSduType*
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static FUNC(LinIf_NodeTypeType, LINIF_CODE) LinTp_GetNodeType(NetworkHandleType ComMChannel)
{
    NetworkHandleType ch;
    LinIf_NodeTypeType retNodeType = LINIF_MASTER;

    for (ch = 0u; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
    {
        if (LINIF_GET_COMM_NETWORK(ch) == ComMChannel)
        {
            retNodeType = LINIF_GET_NODETYPE(ch);
        }
    }
    return retNodeType;
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif /* LINIF_TP_SUPPORTED == STD_ON */
