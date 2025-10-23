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
/*
 ************************************************************************************************************************
 **
 **  @file               : DoIP_Internal_UDP.c
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Implement the UDP part of DoIP
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_TCP.h"
#include "DoIP_Internal_UDP.h"
#include "Dcm.h"

/* ===================================================== macros ===================================================== */

/* identification */

#define DOIP_VIN_LEN            17u /**< Length of VIN field */
#define DOIP_EID_LEN            6u  /**< Length of EID field */
#define DOIP_GID_LEN            6u  /**< Length of GID field */
#define DOIP_FURTHER_ACTION_LEN 1u  /**< Length of further action field */
#define DOIP_SYNC_STATUS_LEN    1u  /**< Length of sync status field */

#define DOIP_IDENTIFICATION_RSP_MSG_LEN         40u /**< Length of identification response message */
#define DOIP_IDENTIFICATION_RSP_MSG_PAYLOAD_LEN 32u /**< Length of identification response payload */
#define DOIP_IDENTIFICATION_RSP_MSG_PAYLOAD_LEN_WITH_SYNC_STATUS \
    33u /**< Length of identification response payload with sync status */

#define DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_VIN \
    8u /**< Location of VIN field in identification response message */
#define DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_LA \
    25u /**< Location of LA field in identification response message */
#define DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_EID \
    27u /**< Location of EID field in identification response message */
#define DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_GID \
    33u /**< Location of GID field in identification response message */
#define DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_FURTHER_ACTION \
    39u /**< Location of further action field in identification response message */
#define DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_SYNC_STATUS \
    40u /**< Location of sync status field in identification response message */

#define DOIP_FURTHER_ACTION_SECURITY 0x10u /**< Routing activation required to initiate central security */
#define DOIP_VIN_GID_SYNCED          0u    /**< VIN and/or GID are synchronized */
#define DOIP_VIN_GID_NOT_SYNCED      0x10u /**< VIN and GID are NOT synchronized */
#define DOIP_RA_ACTIVATION_TYPE_E0   0xE0u /**< Vehicle-manufacturer specific */

/* entity status response */

#define DOIP_ENTITY_STATUS_RSP_MSG_LEN         11u /**< Length of entity status response message */
#define DOIP_ENTITY_STATUS_RSP_MSG_PAYLOAD_LEN 3u  /**< Length of entity status payload */

#define DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_NODE_TYPE \
    8u /**< Location of node type field in entity status response message */
#define DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_MAX_SOCKET \
    9u /**< Location of max socket field in entity status response message */
#define DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_CURRENT_OPEN_SOCKET \
    10u /**< Location of current open socket field in entity status response message */
#define DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_MAX_DATA_SIZE \
    11u /**< Location of max data size field in entity status response message */

#define DOIP_ENTITY_STATUS_RSP_MSG_MAX_DATA_SIZE_LEN \
    4u /**< Length of max data size field in entity status response message */

/* power mode response */

#define DOIP_POWER_MODE_RSP_MSG_LEN         9u /**< Length of power mode response message */
#define DOIP_POWER_MODE_RSP_MSG_PAYLOAD_LEN 1u /**< Length of power mode response payload */

#define DOIP_POWER_MODE_RSP_MSG_FIELD_POSITION_POWER_MODE \
    8u /**< Location of power mode field in power mode response message */

/* ========================================== internal function declarations ======================================== */

/* cfg */

/**
 * @brief       Gets the PC configuration for the specified connection.
 * @param[in]   id: SoAdPduId.
 * @return      boolean
 * @retval      TRUE: Request address assignment.
 * @retval      FALSE: Dont's Request address assignment.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE boolean DOIP_UDP_PCCON_CFG(uint8 id);

/**
 * @brief       Gets the PB configuration for the specified connection.
 * @param[in]   id: Traverse index.
 * @return      DoIP_PBConnectionCfgType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE const DoIP_PBConnectionCfgType* DOIP_UDP_PBCON_CFG(uint8 id); /* PRQA S 5016 */ /* VL_DoIP_5016 */

/**
 * @brief       Gets the PC configuration for the specified connection.
 * @param[in]   id: SoAdPduId.
 * @return      boolean
 * @retval      TRUE: Request address assignment.
 * @retval      FALSE: Dont's Request address assignment.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE boolean DOIP_ANC_PCCON_CFG(uint8 id);

/**
 * @brief       Gets the PB configuration for the specified connection.
 * @param[in]   id: Traverse index.
 * @return      DoIP_PBConnectionCfgType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE const DoIP_PBConnectionCfgType* DOIP_ANC_PBCON_CFG(uint8 id); /* PRQA S 5016 */ /* VL_DoIP_5016 */

