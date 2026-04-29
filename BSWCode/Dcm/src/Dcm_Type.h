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
 **  @file               : Dcm_Type.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Public Type Definition
 **
 ***********************************************************************************************************************/
#ifndef DCM_TYPE_H_
#define DCM_TYPE_H_
/* PRQA S 5004, 1271, 0288 EOF */ /* VL_Dcm_5004, VL_Dcm_1271, VL_Dcm_0288 */
/* =================================================== inclusions =================================================== */
#include "Dcm.h"
#if (STD_ON == DCM_USE_NVM)
#include "NvM.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define DCM_SECURE_CODING 0u /**< Secure coding */
/* ================================================ type definitions ================================================ */
/**
 * @brief Endianness enum
 */
typedef enum
{
    DCM_BIG_ENDIAN,    /**< Big Endian @range 0x00*/
    DCM_LITTLE_ENDIAN, /**< Little Endian @range 0x01*/
    DCM_OPAQUE         /**< opaque @range 0x02*/
} Dcm_EndiannessType;

/**
 * @brief The function confirms to the app the successful transmission or a transmission error of a diagnostic service.
 */
typedef Std_ReturnType (*Dcm_DsdServiceRequestNotificationConfirmationFncType)(
    uint8                      SID,
    uint8                      ReqType,
    uint16                     ConnectionId,
    Dcm_ConfirmationStatusType ConfirmationStatus,
    Dcm_ProtocolType           ProtocolType,
    uint16                     TesterSourceAddress);

/**
 * @brief The function indicates to the app that a service is about to be executed and allows the application to reject
 * the execution of the service request
 */
typedef Std_ReturnType (*Dcm_DsdServiceRequestNotificationIndicationFncType)(
    uint8                         SID,
    const uint8*                  RequestData,
    uint32                        DataSize,
    uint8                         ReqType,
    uint16                        ConnectionId,
    Dcm_NegativeResponseCodeType* ErrorCode,
    Dcm_ProtocolType              ProtocolType,
    uint16                        TesterSourceAddress);

/**
 * @brief service notification callouts
 */
typedef struct
{
    Dcm_DsdServiceRequestNotificationConfirmationFncType
        Confirmation; /**<Handles service request notification confirmation @range NA */
    Dcm_DsdServiceRequestNotificationIndicationFncType
        Indication; /**<Indicates service request notification occurrence @range NA */
} Dcm_DsdServiceRequestNotificationType;

/**
 * @brief authenticationRow type
 */
typedef struct
{
    uint8 AuthenticationRoleBitPosition; /**< DcmDspAuthenticationRoleBitPosition: Defines the bit number that
                                          * represents the role in the authentication bit field. The bit mask is aligned
                                          * with the logical bit calculations: 2ˆDcmDspAuthenticationRoleBit Position
                                          * @range 0..255 */
} Dcm_ServiceRoleType;

/**
 * @brief Request to application for asynchronous comparing key
 */
