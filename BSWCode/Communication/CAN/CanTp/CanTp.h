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
**************************************************************************** **
**                                                                           **
**  FILENAME    : CanTp.h                                                    **
**                                                                           **
**  Created on  : 2021/7/30 14:29:43                                         **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Public interfaces declared by CanTp module                 **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V2.0.0       2020-5-6  tao.yu         create
 *  V2.0.1       2022-4-1  xinrun.wang    move out timing function as callback
 *  V2.1.0       2022-11-3 tao.yu         add diag gateway response nrc78 function
 *  V2.1.1       2023-3-22 tao.yu         remove unused var in CanTp_ConnectionType change version check
 *  V2.1.2       2023-4-26 xinrun.wang    1. Ignore when receive FD with first byte not 0
 *                                        2. Add Rx Queue process
 *  V2.1.3       2023-5-10 tao.yu         fix As is 0, do not call CanTp_ReleaseTxChannel and
 *                                        Rx FC extend address format ta compare
 *  V2.1.4       2023-6-26 xinrun.wang    FC confirmation no longer needs TxNSdu Config
 *  V2.1.5       2023-6-29 tao.yu         Fix CPT-6029: multiple frame sending failures and data errors in parallel
 *  V2.1.6       2023-7-14 xinrun.wang    Release channels when detected padding error
 *  V2.1.7       2023-7-24 xinrun.wang    Disable timer when detected NA timeout
 *  V2.1.8       2023-8-3  tao.yu         QAC change
 *  V2.1.9       2023-8-14 xinrun.wang    release channel when detected NA timeout
 *  V2.1.10      2023-8-16 xinrun.wang    Add option for synchronous rxindication
 *  V2.1.11      2023-8-28 xinrun.wang    Set RxState Idle after TpRxIndication
 *  V2.1.12      2023-8-30 tao.yu         fix 3E 80 Func interrupt
 *  V2.1.13      2023-8-30 xinrun.wang    Avoid dispatch TxEvent while waiting for FC confirmation
 *  V2.1.14      2023-9-6  peng.wu        1.Fix the state is not CANTP_IDLE, the message cannot send CPT-6816
 *                                        2.Fix the status of start STminTimer.CPT-6992
 *  V2.1.15      2023-10-10  peng.wu      Add the macro CANTP_FIX_BS to control the value of BS. CPP-8401
 *               2023-10-13  tao.yu       CPT-7198, cahnge CAN20 to CANTP_CAN_20, CAN_FD to CANTP_CAN_FD
 *  V2.1.16      2023-10-18  xinrun.wang  Adjust mainFunction handle to avoid excessive exclusive area
 *               2023-11-03  xue.han      1. change CanTP_CheckRxChangeParameter() preprocess condition.
 *                                        2. rxNSduCfgPtr unused
 *  V2.1.17      2023-11-21  xinrun.wang  1. Add a few Checks for NULL_PTR vars
 *                                        2. Optimize Code Style, change some helper functions to inline
 *                                        3. Add cfg macros switch for runtime det and rxQueue
 *  V2.1.18      2023-12-26  xue.han      QAC
 *               2023-12-27  xinrun.wang  Add protection for timer checks
 *  V2.1.19      2024-01-03  xinrun.wang  Fix pointer use in gatewayResp Code
 *  V2.1.20      2024-01-22  tao.yu       QAC
 *               2024-02-02  xue.han      QAC
 *               2024-03-11  peng.wu      Fix the Bs value, CPT-8423
 *  V2.1.21      2024-03-22  xinrun.wang  Fix CanTp_ChangeParameter's channelId when detReport disabled
 *               2024-03-26  tao.yu       fix schm
 *               2024-04-26  xinrun.wang  Add macro to match ISO15765 2004 standard
 *               2024-04-29  xinrun.wang  validate sdulength for non-fd communication
 *               2024-05-16  xue.han      Fix the Bs value if NULL_PTR, CPT-8423
 *               2024-05-24  tao.yu       CPT-9061 , fix generic connection (N-PDU with MetaData)
 *  V2.1.22      2024-06-03  hao.wen      Fix calculation currentBS error, CPT-9136
 *               2024-07-03  xue.han      Fix CanTp_TxConfirmation Det error, CPT-9163
 *                                        Fix TA when EXTENDED CanTpRxAddressingFormat,CPT-9169
 *               2024-08-13  xue.han      QAC
 *               2024-09-24  xue.han      CPT-10450，fix PduR_CanTpTxConfirmation call repeated
 *                                        CPT-10075，fix STmin range from 0 to 127
 *               2024-10-24  xue.han      CPT-10607, fix metadata transmition
 *               2024-11-04  xue.han      CPT-10654, fix CanTp exclusiveArea
 *               2024-11-19  xue.han      CPT-10784, Synchronize Compile warning
 *               2024-12-03  xue.han      CPT-11771, fix CanTp QueueProcess
 *               2024-12-11  xue.han      CPT-11918, Add ExclusiveArea when CanTpMainFunction process timer
 *               2024-12-17  xue.han      CPT-11945, Support to directly process SF/FF transmition in CanTp_Transmit()
 *               2024-01-10  xue.han      Adjust CanTp ExclusiveArea
 *               2025-02-25  xue.han      SF/FF transmissions can be configured to synchronous or asynchronous modes
 *               2025-03-12  xue.han      CPT-12764,support transmission immediately during the confirmation
 */