/**
 * @brief       Get EID.
 * @param[in]   soConId: Used for call SoAd_GetPhysAddr.
 * @param[out]  buf: Used for save EID.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_GetEid(SoAd_SoConIdType soConId, uint8 buf[]);

#if (TRUE != DOIP_USE_EID_AS_GID)
/**
 * @brief       Get GID.
 * @param[out]  buf: Used for save GID.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_GetGid(uint8 buf[]);
#endif

/**
 * @brief       Get further action byte from configuration.
 * @param[out]  buf: Used for save further action byte.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_GetFurtherActionByteFromCfg(uint8 buf[]);

/**
 * @brief       Get further action byte.
 * @param[out]  buf: Used for save further action byte.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_GetFurtherActionByte(uint8 buf[]);

/**
 * @brief       Get index of PC configuration.
 * @param[in]   soadRxPduId: Used for match PC configuration.
 * @param[out]  index: Index of PC configuration.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_GetUdpConnIndex(PduIdType soadRxPduId, uint8* index);

/* connect status */

/**
 * @brief       Get index of DoIP_UdpConnStatus.
 * @param[in]   id: Used for match element of DoIP_UdpConnStatus.
 * @param[out]  index: Index of PC DoIP_UdpConnStatus.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_GetUdpConnStatusIndex(SoAd_SoConIdType id, uint8* index);

/* communication */

/**
 * @brief       Compare data.
 * @param[in]   first: Used for compare.
 * @param[in]   second: Used for compare.
 * @param[in]   len: Compare how long the data is.
 * @return      uint8
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE uint8 DoIP_CompareData(const uint8* first, const uint8* second, uint32 len);

/**
 * @brief       Check vehicle identification request message has valid payload len.
 * @param[in]   payloadLen: Paylod len field of vehicle identification request message.
 * @param[in]   type: Message type.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsVehicleIdReqHasValidPayloadLen(uint32 payloadLen, uint16 type);

/**
 * @brief       Check whether should send vehicle identification response message.
 * @param[in]   soConId: Used for call SoAd_GetPhysAddr.
 * @param[in]   buf: Used for compare with EID or VIN.
 * @param[in]   type: Message type.
 * @return      boolean
 * @retval      TRUE: Should send.
 * @retval      FALSE: Shouldn't send.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsShouldSendVehicleIdentificationRsp(SoAd_SoConIdType soConId, const uint8* buf, uint16 type);

/**
 * @brief       Handle vehicle identication request.
 * @param[in]   soadTxPduRef: Used for send response.
 * @param[in]   soConId: Used for call SoAd API.
 * @param[in]   payloadLen: Paylod len field of vehicle identification request message.
 * @param[in]   buf: Saved EID or VIN.
 * @param[in]   type: Message type.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandleVehicleIdReq(
    PduIdType        soadTxPduRef,
    SoAd_SoConIdType soConId,
    uint32           payloadLen,
    const uint8*     buf,
    uint16           type);

/**
 * @brief       Send vehicle identification response message.
 * @param[in]   soadTxPduRef: SoadTxPdu.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendVehicleIdentificationRsp(uint16 soadTxPduRef);

/**
 * @brief       Handle entity status request.
 * @param[in]   soadTxPduRef: Used for send response.
 * @param[in]   soConId: Used for call SoAd API.
 * @param[in]   payloadLen: Paylod len field of entity status request message.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandleEntityStatusReq(PduIdType soadTxPduRef, SoAd_SoConIdType soConId, uint32 payloadLen);

/**
 * @brief       Send entity status response message.
 * @param[in]   soadTxPduRef: SoadTxPdu.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendEntityStatusRsp(PduIdType soadTxPduRef);

/**
 * @brief       Handle power mode request.
 * @param[in]   soadTxPduRef: Used for send response.
 * @param[in]   soConId: Used for call SoAd API.
 * @param[in]   payloadLen: Paylod len field of power mode request message.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandlePowerModeReq(PduIdType soadTxPduRef, SoAd_SoConIdType soConId, uint32 payloadLen);

/**
 * @brief       Send power mode response message.
 * @param[in]   soadTxPduRef: SoadTxPdu.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendPowerModeInfoRsp(PduIdType soadTxPduRef);

/**
 * @brief       Check whether should send vehicle announcement message.
 * @param[inout]   ancConnStatus: Element of DoIP_AncConnStatus.
 * @return      boolean
 * @retval      TRUE: Should send.
 * @retval      FALSE: Shouldn't send.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsSendAncMsg(DoIP_VehicleAncConnStatusType* ancConnStatus);

/* ============================================ internal data definitions =========================================== */

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/**
 * @brief Status of UDP connection
 */
DoIP_UdpConnStatusType DoIP_UdpConnStatus[DOIP_UDP_MAX_CONNECTION];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/**
 * @brief Status of announcement connection
 */
DoIP_VehicleAncConnStatusType DoIP_AncConnStatus[DOIP_ANNOUNCEMENT_MAX_CONNECTION];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

/* ========================================== external function definitions ========================================= */

#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"

/* ----------------------------------------- UDP connect & connect's status ----------------------------------------- */

