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
 **  @file               : UdpNm.h
 **  @author             : caihong.liu
 **  @date               : 2025/01/16
 **  @vendor             : isoft
 **  @description        : Udp Netwprk Manager
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2019-11-18  lili.wang     Initial version
 *  V02.00.00 2020-06-09  rongbo.hu     1. Update code according AUTOSAR R19-11 Specification
 *                                      2. Modify code with the new coding rule
 *                                      3. Fix bugs find in V1.0.4
 *  V02.01.00 2024-12-30  caihong.liu   R23-11 development first release.
 *  V02.01.01 2025-10-22  caihong.liu
 *      1. Fixed CPT-14266, Redundant passive wake-up event flags lead to repeated wake-up actions.
 *      2. Fixed CPT-14866, Fixed race condition in ReadySleepState causing unhandled UdpNm_RepeatMessageRequest due to
 *         immediate transition to PrepareBusSleepMode.
 *      3. Fixed CPT-14491, state machine stuck in ReadySleepState due to nmTimeoutTimer is 0 but nmToutFlg is FALSE.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:UdpNm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_UdpNm_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

#ifndef UDPNM_H_
#define UDPNM_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "UdpNm_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name Published Information of UdpNm
 * @{
 * @brief Published Information
 */
#define UDPNM_VENDOR_ID                   (62u)
#define UDPNM_MODULE_ID                   (33u)
#define UDPNM_INSTANCE_ID                 (1u)
#define UDPNM_AR_RELEASE_MAJOR_VERSION    (4u)
#define UDPNM_AR_RELEASE_MINOR_VERSION    (9u)
#define UDPNM_AR_RELEASE_REVISION_VERSION (0u)
#define UDPNM_SW_MAJOR_VERSION            (2u)
#define UDPNM_SW_MINOR_VERSION            (1u)
#define UDPNM_SW_PATCH_VERSION            (1u)
/** @} */

/* ===================================================== macros ===================================================== */
/**
 * @name Development Errors of UdpNm
 * @{
 * @brief Development Errors
 */
#define UDPNM_E_UNINIT              ((uint8)(0x01u))
#define UDPNM_E_INVALID_CHANNEL     ((uint8)(0x02u))
#define UDPNM_E_INVALID_PDUID       ((uint8)(0x03u))
#define UDPNM_E_INIT_FAILED         ((uint8)(0x04u))
#define UDPNM_E_PARAM_POINTER       ((uint8)(0x12u))
#define UDPNM_E_NETWORK_TIMEOUT     ((uint8)(0x11u))
#define UDPNM_E_PARTITION           ((uint8)(0x13u))
#define UDPNM_E_ALREADY_INITIALIZED ((uint8)(0xF0u))
#define UDPNM_E_INVALID_ID          ((uint8)(0xFFu))
/** @} */

/**
 * @name Service ID of UdpNm
 * @{
 * @brief Service ID
 */
#define UDPNM_SERVICE_ID_INIT                       ((uint8)(0x01u))
#define UDPNM_SERVICE_ID_PASSIVESTARUP              ((uint8)(0x0eu))
#define UDPNM_SERVICE_ID_NETWORKREQUEST             ((uint8)(0x02u))
#define UDPNM_SERVICE_ID_NETWORKRELEASE             ((uint8)(0x03u))
#define UDPNM_SERVICE_ID_DISABLECOMMUNICATION       ((uint8)(0x0cu))
#define UDPNM_SERVICE_ID_ENABLECOMMUNICATION        ((uint8)(0x0du))
#define UDPNM_SERVICE_ID_SETUSERDATA                ((uint8)(0x04u))
#define UDPNM_SERVICE_ID_GETUSERDATA                ((uint8)(0x05u))
#define UDPNM_SERVICE_ID_GETNODEIDENTIFIER          ((uint8)(0x06u))
#define UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER     ((uint8)(0x07u))
#define UDPNM_SERVICE_ID_REPEATMESSAGEREQUEST       ((uint8)(0x08u))
#define UDPNM_SERVICE_ID_GETPDUDATA                 ((uint8)(0x0au))
#define UDPNM_SERVICE_ID_GETSTATE                   ((uint8)(0x0bu))
#define UDPNM_SERVICE_ID_GETVERSIONINFO             ((uint8)(0x09u))
#define UDPNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION  ((uint8)(0x14u))
#define UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION ((uint8)(0x11u))
#define UDPNM_SERVICE_ID_SETCOORDBITS               ((uint8)(0x12u))
#define UDPNM_SERVICE_ID_SETSLEEPREADYBIT           ((uint8)(0x16u))
#define UDPNM_SERVICE_ID_TRANSMIT                   ((uint8)(0x49u))
#define UDPNM_SERVICE_ID_IFTXCONFIRMATION           ((uint8)(0x40u))
#define UDPNM_SERVICE_ID_RXINDICATION               ((uint8)(0x42u))
#define UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT          ((uint8)(0x41u))
#define UDPNM_SERVICE_ID_MAINFUNCTION               ((uint8)(0x13u))
/** @} */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
/**
 * @brief       Initialize the complete UdpNm module.
 * @param[in]   UdpNmConfigPtr   : Pointer to a selected configuration
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71891
 */
void UdpNm_Init(const UdpNm_ConfigType* UdpNmConfigPtr);

/**
 * @brief       Passive startup of the AUTOSAR UdpNm.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK    : No error
 * @retval      E_NOT_OK: Passive startup of network management has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-71892
 */
Std_ReturnType UdpNm_PassiveStartUp(NetworkHandleType nmChannelHandle);

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       Request the network, since ECU needs to communicate on the bus.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK    : No error
 * @retval      E_NOT_OK: Requesting of network has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-71893
 */
Std_ReturnType UdpNm_NetworkRequest(NetworkHandleType nmChannelHandle);

/**
 * @brief       Release the network,since ECU doesn't have to communicate on the bus.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK    : No error
 * @retval      E_NOT_OK: Releasing of network has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-71894
 */
Std_ReturnType UdpNm_NetworkRelease(NetworkHandleType nmChannelHandle);
#endif

#if UDPNM_COM_CONTROL_ENABLED == STD_ON
/**
 * @brief       Disable the NM PDU transmission ability due to a ISO14229 Communication Control (0x28) service.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK    : No error
 * @retval      E_NOT_OK: Disabling of NM PDU transmission ability has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-71895
 */
Std_ReturnType UdpNm_DisableCommunication(NetworkHandleType nmChannelHandle);
/**
 * @brief       Enable the NM PDU transmission ability due to a ISO14229 Communication Control (0x28) service.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK    : No error
 * @retval      E_NOT_OK: Enabling of NM PDU transmission ability has failed
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-71896
 */
Std_ReturnType UdpNm_EnableCommunication(NetworkHandleType nmChannelHandle);
#endif

#if (UDPNM_USER_DATA_ENABLED == STD_ON) && (UDPNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief       Set user data for all NM messages transmitted on the bus after this function has returned without error.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmUserDataPtr   : Pointer where the user data for the next transmitted NM message shall be copied from.
 * @return      Std_ReturnType
 * @retval      E_OK    : No error
 * @retval      E_NOT_OK: Setting of user data has failed.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71897
 */
Std_ReturnType UdpNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr);
#endif

