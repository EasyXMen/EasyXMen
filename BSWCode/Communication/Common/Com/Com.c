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
**  FILENAME    : Com.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for COM                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* Refer to Com.h */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Com<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:The exception is that the number of loops is configurable.

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 2981 MISRA Rule 2.2 .<br>
    Reason:Init may be necessary for return variant.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 4502 MISRA Rule 10.1 .<br>
    Reason:Necessary type conversions.

    \li PRQA S 4340 MISRA Rule 10.5 .<br>
    Reason:Necessary type conversions.

    \li PRQA S 4304 MISRA Rule 10.5 .<br>
    Reason:Necessary type conversions.

    \li PRQA S 4461 MISRA Rule 10.3 .<br>
    Reason:Necessary data type conversion,Actual will not affect the calculation results

    \li PRQA S 3315 MISRA Rule 16.6 .<br>
    Reason:The switch statement allows only the default actions to run, will not affect the running results.

    \li PRQA S 3673 MISRA Rule 8.13 .<br>
    Reason:The "input" parameter is defined from AUTOSAR Communication Services API.

    \li PRQA S 3677 MISRA Rule 8.13 .<br>
    Reason:The parameter will be modified, it could not be qulified as "const".

    \li PRQA S 4391 MISRA Rule 10.8 .<br>
    Reason:Data conversion to a wider range is necessary for accumulation.

    \li PRQA S 1340 MISRA Rule 17.8 .<br>
    Reason:The const "RxDynSignalLength" parameter will not lead to modification.
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
#include "Com_Cbk.h"
#if (STD_ON == COM_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if ((COM_SW_MAJOR_VERSION != 2u) || (COM_SW_MINOR_VERSION != 1u))
#error "Com: Mismatch in Specification Software Version"
#endif

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define COM_START_SEC_VAR_INIT_8
#include "Com_MemMap.h"
static VAR(Com_StatusType, COM_VAR_POWER_ON_INIT) Com_Status = COM_UNINIT;
#define COM_STOP_SEC_VAR_INIT_8
#include "Com_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#define COM_START_SEC_VAR_INIT_PTR
#include "Com_MemMap.h"
P2CONST(Com_ConfigType, COM_VAR, COM_CONST)
Com_ConfigStd = NULL_PTR;
#define COM_STOP_SEC_VAR_INIT_PTR
#include "Com_MemMap.h"

#if (COM_IPDUGROUP_MAX > 0u)
#define COM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
VAR(boolean, COM_VAR) Com_IpduGroupDMEnable[COM_IPDUGROUP_MAX];
#define COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
VAR(boolean, COM_VAR) Com_IpduGroupEnable[COM_IPDUGROUP_MAX];
#define COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
#endif
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/*Disable all Tx/Rx Pdu state*/
#if (0u < COM_RXIPDU_NUMBER) || (0u < COM_TXIPDU_NUMBER)
static FUNC(void, COM_CODE) Com_SetAllPduRToDisableState(void);
#endif
/* called by Com_Init
 * Init the Pdu Runtime Manager and Buffer*/
static FUNC(void, COM_CODE) Com_PduRTAndBufferInit(void);
/* called by Com_Init
 * Init the Runtime RxSignal/RxGroupSignal Buffer*/
static FUNC(void, COM_CODE) Com_SignalBufferInit(void);
/*MaskNewDifferMaskOld filter RxTimeout Init handle*/
#if (0u < COM_RXMASKNEWDIFFERMASKOLD_NUMBER)
static FUNC(void, COM_CODE) Com_RxMaskNewDifferMaskOldTimeOutInit(void);
#endif
/*RxTimeout init handle*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
static FUNC(void, COM_CODE) Com_RxTimeOutInit(void);
#endif
/*Init the counter of ONE_EVERY_N filter*/
#if (0u < COM_ONEEVERYNFILTERSIGNAL_NUMBER)
static FUNC(void, COM_CODE) Com_OneEveryNCntInit(void);
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service initializes internal and external interfaces and
 *                     variables of the AUTOSAR COM module layer for the further processing.
 *                     After calling this function the inter-ECU communication is still disabled.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Config: Pointer to the COM configuration data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_Init(const Com_ConfigType* config) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (NULL_PTR == config)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INIT_ID, COM_E_PARAM_POINTER);
    }
    else if (COM_UNINIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INIT_ID, COM_E_INIT_FAILED);
    }
    else if (
        (config->ComIpduGroupNum > COM_IPDUGROUP_MAX)
#ifdef COM_PDU_MAX
        || ((config->ComRxPduNum + config->ComTxPduNum) > COM_PDU_MAX)
#endif
    )
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INIT_ID, COM_E_INIT_FAILED);
    }
    else
#endif
    {
        Com_ConfigStd = config;
        Com_PduRTAndBufferInit();
        Com_SignalBufferInit();
        Com_TxSignalTMCStateInit();
#if (0u < COM_IPDUGROUP_MAX)
        /*set the IPdu Group to default disable state,DM disable state*/
        (void)ILib_memset(&Com_IpduGroupEnable[0], 0, COM_IPDUGROUP_MAX);
        (void)ILib_memset(&Com_IpduGroupDMEnable[0], 0, COM_IPDUGROUP_MAX);
#endif  /* 0u < COM_IPDUGROUP_MAX */
        /*set all tx signal group state to don't need to trigger transmit*/
#if (0u < COM_TXSIGNALGROUP_NUMBER)
        Com_TxSignalGroupTriggerFlagInit();
#endif /*0u < COM_TXSIGNALGROUP_NUMBER*/
/*set all rx signal,rx signal group's DM timeout counter to 0*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
        Com_RxTimeOutInit();
#endif
/*used for signal filter ONE_EVERY_N Mode(Rx and Tx)*/
#if (0u < COM_ONEEVERYNFILTERSIGNAL_NUMBER)
        /*used for signal filter ONE_EVERY_N Mode(Rx and Tx)*/
        Com_OneEveryNCntInit();
#endif /* 0u < COM_ONEEVERYNFILTERSIGNAL_NUMBER */
/*used for signal filter MASKED_NEW_DIFFERS_MASKED_OLD Mode(Rx Signals)*/
#if (0u < COM_RXMASKNEWDIFFERMASKOLD_NUMBER)
        /*used for signal filter MASKED_NEW_DIFFERS_MASKED_OLD Mode(Rx Signals)*/
        Com_RxMaskNewDifferMaskOldTimeOutInit();
#endif /* 0u < COM_RXMASKNEWDIFFERMASKOLD_NUMBER */

#if COM_GWMAPPING_NUMBER > 0u
        Com_GwInitDestinationRequest();
#endif
        Com_Status = COM_INIT;
    }
    return;
}
/******************************************************************************/
/*
 * Brief               This service stops the inter-ECU communication. All started
 *                     I-PDU groups are stopped and have to be started again, if
 *                     needed, after Com_Init is called. By a call to Com_DeInit
 *                     the AUTOSAR COM module is put into an not initialized state.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_DeInit(void) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_DEINIT_ID, COM_E_UNINIT);
    }
    else
#endif
    {
#if (0u < COM_IPDUGROUP_MAX)
        /* clear All IPduGroup Active Flag */
        (void)ILib_memset(&Com_IpduGroupEnable[0], 0, COM_IPDUGROUP_MAX);
#endif
#if (0u < COM_RXIPDU_NUMBER) || (0u < COM_TXIPDU_NUMBER)
        Com_SetAllPduRToDisableState();
#endif
        /* set Com_Status */
        Com_Status = COM_UNINIT;
    }
    return;
}

