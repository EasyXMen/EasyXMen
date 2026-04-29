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
 **  @file               : CanNm.h
 **  @author             : caihong.liu
 **  @date               : 2024/12/12
 **  @vendor             : isoft
 **  @description        : Can network manager
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2019-09-26  darren.zhang Initial version(ff version 1.16)
 *  V02.00.00 2020-07-29  lili.wang    R19_11 initial version.
 *  V02.00.01 2020-10-19  lili.wang    QAC check,code update
 *  V02.00.02 2020-11-30  lili.wang    1.PduInfoType MetaDataPtr set to NULL_PTR
 *                                     2.Cantata detection modification
 *  V02.00.03 2020-12-11  lili.wang    cantata check,code update
 *  V02.00.04 2021-03-10  lili.wang    1.UnitTest bug update,cannm immediate transmit and Pnresetime bug update
 *                                     2.4.2.2&R19-11 define switch control modify
 *                                     3.CanNmUserDataEnabled and CanNmComUserDataSupport switch control modify
 *  V02.00.05 2021-04-23  lili.wang    Modify compilation errors
 *  V02.00.06 2021-04-26  lili.wang    Solve the problem that data cannot be transmitted when UdpNmMsgCycleOffset
 *                                     is 0 and the immediate transmission is disabled.
 *  V02.00.12 2022-04-24  darren.zhang Adjust the header file inclusion order
 *  V02.00.17 2023-06-12  xiaojian.liang Fixed issue CPT-644 CanNm_SetUserData null pointer exception
 *  V02.00.22 2023-11-20  xiaojian.liang 1. Performance optimization.
 *                                       2. Fixed CPT-6562 Preprocessing could not be performed because the nonexistent
 *                                          macro CANNM_PN_ENABLED was used
 *                                       3. Fixed CPT-8002 CanNmNodeIdEnabled, CanNmNodeDetectionEnabled,
 *                                          CanNmRepeatMsgIndEnabled related features review
 *  V02.00.23 2024-01-31  xiaojian.liang 1. CPT-8061 Source NID is always zero if CanNmNodeIdEnabled is true but
 *                                          CanNmNodeDetectionEnabled is false
 *                                       2. Replace the std library with the istd library
 *                                       3. CPD-33700 Multiple partition support
 *  V02.01.00 2024-12-30  caihong.liu  R23-11 development first release.
 *  V02.01.01 2025-10-22  caihong.liu
 *      1. Fixed CPT-14266, Redundant passive wake-up event flags lead to repeated wake-up actions.
 *      2. Fixed CPT-14866, Fixed race condition in ReadySleepState causing unhandled CanNm_RepeatMessageRequest due to
 *         immediate transition to PrepareBusSleepMode.
 *      3. Fixed CPT-14491, state machine stuck in ReadySleepState due to nmTimeoutTimer is 0 but nmToutFlg is FALSE.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanNm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_CanNm_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

