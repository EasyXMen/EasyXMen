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
 **  @file               : LinTp.c
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : API definitions of LinTp
 **
 ***********************************************************************************************************************/
/* PRQA S 3415 EOF */ /* VL_LinTp_3415 */
/* PRQA S 1510 EOF */ /* VL_LinTp_1510 */
/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#if (STD_ON == LINIF_MASTER_SUPPORT)
#include "LinTp_Master.h"
#endif
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinTp_Slave.h"
#endif
#include "LinTp_Internal.h"
#include "LinIf_Internal.h"
#include "LinTp.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
LINTP_LOCAL_INLINE boolean LinTp_ValidateInitStatus(uint8 appId, ApplicationType applicationId);
LINTP_LOCAL_INLINE boolean LinTp_ValidatePointer(uint8 appId, uint8 errorId, const void* pointer);
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
LINTP_LOCAL_INLINE
boolean LinTp_ValidateRxPduPartition(uint8 appId, ApplicationType applicationId, const LinTp_RxNSduType* rx);
LINTP_LOCAL_INLINE boolean
    LinTp_ValidateTxPduPartition(uint8 appId, ApplicationType applicationId, const LinTp_TxNSduType* tx);
#endif
#endif

LINTP_LOCAL const LinTp_TxNSduType* LinTp_GetTxNSdu(PduIdType txSduId, ApplicationType applicationId);

LINTP_LOCAL const LinTp_RxNSduType* LinTp_GetRxNSdu(PduIdType rxSduId, ApplicationType applicationId);

LINTP_LOCAL LinIf_NodeTypeType LinTp_GetNodeType(NetworkHandleType ComMChannel);

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* ============================================ internal data definition ============================================ */
#define LINIF_START_SEC_VAR_INIT_PTR
#include "LinIf_MemMap.h"

const LinTp_ConfigType* LinTp_ConfigPtr = NULL_PTR;

#define LINIF_STOP_SEC_VAR_INIT_PTR
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 1503,1532 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */

/* ========================================== external function definition ========================================== */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Initializes the LIN Transport Layer.
 */
void LinTp_Init(const LinTp_ConfigType* ConfigPtr)
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinTp_ValidatePointer(LINTP_INIT_ID, LINIF_E_PARAM_POINTER, ConfigPtr))
#endif
    {
        /*@req <SWS_LinIf_00427> */
        if (NULL_PTR == LinTp_ConfigPtr)
        {
            LinTp_ConfigPtr = ConfigPtr;
        }

#if (STD_ON == LINTP_MASTER_SUPPORT)
        LinTp_MasterInit();
#endif

#if (STD_ON == LINTP_SLAVE_SUPPORT)
        LinTp_SlaveInit();
#endif

        /*@req <SWS_LinIf_00320> */
        /* Set the status of LINTP */
        ApplicationType applicationId = LinIf_GetApplicationID();
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        if (NULL_PTR != LinTp_Status[applicationId])
        {
            *LinTp_Status[applicationId] = LINTP_INIT;
        }
#else
        *LinTp_Status[applicationId] = LINTP_INIT;
#endif
    }
}

/**
 * Requests transmission of a PDU.
 */
Std_ReturnType LinTp_Transmit(PduIdType LinTpTxSduId, const PduInfoType* LinTpTxInfoPtr)
{
    Std_ReturnType          ret           = E_NOT_OK;
    ApplicationType         applicationId = LinIf_GetApplicationID();
    const LinTp_TxNSduType* tx            = LinTp_GetTxNSdu(LinTpTxSduId, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinTp_ValidateInitStatus(LINTP_TRANSMIT_ID, applicationId)
        && LinTp_ValidatePointer(LINTP_TRANSMIT_ID, LINIF_E_PARAM_POINTER, LinTpTxInfoPtr)
        && LinTp_ValidatePointer(LINTP_TRANSMIT_ID, LINIF_E_PARAMETER, tx)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinTp_ValidateTxPduPartition(LINTP_TRANSMIT_ID, applicationId, tx)
#endif
    )
#endif
    {
        LinIf_NodeTypeType nodeType = LinTp_GetNodeType(tx->TxNSduChannelRef);

        if (LINIF_MASTER == nodeType)
        {
#if (STD_ON == LINTP_MASTER_SUPPORT)
            ret = LinTp_MasterTransmit(tx, LinTpTxInfoPtr);
#endif
        }
        else
        {
#if (STD_ON == LINTP_SLAVE_SUPPORT)
            ret = LinTp_SlaveTransmit(tx, LinTpTxInfoPtr);
#endif
        }
    }

    return ret;
}