#if (COM_IPDUGROUP_MAX > 0u)
/******************************************************************************/
/*
 * Brief               This service starts I-PDU groups.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ipduGroupVector: I-PDU group vector containing the activation
 *                     state (stopped = 0/started = 1) for all I-PDU groups.
 *                     initialize: flag to request initialization of the I-PDUs which are newly started
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532,3677 ++ */ /* MISRA Rule 8.7,8.13 */
void Com_IpduGroupControl(Com_IpduGroupVector ipduGroupVector, boolean initialize)
/* PRQA S 1532,3677 -- */ /* MISRA Rule 8.7,8.13 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_IPDUGROUPCONTROL_ID, COM_E_UNINIT);
    }
    else
#endif
    {
        uint8 value;
        /*base ipduGroupVector,set the all i-pdu group to enable or disable */
        for (Com_IpduGroupIdType ipduGroupCnt = 0u; ipduGroupCnt < COM_IPDUGROUP_MAX; ++ipduGroupCnt)
        {
            /* PRQA S 2985 ++ */ /* MISRA Rule 2.2 */
            value = ((ipduGroupVector[ipduGroupCnt >> 3u]) >> (ipduGroupCnt & 7u)) & 1u;
            /* PRQA S 2985 -- */ /* MISRA Rule 2.2 */
            Com_IpduGroupEnable[ipduGroupCnt] = 0x01u == value;
        }
/*base i-pdu group state,set all rx pdu to enable or disable.
 * base initialize,select if or not init the rx pdu when state change from disable to enable*/
#if (COM_IPDUGROUP_MAX > 0u) && (COM_RXIPDUGROUP_NUMBER > 0u)
        Com_RxIpduController(initialize);
#endif
/*base i-pdu group state,set all tx pdu to enable or disable.
 * base initialize,select if or not init the tx pdu when state change from disable to enable*/
#if (COM_IPDUGROUP_MAX > 0u) && (COM_TXIPDUGROUP_NUMBER > 0u)
        Com_TxIpduController(initialize);
#endif
    }
    return;
}
#endif /*COM_IPDUGROUP_MAX > 0u*/

#if (COM_IPDUGROUP_MAX > 0u)
/******************************************************************************/
/*
 * Brief               This service enables or disables I-PDU group Deadline Monitoring.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ipduGroupVector: I-PDU group vector containing the activation
 *                     state (stopped = 0/started = 1) for all I-PDU groups.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532,3677 ++ */ /* MISRA Rule 8.7,8.13 */
FUNC(void, COM_CODE)
Com_ReceptionDMControl(Com_IpduGroupVector ipduGroupVector)
/* PRQA S 1532,3677 -- */ /* MISRA Rule 8.7,8.13 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEPTIONDMCONTROL_ID, COM_E_UNINIT);
    }
    else
#endif
    {
        uint8 value;
        /*base ipduGroupVector,set the all i-pdu group's Rx DM to enable or disable */
        for (Com_IpduGroupIdType ipduGroupCnt = 0u; ipduGroupCnt < COM_IPDUGROUP_MAX; ++ipduGroupCnt)
        {
            /* PRQA S 2985 ++ */ /* MISRA Rule 2.2 */
            value = ((ipduGroupVector[ipduGroupCnt >> 3u]) >> (ipduGroupCnt & 7u)) & 1u;
            /* PRQA S 2985 -- */ /* MISRA Rule 2.2 */
            Com_IpduGroupDMEnable[ipduGroupCnt] = 0x01u == value;
        }
/*Set all RxPdu deadline monitoring State and parameter*/
#if (COM_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_RXIPDU_NUMBER)
        Com_RxPduDMCtrHandle();
#endif
    }
    return;
}
#endif /*COM_IPDUGROUP_MAX > 0u*/

#if (COM_IPDUGROUP_MAX > 0u)
/******************************************************************************/
/*
 * Brief               Enables the reception deadline monitoring for the I-PDUs within the given IPDU group.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different I-PDU groups. Non reentrant for the same I-PDU group
 * Param-Name[in]      IpduGroupId: Id of I-PDU group where reception DM shall be enabled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_EnableReceptionDM(Com_IpduGroupIdType IpduGroupId) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
/* Multiple partition check. */
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
    if (GetApplicationID() != Com_GetIPduGrpPartitionId(IpduGroupId))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEPTIONDMCONTROL_ID, COM_E_PARTITION);
    }
    else
#endif
    {
        Com_IpduGroupDMEnable[IpduGroupId] = TRUE;
#if (COM_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_RXIPDU_NUMBER)
        Com_RxPduDMCtrHandle();
#endif
    }
    COM_NOUSED(IpduGroupId);
}
#endif /*COM_IPDUGROUP_MAX > 0u*/

#if (COM_IPDUGROUP_MAX > 0u)
/******************************************************************************/
/*
 * Brief               Disables the reception deadline monitoring for the I-PDUs within the given IPDU group.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different I-PDU groups. Non reentrant for the same I-PDU group.
 * Param-Name[in]      IpduGroupId: Id of I-PDU group where reception DM shall be disabled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_DisableReceptionDM(Com_IpduGroupIdType IpduGroupId) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
/* Multiple partition check. */
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
    if (GetApplicationID() != Com_GetIPduGrpPartitionId(IpduGroupId))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEPTIONDMCONTROL_ID, COM_E_PARTITION);
    }
    else
#endif
    {
        Com_IpduGroupDMEnable[IpduGroupId] = FALSE;
#if (COM_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_RXIPDU_NUMBER)
        Com_RxPduDMCtrHandle();
#endif
    }
    COM_NOUSED(IpduGroupId);
}
#endif /*COM_IPDUGROUP_MAX > 0u*/
/******************************************************************************/
/*
 * Brief               Returns the status of the AUTOSAR COM module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Com_StatusType(COM_UNINIT and COM_INIT)
 *                     COM_UNINIT: the AUTOSAR COM module is not initialized and not usable
 *                     COM_INIT: the AUTOSAR COM module is initialized and usable
 */
/******************************************************************************/
FUNC(Com_StatusType, COM_CODE)
Com_GetStatus(void) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
    return Com_Status;
}

#if (COM_IPDUGROUP_MAX > 0u)
/******************************************************************************/
/*
 * Brief               This service sets all bits of the given Com_IpduGroupVector to 0.
 * ServiceId           0x1c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     ipduGroupVector: I-PDU group vector to be cleared
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_ClearIpduGroupVector(Com_IpduGroupVector ipduGroupVector) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_CLEARIPDUGROUPVECTOR_ID, COM_E_UNINIT);
    }
    else
#endif
    {
        uint8 vectorByteLength = ((COM_IPDUGROUP_MAX - 1u) >> 3u) + 1u;
        (void)ILib_memset(&ipduGroupVector[0], 0, vectorByteLength);
    }
    return;
}
#endif /*COM_IPDUGROUP_MAX > 0u*/
#if (COM_IPDUGROUP_MAX > 0u)
/******************************************************************************/
/*
 * Brief               This service sets the value of a bit in an I-PDU group vector.
 * ServiceId           0x1d
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ipduGroupId: ipduGroup used to identify the corresponding bit in the I-PDU group vector
 *                     bitval: New value of the corresponding bit
 * Param-Name[out]     None
 * Param-Name[in/out]  ipduGroupVector: I-PDU group vector to be modified
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void Com_SetIpduGroup(Com_IpduGroupVector ipduGroupVector, Com_IpduGroupIdType ipduGroupId, boolean bitval)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SETIPDUGROUP_ID, COM_E_UNINIT);
    }
    else if ((ipduGroupId >= COM_IPDUGROUP_MAX) || (ipduGroupId >= Com_ConfigStd->ComIpduGroupNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SETIPDUGROUP_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
        if (GetApplicationID() != Com_GetIPduGrpPartitionId(ipduGroupId))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SETIPDUGROUP_ID, COM_E_PARTITION);
    }
    else
#endif
#endif
    {
        uint8 bitValue = 0x01u << (ipduGroupId & 7u); /* PRQA S 2985 */ /* MISRA Rule 2.2*/

        if (bitval)
        {
            ipduGroupVector[ipduGroupId >> 3u] |= bitValue;
        }
        else
        {
            ipduGroupVector[ipduGroupId >> 3u] &= (uint8)(~bitValue);
        }
    }
    return;
}
#endif /*COM_IPDUGROUP_MAX > 0u*/
/*******************************************************************************
**                      Communication Services                                **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               The service Com_SendSignal updates the signal(include group signal) object
 *                     identified by SignalId with the signal referenced by the SignalDataPtr parameter.
 * ServiceId           0x0a
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant for the same signal. Reentrant for different signals.
 * Param-Name[in]      SignalId: Id of signal to be sent.
 *                     SignalDataPtr: Reference to the signal data to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped
 *                     (or service failed due to development error)
 *                     COM_BUSY: in case the TP-Buffer is locked for large data types handling
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
Com_SendSignal(Com_SignalIdType SignalId, P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNAL_ID, COM_E_UNINIT);
    }
    else if (SignalId >= (Com_ConfigStd->ComTxSignalNum + Com_ConfigStd->ComTxGroupSignalNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNAL_ID, COM_E_PARAM);
    }
    else if (NULL_PTR == SignalDataPtr)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNAL_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (                                                                           \
    (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED) \
    && ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)))
        /* Multiple partition check. */
        uint16 partitionId = (SignalId < COM_TXSIGNAL_NUMBER)
                                 ? Com_GetTxSigPartitionId(SignalId)
                                 : Com_GetTxGrpSigPartitionId(SignalId - COM_TXSIGNAL_NUMBER);
        if (GetApplicationID() != partitionId)
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNAL_ID, COM_E_PARTITION);
        }
        else