/**
 * @brief Open or close Udp connection.
 */
boolean DoIP_SwitchUdp(const boolean* active) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    boolean assignment;
    uint16  soadTxPduRef;
    boolean ret = TRUE;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    for (uint8 i = 0u; (i < DoIP_PCCfgPtr->ConnsCfg->UdpConnNum) && (ret == TRUE); i++)
    {
        assignment   = DOIP_UDP_PCCON_CFG(i);
        soadTxPduRef = DOIP_UDP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef;

        ret = DoIP_SwitchConnection(soadTxPduRef, assignment, *active);
    }
    /* PRQA S 2814 -- */

    return ret;
}

/**
 * @brief Open or close announcement connection.
 */
boolean DoIP_SwitchAnc(const boolean* active) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    boolean assignment;
    uint16  soadTxPduRef;
    boolean ret = TRUE;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    for (uint8 i = 0u; (i < DoIP_PCCfgPtr->ConnsCfg->AncConnNum) && (ret == TRUE); i++)
    {
        assignment   = DOIP_ANC_PCCON_CFG(i);
        soadTxPduRef = DOIP_ANC_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef;

        ret = DoIP_SwitchConnection(soadTxPduRef, assignment, *active);
    }
    /* PRQA S 2814 -- */

    return ret;
}

/**
 * @brief Notify status of UDP.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
void DoIP_SoConModeChgUdp(SoAd_SoConIdType soConId, SoAd_SoConModeType mode)
/* PRQA S 1532 -- */ /* VL_DoIP_RefeOneFile */
{
    SoAd_SoConIdType localSoConId = 0u;

    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_QAC_DerefNullPtr */
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->ConnsCfg->UdpConnNum; i++)
    {
        if (E_OK == SoAd_GetSoConId(DOIP_UDP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef, &localSoConId))
        {
            if (localSoConId == soConId)
            {
                if (SOAD_SOCON_ONLINE == mode)
                {
                    DoIP_UdpConnStatus[i].SoadTxPduId = DOIP_UDP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduId;

                    DoIP_UdpConnStatus[i].SoConId    = soConId;
                    DoIP_UdpConnStatus[i].SoConState = mode;
                    /* PRQA S 2814,2844 -- */
                }
                else
                {
                    DoIP_ResetUdpConnStatus(i);
                }

                break;
            }
        }
    }
}

/**
 * @brief Notify status of announcement connection.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
void DoIP_SoConModeChgAnc(SoAd_SoConIdType soConId, SoAd_SoConModeType mode)
/* PRQA S 1532 -- */
{
    SoAd_SoConIdType localSoConId = 0u;
    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_QAC_DerefNullPtr */
    const DoIP_PCConnectionsCfgType* ConnsCfg = DoIP_PCCfgPtr->ConnsCfg;

    for (uint8 i = 0u; i < ConnsCfg->AncConnNum; i++)
    {
        (void)SoAd_GetSoConId(DOIP_ANC_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef, &localSoConId);

        if (localSoConId == soConId)
        {
            if (SOAD_SOCON_ONLINE == mode)
            {
                DoIP_AncConnStatus[i].SoConId    = soConId;
                DoIP_AncConnStatus[i].SoConState = mode;

                DoIP_AncConnStatus[i].IsAncTimerWork = TRUE;
                DoIP_AncConnStatus[i].AncTimer       = 0u;
                DoIP_AncConnStatus[i].AncCnt         = 0u;
            }
            else
            {
                DoIP_AncConnStatus[i].SoConId    = DOIP_INVALID_UINT8;
                DoIP_AncConnStatus[i].SoConState = SOAD_SOCON_OFFLINE;
                /* PRQA S 2814,2844 -- */
            }

            break;
        }
    }
}

/**
 * @brief Reset of DoIP_UdpConnStatus.
 */
void DoIP_ResetUdpConnStatus(uint8 index)
{
    /* PRQA S 2844 ++ */ /* VL_QAC_DerefNullPtr */
    DoIP_UdpConnStatus[index].SoConId    = DOIP_INVALID_UINT8;
    DoIP_UdpConnStatus[index].SoConState = SOAD_SOCON_OFFLINE;
    /* PRQA S 2844 -- */
}

/* -----------------------------------------  send & rcv ----------------------------------------- */

/**
 * @brief Send UDP NACK.
 */
void DoIP_SendUdpNack(PduIdType soadTxPduRef, uint8 nack)
{
    uint8 buf[DOIP_NACK_MSG_LEN] = {0};

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    /* payload type*/
    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_GENERIC_DOIP_HEADER_NACK);

    /* payload length*/
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_NACK_MSG_PAYLOAD_LEN);

    /* nack */
    buf[DOIP_NACK_MSG_FIELD_POSITION_CODE] = nack;

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_NACK_MSG_LEN;

    (void)SoAd_IfTransmit(soadTxPduRef, &pduInfo);
}