/**
 * Shutdowns the LIN TP.
 */
void LinTp_Shutdown(void)
{
    ApplicationType applicationId = LinIf_GetApplicationID();
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinTp_ValidateInitStatus(LINTP_SHUTDOWN_ID, applicationId))
#endif
    {
#if (STD_ON == LINTP_MASTER_SUPPORT)
        LinTp_MasterShutdown();
#endif

#if (STD_ON == LINTP_SLAVE_SUPPORT)
        LinTp_SlaveShutdown(applicationId);
#endif

        /*@req <SWS_LinIf_00484>*/
        /* Reset the status of LINTP */
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        if (LinTp_Status[applicationId] != NULL_PTR)
        {
            *LinTp_Status[applicationId] = LINTP_UNINIT;
        }
#else
        *LinTp_Status[applicationId] = LINTP_UNINIT;
#endif
    }
}

/**
 * Request to change a specific transport protocol parameter (e.g. block size).
 */
Std_ReturnType LinTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value)
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    ApplicationType         applicationId = LinIf_GetApplicationID();
    const LinTp_RxNSduType* rx            = LinTp_GetRxNSdu(id, applicationId);

    if (LinTp_ValidateInitStatus(LINTP_CHANGEPARAMETER_ID, applicationId)
        && LinTp_ValidatePointer(LINTP_CHANGEPARAMETER_ID, LINIF_E_PARAMETER, rx)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinTp_ValidateRxPduPartition(LINTP_CHANGEPARAMETER_ID, applicationId, rx)
#endif
    )
#endif
    {
    }
    LINTP_NOUSED(id);
    LINTP_NOUSED(parameter);
    LINTP_NOUSED(value);
    /*@req <SWS_LinIf_00592> */
    return E_NOT_OK;
}

/* PRQA S 1503,1532 -- */