#endif
        {
/*the signal is Tx Signal*/
#if (((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER))
            ret = Com_SendSignalHandle(SignalId, SignalDataPtr);
#endif
        }
    }
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}
/******************************************************************************/
/*
 * Brief               The service Com_SendDynSignal updates the signal object identified by SignalId
 *                     with the signal referenced by the SignalDataPtr parameter.
 * ServiceId           0x21
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant for the same signal. Reentrant for different signals.
 * Param-Name[in]      SignalId: Id of signal to be sent.
 *                     SignalDataPtr: Reference to the signal data to be transmitted.
 *                     Length: Length of the dynamic length signal
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,E_NOT_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     E_NOT_OK: in case the Length is greater than the configured ComSignalLength of this sent signal
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to
 * development error) COM_BUSY: in case the TP-Buffer is locked
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
Com_SendDynSignal(Com_SignalIdType SignalId, P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr, uint16 Length)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_UNINIT);
    }
    else if (SignalId >= (Com_ConfigStd->ComTxSignalNum + Com_ConfigStd->ComTxGroupSignalNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM);
    }
    else if (NULL_PTR == SignalDataPtr)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (                                                                           \
    (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED) \
    && ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)))
        /* Multiple partition check. */
        uint16 partitionId = (SignalId < COM_TXSIGNAL_NUMBER)
                                 ? Com_GetTxSigPartitionId(SignalId)
                                 : Com_GetTxGrpSigPartitionId(SignalId - COM_TXSIGNAL_NUMBER);
        if (GetApplicationID() != partitionId)
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARTITION);
        }
        else
#endif
        {
#if (((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER))
            ret = Com_SendDynSignalHandle(SignalId, SignalDataPtr, Length);
#endif
        }
    }
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    COM_NOUSED(Length);
    return ret;
}
/******************************************************************************/
/*
 * Brief               Com_ReceiveSignal copies the data of the signal identified by SignalId to the location specified
 * by SignalDataPtr. ServiceId           0x0b Sync/Async          Synchronous Reentrancy          Non Reentrant for the
 * same signal. Reentrant for different signals. Param-Name[in]      SignalId: Id of signal to be received.
 * Param-Name[out]     SignalDataPtr: Reference to the location where the received signal data shall be stored
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to
 * development error) COM_BUSY: in case the TP-Buffer is locked for large data types handling
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
Com_ReceiveSignal(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNAL_ID, COM_E_UNINIT);
    }
    else if (SignalId >= (Com_ConfigStd->ComRxSignalNum + Com_ConfigStd->ComRxGroupSignalNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNAL_ID, COM_E_PARAM);
    }
    else if (NULL_PTR == SignalDataPtr)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNAL_ID, COM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == COM_DEV_ERROR_DETECT*/
    {
#if (                                                                           \
    (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED) \
    && ((COM_RXSIGNAL_NUMBER > 0u) || ((COM_RXGROUPSIGNAL_NUMBER > 0u) && (COM_RXGROUPSIGNAL_NUMBER > 0u))))
        /* Multiple partition check. */
        uint16 partitionId = (SignalId < COM_RXSIGNAL_NUMBER)
                                 ? Com_GetRxSigPartitionId(SignalId)
                                 : Com_GetRxGrpSigPartitionId(SignalId - COM_RXSIGNAL_NUMBER);
        if (GetApplicationID() != partitionId)
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNAL_ID, COM_E_PARTITION);
        }
        else
#endif
        {
#if (0u < COM_RXIPDU_NUMBER)
            /*the signal is Rx Signal*/
            if (SignalId < COM_RXSIGNAL_NUMBER)
            {
#if (0u < COM_RXSIGNAL_NUMBER)
                ret = Com_ReceiveSignalHandle(SignalId, SignalDataPtr);
#endif /*0u < COM_RXSIGNAL_NUMBER*/
            }
            /*the signal is Rx Group Signal*/
            else
            {
#if (COM_RXSIGNALGROUP_NUMBER > 0u) && (COM_RXGROUPSIGNAL_NUMBER > 0u)
                ret = Com_ReceiveGroupSignalHandle(SignalId, SignalDataPtr);
#endif
            }
#endif /*0u < COM_RXIPDU_NUMBER*/
        }
    }
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}
/******************************************************************************/
/*
 * Brief               Com_ReceiveDynSignal copies the data of the signal identified by SignalId to the
 *                     location specified by SignalDataPtr and stores the length of the dynamical length
 *                     signal at the position given by the Length parameter.
 * ServiceId           0x22
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same signal. Reentrant for different signals.
 * Param-Name[in]      SignalId: Id of signal to be received.
 * Param-Name[out]     SignalDataPtr: reference to the location where the received signal data shall be stored
 * Param-Name[in/out]  Length
 *                     in: maximum length that could be received
 *                     out: length of the dynamic length signal
 * Return              uint8(E_OK,E_NOT_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     E_NOT_OK: in case the Length (as in-parameter) is smaller than the received length of the dynamic
 * length signal COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development
 * error) COM_BUSY: in case the TP-Buffer is locked
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
Com_ReceiveDynSignal(
    Com_SignalIdType SignalId,
    P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    P2VAR(uint16, AUTOMATIC, COM_APPL_CONST) Length) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_UNINIT);
    }
    else if (SignalId >= (Com_ConfigStd->ComRxSignalNum + Com_ConfigStd->ComRxGroupSignalNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARAM);
    }
    else if (NULL_PTR == SignalDataPtr)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (                                                                           \
    (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED) \
    && ((COM_RXSIGNAL_NUMBER > 0u) || ((COM_RXGROUPSIGNAL_NUMBER > 0u) && (COM_RXGROUPSIGNAL_NUMBER > 0u))))
        /* Multiple partition check. */
        uint16 partitionId = (SignalId < COM_RXSIGNAL_NUMBER)
                                 ? Com_GetRxSigPartitionId(SignalId)
                                 : Com_GetRxGrpSigPartitionId(SignalId - COM_RXSIGNAL_NUMBER);
        if (GetApplicationID() != partitionId)
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARTITION);
        }
        else