/**
 * @brief Get SoadTxPduRef and SoConId.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetSoAdTxPduRefAndSoConId(PduIdType soadRxPduId, uint16* soadTxPduRef, SoAd_SoConIdType* soConId)
/* PRQA S 1532 -- */
{
    uint8          index = 0u;
    Std_ReturnType ret   = E_NOT_OK;

    if (E_OK == DoIP_GetUdpConnIndex(soadRxPduId, &index))
    {
        SoAd_SoConIdType localSoConId = 0u;
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        uint16 localSoadTxPduRef = DOIP_UDP_PBCON_CFG(index)->SoAdTxPdu.SoAdTxPduRef;

        if ((E_OK == SoAd_GetSoConId(localSoadTxPduRef, &localSoConId))
            && (E_OK == DoIP_GetUdpConnStatusIndex(localSoConId, &index)) /* PRQA S 3415 */ /* VL_DoIP_3415 */
            && (DoIP_UdpConnStatus[index].SoConState == SOAD_SOCON_ONLINE))
        {
            *soConId      = localSoConId;
            *soadTxPduRef = localSoadTxPduRef;
            /* PRQA S 2814 -- */
            ret = E_OK;
        }
    }

    return ret;
}

/**
 * @brief Get SoadTxPduRef and SoConId.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_CheckUdpMsgPayload(const PduInfoType* pduInfoPtr, uint16 soadTxPduRef, SoAd_SoConIdType soConId)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    /* PRQA S 2824,2814 ++ */ /* VL_QAC_DerefNullPtr */
    uint32 payloadLen  = DOIP_U8_2_U32(&pduInfoPtr->SduDataPtr[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN]);
    uint16 payloadType = DOIP_U8_2_U16(&pduInfoPtr->SduDataPtr[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE]);
    /* PRQA S 2824,2814 -- */

    if ((pduInfoPtr->SduDataPtr[DOIP_HEADER_FIELD_POSITION_VERSION] == 0xffu) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        && (payloadType != DOIP_MSG_TYPE_IDENTIFICATION_REQ)
        && (payloadType != DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_EID)
        && (payloadType != DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_VIN))
    {
        /* SWS_DoIP_00014 */
        DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_INCORRECT_PATTERN_FORMAT);
        DoIP_CloseSoCon(soConId, TRUE);
    }
    else if (payloadLen > (uint32)DOIP_MAX_REQUEST_BYTES) /* PRQA S 1258 */ /* VL_DoIP_1258 */
    {
        DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_MESSAGE_TO_LARGE);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief Handle received IF message.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
void DoIP_HandleIfMsg(
    PduIdType          soadTxPduRef,
    SoAd_SoConIdType   soConId,
    uint32             payloadLen,
    const PduInfoType* pduInfoPtr)
/* PRQA S 1532 -- */
{
    /* PRQA S 2824,2814 ++ */ /* VL_QAC_DerefNullPtr,VL_QAC_DerefNullPtr */
    uint16 payloadType = DOIP_U8_2_U16(&pduInfoPtr->SduDataPtr[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE]);
    /* PRQA S 2824,2814 -- */

    /*RX IF data may be udp message or annoucement*/
    switch (payloadType)
    {
    case DOIP_MSG_TYPE_IDENTIFICATION_REQ:
    case DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_EID:
    case DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_VIN:
    {
        const uint8* payloadPtr = &pduInfoPtr->SduDataPtr[DOIP_HEADER_LEN];
        DoIP_HandleVehicleIdReq(soadTxPduRef, soConId, payloadLen, payloadPtr, payloadType);

        break;
    }
    case DOIP_MSG_TYPE_ENTITY_STATUS_REQ:

        DoIP_HandleEntityStatusReq(soadTxPduRef, soConId, payloadLen);

        break;
    case DOIP_MSG_TYPE_POWER_MODE_REQ:

        DoIP_HandlePowerModeReq(soadTxPduRef, soConId, payloadLen);

        break;
    case DOIP_MSG_TYPE_ANNOUNCEMENT_OR_IDENTIFICATION_RSP:
    case DOIP_MSG_TYPE_GENERIC_DOIP_HEADER_NACK:
    case DOIP_MSG_TYPE_ROUTING_ACTIVATION_REQ:
    case DOIP_MSG_TYPE_DIAG:

        /* ignore */
        SoAd_ReleaseRemoteAddr(soConId);

        break;
    default:

        DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_UNKNOWN_PAYLOAD_TYPE);

        break;
    }
}

/* -----------------------------------------  timer ----------------------------------------- */

/**
 * @brief Handle timers.
 */