/*============================================================================*/

#ifndef CANTP_H
#define CANTP_H

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_PBcfg.h"
#include "CanTp_Cfg.h"
#include "CanTp_Types.h"

/*========[V E R S I O N  I N F O R M A T I O N]=========*/
#define CANTP_AR_RELEASE_MAJOR_VERSION 4U
#define CANTP_AR_RELEASE_MINOR_VERSION 5U
#define CANTP_AR_RELEASE_PATCH_VERSION 0U
#define CANTP_SW_MAJOR_VERSION         2U
#define CANTP_SW_MINOR_VERSION         1U
#define CANTP_SW_PATCH_VERSION         22U

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
BEGIN_C_DECLS
/*=======[E X T E R N A L   D A T A]==========================================*/

extern CONST(CanTp_ConfigType, CANTP_CONST) CanTp_Config;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*
 * Brief               This function initializes the CanTp module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
extern FUNC(void, CANTP_APPL_CODE) CanTp_Init(P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr);

/*
 * Brief               This function to shutdown the CanTp module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
extern FUNC(void, CANTP_APPL_CODE) CanTp_Shutdown(void);

/*
 * Brief               This service is used to request the transfer of segmented data.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be transmitted.
 *                     CanTpTxInfoPtr indicator of a structure with CAN N-SDU related
 *                                    data:indicator of a CAN N-SDU buffer and the length
 *                                    of this buffer.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:request can be started successfully
 *                     E_NOT_OK:request cannot be started
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
    CanTp_Transmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);

#if (CANTP_TC == STD_ON)
/*
 * Brief               This service is used to cancel the transfer of pending
 *                     CAN N-SDUs. The connection is identified by CanTpTxPduId.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId ID of the CAN N-SDU to be canceled.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
extern FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_CancelTransmit(PduIdType TxPduId);

/*
 * Brief               This service is used to cancel the reception of an ongoing N-SDU.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId This parameter contains the unique CanTp module identifier
 *                     of the N-SDU to be cancelled for transmission.
 *                     Range: 0..(maximum number of L-PDU IDs received ) - 1
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
extern FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_CancelReceive(PduIdType RxPduId);
#endif

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/*
 * Brief               Request to change a specific transport protocol parameter (e.g. block size).
 * ServiceId           0x4b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id:Identification of the PDU which the parameter change shall affect.
 *                     parameter:ID of the parameter that shall be changed.
 *                     value: The new value of the parameter.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:The parameter was changed successfully.
 *                     E_NOT_OK:The parameter change was rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
    CanTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);
#endif

#if (CANTP_READ_PARAMETER == STD_ON)
/*
 * Brief               This service is used to read the current value of reception parameters BS and STmin for a
 *                     specified N-SDU.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id:Identification of the PDU which the parameter change shall affect.
 *                     parameter:ID of the parameter that shall be changed.
 * Param-Name[out]     value:Pointer where the parameter value will be provided.
 * Param-Name[in/out]  N/A
 * Return              E_OK:request is accepted
 *                     E_NOT_OK:request is not accepted.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
    CanTp_ReadParameter(PduIdType id, TPParameterType parameter, uint16* value);
#endif

#if (CANTP_VERSION_INFO_API == STD_ON)
/*
 * Brief               This function return the version information of the CANTP module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     versioninfo indicator as to where to store the version information of this module.
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
extern FUNC(void, CANTP_APPL_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    CanTp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) versioninfo);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

END_C_DECLS

#endif /* CANTP_H */

/*=======[E N D   O F   F I L E]==============================================*/