#endif
        {
#if (0u < COM_RXIPDU_NUMBER)
            /*the signal is Rx Signal*/
            if (SignalId < COM_RXSIGNAL_NUMBER)
            {
#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_SIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))
                ret = Com_ReceiveDynSignalHandle(SignalId, SignalDataPtr, Length);
#endif /* 0u < COM_RXSIGNAL_NUMBER) && (0u < COM_SIGNAL_8BITBUFF_SIZE) && (STD_ON == \
          COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE */
            }
            /*the signal is Rx Group Signal*/
            else
            {
#if (((COM_RXSIGNALGROUP_NUMBER > 0u) && (COM_RXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE)) \
    && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
                ret = Com_ReceiveDynGroupSignalHandle(SignalId, SignalDataPtr, Length);
#endif /* 0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE) && (STD_ON == \
          COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
            }
#endif /*0u < COM_RXIPDU_NUMBER*/
        }
    }
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    COM_NOUSED(Length);
    return ret;
}

/******************************************************************************/
/*
 * Brief               The service Com_SendSignalGroup copies the content of the associated shadow buffer to the
 *                     associated I-PDU.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant for the same signal group. Reentrant for different signal groups.
 * Param-Name[in]      SignalGroupId: Id of signal group to be sent.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK:service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to
 *                     development error)
 *                     COM_BUSY: in case the TP-Buffer is locked for large data types handling
 */
/******************************************************************************/
uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUP_ID, COM_E_UNINIT);
    }
    else if (SignalGroupId >= Com_ConfigStd->ComTxSignalGroupNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUP_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (COM_TXSIGNALGROUP_NUMBER > 0u)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetTxSigGrpPartitionId(SignalGroupId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUP_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
#if ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER))
            ret = Com_SendSignalGroupHandle(SignalGroupId);
#endif
        }
    COM_NOUSED(SignalGroupId);
    return ret;
}
/******************************************************************************/
/*
 * Brief               The service Com_ReceiveSignalGroup copies the received signal group from the I-PDU to the
 *                     shadow buffer.
 * ServiceId           0x0e
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same signal group. Reentrant for different signal groups.
 * Param-Name[in]      SignalGroupId: Id of signal group to be received.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to
 *                     development error) COM_BUSY: in case the TP-Buffer is locked for large data types handling
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
/* PRQA S 1532,1503 ++ */ /* MISRA Rule 8.7,2.1 */
Com_ReceiveSignalGroup(Com_SignalGroupIdType SignalGroupId)
/* PRQA S 1532.1503 -- */ /* MISRA Rule 8.7,2.1 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
    PduIdType ipduRef = Com_ConfigStd->ComRxSignalGroup[SignalGroupId].ComIpduRefIndex;
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[ipduRef];
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUP_ID, COM_E_UNINIT);
    }
    else if (SignalGroupId >= Com_ConfigStd->ComRxSignalGroupNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUP_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (COM_RXSIGNALGROUP_NUMBER > 0u)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetRxSigGrpPartitionId(SignalGroupId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUP_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
            uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_RECEIVING_EN;
            /*IPDU is receiving data*/
            if (Com_RX_RECEIVING_EN == Receiving)
            {
                ret = COM_BUSY;
            }
            else
            {
                Com_ReceiveSignalGroupHandle(ipduRef, SignalGroupId);
                uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & Com_RX_ACTIVE_EN;
                if (Com_RX_ACTIVE_EN == activeEnable)
                {
                    ret = E_OK;
                }
            }
#endif /* 0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER */
        }
    COM_NOUSED(SignalGroupId);
    return ret;
}
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
/******************************************************************************/
/*
 * Brief               The service Com_SendSignalGroupArray copies the content of the provided
 *                     SignalGroupArrayPtr to the associated I-PDU. The provided data shall
 *                     correspond to the array representation of the signal group.
 * ServiceId           0x23
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant for the same signal group. Reentrant for different signal groups.
 * Param-Name[in]      SignalGroupId: Id of signal group to be sent.
 *                     SignalGroupArrayPtr: Reference to the signal group array.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to
 * development error) COM_BUSY: in case the TP-Buffer is locked for large data types handling
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Com_SendSignalGroupArray(
    Com_SignalGroupIdType SignalGroupId,
    P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) SignalGroupArrayPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUPARRAY_ID, COM_E_UNINIT);
    }
    else if (SignalGroupId >= Com_ConfigStd->ComTxSignalGroupNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUPARRAY_ID, COM_E_PARAM);
    }
    else if (NULL_PTR == SignalGroupArrayPtr)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUPARRAY_ID, COM_E_PARAM_POINTER);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (COM_TXSIGNALGROUP_NUMBER > 0u)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetTxSigGrpPartitionId(SignalGroupId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUPARRAY_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
#if ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER))
            ret = Com_SendSignalGroupArrayHandle(SignalGroupId, SignalGroupArrayPtr);
#endif
        }
    COM_NOUSED(SignalGroupId);
    COM_NOUSED(SignalGroupArrayPtr);
    return ret;
}
#endif /* STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API */
/******************************************************************************/
/*
 * Brief               The service Com_ReceiveSignalGroupArray copies the received signal group
 *                     array representation from the PDU to the location designated by SignalGroupArrayPtr.
 * ServiceId           0x24
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same signal group. Reentrant for different signal groups.
 * Param-Name[in]      SignalGroupId: Id of signal group to be received.
 * Param-Name[out]     SignalGroupArrayPtr: reference to the location where the received signal group array shall be
 * stored.
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to
 *                     development error)
 *                     COM_BUSY: in case the TP-Buffer is locked for large data types handling
 */
/******************************************************************************/
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
FUNC(uint8, COM_CODE)
/* PRQA S 3432,1532 ++ */ /* MISRA Rule 20.7,8.7 */
Com_ReceiveSignalGroupArray(
    Com_SignalGroupIdType SignalGroupId,
    P2VAR(uint8, AUTOMATIC, COM_APPL_CONST) SignalGroupArrayPtr)
/* PRQA S 3432,1532 -- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUPARRAY_ID, COM_E_UNINIT);
    }
    else if (SignalGroupId >= Com_ConfigStd->ComRxSignalGroupNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUPARRAY_ID, COM_E_PARAM);
    }
    else if (NULL_PTR == SignalGroupArrayPtr)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUPARRAY_ID, COM_E_PARAM_POINTER);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (COM_RXSIGNALGROUP_NUMBER > 0u)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetRxSigGrpPartitionId(SignalGroupId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUPARRAY_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
            ret = Com_ReceiveSignalGroupArrayHandle(SignalGroupId, SignalGroupArrayPtr);
#endif
        }
    COM_NOUSED(SignalGroupId);
    COM_NOUSED(SignalGroupArrayPtr);
    return ret;
}
#endif /*STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API*/
/******************************************************************************/
/*
 * Brief               The service Com_InvalidateSignal invalidates the signal with the given SignalId
 *                     by setting its value to its configured ComSignalDataInvalidValue.
 * ServiceId           0x10
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant for the same signal. Reentrant for different signals.
 * Param-Name[in]      SignalId: Id of signal to be invalidated.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group is stopped, no ComSignalDataInvalidValue
 *                     is configured for the given signalId or service fails due to development error
 *                     COM_BUSY: in case the TP-Buffer is locked
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
/* PRQA S 1532,1503 ++ */ /* MISRA Rule 8.7,2.1 */
Com_InvalidateSignal(Com_SignalIdType SignalId)
/* PRQA S 1532,1503 -- */ /* MISRA Rule 8.7,2.1 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNAL_ID, COM_E_UNINIT);
    }
    else if (SignalId >= Com_ConfigStd->ComTxSignalNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNAL_ID, COM_E_PARAM);
    }
    else
#endif
    {
#if ((STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED))
        /* Multiple partition check. */
        uint16 partitionId = (SignalId < COM_TXSIGNAL_NUMBER)
                                 ? Com_GetTxSigPartitionId(SignalId)
                                 : Com_GetTxGrpSigPartitionId(SignalId - COM_TXSIGNAL_NUMBER);
        if (GetApplicationID() != partitionId)
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNAL_ID, COM_E_PARTITION);
        }
        else