void DoIP_HandleVehicleAncTimeout(void) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    for (uint8 i = 0u; i < DOIP_ANNOUNCEMENT_MAX_CONNECTION; i++)
    {
        if (TRUE == DoIP_AncConnStatus[i].IsAncTimerWork)
        {
            DoIP_AncConnStatus[i].AncTimer += DOIP_MAIN_FUNCTION_PERIOD; /* PRQA S 1252 */ /* VL_QAC_1252 */

            if (DoIP_IsSendAncMsg(&DoIP_AncConnStatus[i]) == TRUE)
            {
                /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
                DoIP_SendVehicleIdentificationRsp(DOIP_ANC_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef);
                /* PRQA S 2814 -- */
            }
        }
    }
}

/* ========================================== internal function definitions ========================================= */

/* ----------------------------------------- cfg ----------------------------------------- */

/**
 * @brief Gets the PC configuration for the specified connection.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DOIP_UDP_PCCON_CFG(uint8 id)
/* PRQA S 1505 -- */
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return DoIP_PCCfgPtr->ConnsCfg->UdpRequestAddressAssignmentArray[id];
    /* PRQA S 2814,2824 -- */
}

/**
 * @brief Gets the PB configuration for the specified connection.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE const DoIP_PBConnectionCfgType* DOIP_UDP_PBCON_CFG(uint8 id)
/* PRQA S 1505 -- */
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return &(DoIP_PBCfgPtr->ConnsCfg->UdpConnCfgArray[id]);
    /* PRQA S 2814,2824 -- */
}

/**
 * @brief Gets the PC configuration for the specified connection.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE boolean DOIP_ANC_PCCON_CFG(uint8 id)
/* PRQA S 1505 -- */
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return DoIP_PCCfgPtr->ConnsCfg->AncRequestAddressAssignmentArray[id];
    /* PRQA S 2814,2824 -- */
}

/**
 * @brief Gets the PB configuration for the specified connection.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE const DoIP_PBConnectionCfgType* DOIP_ANC_PBCON_CFG(uint8 id)
/* PRQA S 1505 -- */
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return &(DoIP_PBCfgPtr->ConnsCfg->AncConnCfgArray[id]);
    /* PRQA S 2814,2824 -- */
}

/**
 * @brief       Get EID.
 */
DOIP_LOCAL void DoIP_GetEid(SoAd_SoConIdType soConId, uint8 buf[]) /* PRQA S 3206  */ /* VL_DoIP_3206 */
{
#if (FALSE == DOIP_USE_MAC_ADRESS_FOR_IDENTIFICATION)

    for (uint8 i = 0u; i < DOIP_EID_LEN; i++)
    {
        /* PRQA S 3120,2814,2824 ++ */ /* VL_QAC_MagicNum,VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
        buf[i] = (uint8)(DoIP_PBCfgPtr->Eid >> (uint8)((5u - i) << (uint8)3u));
        /* PRQA S 3120,2814,2824 -- */
    }
    DOIP_PARAM_UNUSED(soConId);
#else

    /*SWS_DoIP_00074*/
    (void)SoAd_GetPhysAddr(soConId, buf);

#endif
}

#if (TRUE != DOIP_USE_EID_AS_GID)
/**
 * @brief Get GID.
 */
DOIP_LOCAL void DoIP_GetGid(uint8 buf[])
{
#if (TRUE == DOIP_GID_CONFIGURED)

    for (uint8 i = 0u; i < DOIP_GID_LEN; i++)
    {
        /* PRQA S 3120,2814,2824 ++ */ /* VL_QAC_MagicNum,VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
        buf[i] = (uint8)(DoIP_PBCfgPtr->Gid >> ((5u - i) << 3));
        /* PRQA S 3120,2814,2824 -- */
    }

#else

    /*SWS_DoIP_00079 SWS_DoIP_00081*/
    if (NULL_PTR != DoIP_General.GetGidDirect)
    {
        uint8 temp[DOIP_GID_LEN] = {0};

        if (E_OK == DoIP_General.GetGidDirect(temp))
        {
            for (uint8 i = 0u; i < DOIP_GID_LEN; i++)
            {
                buf[i] = temp[i]; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
            }
        }
        else
        {
            for (uint8 i = 0u; i < DOIP_GID_LEN; i++)
            {
                buf[i] = (uint8)(DOIP_GID_INVALIDITY_PATTERN & 0xffu); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            }
        }
    }
    else
    {
        for (uint8 i = 0u; i < DOIP_GID_LEN; i++)
        {
            buf[i] = 0u;
        }
    }
#endif
}
#endif

/**
 * @brief Get further action byte from configuration.
 */