#if UDPNM_USER_DATA_ENABLED == STD_ON
/**
 * @brief       Get user data from the most recently received NM message.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmUserDataPtr   : Pointer where user data out of the most recently received NM PDU shall be copied to.
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of user data has failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71898
 */
Std_ReturnType UdpNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr);
#endif

/**
 * @brief       Get node identifier from the most recently received NM PDU.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[out]  nmNodeIdPtr     : Pointer where node identifier out of the most recently received NM PDU shall be copied
 * to.
 * @return      Std_ReturnType
 * @retval      E_OK            : No error
 * @retval      E_NOT_OK        : Getting of the node identifier out of the most recently received NM PDU has failed or
 * is not configured for this network handle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71899
 */
Std_ReturnType UdpNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Get node identifier configured for the local node.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[out]  nmNodeIdPtr     : Pointer where node identifier of the local node shall be copied to.
 * @return      Std_ReturnType
 * @retval      E_OK            : No error
 * @retval      E_NOT_OK        : Getting of the node identifier of the local node has failed or is not configured for
 *                                this network handle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71900
 */
Std_ReturnType UdpNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Set Repeat Message Request Bit for all NM messages transmitted on the bus after this function has
 *              returned without error.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK            : No error
 * @retval      E_NOT_OK        : Setting of Repeat Message Request Bit has failed or is not configured for this network
 *                                handle.
 * @reentrant   TRUE(but not for the same NM-Channel)
 * @synchronous FALSE
 * @trace       CPD-71901
 */
Std_ReturnType UdpNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);