#endif
        {
#if ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER)
            ret = Com_InvalidateSignalHandle(SignalId);
#endif
        }
    }
    COM_NOUSED(SignalId);
    return ret;
}
/******************************************************************************/
/*
 * Brief               The service Com_InvalidateSignalGroup invalidates all group signals of the signal
 *                     group with the given SignalGroupId by setting their values to their configured
 *                     ComSignalDataInvalidValues.
 * ServiceId           0x1b
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant for the same signal group. Reentrant for different signal groups.
 * Param-Name[in]      SignalGroupId: Id of signal group to be invalidated.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8(E_OK,COM_SERVICE_NOT_AVAILABLE,COM_BUSY)
 *                     E_OK: service has been accepted
 *                     COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group is stopped, no ComSignalDataInvalidValue
 *                     is configured for any of the group signals or service fails due to development error
 *                     COM_BUSY: in case the TP-Buffer is locked
 */
/******************************************************************************/
FUNC(uint8, COM_CODE)
Com_InvalidateSignalGroup(Com_SignalGroupIdType SignalGroupId) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNALGROUP_ID, COM_E_UNINIT);
    }
    else if (SignalGroupId >= Com_ConfigStd->ComTxSignalGroupNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNALGROUP_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (COM_TXSIGNALGROUP_NUMBER > 0u)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetTxSigGrpPartitionId(SignalGroupId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNALGROUP_ID, COM_E_PARTITION);
        }
        else
#endif

#endif
        {
#if (0u < COM_TXIPDU_NUMBER) && ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u))
            ret = Com_InvalidateSignalGroupHandle(SignalGroupId);
#endif
        }
    COM_NOUSED(SignalGroupId);
    return ret;
}
/******************************************************************************/
/*
 * Brief               By a call to Com_TriggerIPDUSend the I-PDU with the given ID is triggered for transmission.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduId: The I-PDU-ID of the I-PDU that shall be triggered for sending
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: I-PDU was triggered for transmission
 *                     E_NOT_OK: I-PDU is stopped, the transmission could not be triggered
 */
/******************************************************************************/
FUNC(Std_ReturnType, COM_CODE)
/* PRQA S 1532 ++ */                                        /* MISRA Rule 8.7 */
Com_TriggerIPDUSend(PduIdType PduId) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
/* PRQA S 1532 -- */                                        /* MISRA Rule 8.7 */
{
    Std_ReturnType returnValue = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSEND_ID, COM_E_UNINIT);
    }
    else if (
        (PduId >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum)) || (PduId < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSEND_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (0u < COM_TXIPDU_NUMBER)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetTxIPduPartitionId(PduId - Com_ConfigStd->ComRxPduNum))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSEND_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
#if (0u < COM_TXIPDU_NUMBER)
            returnValue = Com_TriggerIPDUSendHandle(PduId);
#endif
        }
    COM_NOUSED(PduId);
    return returnValue;
}
#if (STD_ON == COM_METADATA_SUPPORT)
/******************************************************************************/
/*
 * Brief               By a call to Com_TriggerIPDUSendWithMetaData the AUTOSAR COM module
 *                     updates its internal metadata for the I-PDU with the given ID by copying the
 *                     metadata from the given position and with respect to the globally configured metadata
 *                     length of the I-PDU. Then the I-PDU is triggered for transmission.
 * ServiceId           0x28
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduId: The I-PDU-ID of the I-PDU that shall be triggered for sending
 *                     MetaData: A pointer to the metadata for the triggered send-request
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK: I-PDU was triggered for transmission
 *                     E_NOT_OK: I-PDU is stopped, the transmission could not be triggered
 */
/******************************************************************************/
FUNC(Std_ReturnType, COM_CODE)
/* PRQA S 3432,1532,3673 ++ */ /* MISRA Rule 20.7,8.7,8.13 */
Com_TriggerIPDUSendWithMetaData(PduIdType PduId, P2VAR(uint8, AUTOMATIC, COM_APPL_CONST) MetaData)
/* PRQA S 3432,1532 -- */ /* MISRA Rule 20.7,8.7 */
{
    Std_ReturnType returnValue = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSENDWITHMETADATA_ID, COM_E_UNINIT);
    }
    else if (
        (PduId >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum)) || (PduId < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSENDWITHMETADATA_ID, COM_E_PARAM);
    }
    else if (
        (0u == Com_ConfigStd->ComTxIPdu[PduId - Com_ConfigStd->ComRxPduNum].MetaDataLength) || (NULL_PTR == MetaData))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSENDWITHMETADATA_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (0u < COM_TXIPDU_NUMBER)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetTxIPduPartitionId(PduId - Com_ConfigStd->ComRxPduNum))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSENDWITHMETADATA_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
            /*Trigger IPDU(with metadata) send handle*/
#if (0u < COM_TXIPDU_NUMBER)
            returnValue = Com_TriggerIPDUSendWithMetaDataHandle(PduId, MetaData);
#endif
        }
    COM_NOUSED(PduId);
    COM_NOUSED(MetaData);
    return returnValue;
}
#endif /*STD_ON == COM_METADATA_SUPPORT*/
/******************************************************************************/
/*
 * Brief               The service Com_SwitchIpduTxMode sets the transmission mode of the I-PDU referenced by PduId
 * to Mode. In case the transmission mode changes, the new mode shall immediately be effective. In case the
 * requested transmission mode was already active for this I-PDU, the call will have no effect. ServiceId 0x27
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      PduId: Id of the I-PDU of which the transmission mode shall be changed.
 *                     Mode: the transmission mode that shall be set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
Com_SwitchIpduTxMode(PduIdType PduId, boolean Mode) /* PRQA S 1532,1503 */ /* MISRA Rule 8.7,2.1 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SWITCHIPDUTXMODE_ID, COM_E_UNINIT);
    }
    else if (
        (PduId >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum)) || (PduId < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SWITCHIPDUTXMODE_ID, COM_E_PARAM);
    }
    else
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (0u < COM_TXIPDU_NUMBER)
        /* Multiple partition check. */
        if (GetApplicationID() != Com_GetTxIPduPartitionId(PduId - Com_ConfigStd->ComRxPduNum))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SWITCHIPDUTXMODE_ID, COM_E_PARTITION);
        }
        else
#endif
#endif
        {
#if (0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TMS_ENABLE)
            Com_SwitchIpduTxModeHandle(PduId, Mode);
#endif
        }
    COM_NOUSED(PduId);
    COM_NOUSED(Mode);
    return;
}
/******************************************************************************/
/*
 * Brief               This function performs the processing of the AUTOSAR COM module's receive processing that
 *                     are not directly handled within the COM's functions invoked by the PDU-R, for example *
 * Com_RxIndication.
 * ServiceId           0x18
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different mainFunctionId. Non reentrant for the same instance.
 * Param-Name[in]      mainFunctionId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_MainFunctionRx(Com_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if ((COM_INIT == Com_Status) && (mainFunctionId < COM_NUMBER_OF_MAIN_FUNCTION_RX))
    {
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
        /* Multiple partition check. */
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != Com_GetRxMFPartitionId(mainFunctionId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_MAINFUNCTIONRX_ID, COM_E_PARTITION);
        }
        else