DOIP_LOCAL void DoIP_GetFurtherActionByteFromCfg(uint8 buf[]) /* PRQA S 3450 */ /* VL_DoIP_WithoutPreDeclaration */
{
    boolean                     isFind = FALSE;
    uint8                       raNum;
    uint8                       testerNum = DoIP_PCCfgPtr->TesterNum; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    uint16                      sa        = DOIP_INVALID_UINT16;
    const uint8*                raRefArray;
    const DoIP_PBTesterCfgType* testerCfg;
    const DoIP_PBRaCfgType*     raCfgArray = DoIP_PBCfgPtr->RaCfgArray; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

    for (uint8 i = 0u; i < testerNum; i++)
    {
        testerCfg  = &DoIP_PBCfgPtr->TesterCfgArray[i]; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
        raNum      = testerCfg->RaNum; /* PRQA S 2814 */                  /* VL_QAC_DerefNullPtr */
        raRefArray = testerCfg->RaRefArray;

        for (uint8 j = 0u; j < raNum; j++)
        {
            /* PRQA S 2824 ++ */ /* VL_DoIP_ArithNullptr */
            if (DOIP_RA_ACTIVATION_TYPE_E0 == raCfgArray[raRefArray[j]].RaType)
            /* PRQA S 2824 -- */
            {
                sa     = testerCfg->TesterSA;
                isFind = TRUE;
                break;
            }
        }
    }

    if (TRUE == isFind)
    {
        uint8 index = 0u;

        /*SWS_DoIP_00082*/
        if ((E_OK == DoIP_GetTcpConnStatusIdxBySa(sa, &index))
            && (DOIP_SOCKET_ACTIVATED != DoIP_TcpConnStatus[index].RaState)) /* PRQA S 2844 */ /* VL_QAC_DerefNullPtr */
        {
            buf[0] = DOIP_FURTHER_ACTION_SECURITY; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
        }
    }
}

/**
 * @brief Get further action byte.
 */
DOIP_LOCAL void DoIP_GetFurtherActionByte(uint8 buf[])
{
    /* SWS_DoIP_00083 SWS_DoIP_00084 */
    buf[0] = 0x00u; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */

    if (DoIP_General.FurtherActionByteDirect != NULL_PTR)
    {
        uint8 byte = 0u;

        if (E_OK == DoIP_General.FurtherActionByteDirect(&byte))
        {
            buf[0] = byte;
        }
        else
        {
            DoIP_GetFurtherActionByteFromCfg(buf);
        }
    }
}

/**
 * @brief Get index of PC configuration.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_GetUdpConnIndex(PduIdType soadRxPduId, uint8* index)
/* PRQA S 1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    uint8                           connNbr = DoIP_PCCfgPtr->ConnsCfg->UdpConnNum;
    const DoIP_PBConnectionCfgType* conn    = DoIP_PBCfgPtr->ConnsCfg->UdpConnCfgArray;

    for (uint8 i = 0u; i < connNbr; i++)
    {
        if (conn[i].SoAdRxPdu.SoAdRxPduId == soadRxPduId)
        {
            *index = i;
            /* PRQA S 2814,2824 -- */
            ret = E_OK;
            break;
        }
    }

    return ret;
}

/* ----------------------------------------- connect status ----------------------------------------- */

/**
 * @brief Get index of DoIP_UdpConnStatus.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE Std_ReturnType DoIP_GetUdpConnStatusIndex(SoAd_SoConIdType id, uint8* index)
/* PRQA S 1505 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    for (uint8 i = 0u; i < DOIP_UDP_MAX_CONNECTION; i++)
    {
        if (DoIP_UdpConnStatus[i].SoConId == id)
        {
            *index = i; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            ret    = E_OK;
            break;
        }
    }

    return ret;
}

/* ----------------------------------------- communication ----------------------------------------- */

/**
 * @brief Compare data.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE uint8 DoIP_CompareData(const uint8* first, const uint8* second, uint32 len)
/* PRQA S 1505 -- */
{
    uint8 ret = 0u;

    for (uint32 i = 0u; i < len; i++)
    {
        ret = first[i] - second[i]; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
        if (ret != 0u)
        {
            break;
        }
    }

    return ret;
}

/**
 * @brief Check vehicle identification request message has valid payload len.
 */
DOIP_LOCAL boolean DoIP_IsVehicleIdReqHasValidPayloadLen(uint32 payloadLen, uint16 type)
{
    boolean ret = FALSE;

    if (type == DOIP_MSG_TYPE_IDENTIFICATION_REQ)
    {
        /* SWS_DoIP_00059*/
        if (0uL == payloadLen)
        {
            ret = TRUE;
        }
    }
    else if (type == DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_EID)
    {
        if (DOIP_EID_LEN == payloadLen) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            ret = TRUE;
        }
    }
    else if (type == DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_VIN)
    {
        if (DOIP_VIN_LEN == payloadLen) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            ret = TRUE;
        }
    }
    else
    {
        /* QAC */
    }

    return ret;
}

/**
 * @brief Check whether should send vehicle identification response message.
 */