/**
 * @brief       Get the whole PDU data out of the most recently received NM message.
 * @id          0x0a
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[out]  nmPduDataPtr    : Pointer where NM PDU shall be copied to.
 * @return      Std_ReturnType
 * @retval      E_OK            : No error
 * @retval      E_NOT_OK        : Getting of NM PDU Data has failed or is not configured for this network handle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71902
 */
Std_ReturnType UdpNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr);

/**
 * @brief       Returns the state and the mode of the network management.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[out]  nmStatePtr : Pointer where state of the network management shall be copied to.
 * @param[out]  nmModePtr  : Pointer where the mode of the network management shall be copied to
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Getting of NM state has failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71903
 */
Std_ReturnType UdpNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

#if UDPNM_VERSION_INFO_API == STD_ON
/**
 * @brief       Returns the version information of this module.
 * @param[in]   versioninfo   : Pointer to where to store the version information of this module
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71904
 */
void UdpNm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if (UDPNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief       Request bus synchronization.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Requesting of bus synchronization has failed
 * @reentrant   FALSE
 * @synchronous FALSE
 * @trace       CPD-71905
 */
Std_ReturnType UdpNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);
#endif

#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
/**
 * @brief       Check if remote sleep indication takes place or not.
 * @param[in]   nmChannelHandle   : Identification of the NM-channel
 * @param[out]  nmRemoteSleepIndPtr : Pointer where check result of remote sleep indication shall be copied to.
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Checking of remote sleep indication bits failed
 * @reentrant   TRUE
 * @synchronous TRUE(but not for the same NM-Channel)
 * @trace       CPD-71906
 */
Std_ReturnType UdpNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
/**
 * @brief       Set the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 * @param[in]   nmChannelHandle : Identification of the NM-channel
 * @param[in]   nmSleepReadyBit : Value written to ReadySleep Bit in CBV
 * @return      Std_ReturnType
 * @retval      E_OK     : No error
 * @retval      E_NOT_OK : Writing of remote sleep indication bit has failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-71907
 */
Std_ReturnType UdpNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);
#endif

#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief       Requests transmission of a PDU.
 * @param[in]   TxPduId    : Identifier of the PDU to be transmitted
 * @param[in]   PduInfoPtr : Length of and pointer to the PDU data and pointer to MetaData.
 * @return      Std_ReturnType
 * @retval      E_OK     :  Transmit request has been accepted.
 * @retval      E_NOT_OK :  Transmit request has not been accepted.
 * @reentrant   TRUE(for different PduIds. Non reentrant for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-71908
 */
Std_ReturnType UdpNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif

/**
 * @brief       The lower layer communication interface module confirms the transmission of an IPDU,
 *              or the failure to transmit a PDU.
 * @param[in]   TxPduId  : Identifier of the PDU to be transmitted
 * @param[in]   result   : E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * @reentrant   TRUE(for different PduIds. Non reentrant for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-71909
 */
void UdpNm_SoAdIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 * @brief       Indication of a received PDU from a lower layer communication interface module.
 * @param[in]   RxPduId    : ID of the received PDU.
 * @param[in]   PduInfoPtr : Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 * (SduDataPtr) containing the I-PDU.
 * @reentrant   TRUE(for different PduIds. Non reentrant for the same PduId.)
 * @synchronous TRUE
 * @trace       CPD-71910
 */
void UdpNm_SoAdIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#if UDPNM_TRIGGER_TRANSMIT_API == STD_ON
/**
 * @brief       Within this API, the upper layer module (called module) shall check whether the available data
                fits into the buffer size reported by PduInfoPtr->SduLength. If it fits, it shall copy its data into the
                buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied data in
                PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 * @param[in]   TxPduId    : ID of the SDU that is requested to be transmitted.
 * @param[in]   PduInfoPtr : Contains a pointer to a buffer (SduDataPtr) to where the SDU
                             data shall be copied, and the available buffer size in SduLengh.
                             On return, the service will indicate the length of the copied SDU
                             data in SduLength.
 * @reentrant   TRUE(for different PduIds. Non reentrant for the same PduId.)
 * @synchronous TRUE
 * @trace       CPD-71911
 */
Std_ReturnType UdpNm_SoAdIfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#endif

/**
 * @brief       Main function of the UdpNm which processes the algorithm describes in that document.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE(for different NM-channel)
 * @synchronous TRUE
 * @trace       CPD-71912
 */
void UdpNm_MainFunction(UdpNm_ChannelIndexType chIndex);
/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