#ifndef CANNM_H_
#define CANNM_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "NmStack_Types.h"
#include "CanNm_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name Published Information of CanNm
 * @{
 * @brief Published Information
 */
#define CANNM_VENDOR_ID                   (62u)
#define CANNM_MODULE_ID                   (31u)
#define CANNM_INSTANCE_ID                 (0u)
#define CANNM_AR_RELEASE_MAJOR_VERSION    (4u)
#define CANNM_AR_RELEASE_MINOR_VERSION    (9u)
#define CANNM_AR_RELEASE_REVISION_VERSION (0u)
#define CANNM_SW_MAJOR_VERSION            (2u)
#define CANNM_SW_MINOR_VERSION            (1u)
#define CANNM_SW_PATCH_VERSION            (1u)
/** @} */

/* ===================================================== macros ===================================================== */
/**
 * @name Development Errors of CanNm
 * @{
 * @brief Development Errors
 */
#define CANNM_E_UNINIT                    ((uint8)(0x01u))
#define CANNM_E_INVALID_CHANNEL           ((uint8)(0x02u))
#define CANNM_E_INVALID_PDUID             ((uint8)(0x03u))
#define CANNM_E_NET_START_IND             ((uint8)(0x04u))
#define CANNM_E_INIT_FAILED               ((uint8)(0x05u))
#define CANNM_E_INVALID_PARTITION_CONTEXT ((uint8)(0x06u))
#define CANNM_E_NETWORK_TIMEOUT           ((uint8)(0x11u))
#define CANNM_E_PARAM_POINTER             ((uint8)(0x12u))
#define CANNM_E_NOT_IN_BUS_SLEEP          ((uint8)(0x13u))
#define CANNM_E_PARTITION                 ((uint8)(0x14u))
#define CANNM_E_INVALID_ID                ((uint8)(0xFFu))
/** @} */

/**
 * @name Service ID of CanNm
 * @{
 * @brief Service ID
 */
#define CANNM_SERVICE_ID_INIT                       ((uint8)(0x00u))
#define CANNM_SERVICE_ID_DEINIT                     ((uint8)(0x10u))
#define CANNM_SERVICE_ID_PASSIVESTARUP              ((uint8)(0x01u))
#define CANNM_SERVICE_ID_NETWORKREQUEST             ((uint8)(0x02u))
#define CANNM_SERVICE_ID_NETWORKRELEASE             ((uint8)(0x03u))
#define CANNM_SERVICE_ID_DISABLECOMMUNICATION       ((uint8)(0x0cu))
#define CANNM_SERVICE_ID_ENABLECOMMUNICATION        ((uint8)(0x0du))
#define CANNM_SERVICE_ID_SETUSERDATA                ((uint8)(0x04u))
#define CANNM_SERVICE_ID_GETUSERDATA                ((uint8)(0x05u))
#define CANNM_SERVICE_ID_TRANSMIT                   ((uint8)(0x14u))
#define CANNM_SERVICE_ID_GETNODEIDENTIFIER          ((uint8)(0x06u))
#define CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER     ((uint8)(0x07u))
#define CANNM_SERVICE_ID_REPEATMESSAGEREQUEST       ((uint8)(0x08u))
#define CANNM_SERVICE_ID_GETPDUDATA                 ((uint8)(0x0au))
#define CANNM_SERVICE_ID_GETSTATE                   ((uint8)(0x0bu))
#define CANNM_SERVICE_ID_GETVERSIONINFO             ((uint8)(0xf1u))
#define CANNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION  ((uint8)(0xc0u))
#define CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION ((uint8)(0xd0u))
#define CANNM_SERVICE_ID_SETSLEEPREADYBIT           ((uint8)(0x17u))
#define CANNM_SERVICE_ID_TXCONFIRMATION             ((uint8)(0x40u))
#define CANNM_SERVICE_ID_RXINDICATION               ((uint8)(0x42u))
#define CANNM_SERVICE_ID_CONFIRMPNAVAILABILITY      ((uint8)(0x16u))
#define CANNM_SERVICE_ID_TRIGGERTRANSMIT            ((uint8)(0x41u))
#define CANNM_SERVICE_ID_MAINFUNCTION               ((uint8)(0x13u))
/** @} */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/******************************************************************************/
/**
 * @brief       Initialize the CanNm module.
 * @param[in]   CanNmConfigPtr : Pointer to a selected configuration structure.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70050
 */
void CanNm_Init(const CanNm_ConfigType* CanNmConfigPtr);

/**
 * @brief       De-initializes the CanNm module.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70051
 */
void CanNm_DeInit(void);

/**
 * @brief       Passive startup of the AUTOSAR CAN NM. It triggers the transition from Bus-Sleep Mode or Prepare Bus
 * Sleep Mode to the Network Mode in Repeat Message State.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Passive startup of network management has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-70052
 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle);

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       Request the network, since ECU needs to communicate on the bus.
 * @param[in]   nmChannelHandle : Identification of the NM-channel.
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Requesting of network has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-70053
 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);

/**
 * @brief       Release the network, since ECU doesn't have to communicate on the bus.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Releasing of network has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-70054
 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);
#endif

#if CANNM_COM_CONTROL_ENABLED == STD_ON
/**
 * @brief       Disable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Disabling of NM PDU transmission ability has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-70055
 */
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);

/**
 * @brief       Enable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Enabling of NM PDU transmission ability has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-70056
 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);
#endif

#if (CANNM_USER_DATA_ENABLED == STD_ON) && (CANNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief       Set user data for NM PDUs transmitted next on the bus.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmUserDataPtr   : Pointer where the user data for the next transmitted NM PDU shall be copied from
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Setting of user data has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous TRUE
 * @trace       CPD-70057
 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr);
#endif

#if CANNM_USER_DATA_ENABLED == STD_ON
/**
 * @brief       Get user data out of the most recently received NM PDU.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmUserDataPtr   : Pointer where user data out of the most recently received NM PDU shall be copied to
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of user data has failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70058
 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr);
#endif

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief       Requests transmission of a PDU.
 * @param[in]   TxPduId    : Identifier of the PDU to be transmitted
 * @param[in]   PduInfoPtr : Length of and pointer to the PDU data and pointer to MetaData.
 * @return      Std_ReturnType
 * @retval      E_OK     : Transmit request has been accepted.
 * @retval      E_NOT_OK : Transmit request has not been accepted.
 * @reentrant   TRUE(but not for the same PduIds)
 * @synchronous TRUE
 * @trace       CPD-70059
 */
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif

/**
 * @brief       Get node identifier out of the most recently received NM PDU.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmNodeIdPtr     : Pointer where node identifier out of the most recently received NM PDU shall be copied
 * to.
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of the node identifier out of the most recently received NM PDU has failed or is not
 * configured for this network handle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70060
 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Get node identifier configured for the local node.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmNodeIdPtr     : Pointer where node identifier of the local node shall be copied to.
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of the node identifier of the local node has failed or is not configured for this
 * network handle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70061
 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Set Repeat Message Request Bit for NM PDUs transmitted next on the bus.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Setting of Repeat Message Request Bit has failed or is not configured for this network
 *                         handle.
 * @reentrant   FALSE
 * @synchronous TRUE (but not for the same NM-channel)
 * @trace       CPD-70062
 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);

/**
 * @brief       Get the whole PDU data out of the most recently received NM PDU.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmPduDataPtr    : Pointer where NM PDU shall be copied to.
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of NM PDU Data has failed or is not configured for this network handle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70063
 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr);

/**
 * @brief       Returns the state and the mode of the network management.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[out]  nmStatePtr      : Pointer where state of the network management shall be copied to
 * @param[out]  nmModePtr       : Pointer where the mode of the network management shall be copied to
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of NM state has failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70064
 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

#if CANNM_VERSION_INFO_API == STD_ON
/**
 * @brief       This service returns the version information of this module.
 * @param[out]   versioninfo : Pointer to where to store the version information of this module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70065
 */
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if (CANNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief       Request bus synchronization.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Requesting of bus synchronization has failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70066
 */
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);
#endif

#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
/**
 * @brief       Check if remote sleep indication takes place or not.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @param[out]   nmRemoteSleepIndPtr   : Pointer where check result of remote sleep indication shall be copied to
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Checking of remote sleep indication bits has failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70067
 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
/**
 * @brief       Set the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @param[in]   nmSleepReadyBit   : Pointer where check result of remote sleep indication shall be copied to
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Writing of remote sleep indication bit has failed
 * @reentrant   TRUE (but not for the same NM-channel)
 * @synchronous TRUE
 * @trace       CPD-70068
 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);
#endif

/**
 * @brief       Indication of a received PDU from a lower layer communication interface module.
 * @param[in]   RxPduId    : ID of the received PDU.
 * @param[in]   PduInfoPtr : Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 *                           containing the PDU, and the MetaData related to this PDU.
 * @reentrant   TRUE (but not for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-70070
 */
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#if (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief       The lower layer communication interface module confirms the transmission of a PDU, or the failure to
 *              transmit a PDU.
 * @param[in]   TxPduID   : ID of the PDU that has been transmitted.
 * @param[in]   result    : E_OK: The PDU was transmitted
 *                          E_NOT_OK: Transmission of the PDU failed.
 * @reentrant   TRUE (but not for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-70069
 */
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
/**
 * @brief       Enables the PN filter functionality on the indicated NM channel.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel.
 * @reentrant   TRUE (but not for the same NM-channel)
 * @synchronous TRUE
 * @trace       CPD-70071
 */
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);
#endif

#if CANNM_TRIGGER_TRANSMIT_API == STD_ON
/**
 * @brief        Within this API, the upper layer module (called module) shall check whether the available data fits
 *               into the buffer size reported by PduInfoPtr->SduLength.
 * @param[in]    TxPduId    : ID of the SDU that is requested to be transmitted.
 * @param[inout] PduInfoPtr : Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied, and
 *               the available buffer size in SduLengh. On return, the service will indicate the length of the copied
 *               SDU data in SduLength.
 * @return       Std_ReturnType
 * @retval       E_OK       : SDU has been copied and SduLength indicates the number of copied bytes.
 * @retval       E_NOT_OK   : No SDU data has been copied. PduInfoPtr must not be used since it may contain a NULL
 *                            pointer or point to invalid data.
 * @reentrant   TRUE (but not for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-70072
 */
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#endif

/**
 * @brief        Main function of the CanNm which processes the algorithm describes in that document.
 * @param[in]    chIndex    : Identification of the NM-channel
 * @reentrant   TRUE (but not for the same Identification of the NM-channel)
 * @synchronous TRUE
 * @trace       CPD-70073
 */
void CanNm_MainFunction(CanNm_ChannelIndexType chIndex);
/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