#endif
        {
#if (0u < COM_RXIPDU_NUMBER)
            Com_MainFunctionRxHandle(mainFunctionId);
#endif
        }
    }
}
/******************************************************************************/
/*
 * Brief               This function performs the processing of the AUTOSAR COM module's transmission activities
 * that are not directly handled within the COM's function invoked by the RTE, for example Com_SendSignal. ServiceId
 * 0x19 Sync/Async          Synchronous Reentrancy          Reentrant for different mainFunctionId. Non reentrant
 * for the same instance. Param-Name[in]      mainFunctionId Param-Name[out]     None Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_MainFunctionTx(Com_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if ((COM_INIT == Com_Status) && (mainFunctionId < COM_NUMBER_OF_MAIN_FUNCTION_TX))
    {
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
        /* Multiple partition check. */
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != Com_GetTxMFPartitionId(mainFunctionId))
        {
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_MAINFUNCTIONTX_ID, COM_E_PARTITION);
        }
        else
#endif
        {
#if (0u < COM_TXIPDU_NUMBER)
            Com_MainFunctionTxHandle(mainFunctionId);
#endif
        }
    }
}

/******************************************************************************/
/*
 * Brief               Calls the signal gateway part of the AUTOSAR COM module to forward received signals to be routed.
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different mainFunctionId. Non reentrant for the same instance.
 * Param-Name[in]      mainFunctionId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void Com_MainFunctionRouteSignals(Com_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COM_DEV_ERROR_DETECT == STD_ON
    if (mainFunctionId >= COM_NUMBER_OF_MAIN_FUNCTION_ROUTE_SIGNALS)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_MAINFUNCTIONROUTESIGNALS_ID, COM_E_PARAM);
        return;
    }
#if COM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
    if (applicationID != Com_MainFunctionRouteSignalsApplcation[mainFunctionId])
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_MAINFUNCTIONROUTESIGNALS_ID, COM_E_PARTITION);
        return;
    }
#endif
#endif

    if (Com_Status == COM_INIT)
    {
#if COM_GWMAPPING_NUMBER > 0u
        Com_MainFunctionRouteSignalsHandle(mainFunctionId);
#endif
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module) shall check whether the
 *                     available data fits into the buffer size reported by PduInfoPtr->SduLength.
 *                     If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr
 *                     and update the length of the actual copied data in PduInfoPtr->SduLength.
 *                     If not, it returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the SDU that is requested to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU
 *                     data shall be copied, and the available buffer size in SduLengh.
 *                     On return, the service will indicate the length of the copied SDU
 *                     data in SduLength.
 * Return              Std_ReturnType
 *                     E_OK: SDU has been copied and SduLength indicates the number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must not
 *                     be used since it may contain a NULL pointer or point to invalid data.
 */
/******************************************************************************/
#define COM_START_SEC_COMTRIGGERTRANSMIT_CALLBACK_CODE
#include "Com_MemMap.h"
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, COM_CODE)
Com_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    Std_ReturnType returnValue = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERTRANSMIT_ID, COM_E_UNINIT);
    }
    else if (
        (TxPduId >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum))
        || (TxPduId < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERTRANSMIT_ID, COM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERTRANSMIT_ID, COM_E_PARAM_POINTER);
    }
    else if (
        (0u < Com_ConfigStd->ComTxIPdu[TxPduId - Com_ConfigStd->ComRxPduNum].MetaDataLength)
        && (NULL_PTR == PduInfoPtr->MetaDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERTRANSMIT_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (0u < COM_TXIPDU_NUMBER)
        returnValue = Com_TriggerTransmitHandle(TxPduId, PduInfoPtr);
#endif
    }
    COM_NOUSED(TxPduId);
    COM_NOUSED(PduInfoPtr);
    return returnValue;
}
#define COM_STOP_SEC_COMTRIGGERTRANSMIT_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received I-PDU from a lower layer communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      RxPduId: ID of the received I-PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the received I-PDU and a pointer to
 *                     a buffer (SduDataPtr) containing the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define COM_START_SEC_COMRXINDICATION_CALLBACK_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Com_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RXINDICATION_ID, COM_E_UNINIT);
    }
    else if (RxPduId >= Com_ConfigStd->ComRxPduNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RXINDICATION_ID, COM_E_PARAM);
    }
    else if (COM_PDU_NORMAL != Com_ConfigStd->ComRxIPdu[RxPduId].ComIPduType)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RXINDICATION_ID, COM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RXINDICATION_ID, COM_E_PARAM_POINTER);
    }
    else if ((0u < Com_ConfigStd->ComRxIPdu[RxPduId].MetaDataLength) && (NULL_PTR == PduInfoPtr->MetaDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RXINDICATION_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (0u < COM_RXIPDU_NUMBER)
        Com_RxIndicationHandle(RxPduId, PduInfoPtr);
#endif
    }
    COM_NOUSED(RxPduId);
    COM_NOUSED(PduInfoPtr);
    return;
}
#define COM_STOP_SEC_COMRXINDICATION_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called after an I-PDU has been received via the TP API,
 *                     the result indicates whether the transmission was successful or not.
 * ServiceId           0x45
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the received I-PDU.
 *                     result: Result of the reception.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define COM_START_SEC_COMTPRXINDICATION_CALLBACK_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Com_TpRxIndication(PduIdType id, Std_ReturnType result)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPRXINDICATION_ID, COM_E_UNINIT);
    }
    else if (id >= Com_ConfigStd->ComRxPduNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPRXINDICATION_ID, COM_E_PARAM);
    }
    else if (COM_PDU_TP != Com_ConfigStd->ComRxIPdu[id].ComIPduType)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPRXINDICATION_ID, COM_E_PARAM);
    }
    else
#endif
    {
#if (STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXIPDU_NUMBER)
        Com_TpRxIndicationHandle(id, result);
#endif /* STD_ON == COM_RXTPPDU_SUPPORT */
    }
    COM_NOUSED(id);
    COM_NOUSED(result);
    return;
}
#define COM_STOP_SEC_COMTPRXINDICATION_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms the transmission of an IPDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the I-PDU that has been transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define COM_START_SEC_COMTXCONFIRMATION_CALLBACK_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE)
Com_TxConfirmation(PduIdType TxPduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TXCONFIRMATION_ID, COM_E_UNINIT);
    }
    else if (
        (TxPduId >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum))
        || (TxPduId < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TXCONFIRMATION_ID, COM_E_PARAM);
    }
    else if (COM_PDU_NORMAL != Com_ConfigStd->ComTxIPdu[TxPduId - Com_ConfigStd->ComRxPduNum].ComIPduType)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TXCONFIRMATION_ID, COM_E_PARAM);
    }
    else
#endif
    {
#if (0u < COM_TXIPDU_NUMBER)
        Com_TxConfirmationHandle(TxPduId);
#endif
    }
    COM_NOUSED(TxPduId);
    return;
}
#define COM_STOP_SEC_COMTXCONFIRMATION_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called after the I-PDU has been transmitted on its network, the
 *                     result indicates whether the transmission was successful or not.
 * ServiceId           0x48
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     result: Result of the transmission of the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define COM_START_SEC_COMTPTXCONFIRMATION_CALLBACK_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE)
Com_TpTxConfirmation(PduIdType id, Std_ReturnType result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPTXCONFIRMATION_ID, COM_E_UNINIT);
    }
    else if ((id >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum)) || (id < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPTXCONFIRMATION_ID, COM_E_PARAM);
    }
    else if (COM_PDU_TP != Com_ConfigStd->ComTxIPdu[id - Com_ConfigStd->ComRxPduNum].ComIPduType)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPTXCONFIRMATION_ID, COM_E_PARAM);
    }
    else