DOIP_LOCAL boolean DoIP_IsShouldSendVehicleIdentificationRsp(SoAd_SoConIdType soConId, const uint8* buf, uint16 type)
{
    boolean ret = FALSE;

    if (type == DOIP_MSG_TYPE_IDENTIFICATION_REQ)
    {
        ret = TRUE;
    }

    if (type == DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_EID)
    {
        uint8 eid[DOIP_EID_LEN];

        /*SWS_DoIP_00066*/
        DoIP_GetEid(soConId, eid);

        if (0u == DoIP_CompareData(eid, buf, DOIP_EID_LEN)) /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
        {
            ret = TRUE;
        }
    }

    if (type == DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_VIN)
    {
        uint8 vin[DOIP_VIN_LEN];

        /* SWS_DoIP_00070*/
        if (E_OK != Dcm_GetVin(vin))
        {
            for (uint8 i = 0u; i < DOIP_VIN_LEN; i++)
            {
                vin[i] = DOIP_VIN_INVALIDITY_PATTERN & (0xffu); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
            }
        }

        if (0u == DoIP_CompareData(vin, buf, DOIP_VIN_LEN))
        {
            ret = TRUE;
        }
    }

    return ret;
}

/**
 * @brief Handle vehicle identication request.
 */
DOIP_LOCAL void DoIP_HandleVehicleIdReq(
    PduIdType        soadTxPduRef,
    SoAd_SoConIdType soConId,
    uint32           payloadLen,
    const uint8*     buf,
    uint16           type)
{
    if (DoIP_IsVehicleIdReqHasValidPayloadLen(payloadLen, type) == TRUE)
    {
        if (DoIP_IsShouldSendVehicleIdentificationRsp(soConId, buf, type) == TRUE)
        {
            DoIP_SendVehicleIdentificationRsp(soadTxPduRef);
        }
    }
    else
    {
        DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_INVALID_PAYLOAD_LENGTH);
        DoIP_CloseSoCon(soConId, TRUE);
    }
}

/**
 * @brief Send vehicle identification response message.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_DoIP_STCAL */
DOIP_LOCAL void DoIP_SendVehicleIdentificationRsp(uint16 soadTxPduRef)
{
    uint8 buf[DOIP_IDENTIFICATION_RSP_MSG_LEN + 1u] = {0u}; /* PRQA S 0686 */ /* VL_DoIP_0686 */

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    /* payload type */
    DoIP_u16_2_u8(
        &buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE],
        (uint16)DOIP_MSG_TYPE_ANNOUNCEMENT_OR_IDENTIFICATION_RSP);

    /* payload length */
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_IDENTIFICATION_RSP_MSG_PAYLOAD_LEN);

    /* VIN - 8~24 */
    if (E_NOT_OK == Dcm_GetVin(&buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_VIN]))
    {
        for (uint8 i = 0u; i < DOIP_VIN_LEN; i++)
        {
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            buf[i + DOIP_HEADER_LEN] = (uint8)(DOIP_VIN_INVALIDITY_PATTERN & 0xffu);
            /* PRQA S 3120 -- */
        }
    }

    /* LA - 25~26 */
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    DoIP_u16_2_u8(&buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_LA], DoIP_PBCfgPtr->LogicalAddress);
    /* PRQA S 2814 -- */

    /* EID - 27~32 */
    SoAd_SoConIdType soConId;
    (void)SoAd_GetSoConId(soadTxPduRef, &soConId);
    DoIP_GetEid(soConId, &buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_EID]);

    /* GID - 33~38 */
#if (TRUE == DOIP_USE_EID_AS_GID)

    for (uint8 i = 0u; i < DOIP_EID_LEN; i++)
    {
        buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_GID + i] =
            buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_EID + i];
    }

#else

    DoIP_GetGid(&buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_GID]);

#endif

    /* Further action required Byte - 39 */
    DoIP_GetFurtherActionByte(&buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_FURTHER_ACTION]);

    uint8 bufLen = DOIP_IDENTIFICATION_RSP_MSG_LEN;

#if (TRUE == DOIP_USE_VEHICLE_IDENTIFICATION_SYNC_STATUS)

    DoIP_u32_2_u8(
        &buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN],
        DOIP_IDENTIFICATION_RSP_MSG_PAYLOAD_LEN_WITH_SYNC_STATUS);

    /* VIN/GID Status - 40 */
    uint8 tmp[DOIP_VIN_LEN];

    if (E_NOT_OK == Dcm_GetVin(tmp))
    {
        if (TRUE != DoIP_GidSynStatus)
        {
            buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_SYNC_STATUS] = DOIP_VIN_GID_NOT_SYNCED; /*SWS_DoIP_00088*/
        }
        else
        {
            buf[DOIP_IDENTIFICATION_RSP_MSG_FIELD_POSITION_SYNC_STATUS] = DOIP_VIN_GID_SYNCED; /*SWS_DoIP_00089*/
        }
    }

    bufLen = DOIP_IDENTIFICATION_RSP_MSG_LEN + 1u;

#endif

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = bufLen;

    (void)SoAd_IfTransmit(soadTxPduRef, &pduInfo);
}
/* PRQA S 6070 -- */

/**
 * @brief Handle entity status request.
 */