/* ========================================== internal function definition ========================================== */
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief           Validate lintp init status
 * @param[in]       appId: ID of API service in which error is detected
 * @param[in]       applicationId: Current partition id
 * @return          TRUE: Development error not occurred
 * @return          FALSE: Development error occurred
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL_INLINE boolean LinTp_ValidateInitStatus(uint8 appId, ApplicationType applicationId)
{
    boolean ret = TRUE;
    if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        ((NULL_PTR != LinTp_Status[applicationId]) && (NULL_PTR != LinIf_Status[applicationId])) &&
#endif
        ((LINIF_UNINIT == *LinIf_Status[applicationId]) || (LINTP_UNINIT == *LinTp_Status[applicationId])))
    {
        /*@req <SWS_LinIf_00535>,<SWS_LinIf_00687> */
        LinIf_DetReportError(appId, LINIF_E_UNINIT);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief           Check pointer validity
 * @param[in]       appId: ID of API service in which error is detected
 * @param[in]       errorId: ID of error reason
 * @param[in]       pointer: The pointer being tested
 * @return          TRUE: Development error not occurred
 * @return          FALSE: Development error occurred
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL_INLINE boolean LinTp_ValidatePointer(uint8 appId, uint8 errorId, const void* pointer)
{
    boolean ret = TRUE;
    if (NULL_PTR == pointer)
    {
        LinIf_DetReportError(appId, errorId);
        ret = FALSE;
    }
    return ret;
}

#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
/**
 * @brief           Verify the validity of rxpdu partitions
 * @param[in]       appId: ID of API service in which error is detected
 * @param[in]       applicationId: Current partition id
 * @param[in]       rx: Pointer of lintp rx sdu
 * @return          TRUE: Development error not occurred
 * @return          FALSE: Development error occurred
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL_INLINE boolean
    LinTp_ValidateRxPduPartition(uint8 appId, ApplicationType applicationId, const LinTp_RxNSduType* rx)
{
    boolean ret = TRUE;
    if (rx->PduApplicationId != applicationId)
    {
        LinIf_DetReportError(appId, LINIF_E_ERROR_PARTITION);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief           Verify the validity of txpdu partitions
 * @param[in]       appId: ID of API service in which error is detected
 * @param[in]       applicationId: Current partition id
 * @param[in]       tx: Pointer of lintp tx sdu
 * @return          TRUE: Development error not occurred
 * @return          FALSE: Development error occurred
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL_INLINE boolean
    LinTp_ValidateTxPduPartition(uint8 appId, ApplicationType applicationId, const LinTp_TxNSduType* tx)
{
    boolean ret = TRUE;
    if (tx->PduApplicationId != applicationId)
    {
        LinIf_DetReportError(appId, LINIF_E_ERROR_PARTITION);
        ret = FALSE;
    }
    return ret;
}
#endif
#endif

/**
 * @brief           Gets configured TxNSdu pointer by parameter 'txSduId'
 * @param[in]       txSduId: LinTp tx sdu id
 * @param[in]       applicationId: Current partition id
 * @return          retTxNSdu: pointer of lintp tx sdu
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL const LinTp_TxNSduType* LinTp_GetTxNSdu(PduIdType txSduId, ApplicationType applicationId)
{
    const LinTp_TxNSduType* retTxNSdu = NULL_PTR;

    if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        ((NULL_PTR != LinTp_Status[applicationId]) && (NULL_PTR != LinIf_Status[applicationId])) &&
#endif
        ((LINIF_INIT == *LinIf_Status[applicationId]) && (LINTP_INIT == *LinTp_Status[applicationId])))
    {
        const LinTp_TxNSduType* tx = LinTp_GetTxNSduCfg(0u);

        uint16 idx = LinTp_GetTxNSduNum();
        while (idx > 0u)
        {
            if (tx->TxNSduId == txSduId)
            {
                retTxNSdu = tx;
                break;
            }
            tx++;
            idx--;
        }
    }

    return retTxNSdu;
}
/**
 * @brief           Gets configured RxNSdu pointer by parameter 'rxSduId'
 * @param[in]       txSduId: LinTp rx sdu id
 * @param[in]       applicationId: Current partition id
 * @return          retTxNSdu: pointer of lintp rx sdu
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL const LinTp_RxNSduType* LinTp_GetRxNSdu(PduIdType rxSduId, ApplicationType applicationId)
{
    const LinTp_RxNSduType* retRxNSdu = NULL_PTR;

    if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        ((NULL_PTR != LinTp_Status[applicationId]) && (NULL_PTR != LinIf_Status[applicationId])) &&
#endif
        ((LINIF_INIT == *LinIf_Status[applicationId]) && (LINTP_INIT == *LinTp_Status[applicationId])))
    {
        const LinTp_RxNSduType* rx  = LinTp_GetRxNSduCfg(0u);
        uint16                  idx = LinTp_GetRxNSduNum();

        while (idx > 0u)
        {
            if (rx->RxNSduId == rxSduId)
            {
                retRxNSdu = rx;
            }
            rx++;
            idx--;
        }
    }

    return retRxNSdu;
}

/**
 * @brief           Gets the configuration pointer by transmit 'NSduId'
 * @param[in]       ComMChannel: Identification of the ComM channel
 * @return          retNodeType: LinIf node type
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINTP_LOCAL LinIf_NodeTypeType LinTp_GetNodeType(NetworkHandleType ComMChannel)
{
    NetworkHandleType  ch;
    LinIf_NodeTypeType retNodeType = LINIF_MASTER;

    for (ch = 0u; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
    {
        if (LinIf_GetComMNetwork(ch) == ComMChannel)
        {
            retNodeType = LinIf_GetNodeType(ch);
        }
    }
    return retNodeType;
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824 -- */

#endif