#endif
    {
#if ((0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TXTPPDU_SUPPORT))
        Com_TpTxConfirmationHandle(id, result);
#endif
    }
    COM_NOUSED(id);
    COM_NOUSED(result);
    return;
}
#define COM_STOP_SEC_COMTPTXCONFIRMATION_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called at the start of receiving an N-SDU. The N-SDU might be
 *                     fragmented into multiple N-PDUs (FF with one or more following CFs) or might
 *                     consist of a single N-PDU (SF).
 * ServiceId           0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the payload
 *                     data (without protocol information) and payload length of the
 *                     first frame or single frame of a transport protocol I-PDU reception.
 *                     Depending on the global parameter MetaDataLength, additional bytes containing
 *                     MetaData (e.g. the CAN ID) are appended after the payload data, increasing the length
 *                     accordingly. If neither first/single frame data nor MetaData
 *                     are available, this parameter is set to NULL_PTR.
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving module. This parameter will
 *                     be used to compute the Block Size (BS) in the transport protocol module.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType(BUFREQ_OK,BUFREQ_E_NOT_OK,BUFREQ_E_OVFL)
 *                     BUFREQ_OK: Connection has been accepted. bufferSizePtr
 *                     indicates the available receive buffer; reception is continued.
 *                     If no buffer of the requested size is available, a receive buffer
 *                     size of 0 shall be indicated by bufferSizePtr.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected; reception is aborted. bufferSizePtr remains
 * unchanged. BUFREQ_E_OVFL: No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 * remains unchanged.
 */
/******************************************************************************/
#define COM_START_SEC_COMSTARTOFRECEPTION_CALLBACK_CODE
#include "Com_MemMap.h"
/* PRQA S 1532,3432 ++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, COM_CODE)
Com_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr)
/* PRQA S 1532,3432 -- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_STARTOFRECEPTION_ID, COM_E_UNINIT);
    }
    else if ((id >= Com_ConfigStd->ComRxPduNum) || (COM_PDU_TP != Com_ConfigStd->ComRxIPdu[id].ComIPduType))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_STARTOFRECEPTION_ID, COM_E_PARAM);
    }
    else if (
        ((0u < Com_ConfigStd->ComRxIPdu[id].MetaDataLength) && ((NULL_PTR == info) || (NULL_PTR == info->MetaDataPtr)))
        || (((NULL_PTR != info) && (0u < info->SduLength)) && (NULL_PTR == info->SduDataPtr))
        || (NULL_PTR == bufferSizePtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_STARTOFRECEPTION_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXIPDU_NUMBER)
        bufReq = Com_StartOfReceptionHandle(id, info, TpSduLength, bufferSizePtr);
#endif
    }
    COM_NOUSED(id);
    COM_NOUSED(info);
    COM_NOUSED(TpSduLength);
    COM_NOUSED(bufferSizePtr);
    return bufReq;
}
#define COM_STOP_SEC_COMSTARTOFRECEPTION_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper
 * layer. Each call to this function provides the next part of the I-PDU data. The size of the remaining data is written
 * to the position indicated by bufferSizePtr. ServiceId           0x44 Sync/Async          Synchronous Reentrancy
 * Reentrant Param-Name[in]      id: Identification of the received I-PDU. info: Provides the source buffer (SduDataPtr)
 * and the number of bytes to be copied (SduLength). An SduLength of 0 can be used to query the current amount of
 * available buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR. Param-Name[out]
 * bufferSizePtr: Available receive buffer after data has been copied. Param-Name[in/out]  None Return BufReq_ReturnType
 *                     BUFREQ_OK: Data copied successfully
 *                     BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 */
/******************************************************************************/
#define COM_START_SEC_COMCOPYRXDATA_CALLBACK_CODE
#include "Com_MemMap.h"
FUNC(BufReq_ReturnType, COM_CODE)
/* PRQA S 3432,1532 ++ */ /* MISRA Rule 20.7,8.7 */
Com_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr)
/* PRQA S 3432,1532 -- */ /* MISRA Rule 20.7,8.7 */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYRXDATA_ID, COM_E_UNINIT);
    }
    else if (id >= Com_ConfigStd->ComRxPduNum)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYRXDATA_ID, COM_E_PARAM);
    }
    else if (COM_PDU_TP != Com_ConfigStd->ComRxIPdu[id].ComIPduType)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYRXDATA_ID, COM_E_PARAM);
    }
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYRXDATA_ID, COM_E_PARAM_POINTER);
    }
    else if ((0u < Com_ConfigStd->ComRxIPdu[id].MetaDataLength) && (NULL_PTR == info->MetaDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYRXDATA_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXIPDU_NUMBER)
        bufReq = Com_CopyRxDataHandle(id, info, bufferSizePtr);
#endif
    }
    COM_NOUSED(id);
    COM_NOUSED(info);
    COM_NOUSED(bufferSizePtr);
    return bufReq;
}
#define COM_STOP_SEC_COMCOPYRXDATA_CALLBACK_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *                     Each call to this function provides the next part of the I-PDU data unless retry->TpDataState
 *                     is TP_DATARETRY. In this case the function restarts to copy the data beginning at the offset
 *                     from the current position indicated by retry->TxTpDataCnt. The size of the remaining data is
 *                     written to the position indicated by availableDataPtr.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     retry: This parameter is used to acknowledge transmitted data or to retransmit data after
 * transmission problems. Param-Name[out]     availableDataPtr: Indicates the remaining number of bytes that are
 * available in the upper layer module's Tx buffer. availableDataPtr can be used by TP modules that support dynamic
 * payload lengths (e.g. FrIsoTp) to determine the size of the following CFs. Param-Name[in/out]  None Return
 * BufReq_ReturnType BUFREQ_OK: Data has been copied to the transmit buffer completely as requested. BUFREQ_E_BUSY:
 * Request could not be fulfilled, because the required amount of Tx data is not available. The lower layer module may
 * retry this call later on. No data has been copied. BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 */
/******************************************************************************/
#define COM_START_SEC_COMCOPYTXDATA_CALLBACK_CODE
#include "Com_MemMap.h"
FUNC(BufReq_ReturnType, COM_CODE)
/* PRQA S 3432,1532 ++ */ /* MISRA Rule 20.7,8.7 */
Com_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, COM_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) availableDataPtr)
/* PRQA S 3432,1532 ++ */ /* MISRA Rule 20.7,8.7 */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (COM_INIT != Com_Status)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYTXDATA_ID, COM_E_UNINIT);
    }
    else if ((id >= (Com_ConfigStd->ComRxPduNum + Com_ConfigStd->ComTxPduNum)) || (id < Com_ConfigStd->ComRxPduNum))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYTXDATA_ID, COM_E_PARAM);
    }
    else if (COM_PDU_TP != Com_ConfigStd->ComTxIPdu[id - Com_ConfigStd->ComRxPduNum].ComIPduType)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYTXDATA_ID, COM_E_PARAM);
    }
    else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYTXDATA_ID, COM_E_PARAM_POINTER);
    }
    else if ((0u < info->SduLength) && (NULL_PTR == info->SduDataPtr))
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYTXDATA_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
#if ((0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TXTPPDU_SUPPORT))
        bufReq = Com_CopyTxDataHandle(id, info, retry, availableDataPtr);