DOIP_LOCAL void DoIP_HandleEntityStatusReq(PduIdType soadTxPduRef, SoAd_SoConIdType soConId, uint32 payloadLen)
{
    if (payloadLen == 0uL)
    {
        DoIP_SendEntityStatusRsp(soadTxPduRef);
    }
    else
    {
        DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_INVALID_PAYLOAD_LENGTH);
        DoIP_CloseSoCon(soConId, TRUE);
    }
}

/**
 * @brief Send entity status response message.
 */
DOIP_LOCAL void DoIP_SendEntityStatusRsp(PduIdType soadTxPduRef)
{
    uint8 buf[DOIP_ENTITY_STATUS_RSP_MSG_LEN + DOIP_ENTITY_STATUS_RSP_MSG_MAX_DATA_SIZE_LEN];

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    /* payload type */
    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_ENTITY_STATUS_RSP);

    /* payload length */
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_ENTITY_STATUS_RSP_MSG_PAYLOAD_LEN);

    /* Node Type */
    buf[DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_NODE_TYPE] = 1u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */

    /* Max open sockets */
    buf[DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_MAX_SOCKET] = DOIP_MAX_TESTER_CONNECTION;

    /* Currently open sockets */
    buf[DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_CURRENT_OPEN_SOCKET] = DoIP_CurrentTcpConnCnt;

    uint16 bufLen = DOIP_ENTITY_STATUS_RSP_MSG_LEN;

#if (TRUE == DOIP_ENTITY_STATUS_MAXFIELD_USE)

    DoIP_u32_2_u8(
        &buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN],
        DOIP_ENTITY_STATUS_RSP_MSG_PAYLOAD_LEN + DOIP_ENTITY_STATUS_RSP_MSG_MAX_DATA_SIZE_LEN);

    DoIP_u32_2_u8(&buf[DOIP_ENTITY_STATUS_RSP_MSG_FIELD_POSITION_MAX_DATA_SIZE], DOIP_MAX_REQUEST_BYTES);

    bufLen = DOIP_ENTITY_STATUS_RSP_MSG_LEN + DOIP_ENTITY_STATUS_RSP_MSG_MAX_DATA_SIZE_LEN;

#endif

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = bufLen;

    (void)SoAd_IfTransmit(soadTxPduRef, &pduInfo);
}

/**
 * @brief Handle power mode request.
 */
DOIP_LOCAL void DoIP_HandlePowerModeReq(PduIdType soadTxPduRef, SoAd_SoConIdType soConId, uint32 payloadLen)
{
    if (payloadLen == 0uL)
    {
        DoIP_SendPowerModeInfoRsp(soadTxPduRef);
    }
    else
    {
        DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_INVALID_PAYLOAD_LENGTH);
        DoIP_CloseSoCon(soConId, TRUE);
    }
}

/**
 * @brief Send power mode response message.
 */
DOIP_LOCAL void DoIP_SendPowerModeInfoRsp(PduIdType soadTxPduRef)
{
    uint8 buf[DOIP_POWER_MODE_RSP_MSG_LEN] = {0};

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    /* payload type */
    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_POWER_MODE_RSP);

    /* payload length */
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_POWER_MODE_RSP_MSG_PAYLOAD_LEN);

    if (DoIP_General.PowerModeDirect != NULL_PTR)
    {
        uint8 tmp;
        if (E_OK == (*DoIP_General.PowerModeDirect)(&tmp))
        {
            buf[DOIP_POWER_MODE_RSP_MSG_FIELD_POSITION_POWER_MODE] = tmp;
        }
    }

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_POWER_MODE_RSP_MSG_LEN;

    (void)SoAd_IfTransmit(soadTxPduRef, &pduInfo);
}

/**
 * @brief Check whether should send vehicle announcement message.
 */
DOIP_LOCAL boolean DoIP_IsSendAncMsg(DoIP_VehicleAncConnStatusType* ancConnStatus)
{
    boolean ret = FALSE;

    if (ancConnStatus->AncCnt == 0u) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        /* PRQA S 1258 ++ */ /* VL_DoIP_1258 */
        if (ancConnStatus->AncTimer >= (uint32)DOIP_INITIAL_VEHICLE_ANNOUNCEMENT_TIME)
        /* PRQA S 1258 -- */
        {
            /*SWS_DoIP_00205*/
            ret = TRUE;
        }
    }
    else if (ancConnStatus->AncCnt < DOIP_VEHICLE_ANNOUNCEMENT_COUNT)
    {
        if (ancConnStatus->AncTimer >= DOIP_VEHICLE_ANNOUNCEMENT_INTRRNAL) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            /*SWS_DoIP_00205*/
            ret = TRUE;
        }
    }
    else
    {
        ancConnStatus->IsAncTimerWork = FALSE;
        ancConnStatus->AncCnt         = 0u;
        ancConnStatus->AncTimer       = 0u;
    }

    return ret;
}

#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