typedef Std_ReturnType (*Dcm_DspSecurityCompareKeyFncType)(
    const uint8*                  Key,
    Dcm_OpStatusType              OpStatus,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief Request to application for asynchronous provision of seed value
 */
typedef Std_ReturnType (*Dcm_DspSecurityGetSeedFncType)(
    const uint8*                  SecurityAccessDataRecord,
    Dcm_OpStatusType              OpStatus,
    uint8*                        Seed,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief Read the attempt counter for a specific security level from the application
 */
typedef Std_ReturnType (*Dcm_DspSecurityGetAttemptCounterFncType)(Dcm_OpStatusType OpStatus, uint8* AttemptCounter);

/**
 * @brief Set the attempt counter for a specific security level in the application
 */
typedef Std_ReturnType (*Dcm_DspSecuritySetAttemptCounterFncType)(Dcm_OpStatusType OpStatus, uint8 AttemptCounter);

/**
 * @brief DcmDspSecurityRow configuration
 */
typedef struct
{
    uint32 AdrSize; /**< DcmDspSecurityADRSize: Size in bytes of the AccessDataRecord used in GetSeed @range
                       0..0xFFFFFFFF*/
    boolean AttemptCounterEnabled; /**< DcmDspSecurityAttemptCounterEnabled: controls the existence of the APIs to set /
                                    * get the attempt counter values towards application @range 0..1*/
    Dcm_DspSecurityCompareKeyFncType CompareKeyFnc; /**< DcmDspSecurityCompareKeyFnc: Function name to request the
                                                       result of a key comparison @range NA*/
    uint32 DelayTime;       /**< DcmDspSecurityDelayTime: Delay time after failed security access in seconds @range
                               0..0xFFFFFFFF*/
    uint32 DelayTimeOnBoot; /**< DcmDspSecurityDelayTimeOnBoot: Value of the delay timer in case of ’power on’ in
                               seconds @range 0..0xFFFFFFFF */
    Dcm_DspSecurityGetAttemptCounterFncType
        GetAttemptCounterFnc; /**< DcmDspSecurityGetAttemptCounterFnc: Function name to request the value of an attempt
                                 counter @range NA */
    Dcm_DspSecurityGetSeedFncType
           GetSeedFnc;    /**< DcmDspSecurityGetSeedFnc: Callout function name used to request a seed @range NA */
    uint32 KeySize;       /**< DcmDspSecurityKeySize: size of the security key (in Bytes) @range 0..0xFFFFFFFF */
    uint8  SecurityLevel; /**< DcmDspSecurityLevel: Value of Security level @range 0..255*/
    uint8  NumAttDelay; /**< DcmDspSecurityNumAttDelay: Number of failed security accesses after which the delay time is
                           activated @range 0..255*/
    uint32 SeedSize;    /**< DcmDspSecuritySeedSize: size of the security seed (in Bytes) @range 0..0xFFFFFFFF*/
    Dcm_DspSecuritySetAttemptCounterFncType SetAttemptCounterFnc; /**< DcmDspSecuritySetAttemptCounterFnc: Function name
                                                                     to set the value of an attempt counter @range NA */
} Dcm_DspSecurityRowType;

/**
 * @brief SessionForBoot enum
 */
typedef enum
{
    DCM_NO_BOOT,  /**< This diagnostic session doesn’t allow to jump to Bootloader @range 0x00*/
    DCM_OEM_BOOT, /**< This diagnostic session allows to jump to OEM Bootloader and bootloader sends final response
                     @range 0x01*/
    DCM_OEM_BOOT_RESPAPP, /**< This diagnostic session allows to jump to OEM Bootloader and application sends final
                             response @range 0x02 */
    DCM_SYS_BOOT, /**< This diagnostic session allows to jump to System Supplier Bootloader and bootloader sends final
                     response @range 0x03 */
    DCM_SYS_BOOT_RESPAPP /**< This diagnostic session allows to jump to System Supplier Bootloader and application sends
                            final response @range 0x04*/
} Dcm_DspSessionForBootType;

/**
 * @brief DcmDspSessionRow Configuration
 */
typedef struct
{
    Dcm_DspSessionForBootType
        SessionForBoot; /**< DcmDspSessionForBoot: This parameter defines whether this diagnostic session allows to jump
                         * to Bootloader and determines, from which unit the final response will be sent @range NA */
    uint8  SessionLevel;    /**< DcmDspSessionLevel: subFunction value of the DiagnosticSession @range 0..255*/
    uint16 P2ServerMax;     /**< DcmDspSessionP2ServerMax: This is the session value for P2ServerMax in seconds (per
                               Session) @range 0..0xFFFF */
    uint32 P2StarServerMax; /**< DcmDspSessionP2StarServerMax: This is the session value for P2*ServerMax in seconds
                               (per Session) @range 0..0xFFFFFFFF */
} Dcm_DspSessionRowType;

/**
 * @brief Function prototype for service entry, named by parameter SidTabFnc
 */
typedef Std_ReturnType (*Dcm_ServcieFncType)(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief general mode rule function
 */
typedef Std_ReturnType (*Dcm_ModeRuleFncType)(Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_ADDRESS_FORMAT)
/**
 * @brief service request addressing format
 */
typedef enum
{
    DCM_PHYSICAL   = 0u, /**< Physical addressing format @range 0x00*/
    DCM_FUNCTIONAL = 1u, /**< Functional addressing format @range 0x00*/
    DCM_PHYANDFUNC = 2u  /**< Physical and functional addressing format @range 0x00*/
} Dcm_DcmDsdAddressingFormatType;
#endif

/**
 * @brief DcmDsdSubService Configuration
 */
typedef struct
{
    Dcm_ServcieFncType SubServiceFnc; /**< DcmDsdSubServiceFnc: Callback function of the ECU Supplier specific component
                                         for the particular service @range NA */
    uint8                     SubServiceId; /**< DcmDsdSubServiceId: Identifier of the subservice @range 0..255*/
    const Dcm_ModeRuleFncType SubServiceModeRuleRef; /**< DcmDsdSubServiceModeRuleRef: Reference to a DcmDspModeRule
                                                        which controls the execution of the subservice @range NA */
    uint8 SubServiceRoleRefNum;                      /**< The number of SubServiceRoleRef @range 0..255*/
    const uint8*
        SubServiceRoleRef; /**< DcmDsdSubServiceRoleRef: Reference to DcmDspAuthenticationRow that defines a role in
                            * that the service with this subfunction is allowed to be executed @range NA*/
    uint8 SubServiceSecurityLevelRefNum; /**< The number of SubServiceSecurityLevelRef @range 0..255*/
    const Dcm_SecLevelType*
        SubServiceSecurityLevelRef; /**< DcmDsdSubServiceSecurityLevelRef: Reference to a Security Level in which the
                                     * subservice is allowed to be executed @range NA */
    uint8                  SubServiceSessionLevelRefNum; /**< The number of SubServiceSessionLevelRef @range 0..255*/
    const Dcm_SesCtrlType* SubServiceSessionLevelRef; /**< DcmDsdSubServiceSessionLevelRef: Reference to a Session Level
                                                         in which the subservice is allowed to be executed @range NA*/
#if (STD_ON == DCM_ADDRESS_FORMAT)
    Dcm_DcmDsdAddressingFormatType
        SubServiceAddressingFormat; /**< DcmDsdSubServiceAddressingFormat: supported addressing format @range NA*/
#endif
#if (STD_ON == DCM_P4_SERVER)
    uint32 SubServiceP4ServerMax; /**< DcmDsdSubServiceP4ServerMax: value for P4ServerMax in seconds (per SubService)
                                     @range 0..0xFFFFFFFF*/
#endif
} Dcm_DsdSubServiceType;

/**
 * @brief DcmDsdService Configuration
 */
typedef struct
{
    Dcm_ServcieFncType SidTabFnc; /**< DcmDsdSidTabFnc: Callback function of the ECU Supplier specific component for the
                                     particular service @range NA */
    uint8   SidTabServiceId;      /**< DcmDsdSidTabServiceId: Identifier of the service @range 0..255*/
    boolean SubfuncAvail; /**< DcmDsdSidTabSubfuncAvail: Information about whether the service has subfunctions or not
                             @range 0..1 */
#if (STD_ON == DCM_SUPPRESS_POS_RSP)
    boolean SuppressPosRsp;
#endif

    uint8        ServiceRoleRefNum; /**< The number of ServiceRoleRef @range 0..255*/
    const uint8* ServiceRoleRef; /**< DcmDsdServiceRoleRef: Reference to DcmDspAuthenticationRow that defines a role in
                                    that the service is allowed to be executed @range NA */
    const Dcm_ModeRuleFncType SidTabModeRuleRef;       /**< DcmDsdSidTabModeRuleRef: Reference to a DcmDspModeRule which
                                                          controls the execution of the service @range NA */
    uint8                   SidTabSecurityLevelRefNum; /**< The number of DcmDsdSidTabSecurityLevelRef @range 0..255*/
    const Dcm_SecLevelType* SidTabSecurityLevelRef;  /**< DcmDsdSidTabSecurityLevelRef: Reference to a Security Level in
                                                        which the service is allowed to be executed @range NA */
    uint8                  SidTabSessionLevelRefNum; /**< The number of SidTabSessionLevelRef @range 0..255*/
    const Dcm_SesCtrlType* SidTabSessionLevelRef;    /**< DcmDsdSidTabSessionLevelRef: Reference to a Session Level in
                                                        which the service is allowed to be executed @range NA */
    uint8  SubServiceNum;                            /**< The number of DcmDsdSubService @range 0..255*/
    uint16 SubServiceIndex; /**< DcmDsdSubService: The related DcmDsdSubService index @range 0..0xFFFF*/
#if (STD_ON == DCM_ADDRESS_FORMAT)
    Dcm_DcmDsdAddressingFormatType
        SidTabAddressingFormat; /**< DcmDsdSidTabAddressingFormat: supported addressing format @range NA*/
#endif
#if (STD_ON == DCM_P4_SERVER)
    uint32 SidTabP4ServerMax; /**< DcmDsdSidTabP4ServerMax: value for P4ServerMax in seconds (per Service) @range
                                 0..0xFFFFFFFF*/
#endif
} Dcm_DsdServiceType;

/**
 * @brief DcmDsdServiceTable Configuration
 */
typedef struct
{
    uint16 ServiceNum;   /**< The number of configured DcmDsdService @range 0..0xFFFF*/
    uint16 ServiceIndex; /**< The related DcmDsdService index @range 0..0xFFFF*/
} Dcm_DsdServiceTableType;

/**
 * @brief This function allows the application to examine the environment conditions and enable/disable further
 * processing of the protocol for StartProtocol and informs the application of the protocol stop for StopProtocol
 */
typedef Std_ReturnType (*Dcm_DslCallbackDCMRequestServiceFncType)(
    Dcm_ProtocolType ProtocolType,
    uint16           TesterSourceAddress,
    uint16           ConnectionId);

/**
 * @brief CallbackDCMRequestService Configuration
 */
typedef struct
{
    Dcm_DslCallbackDCMRequestServiceFncType StartProtocol; /**< The Function name for start protocol @range NA*/
    Dcm_DslCallbackDCMRequestServiceFncType StopProtocol;  /**< The Function name for stop protocol @range NA*/
} Dcm_DslCallbackDCMRequestServiceType;

/**
 * @brief The transmission type of the protocol.
 */
typedef enum
{
    DCM_TYPE1, /**< Messages on the DcmTxPduId already used for normal diagnostic responses. The outgoing messages must
                * be synchronized with ’normal outgoing messages’, which have a higher priority @range 0x00*/
    DCM_TYPE2  /**< Messages on a separate DcmTxPduId @range 0x01*/
} Dcm_DslProtocolTransType;

/**
 * @brief The protocolRxAddr enum
 */
typedef enum
{
    DCM_PHYSICAL_TYPE   = 0u, /**< PHYSICAL = 1 to 1 communications using physical addressing @range 0x00*/
    DCM_FUNCTIONAL_TYPE = 1u, /**< FUNCTIONAL = 1 to n communication @range 0x01*/
} Dcm_DslProtocolRxAddrType;

/**
 * @brief DcmDslProtocolRx Configuration
 */
typedef struct
{
    Dcm_DslProtocolRxAddrType
           AddrType; /**< DcmDslProtocolRxAddrType: Selects the addressing type of the reception channel @range NA */
    uint16 ParentMainConnectionId; /**< The parent mainConnection index @range 0..0xFFFF*/
} Dcm_DslProtocolRxType;

/**
 * @brief general communication control function
 */
typedef Std_ReturnType (*Dcm_ComCtrlFncType)(uint8 mode);

/**
 * @brief mainConnection configuration
 */
typedef struct
{
    uint16 RxConnectionId; /**< DcmDslProtocolRxConnectionId: Unique identifier of the tester which uses this connection
                            * for diagnostic communication @range 0..0xFFFF*/
    uint16 TesterSourceAddr; /**< DcmDslProtocolRxTesterSourceAddr: Tester source address uniquely describes a client
                                and will be used @range 0..0xFFFF*/
    uint16 PeriodicConnectionIdNum; /**< the number of DcmDslProtocolPeriodicConnection @range 0..0xFFFF*/
    uint16 PeriodicConnectionId;    /**< DcmDslPeriodicTransmissionConRef: the related DcmDslProtocolPeriodicConnection
                                       index @range 0..0xFFFF*/
    NetworkHandleType ProtocolComMChannelRef;   /**< DcmDslProtocolComMChannelRef: Reference to the ComMChannel on which
                                                 * the DcmDslProtocolRxPdu is received and the   DcmDslProtocolTxPdu is
                                                 * transmitted @range NA*/
    Dcm_ComCtrlFncType ProtocolComCtrlFnc;      /**< communication mode switch interface @range NA*/
    const PduIdType*   ProtocolTx;              /**< DcmDslProtocolTx: the configured DcmDslProtocolTx @range NA*/
    uint8              ParentProtocolId;        /**< the parent protocol index @range 0..255*/
    boolean MainConnectionUseGenericConnection; /**< TRUE when any referenced pdu has configured metadata sa/ta @range
                                                   0..1*/
#if (STD_ON == DCM_MULTICORE_ENABLED)
    uint16 ApplicationId; /**< the application id the ComM channel belongs to @range 0..0xFFFF*/
#endif
} Dcm_DslMainConnectionType;

/**
 * @brief This container contains the configuration of one particular diagnostic protocol used in Dcm.
 */
typedef struct
{
    uint16 MaximumResponseSize; /**< DcmDslProtocolMaximumResponseSize: This parameter defines the maximum length of the
                                 * response message case DcmPagedBufferEnabled == TRUE @range 0..0xFFFF*/
    uint8 Priority; /**< DcmDslProtocolPriority: Protocol priority used during protocol preemption @range 0..255*/
    Dcm_ProtocolType ProtocolType; /**< DcmDslProtocolType: The diagnostic protocol type for the DCM DSL protocol that
                                      is being configured @range NA */
    uint16 EcuAddr; /**< DcmDspProtocolEcuAddr: Ecu source address used for diagnostic communication @range 0..0xFFFF */
    boolean SendRespPendOnRestart; /**< DcmSendRespPendOnRestart: If set to TRUE, the Dcm will send a NRC 0x78 before a
                                    * transition to bootloader or performing an ECU reset. If set to False, no 0x78 is
                                    * send in this case @range 0..1 */
    uint16 P2ServerAdjust;         /**< DcmTimStrP2ServerAdjust: This parameter is used to guarantee that the diagnostic
                                    * response is available on the bus         before reaching P2 by adjusting the current
                                    * P2ServerMax
                                    * @range 0..0xFFFF */
    uint16 P2StarServerAdjust; /**< DcmTimStrP2StarServerAdjust: This parameter is used to guarantee that the diagnostic
                                * response is available on the bus before reaching P2Star by adjusting the current
                                * P2StarServerMax @range 0..0xFFFF */
    uint8  DemClientRef;       /**< DcmDemClientRef: Reference to DemClient in Dem configuration @range 0..255 */
    uint32 RxBufferSize;       /**< DcmDslProtocolRxBufferRef: The size of referenced diagnostic buffer that is used for
                                * diagnostic request reception       for the protocol @range 0..0xFFFFFFFF*/
    uint32 RxBufferOffset; /**< the calculated offset of this protocol in the whole RxBuffer area @range 0..0xFFFFFFFF*/
    const Dcm_DsdServiceTableType* SidTable; /**< DcmDslProtocolSIDTable: Reference to a service table that is used for
                                              * diagnostic request processing for this protocol @range NA */
    uint32 TxBufferSize; /**< DcmDslProtocolTxBufferRef: The size of referenced diagnostic buffer that is used for
                          * diagnostic response transmission for the protocol @range 0..0xFFFFFFFF*/
    uint32
        TxBufferOffset; /**< the calculated offset of this protocol in the whole TxBuffer area  @range 0..0xFFFFFFFF*/
    uint16 MainConnectionIdNum; /**< The number of configured DcmDslProtocolMainConnection @range 0..0xFFFF*/
    uint16 MainConnectionId;    /**< DcmDslMainConnection: The related DcmDslProtocolMainConnection start index @range
                                   0..0xFFFF*/
    const Dcm_SesCtrlType*
          UDSControlDTCSessionRef;    /**< UDS 0x85 sessionRowRef belongs to this Protocol (if have) @range NA*/
    uint8 UDSControlDTCSessionRefNum; /**< Number of UDS 0x85 sessionRowRef belongs to this Protocol (if have) @range
                                         0..255*/
    const Dcm_SesCtrlType*
          UDSComControlSessionRef;      /**< UDS 0x28 sessionRowRef belongs to this Protocol (if have) @range NA*/
    uint8 UDSComControlSessionRefNum;   /**< Number of UDS 0x28 sessionRowRef belongs to this Protocol (if have) @range
                                           0..255*/
    const uint8* ShareBufferProtocolId; /**< The protocolIds of protocols which share the same buffer @range NA*/
    uint8 ShareBufferProtocolIdNum; /**< The number of the protocolIds of protocols which share the same buffer @range
                                       0..255*/
    boolean ProtocolUseGenericConnection; /**< TRUE when any referenced pdu has configured metadata sa/ta @range 0..1*/
    boolean RequestQueued; /**< DcmDslProtocolRequestQueued: TRUE to support an extra buffer for queueud request @range
                              0..1*/
#if (DCM_DSL_RX_TX_SHARED_BUGGER == STD_ON)
    boolean SharedRxTxBuffer; /**< True when Shared buffer between DcmDslProtocolRxBufferRef and
                                 DcmDslProtocolTxBufferRef @range 0..1*/
#endif
} Dcm_DslProtocolRowType;

/**
 * @brief DcmDspAuthenticationConnection Configuration
 */
typedef struct
{
    uint16 EcuChallengeLength; /**< DcmDspAuthenticationEcuChallengeLength: Length in byte of the ECU challenge @range
                                  0..0xFFFF*/
    uint32
        CertificatePublicKeyStoreJobRef; /**< DcmDspAuthenticationCertificatePublicKeyStoreJobRef: Reference to a CsmJob
                                          * used to store the public key within the Csm @range 0..0xFFFFFFFF */
    uint32 ClientChallengeSignJobRef;    /**< DcmDspAuthenticationClientChallengeSignJobRef: Reference to a job used to
                                            sign the client challenge @range 0..0xFFFFFFFF */
    uint16 ConnectionCertificateRef; /**< DcmDspAuthenticationConnectionCertificateRef: Reference to a KeyMCertificate
                                      * used to store the certificate within the KeyM @range 0..0xFFFF */
    uint16 ECUCertificateRef; /**< DcmDspAuthenticationECUCertificateRef: Reference to a KeyMCertificate with the server
                               * certificate for bi-directional authentication @range 0..0xFFFF*/
    uint16 PublicKeyElementRef; /**< DcmDspAuthenticationPublicKeyElementRef: Reference to a certificate data element
                                 * with the public key in the certificate @range 0..0xFFFF */
    uint32 RandomJobRef;   /**< DcmDspAuthenticationRandomJobRef: Reference to a certificate parse job used to parse the
                            * authentication   certificate @range 0..0xFFFFFFFF*/
    uint16 RoleElementRef; /**< DcmDspAuthenticationRoleElementRef: Reference to a certificate data element with the
                              role in the certificate @range 0..0xFFFF */
    uint32 VerifyProofOfOwnerShipClientJobRef; /**< DcmDspAuthenticationVerifyProofOfOwnerShipClientJobRef: Reference to
                                                * a CsmJob used to verify the proof of ownership client in the Csm
                                                * @range 0..0xFFFFFFFF */
    uint16
        WhiteListDIDElementRef; /**< DcmDspAuthenticationWhiteListDIDElementRef: Reference to a certificate data element
                                 * with the white list for data identifiers in the certificate @range 0..0xFFFF*/
    uint16 WhiteListMemorySelectionElementRef; /**< DcmDspAuthenticationWhiteListMemorySelectionElementRef: Reference to
                                                * a certificate data element with the white list for user defined memory
                                                * selection in the certificate @range 0..0xFFFF*/
    uint16
        WhiteListRIDElementRef; /**< DcmDspAuthenticationWhiteListRIDElementRef: Reference to a certificate data element
                                 * with the white list for routine identifiers in the certificate @range 0..0xFFFF*/
    uint16 WhiteListServicesElementRef; /**< DcmDspAuthenticationWhiteListServicesElementRef: Reference to a certificate
                                         * data element with the white list in the certificate @range 0..0xFFFF*/
} Dcm_DspAuthenticationConnectionType;

/**
 * @brief DcmDspAuthenticationTransmitCertificate Configuration
 */
typedef struct
{
    uint16 TransmitCertificateEvaluationId; /**< DcmDspAuthenticationTransmitCertificateEvaluationId: Certificate
                                               evaluation id @range 0..0xFFFF */
    uint8
        TransmitCertificateFunction; /**< DcmDspAuthenticationTransmitCertificateFunction: Provides a fixed semantics to
                                      * this certificate that will lead to predefined actions in the Dcm @range 0..255*/
    uint16 TransmitCertificateRef; /**< DcmDspAuthenticationTransmitCertificateRef: Reference to a KeyMCertificate used
                                      to handle the certificate @range 0..0xFFFF*/
} Dcm_DspAuthenticationTransmitCertificateType;

/**
 * @brief DcmDspAuthenticationRow Configuration
 */
typedef struct
{
    uint8 AuthenticationRoleBitPosition; /**< DcmDspAuthenticationRoleBitPosition: Defines the bit number that
                                          * represents the role in the authentication bit field. The bit mask is aligned
                                          * with the logical bit calculations: 2ˆDcmDspAuthenticationRoleBit Position
                                          * @range 0..255*/
} Dcm_DspAuthenticationRowType;

/**
 * @brief DcmDspAuthentication Configuration
 */
typedef struct
{
    uint8 DeauthenticatedRoleRefNum; /**< The number of DeauthenticatedRoleRef @range 0..255*/
    const uint8*
        DeauthenticatedRoleRef; /**< DcmDspAuthenticationDeauthenticatedRoleRef: Reference to DcmDspAuthenticationRow
                                 * that defines a role in that is used as deauthenticated role @range NA*/
    const Dcm_ModeRuleFncType
        GeneralNRCModeRuleRef; /**< DcmDspAuthenticationGeneralNRCModeRuleRef: Mode rule that defines if the general NRC
                                * shall be send for all failures due to invalid certificate or content @range NA*/
    const Dcm_ModeRuleFncType
        PersistStateModeRuleRef; /**< DcmDspAuthenticationPersistStateModeRuleRef: References a mode rule that defines
                                  * if the authentication State shall be persisted in non-volatile memory @range NA*/
} Dcm_DspAuthenticationType;

/**
 * @brief Callout function for condition check, manufacturer / supplier specific checks on the groupOf DTC, which is
 * requested to clear.
 */
typedef Std_ReturnType (*Dcm_DspClearDTCCheckFncType)(uint32 GoDTC, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspClearDTC Configuration
 */
typedef struct
{
    Dcm_DspClearDTCCheckFncType
        CheckFnc; /**< DcmDspClearDTCCheckFnc: Callback function for condition check, manufacturer / supplier specific
                   * checks on the groupOfDTC, which is requested to clear @range NA*/
    const Dcm_ModeRuleFncType ClearDTCModeRuleRef; /**< DcmDspClearDTCModeRuleRef: Reference to DcmModeRule @range NA*/
} Dcm_DspClearDTCType;

/**
 * @brief DcmDspComControlAllChannel Configuration
 */
typedef struct
{
    NetworkHandleType  AllComMChannelRef;    /**< DcmDspAllComMChannelRef: Reference to ComM channel @range NA*/
    Dcm_ComCtrlFncType AllChannelComCtrlFnc; /**< ComControl function @range NA*/
} Dcm_DspComControlAllChannelType;

/**
 * @brief DcmDspComControlSpecificChannel Configuration
 */
typedef struct
{
    uint8 SubnetNumber; /**< DcmDspSubnetNumber: Subnet Number which controls the specific ComMChannel @range NA*/
    NetworkHandleType  SpecificComMChannelRef; /**< DcmDspSpecificComMChannelRef: Reference to ComM channel @range NA*/
    Dcm_ComCtrlFncType SpecificChannelComCtrlFnc; /**< ComControl function @range NA*/
} Dcm_DspComControlSpecificChannelType;

/**
 * @brief DcmDspComControlSubNode Configuration
 */
typedef struct
{
    uint16 SubNodeId; /**< DcmDspComControlSubNodeId: The node identification number DcmDspComControlSubNodeId is
                       * addressed by the CommunicationControl (0x28) request @range NA*/
    NetworkHandleType
        SubNodeComMChannelRef; /**< DcmDspComControlSubNodeComMChannelRef: Reference to ComM channel @range NA*/
    Dcm_ComCtrlFncType SubNodeComCtrlFnc; /**< ComControl function @range NA*/
} Dcm_DspComControlSubNodeType;

/**
 * @brief DcmDspComControl Configuration
 */
typedef struct
{
    const Dcm_ModeRuleFncType
        ComControlCommunicationReEnableModeRuleRef; /**< DcmDspComControlCommunicationReEnableModeRuleRef: Reference to
                                                     * DcmModeRule Mode rule which controls re-enabling of communication
                                                     * by DCM @range NA*/
} Dcm_DspComControlType;

/**
 * @brief DcmDspCommonAuthorization Configuration
 */
typedef struct
{
    const Dcm_ModeRuleFncType
        CommonAuthorizationModeRuleRef; /**< DcmDspCommonAuthorizationModeRuleRef: Reference to DcmModeRule @range NA*/
    uint8 CommonAuthorizationSecurityLevelRefNum; /**< Number of CommonAuthorizationSecurityLevelRef @range NA*/
    const Dcm_SecLevelType*
        CommonAuthorizationSecurityLevelRef; /**< DcmDspCommonAuthorizationSecurityLevelRef: Reference to
                                              * DcmDspSecurityRow Security levels allowed to control this
                                              * service/sub-service @range NA */
    uint8 CommonAuthorizationSessionRefNum;  /**< Number of CommonAuthorizationSessionRef @range NA*/
    const Dcm_SesCtrlType*
        CommonAuthorizationSessionRef; /**< DcmDspCommonAuthorizationSessionRef: Reference to DcmDspSessionRow Sessions
                                        * allowed to control this service/sub-service @range NA*/
} Dcm_DspCommonAuthorizationType;

/**
 * @brief DcmDspControlDTCSetting Configuration
 */
typedef struct
{
    const Dcm_ModeRuleFncType ControlDTCSettingReEnableModeRuleRef; /**< DcmDspControlDTCSettingReEnableModeRuleRef:
                                                                       Reference to DcmModeRule @range NA*/
} Dcm_DspControlDTCSettingType;

/**
 * @brief DcmDspDataConditionCheckRead function prototype
 */
typedef Std_ReturnType (
    *Dcm_DspDataConditionCheckReadFncType)(Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief EcuSignal function prototype
 */
/* PRQA S 4604++ */ /* VL_Dcm_4604 */
typedef Std_ReturnType (*Dcm_DspDataEcuSignalType)(uint8 action, uint8* signal);
/* PRQA S 4604-- */ /* VL_Dcm_4604 */

/**
 * @brief DcmDspDataFreezeCurrentState function prototype
 */
typedef Std_ReturnType (*Dcm_DspDataFreezeCurrentStateFncType)(
    Dcm_OpStatusType              OpStatus,
    const uint8*                  controlMask,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDataGetScalingInfo function prototype
 */
typedef Std_ReturnType (*Dcm_DspDataGetScalingInfoFncType)(
    Dcm_OpStatusType              OpStatus,
    uint8*                        ScalingInfo,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDataReadDataLength function prototype
 */
typedef Std_ReturnType (*Dcm_DspDataReadDataLengthFncType)(Dcm_OpStatusType OpStatus, uint16* DataLength);

/**
 * @brief ReadEcuSignal function prototype
 */
/* PRQA S 4604++ */ /* VL_Dcm_4604 */
typedef Std_ReturnType (*Dcm_DspDataReadEcuSignalType)(uint8* signal);
/* PRQA S 4604-- */ /* VL_Dcm_4604 */

/**
 * @brief DcmDspDataRead function prototype
 */
typedef Std_ReturnType (
    *Dcm_DspDataReadFncType)(Dcm_OpStatusType OpStatus, uint8* Data, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspPidDataRead function prototype
 */
typedef Std_ReturnType (*Dcm_DspPidDataReadFncType)(uint8* Data);

/**
 * @brief DcmDspDataResetToDefault function prototype
 */
typedef Std_ReturnType (*Dcm_DspDataResetToDefaultFncType)(
    Dcm_OpStatusType              OpStatus,
    const uint8*                  controlMask,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDataReturnControlToEcu function prototype
 */
typedef Std_ReturnType (
    *Dcm_DspDataReturnControlToEcuFncType)(const uint8* controlMask, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDataShortTermAdjustment function prototype
 */
typedef Std_ReturnType (*Dcm_DspDataShortTermAdjustmentFncType)(
    const uint8*                  ControlStateInfo,
    uint16                        DataLength,
    Dcm_OpStatusType              OpStatus,
    const uint8*                  controlMask,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDataWrite function prototype
 */
typedef Std_ReturnType (*Dcm_DspDataWriteFncType)(
    const uint8*                  Data,
    const uint16                  DataLength,
    Dcm_OpStatusType              OpStatus,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DspDataType Enum
 */
typedef enum
{
    DCM_BOOLEAN,   /**< Boolean @range 0x00*/
    DCM_FLOAT,     /**< Float @range 0x01*/
    DCM_FLOAT_N,   /**< Float_N @range 0x02*/
    DCM_SINT16,    /**< SINT16 @range 0x03*/
    DCM_SINT16_N,  /**< SINT16_N @range 0x04*/
    DCM_SINT32,    /**< SINT32 @range 0x05*/
    DCM_SINT32_N,  /**< SINT32_N @range 0x06*/
    DCM_SINT8,     /**< SINT8 @range 0x07*/
    DCM_SINT8_N,   /**< SINT8_N @range 0x08*/
    DCM_UINT16,    /**< UINT16 @range 0x09*/
    DCM_UINT16_N,  /**< UINT16_N @range 0x0A*/
    DCM_UINT32,    /**< UINT32 @range 0x0B*/
    DCM_UINT32_N,  /**< UINT32_N @range 0x0C*/
    DCM_UINT8,     /**< UINT8 @range 0x0D*/
    DCM_UINT8_DYN, /**< UINT8_DYN @range 0x0E*/
    DCM_UINT8_N    /**< UINT8_N @range 0x0F*/
} Dcm_DspDataType;

/**
 * @brief DspDataUsePort Enum
 */
typedef enum
{
    DCM_USE_BLOCK_ID,                        /**< Use NvMBlockId to read/write @range 0x00*/
    DCM_USE_DATA_ASYNCH_CLIENT_SERVER,       /**< R-Port: asynchronous ClientServertInterface DataServices_{Data} @range
                                                0x01*/
    DCM_USE_DATA_ASYNCH_CLIENT_SERVER_ERROR, /**< R-Port: asynchronous with error
                                                ClientServertInterfaceDataServices_{Data} @range 0x02 */
    DCM_USE_DATA_ASYNCH_FNC,                 /**< User-Defined asynchronous function @range 0x03*/
    DCM_USE_DATA_ASYNCH_FNC_ERROR,           /**< User-Defined asynchronous function with error @range 0x04*/
    DCM_USE_DATA_SENDER_RECEIVER,            /**< R-Port: SenderReceiverInterface @range 0x05*/
    DCM_USE_DATA_SENDER_RECEIVER_AS_SERVICE, /**< R-Port: SenderReceiverInterface (with isService=true) @range 0x06*/
    DCM_USE_DATA_SYNCH_CLIENT_SERVER, /**< R-Port: synchronous ClientServertInterface DataServices_{Data} @range 0x07 */
    DCM_USE_DATA_SYNCH_FNC,           /**< User-Defined synchronous function @range 0x08*/
    DCM_USE_ECU_SIGNAL                /**< Use IoHwAb provided functions @range 0x09*/
} Dcm_DspDataUsePortType;

/**
 * @brief DcmDspData Configuration
 */
typedef struct
{
    uint16 DspDataByteSize; /**< DcmDspDataByteSize: Defines the array length in bytes or the maximum array length for
                               variable datalengths @range 0..65535 */
    Dcm_DspDataConditionCheckReadFncType
        ConditionCheckReadFnc; /**< DcmDspDataConditionCheckReadFnc: This parameter determines if a condition check
                                  function is available or not @range NA */
    Dcm_DspDataEcuSignalType EcuSignal; /**< DcmDspDataEcuSignal: Function name to control the access to a certain ECU
                                           Signal by the DCM @range NA */
    Dcm_DspDataFreezeCurrentStateFncType
        FreezeCurrentStateFnc; /**< DcmDspDataFreezeCurrentStateFnc: Function name to request to application to freeze
                                * the current State of an IOControl @range NA*/
    Dcm_DspDataGetScalingInfoFncType GetScalingInfoFnc; /**< DcmDspDataGetScalingInfoFnc: Function name to request to
                                                           application the scaling information of the DID @range NA */
    Dcm_DspDataReadDataLengthFncType ReadDataLengthFnc; /**< DcmDspDataReadDataLengthFnc: Function name to request from
                                                           application the data length of a DID @range NA */
    Dcm_DspDataReadEcuSignalType ReadEcuSignal; /**< DcmDspDataReadEcuSignal: Function name for read access to a certain
                                                   ECU Signal by the DCM @range NA*/
    Dcm_DspDataReadFncType DspDataReadFnc; /**< DcmDspDataReadFnc: Function name to request from application the data
                                              value of a DID @range NA */
    Dcm_DspDataResetToDefaultFncType
        ResetToDefaultFnc; /**< DcmDspDataResetToDefaultFnc: Function name to request to application to reset an
                              IOControl to default value @range NA */
    Dcm_DspDataReturnControlToEcuFncType
        ReturnControlToEcuFnc; /**< DcmDspDataReturnControlToEcuFnc: Function name to request to application to return
                                * control to ECU of an IOControl @range NA */
    Dcm_DspDataShortTermAdjustmentFncType
        ShortTermAdjustmentFnc; /**< DcmDspDataShortTermAdjustmentFnc: Function name to request to application to adjust
                                   the IO signal @range NA*/
    Dcm_DspDataType
        DspDataType; /**< DcmDspDataType: Provide the implementation data type of data belonging to a DID @range NA*/
    Dcm_DspDataUsePortType
        DspDataUsePort; /**< DcmDspDataUsePort: Defines which interface shall be used to access the data @range NA */
    Dcm_DspDataWriteFncType DataWriteFnc; /**< DcmDspDataWriteFnc: Function name to request application to write the
                                             data value of a DID @range NA */
#if (STD_ON == DCM_USE_NVM)
    NvM_BlockIdType DspDataBlockIdRef; /**< DcmDspDataBlockIdRef: required for DspDataUsePort==DCM_USE_BLOCK_ID to
                                          define used NvMBlockId @range NA*/
#endif
    uint32 ScalingInfoSize; /**< DcmDspDataScalingInfoSize: If Scaling information service is available for this Data,
                             * it provides the size in bytes of the scaling information @range 0xFFFFFFFF*/
} Dcm_DspDataCfgType;

/**
 * @brief DidUsePort Enum
 */
typedef enum
{
    DCM_USE_ATOMIC_BNDM,              /**< The DID shall be read/written from/to BndM @range 0x00 */
    DCM_USE_ATOMIC_NV_DATA_INTERFACE, /**< A single sender receiver interface with NvData-Ports is used for all data
                                         elements of this DID @range 0x01 */
    DCM_USE_ATOMIC_SENDER_RECEIVER_INTERFACE, /**< A single sender receiver interface is used to access all data
                                               * elements of this DID. The sender receiver interface is typed with
                                               * IsService=false @range 0x02 */
    DCM_USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE, /**< A single sender receiver interface is used to access all
                                                          * data elements of this DID. The sender receiver interface is
                                                          * typed with IsService=true @range 0x03 */
    DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES /**< The data elements of this DID are collected by using the data element
                                              * specific interfaces defined by DspDataUsePort @range 0x04*/
} Dcm_DspDidUsePortType;

/**
 * @brief DcmDspDidSignal Configuration
 */
typedef struct
{
    uint16 DidByteOffset; /**< DcmDspDidByteOffset: Defines the absolute byte offset of the data defined by DidDataRef
                           * reference to DcmDspData container in the DID @range 0..65535*/
    const Dcm_DspDataCfgType*
        DidDataRef; /**< DcmDspDidDataRef: Reference to 1 DcmDspData container relevant for this DID @range NA*/
} Dcm_DspDidSignalType;

/**
 * @brief General didRead function prototype, used when DidUsePort is not
 * DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES
 */
typedef Std_ReturnType (*Dcm_DspDidReadFncType)(uint8* Data);
/**
 * @brief General didWrite function prototype, used when DidUsePort is not
 * DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES
 */
typedef Std_ReturnType (
    *Dcm_DspDidWriteFncType)(Dcm_OpStatusType OpStatus, const uint8* Data, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDid Configuration
 */
typedef struct
{
    uint16 DidIdentifier; /**< DcmDspDidIdentifier: 2 byte Identifier of the DID @range 0..65535 */
    uint16 DidSize;       /**< DcmDspDidSize: Length of a DID in byte(s) @range 0..65535 */
    Dcm_DspDidUsePortType
        DidUsePort; /**< DcmDspDidUsePort: Selects application interface type for DID data elements between a single
                     * operation for all data elements or data element specific operations @range NA*/
#if (STD_ON == DCM_USE_BNDM)
    BndM_BlockIdType
           DidBndMBlockIdRef; /**< DcmDspDidBndMBlockIdRef: BndM BlockId when DidUsePort==USE_ATOMIC_BNDM @range NA*/
    uint16 DidBndMIndex; /**< DcmDspDidBndMIndex: specific index for did using DidUsePort==USE_ATOMIC_BNDM @range NA*/
#endif
    uint16 DidInfoRef;          /**< DcmDspDidInfoRef: referenced DcmDspDidInfo index @range 0..65535*/
    uint16 DidSignalIndexStart; /**< DcmDspDidSignal: the configured DcmDspDidSignal reference start index @range
                                   0..65535*/
    uint16 DidSignalIndexEnd; /**< DcmDspDidSignal: the configured DcmDspDidSignal reference end index @range 0..65535*/
    Dcm_DspDidReadFncType
        DidReadFnc; /**< general data read fnc for DidUsePort != DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES @range NA*/
    Dcm_DspDidWriteFncType
        DidWriteFnc; /**< general data write fnc for DidUsePort != DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES @range NA*/
} Dcm_DspDidType;

/**
 * @brief controlMask Enum
 */
typedef enum
{
    DCM_CONTROLMASK_EXTERNAL, /**< The control enable mask record shall be forwarded within each interface and is
                                 handled externally @range 0x01*/
    DCM_CONTROLMASK_INTERNAL, /**< The control enable mask record is handled internally and Dcm controls only the
                                 included signals @range 0x02*/
    DCM_CONTROLMASK_NO        /**< No control enable mask handling @range 0x03*/
} Dcm_DspDidControlMaskType;

/**
 * @brief didControl Configuration
 */
typedef struct
{
    Dcm_DspDidControlMaskType
        ControlMask;        /**< DcmDspDidControlMask: This indicates the presence of "controlEnableMask" in SWC service
                             * interfaces and defines        how the Dcm treats a service request @range NA*/
    uint32 ControlMaskSize; /**< DcmDspDidControlMaskSize: The value defines the size of the controlEnableMaskRecord in
                               bytes @range NA*/
    boolean FreezeCurrentState; /**< DcmDspDidFreezeCurrentState: This indicates the presence of "FreezeCurrentState"
                                   @range NA */
    boolean ResetToDefault; /**< DcmDspDidResetToDefault: This indicates the presence of "ResetToDefault" @range NA*/
    boolean ShortTermAdjustment; /**< DcmDspDidShortTermAdjustment: This indicates the presence of "ShortTermAdjustment"
                                    @range NA */
    const Dcm_ModeRuleFncType
                 DidControlModeRuleRef; /**< DcmDspDidControlModeRuleRef: Reference to DcmModeRule @range NA*/
    uint8        DidControlRoleRefNum;  /**< The number of DidControlRoleRef @range NA*/
    const uint8* DidControlRoleRef; /**< DcmDspDidControlRoleRef: Reference to DcmDspAuthenticationRow that defines a
                                     * role in that this IO can be be controlled @range NA*/
    uint8 DidControlSecurityLevelRefNum; /**< The number of DcmDspDidControlSecurityLevelRef @range NA*/
    const Dcm_SecLevelType*
        DidControlSecurityLevelRef; /**< DcmDspDidControlSecurityLevelRef: Reference to DcmDspSecurityRow Security
                                       levels allowed to control this DID @range NA*/
    uint8                  DidControlSessionRefNum; /**< The number of DcmDspDidControlSessionRef @range NA*/
    const Dcm_SesCtrlType* DidControlSessionRef; /**< DcmDspDidControlSessionRef: Reference to DcmDspSessionRow Sessions
                                                    allowed to control this DID @range NA*/
} Dcm_DspDidControlType;

/**
 * @brief DcmDspDidReadWrite Configuration
 */
typedef struct
{
    const Dcm_ModeRuleFncType
                 DidReadWriteModeRuleRef;  /**< DcmDspDidReadWriteModeRuleRef: Reference to DcmModeRule @range NA*/
    uint8        DidReadWriteRoleRefNum;   /**< The number of didRead/WriteRoleRef @range NA*/
    const uint8* DidReadWriteRoleRef;      /**< DcmDspDidReadWriteRoleRef: Reference to DcmDspAuthenticationRow for
                                              read/write operation @range NA*/
    uint8 DidReadWriteSecurityLevelRefNum; /**< The number of didRead/WriteSecurityLevelRef @range NA*/
    const Dcm_SecLevelType*
        DidReadWriteSecurityLevelRef; /**< DcmDspDidReadWriteSecurityLevelRef: Reference to DcmDspSecurityRow Security
                                       * levels allowed for read/write operation @range NA*/
    uint8                  DidReadWriteSessionRefNum; /**< The number of didRead/WriteSessionRef @range NA*/
    const Dcm_SesCtrlType* DidReadWriteSessionRef;    /**< DcmDspDidReadWriteSessionRef: Reference to DcmDspSessionRow
                                                         Sessions allowed for read/write operation @range NA*/
} Dcm_DspDidReadWriteType;

/**
 * @brief DcmDspDidInfo Configuration
 */
typedef struct
{
    uint8   DDDIDMaxElements;   /**< DcmDspDDDIDMaxElements: Maximum number of source elements of a DDDID @range NA*/
    boolean DynamicallyDefined; /**< DcmDspDidDynamicallyDefined: Indicates if this DID can be dynamically defined
                                   @range NA*/
    uint16                         DDDidIndex; /**< The correspondig DcmDspDDDID index @range NA*/
    const Dcm_DspDidControlType*   DidControl; /**< DcmDspDidControl: Configured DcmDspDidControl @range NA*/
    const Dcm_DspDidReadWriteType* DidRead;    /**< DcmDspDidRead: Configured DcmDspDidRead @range NA*/
    const Dcm_DspDidReadWriteType* DidWrite;   /**< DcmDspDidWrite: Configured DcmDspDidWrite @range NA*/
} Dcm_DspDidInfoType;

/**
 * @brief DcmDspDidRangeIsDidAvailable function prototype
 */
typedef Std_ReturnType (
    *Dcm_DspDidRangeIsDidAvailableFncType)(uint16 DID, Dcm_OpStatusType OpStatus, Dcm_DidSupportedType* supported);

/**
 * @brief DcmDspDidRangeReadDataLength function prototype
 */
typedef Std_ReturnType (
    *Dcm_DspDidRangeReadDataLengthFncType)(uint16 DID, Dcm_OpStatusType OpStatus, uint16* DataLength);

/**
 * @brief DcmDspDidRangeReadDid function prototype
 */
typedef Std_ReturnType (*Dcm_DspDidRangeReadDidFncType)(
    uint16                        DID,
    uint8*                        Data,
    Dcm_OpStatusType              OpStatus,
    uint16                        DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDidRangeWriteDid function prototype
 */
typedef Std_ReturnType (*Dcm_DspDidRangeWriteDidFncType)(
    uint16                        DID,
    const uint8*                  Data,
    Dcm_OpStatusType              OpStatus,
    uint16                        DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspDidRange Configuration
 */
typedef struct
{
    uint16 DidRangeIdentifierLowerLimit; /**< DcmDspDidRangeIdentifierLowerLimit: Lower limit of DID range @range NA */
    uint16 DidRangeIdentifierUpperLimit; /**< DcmDspDidRangeIdentifierUpperLimit: Upper limit of DID range @range NA */
    Dcm_DspDidRangeIsDidAvailableFncType
        IsDidAvailableFnc; /**< DcmDspDidRangeIsDidAvailableFnc: Function name to request from application if a specific
                            * DID is available within the range or not @range NA */
    uint16 DidRangeMaxDataLength; /**< DcmDspDidRangeMaxDataLength: Maximum data length in bytes @range NA*/
    Dcm_DspDidRangeReadDataLengthFncType
        DidRangeReadDataLengthFnc; /**< DcmDspDidRangeReadDataLengthFnc: Function name to request from application the
                                    * length of the data of a range DID @range NA
                                    */
    Dcm_DspDidRangeReadDidFncType DidRangeReadDidFnc; /**< DcmDspDidRangeReadDidFnc: Function name to request from
                                                         application the data range value of a DID @range NA*/
    Dcm_DspDidRangeWriteDidFncType
        DidRangeWriteDidFnc; /**< DcmDspDidRangeWriteDidFnc: Function name to request application to write the data
                                range value of a DID @range NA */
    uint16 DidRangeInfoRef;  /**< DcmDspDidRangeInfoRef: Reference to DcmDspDidInfo containing information on this DID
                                Range @range NA*/
} Dcm_DspDidRangeType;

/**
 * @brief DcmDspResponseToEcuReset Enum
 */
typedef enum
{
    DCM_AFTER_RESET, /**< Answer to EcuReset service should come after the reset @range NA */
    DCM_BEFORE_RESET /**< Answer to EcuReset service should come before the reset @range NA*/
} Dcm_ResponseToEcuResetType;

/**
 * @brief DcmDspEcuResetRow Configuration
 */
typedef struct
{
    uint8                      EcuResetId;         /**< DcmDspEcuResetId: Defines the subfunction ID @range NA*/
    Dcm_ResponseToEcuResetType ResponseToEcuReset; /**< DcmResponseToEcuReset: Defines the answer to EcuReset service
                                                      should come: Before or after the reset @range NA */
} Dcm_DspEcuResetRowType;

/**
 * @brief DcmDspRead/WriteMemoryRangeInfo Configuration
 */
typedef struct
{
    uint32 MemoryRangeHigh; /**< DcmDspMemoryRangeHigh: High memory address of a range allowed @range NA*/
    uint32 MemoryRangeLow;  /**< DcmDspMemoryRangeLow: Low memory address of a range allowed @range NA*/
    const Dcm_ModeRuleFncType
          MemoryRangeModeRuleRef;         /**< DcmDspMemoryRangeModeRuleRef: Reference to DcmModeRule @range NA*/
    uint8 MemoryRangeSecurityLevelRefNum; /**< The number of DcmDspRead/WriteMemoryRangeSecurityLevelRef @range NA*/
    const Dcm_SecLevelType*
        MemoryRangeSecurityLevelRef; /**< DcmDspMemoryRangeSecurityLevelRef: Link to the Security Access Levels needed
                                        for access on this memory address @range NA*/
    uint8 MemoryRangeSessionLevelRefNum; /**< The number of DcmDspRead/WriteMemoryRangeSessionLevelRef @range NA*/
    const Dcm_SesCtrlType* MemoryRangeSessionLevelRef; /**< DcmDspMemoryRangeSessionLevelRef: Link to the session level
                                                          needed for access to this memory address range @range NA*/
} Dcm_DspMemoryRangeInfoType;

/**
 * @brief DcmDspMemoryIdInfo Configuration
 */
typedef struct
{
    const uint8* MemoryIdValue; /**< DcmDspMemoryIdValue: Value of the memory device identifier used @range NA */
    uint16       ReadMemoryRangeInfoNum; /**< The number of configured ReadMemoryRangeInfo @range NA*/
    const Dcm_DspMemoryRangeInfoType*
           ReadMemoryRangeInfo;     /**< DcmDspReadMemoryRangeInfo: Configured ReadMemoryRangeInfo @range NA*/
    uint16 WriteMemoryRangeInfoNum; /**< The number of configured WriteMemoryRangeInfo @range NA*/
    const Dcm_DspMemoryRangeInfoType*
        WriteMemoryRangeInfo; /**< DcmDspWriteMemoryRangeInfo: Configured WriteMemoryRangeInfo @range NA*/
} Dcm_DspMemoryIdInfoType;

/**
 * @brief DcmDspMemory Configuration
 */
typedef struct
{
    uint8 SupportedAddressAndLengthFormatIdentifierNum; /**< number of SupportedAddressAndLengthFormatIdentifier  @range
                                                           NA*/
    const uint8*
        SupportedAddressAndLengthFormatIdentifier;  /**< DcmDspSupportedAddressAndLengthFormatIdentifier: This parameter
                                                     * defines the supported  AddressAndLengthFormatIdentifier of the
                                                     * request message @range NA */
    uint8                          MemoryIdInfoNum; /**< number of configured MemoryIdInfo @range NA*/
    const Dcm_DspMemoryIdInfoType* MemoryIdInfo; /**< DcmDspMemoryIdInfo: Provides the value of memory identifier used
                                                    to select the desired memory device @range NA*/
} Dcm_DspMemoryType;

/**
 * @brief DcmDspPidServiceType Enum
 */
typedef enum
{
    DCM_SERVICE_01,    /**< PID service 1 @range NA*/
    DCM_SERVICE_01_02, /**< PID service 1 and 2 @range NA*/
    DCM_SERVICE_02     /**< PID service 2 @range NA*/
} Dcm_DspPidServiceType;

/**
 * @brief DcmDspDidService01 Configuration
 */
typedef struct
{
    Dcm_DspPidDataReadFncType
        PidDataReadFnc; /**<DcmDspPidDataReadFnc: Function name for reading PID data value @range NA*/
} Dcm_DspPidService01Type;

/**
 * @brief PidData Configuration
 */
typedef struct
{
    uint8 PidByteOffset; /**< DcmDspPidByteOffset: This is the position in bytes of the PID structure and will not start
                          * at position 0 in case a support information is available @range NA*/
    uint8 PidDataByteSize; /**< DcmDspPidDataByteSize: Defines the array length in bytes or the maximum array length for
                              variable datalengths @range NA*/
    const Dcm_DspPidService01Type* PidService01; /**< DcmDspPidService01: Contains specific configuration parameter of
                                                    PID for service $01 @range NA*/
    uint8 PidDataPosition; /**< The position of this PidData based on its PidByteOffset @range NA*/
} Dcm_DspPidDataType;

/**
 * @brief DcmDspPid Configuration
 */
typedef struct
{
    uint8 PidIdentifier; /**< DcmDspPidIdentifier: 1 byte Identifier of the PID @range NA*/
    Dcm_DspPidServiceType
          PidService; /**< DcmDspPidService: Indicates if a PID is used with service $01 and/or $02 @range NA*/
    uint8 PidSize;    /**< DcmDspPidSize: Length of a PID in byte(s) @range NA*/
    uint8 PidDataNum; /**< The number of configured DcmDspPid @range NA*/
    const Dcm_DspPidDataType*
        PidData; /**< DcmDspPidData: This container defines the parameter for a Signal in the PID @range NA */
} Dcm_DspPidType;

/**
 * @brief RequestControl function type
 */
typedef Std_ReturnType (*Dcm_RequestControlFncType)(uint8* OutBuffer, const uint8* InBuffer);

/**
 * @brief DcmDspRequestControl Configuration
 */
typedef struct
{
    uint8 InBufferSize; /**< DcmDspRequestControlInBufferSize: Number of bytes to be provided in the input buffer of the
                         * interface RequestControl Services_{Tid} for OBD Service $08 @range NA*/
    uint8 OutBufferSize; /**< DcmDspRequestControlOutBufferSize: Number of bytes to be provided in the output buffer of
                          * the interface RequestControl Services_{Tid} for OBD Service $08 @range NA*/
    uint8 TestId;        /**< DcmDspRequestControlTestId: Test Id for Service $08  @range NA*/
    Dcm_RequestControlFncType
                 RequestControlFnc;      /**< DcmDspRequestControlFnc: request control callout function @range NA*/
    const uint8* RequestControlInfoByte; /**< DcmDspRequestControlInfoByte: Manufacturer specific value reported to the
                                          * tester for the record identifiers 0xE000 to OxE1FF @range NA*/
} Dcm_DspRequestControlType;

/**
 * @brief general routine request function type
 */
typedef Std_ReturnType (*Dcm_DspRoutineFncType)(
    const uint8*                  InBuffer,
    Dcm_OpStatusType              OpStatus,
    uint8*                        OutBuffer,
    uint16*                       currentDataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief DcmDspRoutineSignal Configuration
 */
typedef struct
{
    uint16 ParameterSize; /**< Provide the size of a RoutineControl parameter in bytes @range NA*/
    uint16
        RoutineSignalPos; /**< Provide the position of the signal in the RoutineControl request/response @range NA. */
} Dcm_DspRoutineSignalType;

/**
 * @brief DcmDspStart/Stop/RequestResultsRoutine Configuration
 */
typedef struct
{
    Dcm_DspRoutineFncType RoutineFnc; /**< Start/Stop/Request Routine general function @range NA*/
    const Dcm_DspCommonAuthorizationType*
          RoutineCommonAuthorizationRef; /**< Reference to DcmDspCommonAuthorization @range NA*/
    uint8 RoutineRoleRefNum;             /**< The number of configured DcmDspRoutineRoleRef @range NA*/
    const uint8*
        RoutineRoleRef; /**< Reference to DcmDspAuthenticationRow that defines a role in that this routine @range NA. */
    boolean InSignalVariableLength; /**< whether input has the last signal configured as variable length @range NA*/
    uint16  InSignalDataLength;     /**< the total length of input except variable one @range NA*/
    uint8   InSignalNum;            /**< the number of InSignal @range NA*/
    const Dcm_DspRoutineSignalType* InSignal; /**< DcmDspRoutineInSignal: InSignal reference @range NA*/
    boolean OutSignalVariableLength; /**< whether output has the last signal configured as variable length @range NA*/
    uint16  OutSignalDataLength;     /**< the total length of output except variable one @range NA*/
    uint8   OutSignalNum;            /**< the number of outSignal @range NA*/
    const Dcm_DspRoutineSignalType* OutSignal; /**< DcmDspRoutineOutSignal: outSignal reference  @range NA*/
    boolean RoutineRestartEnabled; /**< DcmDspRoutineRestartEnabled: TRUE to allow repeated startRoutine, otherwise will
                                      return NRC 0x24 @range NA*/
} Dcm_DspRoutineSubType;

/**
 * @brief DcmDspRoutine Configuration
 */
typedef struct
{
    uint16       RoutineIdentifier; /**< DcmDspRoutineIdentifier: 2 bytes Identifier of the RID @range NA*/
    const uint8* RoutineInfoByte; /**< DcmDspRoutineInfoByte: Manufacturer specific value reported to the tester for the
                                   * record identifiers 0xE000 to OxE1FF. (OBD use cases) @range NA
                                   */
    const Dcm_DspRoutineSubType*
        RequestRoutineResults; /**< DcmDspRequestRoutineResults: Provides the configuration of RequestResult subservice
                                * for RoutineControl service. Existence indicates that the Request RoutineResults in the
                                * RoutineControl is supported @range NA */
    const Dcm_DspRoutineSubType* StartRoutine; /**< DcmDspStartRoutine: Provides the configuration of Start subservice
                                                  for RoutineControl service @range NA */
    const Dcm_DspRoutineSubType*
        StopRoutine; /**< DcmDspStopRoutine: Provides the configuration of Stop subservice for RoutineControl service.
                      * Existence indicates that the StopRoutine in the RoutineControl is supported @range NA*/
#if (STD_ON == DCM_P4_SERVER)
    uint32 RoutineP4ServerMax; /**< DcmDspRoutineP4ServerMax: value for P4ServerMax in seconds (per rid) @range
                                  0..0xFFFFFFFF */
#endif
} Dcm_DspRoutineType;

/**
 * @brief VehInfoDataRead function type
 */
typedef Std_ReturnType (
    *Dcm_DspVehInfoDataReadFncType)(Dcm_OpStatusType OpStatus, uint8* DataValueBuffer, uint8* DataValueBufferSize);

/**
 * @brief vehInfoData Configuration
 */
typedef struct
{
    Dcm_DspVehInfoDataReadFncType VehInfoDataReadFnc; /**< DcmDspVehInfoDataReadFnc: Callout function name for reading
                                                         InfoType data item @range NA */
    uint8 VehInfoDataSize; /**< DcmDspVehInfoDataSize: Size in bytes of the InfoType data item @range NA */
} Dcm_DspVehInfoDataType;

/**
 * @brief DcmDspVehInfo Configuration
 */
typedef struct
{
    uint8                         VehInfoInfoType; /**< DcmDspVehInfoInfoType: value of InfoType @range 0..255. */
    uint8                         VehInfoDataNum;  /**< number of configured vehInfoData @range 0..255 */
    const Dcm_DspVehInfoDataType* VehInfoData; /**< DcmDspVehInfoData: Data Item of an InfoType; ShortName is post-fix
                                                  of the port interface name.@range NA */
    boolean VehInfoNODIProvResp; /**< DcmDspVehInfoNODIProvResp: Indicate the Dcm, which side is responsible to fill the
                                    number of data items (NODI) @range NA*/
} Dcm_DspVehInfoType;

/**
 * @brief DcmCondition Enum
 */
typedef enum
{
    DCM_EQUALS,           /**< Checks if two values are equal. @range 0x00*/
    DCM_EQUALS_NOT,       /**< Checks if two values are not equal. @range 0x01*/
    DCM_GREATER_OR_EQUAL, /**< Checks if one value >= another. @range 0x02*/
    DCM_GREATER_THAN,     /**< Checks if one value > another. @range 0x03*/
    DCM_LESS_OR_EQUAL,    /**< Checks if one value <= another. @range 0x04*/
    DCM_LESS_THAN         /**< Checks if one value < another. @range 0x05*/
} Dcm_ConditionType;

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
/**
 * @brief QueuedRequest State enum
 */
typedef enum
{
    DCM_PENDING_REQUEST_NONE    = 0, /**< No pending request @range 0x00*/
    DCM_PENDING_REQUEST_RECEIVE = 1, /**< Pending request is receiving @range 0x01*/
    DCM_PENDING_REQUEST_READY   = 2, /**< Pending request is ready @range 0x02*/
    DCM_PENDING_REQUEST_PROCESS = 3  /**< Pending Request is processing @range 0x03*/
} Dcm_PendingRequestStateType;

/**
 * @brief QueuedRequest control unit
 */
typedef struct QueuedRequestCtrlType
{
    uint8     QueuedRequestProtocolId;   /**< the requested procotol id @range 0..255*/
    PduIdType QueuedRequestRxPduId;      /**< the requested RxPdu Id @range NA*/
    uint16    QueuedRequestConnectionId; /**< the requested connection id @range 0..0xFFFF */
    uint32    QueuedRxCopyLen;           /**< already copied queued rxCopy length @range 0..0xFFFFFFFF */
    uint32    QueuedRequestLength;       /**< The whole request length (totallength - SID) @range 0..0xFFFFFFFF */
    Dcm_PendingRequestStateType PendingRequestState;    /**< current queued request State @range NA */
    Dcm_MsgType                 QueuedRequestOldBuffer; /**< to store the previous used reqData addr @range NA*/
} Dcm_QueuedRequestCtrlType;
#endif

#ifdef __cplusplus
}
#endif
#endif