#endif
    }
    COM_NOUSED(id);
    COM_NOUSED(info);
    COM_NOUSED(retry);
    COM_NOUSED(availableDataPtr);
    return bufReq;
}
#define COM_STOP_SEC_COMCOPYTXDATA_CALLBACK_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
#if (0u < COM_RXIPDU_NUMBER) || (0u < COM_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Disable all Tx/Rx Pdu state
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_SetAllPduRToDisableState(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

    uint16 cirCnt;
#if (0u < COM_RXIPDU_NUMBER)
    /* clear All Rx IPdu Active Flag */
    Com_RxIPduRunTimeStateType* rxIpduStatePtr;
    for (cirCnt = 0u; cirCnt < COM_RXIPDU_NUMBER; ++cirCnt)
    {
        rxIpduStatePtr = &Com_RxIPduRunTimeState[cirCnt];
        rxIpduStatePtr->RxIpduRTStFlag &= Com_RX_ACTIVE_DIS;
    }
#endif /* 0u < COM_RXIPDU_NUMBER */
#if (0u < COM_TXIPDU_NUMBER)
    /* clear All Tx IPdu Active Flag */
    Com_TxIPduRunTimeStateType* txIpduStatePtr;
    for (cirCnt = 0u; cirCnt < COM_TXIPDU_NUMBER; ++cirCnt)
    {
        txIpduStatePtr = &Com_TxIPduRunTimeState[cirCnt];
        txIpduStatePtr->TxIpduRTStFlag &= Com_TX_ACTIVE_DIS;
    }
#endif /* 0u < COM_TXIPDU_NUMBER */
}
#endif
/******************************************************************************/
/*
 * Brief               Init the Pdu Runtime Manager and Buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_PduRTAndBufferInit(void)
{
#if (0u < COM_RXIPDU_NUMBER)
    /* Rx IPdu's RunTimeState init */
    (void)ILib_memcpy(&Com_RxIPduRunTimeState[0], &Com_RxIPduInitState[0], sizeof(Com_RxIPduRunTimeState));
#endif
#if (0u < COM_TXIPDU_NUMBER)
    /* Tx IPdu's RunTimeState init */
    (void)ILib_memcpy(&Com_TxIPduRunTimeState[0], &Com_TxIPduInitState[0], sizeof(Com_TxIPduRunTimeState));
#endif
#if (0u < COM_TXIPDUBUFF_SIZE)
    /* TxIPdu Init value, based on signal or group signal init value */
    (void)ILib_memcpy(&Com_TxIPduRuntimeBuff[0], &Com_TxIPduInitValue[0], sizeof(Com_TxIPduRuntimeBuff));
#endif
#if (0u < COM_RXIPDUBUFF_SIZE)
    /* RxIPdu Init value, based on signal or group signal init value */
    (void)ILib_memcpy(&Com_RxIPduRuntimeBuff[0], &Com_RxIPduInitValue[0], sizeof(Com_RxIPduRuntimeBuff));
#endif
}

/* called by Com_Init
 * Init the Runtime RxSignal/RxGroupSignal Buffer*/
static FUNC(void, COM_CODE) Com_SignalBufferInit(void)
{
#if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
    /* Bool signal Runtime value, based on bool signals or dest description(source description signals are not
     * included) signals init value */
#if (0u < COM_RXGROUPSIGNAL_BOOLBUFF_SIZE)
    (void)ILib_memcpy(
        &Com_RxGroupSignalBoolShadowBuff[0],
        &Com_SignalBoolInitValue[0],
        sizeof(Com_RxGroupSignalBoolShadowBuff));
#endif /*0u < COM_RXGROUPSIGNAL_BOOLBUFF_SIZE*/
    (void)ILib_memcpy(&Com_SignalBoolRuntimeBuff[0], &Com_SignalBoolInitValue[0], sizeof(Com_SignalBoolRuntimeBuff));
#endif /* 0u < COM_SIGNAL_BOOLBUFF_SIZE */
#if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    /* 8Bit signal Runtime value, based on uint8, sint8, uint8N,uint8DYN signals or dest description(source
     * description signals are not included) init value */
#if (0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE)
    (void)
        ILib_memcpy(&Com_RxGroupSignal8BitShadowBuff[0], &Com_Signal8BitInitValue[0], COM_RXGROUPSIGNAL_8BITBUFF_SIZE);
#endif /* 0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE */
    (void)ILib_memcpy(&Com_Signal8BitRuntimeBuff[0], &Com_Signal8BitInitValue[0], COM_SIGNAL_8BITBUFF_SIZE);
#endif /* 0u < COM_SIGNAL_8BITBUFF_SIZE */
#if (0u < COM_SIGNAL_16BITBUFF_SIZE)
    /* 16Bit signal Runtime value, based on uint16, sint16 signals or dest description(source description signals
     * are not included) signals init value */
#if (0u < COM_RXGROUPSIGNAL_16BITBUFF_SIZE)
    (void)ILib_memcpy(
        &Com_RxGroupSignal16BitShadowBuff[0],
        &Com_Signal16BitInitValue[0],
        sizeof(Com_RxGroupSignal16BitShadowBuff));
#endif /* 0u < COM_RXGROUPSIGNAL_16BITBUFF_SIZE */
    (void)ILib_memcpy(&Com_Signal16BitRuntimeBuff[0], &Com_Signal16BitInitValue[0], sizeof(Com_Signal16BitRuntimeBuff));
#endif /* 0u < COM_SIGNAL_16BITBUFF_SIZE */
#if (0u < COM_SIGNAL_32BITBUFF_SIZE)
    /* 32Bit signal Runtime value, based on uint32, sint32,float32 signals or dest description(source description
     * signals are not included) signals init value */
#if (0u < COM_RXGROUPSIGNAL_32BITBUFF_SIZE)
    (void)ILib_memcpy(
        &Com_RxGroupSignal32BitShadowBuff[0],
        &Com_Signal32BitInitValue[0],
        sizeof(Com_RxGroupSignal32BitShadowBuff));
#endif /* 0u < COM_RXGROUPSIGNAL_32BITBUFF_SIZE */
    (void)ILib_memcpy(&Com_Signal32BitRuntimeBuff[0], &Com_Signal32BitInitValue[0], sizeof(Com_Signal32BitRuntimeBuff));
#endif /* 0u < COM_SIGNAL_32BITBUFF_SIZE */
#if (0u < COM_SIGNAL_64BITBUFF_SIZE)
    /* 64Bit signal Runtime value, based on uint64, sint64,float64 signals or dest description(source description
     * signals are not included) signals init value */
#if (0u < COM_RXGROUPSIGNAL_64BITBUFF_SIZE)
    (void)ILib_memcpy(
        &Com_RxGroupSignal64BitShadowBuff[0],
        &Com_Signal64BitInitValue[0],
        sizeof(Com_RxGroupSignal64BitShadowBuff));
#endif /* 0u < COM_RXGROUPSIGNAL_32BITBUFF_SIZE */
    (void)ILib_memcpy(&Com_Signal64BitRuntimeBuff[0], &Com_Signal64BitInitValue[0], sizeof(Com_Signal64BitRuntimeBuff));
#endif /* 0u < COM_SIGNAL_64BITBUFF_SIZE */
    return;
}

#if (0u < COM_RXMASKNEWDIFFERMASKOLD_NUMBER)
/******************************************************************************/
/*
 * Brief               MaskNewDifferMaskOld filter RxTimeout Init handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_RxMaskNewDifferMaskOldTimeOutInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    (void)ILib_memset(&Com_MaskNewDifferMaskOldTimeOut[0], 0, COM_RXMASKNEWDIFFERMASKOLD_NUMBER);
}
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
/******************************************************************************/
/*
 * Brief               RxTimeout init handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COM_CODE) Com_RxTimeOutInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /*set all rx signal,rx signal group's DM timeout counter to 0*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    (void)ILib_memset(&Com_RxSignalTimeOutCnt[0], 0, sizeof(Com_RxSignalTimeOutCnt));
#endif /*COM_RXSIGNAL_NUMBER > 0u*/
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    (void)ILib_memset(&Com_RxSignalGroupTimeOutCnt[0], 0, sizeof(Com_RxSignalGroupTimeOutCnt));
#endif
}
#endif

#if (0u < COM_ONEEVERYNFILTERSIGNAL_NUMBER)
/******************************************************************************/
/*
 * Brief               Init the counter of ONE_EVERY_N filter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COM_CODE)
static Com_OneEveryNCntInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    (void)ILib_memset(&Com_OneEveryNcnt[0], 0, sizeof(Com_OneEveryNcnt));
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
