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
 **  @file               : DoIP_Internal_TCP_Common.c
 **  @author             : hao.wen
 **  @date               : 2024/11/01
 **  @vendor             : isoft
 **  @description        : Implement the common part of TCP
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_TCP.h"
#include "DoIP_Internal_TCP_Common.h"
#include "DoIP_Internal_TCP_Diagnostic.h"
#include "DoIP_Internal_TCP_Non_Diagnostic.h"

/* ===================================================== macros ===================================================== */

#define DOIP_MAX_NON_DIAGNOSTIC_DATA_LENGTH 0x19u /**< MAX length of non diagnostic message. */

/* ========================================== internal function definitions ========================================= */

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
DOIP_LOCAL boolean DOIP_TCP_PCCON_CFG(uint8 id);

/**
 * @brief       Get index of channel.
 * @param[in]   sa: SA of channel.
 * @param[in]   ta: TA of channel.
 * @param[out]  channelIdx: Index of channel.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL Std_ReturnType DoIP_GetChannelIdxBySaTa(uint16 sa, uint16 ta, uint16* channelIdx);

/* pending */

/**
 * @brief       Clear all queue.
 * @param[in]   ctx: Send receive context.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE void DoIP_ResetQueue(DoIP_TcpTxRxContextType* ctx);

/* rcv */

/**
 * @brief       Whether it is a UDP message.
 * @param[in]   payloadType: Payload type.
 * @return      boolean
 * @retval      TRUE: It's UDP message.
 * @retval      FALSE: It not UDP message.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsUdpMsg(uint16 payloadType);

/**
 * @brief       Whether the payloadlen field value of the message is correct.
 * @param[in]   soadTxPduRef: Used for send NACK message.
 * @param[in]   txRxCtxIdx: Provide context for send message.
 * @param[in]   payloadType: PayloadType field of message.
 * @param[in]   payloadLen: PayloadLen field of message.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean
    DoIP_IsValidPayloadLen(uint16 soadTxPduRef, uint8 txRxCtxIdx, uint16 payloadType, PduLengthType payloadLen);

/**
 * @brief       Check whether the payload of the message is valid.
 * @param[in]   soadTxPduRef: Used for send NACK message.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @param[out]  soConId: Used for get DoIP_TcpConnStatus.
 * @return      Std_ReturnType
 * @retval      E_OK: Valid.
 * @retval      E_NOT_OK: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL Std_ReturnType DoIP_CheckTcpMsgPayload(uint16 soadTxPduRef, uint8 txRxCtxIdx, SoAd_SoConIdType soConId);

/**
 * @brief       Check whether the header of the message is valid.
 * @param[in]   soadTxPduRef: Used for send NACK message.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      Std_ReturnType
 * @retval      E_OK: Valid.
 * @retval      E_NOT_OK: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL Std_ReturnType DoIP_IsValidHeader(uint16 soadTxPduRef, uint8 txRxCtxIdx);

/* send */

/**
 * @brief       Send TCP NACK message.
 * @param[in]   soadTxPduRef: Used for send NACK message.
 * @param[in]   nack: NACK code.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendTcpNack(PduIdType soadTxPduRef, uint8 nack, uint8 txRxCtxIdx);

/* ============================================ internal data definitions =========================================== */

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/**
 * @brief Status of TCP connection
 */
DoIP_TcpConnStatusType DoIP_TcpConnStatus[DOIP_TCP_MAX_CONNECTION];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/**
 * @brief Send and receive context of TCP
 */
DoIP_TcpTxRxContextType DoIP_TcpTxRxContext[DOIP_TCP_MAX_CONNECTION];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_INIT_8
#include "DoIP_MemMap.h"
/**
 * @brief Number of connections that have been established
 */
uint8 DoIP_CurrentTcpConnCnt = 0u;
#define DOIP_STOP_SEC_VAR_INIT_8
#include "DoIP_MemMap.h"

/* ========================================== external function definitions ========================================= */

#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"

/* ----------------------------------------- cfg ----------------------------------------- */
/* PRQA S 3415 ++ */ /* VL_DoIP_3415 */
/**
 * @brief Gets the Pb configuration for the specified connection.
 */
const DoIP_PBConnectionCfgType* DOIP_TCP_PBCON_CFG(uint8 id)
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return &(DoIP_PBCfgPtr->ConnsCfg->TcpConnCfgArray[id]);
    /* PRQA S 2814,2824 -- */
}

/**
 * @brief Get target address.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType DoIP_GetTaFromChannel(uint16 sa, uint16* ta)
{
    Std_ReturnType ret = E_NOT_OK;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->ChannelNum; i++)
    {
        if ((DoIP_PBCfgPtr->ChannelCfgArray[i].ChannelSaRef->TesterSA == sa)
            && (NULL_PTR != DoIP_PBCfgPtr->ChannelCfgArray[i].PduRTxPdu)
            && (NULL_PTR != DoIP_PBCfgPtr->ChannelCfgArray[i].PduRRxPdu))
        {
            *ta = DoIP_PBCfgPtr->ChannelCfgArray[i].ChannelTa;
            /* PRQA S 2814,2824 -- */
            ret = E_OK;

            break;
        }
    }

    return ret;
}
/* PRQA S 1532 -- */

/**
 * @brief  Get PduRRxPdu.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetPduRRxPdu(uint16 sa, uint16 ta, const DoIP_PBPduRRxPduType** pdurRxPdu)
/* PRQA S 1505 -- */
{
    uint16         channelIdx = 0u;
    Std_ReturnType ret        = DoIP_GetChannelIdxBySaTa(sa, ta, &channelIdx);

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    if ((ret == E_OK) && (NULL_PTR != DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].PduRRxPdu))
    {
        *pdurRxPdu = DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].PduRRxPdu;
        /* PRQA S 2814,2824 -- */
    }

    return ret;
}

/**
 * @brief Get TCP configuration.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
const DoIP_PBConnectionCfgType* DoIP_GetTcpConnBySoAdRxPduId(PduIdType soadRxPduId)
/* PRQA S 1532 -- */
{
    const DoIP_PBConnectionCfgType* ret = NULL_PTR;
    const DoIP_PBConnectionCfgType* tcpConn;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->ConnsCfg->TcpConnNum; i++)
    {
        tcpConn = &(DoIP_PBCfgPtr->ConnsCfg->TcpConnCfgArray[i]);
        if (tcpConn->SoAdRxPdu.SoAdRxPduId == soadRxPduId)
        {
            /* PRQA S 2814,2824 -- */
            ret = tcpConn;
            break;
        }
    }

    return ret;
}

/**
 * @brief Get PdurRxPdu.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetPduRRxPduRef(uint16 sa, uint16 ta, PduIdType* pdurRxPduRef)
/* PRQA S 1532 -- */
{
    const DoIP_PBPduRRxPduType* pdurRxPdu;
    Std_ReturnType              ret = DoIP_GetPduRRxPdu(sa, ta, &pdurRxPdu);
    if (ret == E_OK)
    {
        *pdurRxPduRef = pdurRxPdu->PdurRxPduRef; /* PRQA S 2814,2962 */ /* VL_QAC_DerefNullPtr,VL_DoIP_UseUninitValue */
    }

    return ret;
}

/**
 * @brief Get SoadTxPduRef.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetSoAdTxPduRefBySoAdRxPduId(PduIdType soadRxPduId, uint16* soadTxPduRef)
/* PRQA S 1532 -- */
{
    Std_ReturnType                  ret = E_NOT_OK;
    const DoIP_PBConnectionCfgType* tcpConn;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->ConnsCfg->TcpConnNum; i++)
    {
        tcpConn = &DoIP_PBCfgPtr->ConnsCfg->TcpConnCfgArray[i];
        if (tcpConn->SoAdRxPdu.SoAdRxPduId == soadRxPduId)
        {
            *soadTxPduRef = tcpConn->SoAdTxPdu.SoAdTxPduRef;
            /* PRQA S 2814,2824 -- */
            ret = E_OK;
            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of channel.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetChannelIdxByPduRRxPduId(PduIdType pdurRxPduId, uint16* index)
/* PRQA S 1532 -- */
{
    Std_ReturnType              ret = E_NOT_OK;
    const DoIP_PBPduRRxPduType* pdurRxPdu;
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint16 i = 0u; i < DoIP_PCCfgPtr->ChannelNum; i++)
    {
        pdurRxPdu = DoIP_PBCfgPtr->ChannelCfgArray[i].PduRRxPdu;

        if ((NULL_PTR != pdurRxPdu) && (pdurRxPdu->PdurRxPduId == pdurRxPduId))
        {
            *index = i;
            /* PRQA S 2814,2824 -- */
            ret = E_OK;
            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of channel.
 */
Std_ReturnType DoIP_GetChannelIdxByPduRTxPduId(PduIdType pdurTxPduId, DoIP_PBPduTypeType pduType, uint16* index)
{
    Std_ReturnType              ret = E_NOT_OK;
    const DoIP_PBPduRTxPduType* pdurTxPdu;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint16 i = 0u; i < DoIP_PCCfgPtr->ChannelNum; i++)
    {
        pdurTxPdu = DoIP_PBCfgPtr->ChannelCfgArray[i].PduRTxPdu;

        if ((NULL_PTR != pdurTxPdu) && (pdurTxPdu->PdurTxPduId == pdurTxPduId) && (pdurTxPdu->PduType == pduType))
        {
            *index = i;
            /* PRQA S 2814,2824 -- */
            ret = E_OK;

            break;
        }
    }

    return ret;
}

/* ----------------------------------------- connect ----------------------------------------- */

/**
 * @brief Open or close TCP connection.
 */
boolean DoIP_SwitchTcp(const boolean* active) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    boolean assignment;
    uint16  soadTxPduRef;
    boolean ret = TRUE;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    for (uint8 i = 0u; (i < DoIP_PCCfgPtr->ConnsCfg->TcpConnNum) && (ret == TRUE); i++)
    {
        soadTxPduRef = DOIP_TCP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef;
        assignment   = DOIP_TCP_PCCON_CFG(i);

        ret = DoIP_SwitchConnection(soadTxPduRef, assignment, *active);
        /* PRQA S 2814 -- */
    }

    return ret;
}

/**
 * @brief Return status of TCP.
 */
boolean DoIP_IsSoConOnline(uint16 channelIdx) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    boolean ret = FALSE;

    uint8 index;
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    uint16 sa = DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].ChannelSaRef->TesterSA;
    /* PRQA S 2814,2824 -- */

    if ((E_OK == DoIP_GetTcpConnStatusIdxBySa(sa, &index))
        && (SOAD_SOCON_ONLINE == DoIP_TcpConnStatus[index].SoConState))
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief Notify status of TCP.
 */
void DoIP_SoConModeChgTcp(SoAd_SoConIdType soConId, SoAd_SoConModeType mode) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->ConnsCfg->TcpConnNum; i++)
    {
        SoAd_SoConIdType localSoConId;
        if (E_OK == SoAd_GetSoConId(DOIP_TCP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef, &localSoConId))
        {
            if (localSoConId == soConId)
            {
                if (SOAD_SOCON_ONLINE == mode)
                {
                    DoIP_TcpConnStatus[i].SoConState        = mode;
                    DoIP_TcpConnStatus[i].InitInactiveTimer = 0u;
                    DoIP_TcpConnStatus[i].SoConId           = soConId;
                    DoIP_TcpConnStatus[i].SoadRxPduId       = DOIP_TCP_PBCON_CFG(i)->SoAdRxPdu.SoAdRxPduId;
                    DoIP_TcpConnStatus[i].SoadTxPduId       = DOIP_TCP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduId;
                    DoIP_TcpConnStatus[i].SoadTxPduRef      = DOIP_TCP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef;
                    /* PRQA S 2814,2844 -- */
                    DoIP_CurrentTcpConnCnt++;
                }
                else
                {
                    /*SWS_DoIP_00243*/
                    DoIP_CloseTcpConnection(soConId);
                }
                break;
            }
        }
    }
}

/**
 * @brief Close TCP.
 */
void DoIP_CloseTcpConnection(SoAd_SoConIdType soConId)
{
    uint8 tcpConnStatusIdx;

    if (E_OK == DoIP_GetTcpConnStatusIdxBySoConId(soConId, &tcpConnStatusIdx))
    {
        PduIdType soadRxPduId = DoIP_TcpConnStatus[tcpConnStatusIdx].SoadRxPduId;
        DoIP_ResetTcpConnStatus(tcpConnStatusIdx);

        DoIP_CurrentTcpConnCnt--;

        uint8 txRxCtxIdx;
        if (E_OK == DoIP_GetTcpTxRxCtxIdxBySoAdRxPduId(soadRxPduId, &txRxCtxIdx))
        {
            /*colse shall notify DCM to reset resource*/
            DoIP_NotifyPduRWhenCloseTcpConnection(txRxCtxIdx);

            /*Handle the pending routing activation if a connection is closed.*/
            /*todo, check role*/
            DoIP_HandleRaWhenCloseTcpConnection();

            DoIP_ResetTcpTxRxContext(txRxCtxIdx);
        }
    }
}

/* ----------------------------------------- connect's status ----------------------------------------- */

/**
 * @brief Get index of DoIP_TcpConnStatus.
 */
/* PRQA S 1505 ++*/ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySoConId(SoAd_SoConIdType soConId, uint8* index)
/* PRQA S 1505 -- */
{
    uint8 ret = E_NOT_OK;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        if (DoIP_TcpConnStatus[i].SoConId == soConId)
        {
            *index = i; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            ret    = E_OK;

            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of DoIP_TcpConnStatus.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySoAdTxPduId(uint16 soadTxPduId, uint8* index)
/* PRQA S 1532 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        if (DoIP_TcpConnStatus[i].SoadTxPduId == soadTxPduId)
        {
            *index = i; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            ret    = E_OK;
            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of DoIP_TcpConnStatus.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySoAdRxPduId(uint16 soadRxPduId, uint8* index)
/* PRQA S 1532 -- */
{
    uint8 ret = E_NOT_OK;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        if (DoIP_TcpConnStatus[i].SoadRxPduId == soadRxPduId)
        {
            *index = i; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            ret    = E_OK;

            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of DoIP_TcpConnStatus.
 */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySa(uint16 sa, uint8* index)
{
    uint8 ret = E_NOT_OK;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        if (DoIP_TcpConnStatus[i].Sa == sa)
        {
            *index = i; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            ret    = E_OK;
            break;
        }
    }

    return ret;
}

/**
 * @brief Reset of DoIP_TcpConnStatus.
 */
void DoIP_ResetTcpConnStatus(uint8 index)
{
    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpConnStatus[index].SoConId              = DOIP_INVALID_UINT8;
    DoIP_TcpConnStatus[index].SoadRxPduId          = DOIP_INVALID_UINT16;
    DoIP_TcpConnStatus[index].SoadTxPduId          = DOIP_INVALID_UINT16;
    DoIP_TcpConnStatus[index].SoadTxPduRef         = DOIP_INVALID_UINT16;
    DoIP_TcpConnStatus[index].Sa                   = DOIP_INVALID_UINT16;
    DoIP_TcpConnStatus[index].Ta                   = DOIP_INVALID_UINT16;
    DoIP_TcpConnStatus[index].RaType               = DOIP_INVALID_UINT8;
    DoIP_TcpConnStatus[index].InitInactiveTimer    = DOIP_INVALID_UINT32;
    DoIP_TcpConnStatus[index].GeneralInactiveTimer = DOIP_INVALID_UINT32;
    DoIP_TcpConnStatus[index].AuthState            = FALSE;
    DoIP_TcpConnStatus[index].ConfirmState         = FALSE;
    DoIP_TcpConnStatus[index].SoConState           = SOAD_SOCON_OFFLINE;
    DoIP_TcpConnStatus[index].RaState              = DOIP_SOCKET_NONACTIVATED;
    DoIP_TcpConnStatus[index].TxRxCtxIdx           = DOIP_INVALID_UINT8;
    DoIP_TcpConnStatus[index].WaitCloseSocket      = FALSE;
    DoIP_TcpConnStatus[index].WaitAliveCheckRsp    = FALSE;
    /* PRQA S 2844 -- */
}

/* ----------------------------------------- tx rx context ----------------------------------------- */

/**
 * @brief Get index of DoIP_TcpTxRxContext.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
Std_ReturnType DoIP_GetTcpTxRxCtxIdxByPduRTxPduId(PduIdType pdurTxPduId, uint8* txRxCtxIdx)
/* PRQA S 1532 -- */
{
    uint8                     ret = E_NOT_OK;
    const DoIP_TcpTxCtrlType* txCtrl;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        txCtrl = &DoIP_TcpTxRxContext[i].TxCtrl;
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        if (txCtrl->PdurTxPduId == pdurTxPduId)
        {
            *txRxCtxIdx = i;
            /* PRQA S 2814 -- */
            ret = E_OK;
            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of DoIP_TcpTxRxContext.
 */
Std_ReturnType DoIP_GetTcpTxRxCtxIdxBySoAdRxPduId(PduIdType soadRxPduId, uint8* txRxCtxIdx)
{
    uint8 ret = E_NOT_OK;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        if (DoIP_TcpTxRxContext[i].RxCtrl.SoadRxPduId == soadRxPduId)
        {
            *txRxCtxIdx = i; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            ret         = E_OK;
            break;
        }
    }

    return ret;
}

/**
 * @brief Get index of DoIP_TcpTxRxContext.
 */
Std_ReturnType DoIP_GetTcpTxRxCtxIdxByChannelIdx(uint16 channelIdx, uint8* txRxCtxIdx)
{
    Std_ReturnType ret = E_NOT_OK;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    uint16 sa = DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].ChannelSaRef->TesterSA;
    /* PRQA S 2814,2824 -- */

    uint8 index;
    if (E_OK == DoIP_GetTcpConnStatusIdxBySa(sa, &index))
    {
        *txRxCtxIdx = DoIP_TcpConnStatus[index].TxRxCtxIdx; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        ret         = E_OK;
    }

    return ret;
}

/**
 * @brief Reset of send DoIP_TcpTxRxContext.
 */
void DoIP_ResetTcpTxContext(uint8 txRxCtxIdx)
{
    for (uint32 i = 0u; i < DOIP_TX_BUFFER_SIZE; i++)
    {
        DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[i] = DOIP_INVALID_UINT8; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
    }

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
    /* PRQA S 2844 -- */

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    txCtrl->TxBufPos        = 0u;
    txCtrl->PdurTxPduId     = (PduIdType)DOIP_INVALID_UINT16;
    txCtrl->PdurTxPduRef    = (PduIdType)DOIP_INVALID_UINT16;
    txCtrl->TxBufState      = DOIP_BUFFER_IDLE;
    txCtrl->TxMaxDataLength = DOIP_INVALID_UINT16;
    txCtrl->TxMsgType       = DOIP_TX_NON_MSG;
    txCtrl->TxState         = DOIP_TX_STATE_IDLE;
    /* PRQA S 2814 -- */
}

/**
 * @brief Reset of receive DoIP_TcpTxRxContext.
 */
void DoIP_ResetTcpRxContext(uint8 txRxCtxIdx)
{
    for (uint32 i = 0u; i < DOIP_RX_BUFFER_SIZE; i++)
    {
        DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[i] = DOIP_INVALID_UINT8; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
    }

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl;
    /* PRQA S 2844 -- */
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    rxCtrl->RxBufState   = DOIP_BUFFER_IDLE;
    rxCtrl->BlockSize    = DOIP_INVALID_UINT16;
    rxCtrl->PayloadLen   = DOIP_INVALID_UINT16;
    rxCtrl->PayloadType  = DOIP_INVALID_UINT16;
    rxCtrl->RxState      = DOIP_RX_STATE_START;
    rxCtrl->RxBufPos     = 0u;
    rxCtrl->RxBufRemain  = DOIP_INVALID_UINT16;
    rxCtrl->RxDataRemain = DOIP_INVALID_UINT16;
    rxCtrl->PdurRxPduRef = (PduIdType)DOIP_INVALID_UINT16;
    /* PRQA S 2814 -- */
}

/**
 * @brief Reset of send/receive DoIP_TcpTxRxContext.
 */
void DoIP_ResetTcpTxRxContext(uint8 txRxCtxIdx)
{
    DoIP_TcpTxRxContextType* ctx = &DoIP_TcpTxRxContext[txRxCtxIdx]; /* PRQA S 2934 */ /* VL_DoIP_ComputeInvalidPtr */

    DoIP_ResetTcpRxContext(txRxCtxIdx);

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    /* Don't add to DoIP_ResetTcpRxContext, DoIP_ResetTcpRxContext will be called elsewhere*/
    ctx->RxCtrl.SoadRxPduId = (PduIdType)DOIP_INVALID_UINT16;
    ctx->RxCtrl.RxState     = DOIP_RX_STATE_IDLE;
    /* PRQA S 2844 -- */

    DoIP_ResetTcpTxContext(txRxCtxIdx);

    DoIP_ResetQueue(&DoIP_TcpTxRxContext[txRxCtxIdx]); /* PRQA S 2934 */ /* VL_DoIP_ComputeInvalidPtr */
}

/* ----------------------------------------- pending ----------------------------------------- */

/**
 * @brief Handle pending message.
 */
void DoIP_HandlePendingMsg(void) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    const DoIP_TcpTxCtrlType* txCtrl;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        txCtrl = &DoIP_TcpTxRxContext[i].TxCtrl;

        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
        /* PRQA S 2814 -- */
        {
            /* PRQA S 4558,4404,4115 ++ */ /* VL_DoIP_4558,VL_DoIP_4404,VL_DoIP_4115 */
            boolean ret = DoIP_HandlePendingTpMsg(&DoIP_TcpTxRxContext[i]);
            ret         = ret && DoIP_HandlePendingIfMsg(&DoIP_TcpTxRxContext[i]);
            /* PRQA S 2982 ++ */ /* VL_DoIP_UsedBeforModify */
            ret = ret && DoIP_HandlePendingNonDiagMsg(&DoIP_TcpTxRxContext[i]);
            /* PRQA S 2982 -- */
            /* PRQA S 4558,4404,4115 -- */
        }
    }
}

/* ----------------------------------------- rcv ----------------------------------------- */

/**
 * @brief Get resource for receive TP message.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
BufReq_ReturnType DoIP_GetResourseForRcvTpMsg(
    PduIdType         soadRxPduId,
    uint16*           soadTxPduRef,
    SoAd_SoConIdType* soConId,
    uint8*            txRxCtxIdx,
    uint8*            tcpConnStatusIdx)
/* PRQA S 1532 -- */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

    /*get rxbuffer and soConId*/
    if ((DoIP_GetSoAdTxPduRefBySoAdRxPduId(soadRxPduId, soadTxPduRef) == E_OK)
        && (E_OK == DoIP_GetTcpTxRxCtxIdxBySoAdRxPduId(soadRxPduId, txRxCtxIdx))
        && (E_OK == SoAd_GetSoConId(*soadTxPduRef, soConId)) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        && (E_OK == DoIP_GetTcpConnStatusIdxBySoAdRxPduId(soadRxPduId, tcpConnStatusIdx)))
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

/**
 * @brief Get resource for receive TP message.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
BufReq_ReturnType DoIP_HandleTpRxHeader(
    const PduInfoType* pduInfoPtr,
    uint16             soadTxPduRef,
    PduLengthType*     bufferSizePtr,
    uint8              txRxCtxIdx)
/* PRQA S 1532 -- */
{
    BufReq_ReturnType ret = BUFREQ_OK;

    /* PRQA S 2814,2934,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr,VL_DoIP_DerefInvalidPtr */
    DoIP_TcpTxRxContextType* ctx    = &DoIP_TcpTxRxContext[txRxCtxIdx];
    DoIP_TcpRxCtrlType*      rxCtrl = &ctx->RxCtrl;

    if (pduInfoPtr->SduLength == 0u)
    {
        *bufferSizePtr      = (PduLengthType)DOIP_HEADER_LEN - rxCtrl->RxBufPos;
        rxCtrl->RxBufRemain = *bufferSizePtr;
    }
    else if (pduInfoPtr->SduLength <= (PduLengthType)DOIP_HEADER_LEN)
    {
        DoIP_MemCpy(&ctx->RxBuf[rxCtrl->RxBufPos], pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
        rxCtrl->RxBufPos += pduInfoPtr->SduLength;
        /* PRQA S 2814,2934,2844 -- */

        /* has received ennough data to evaluate DoIP header */
        if (rxCtrl->RxBufPos == (PduLengthType)DOIP_HEADER_LEN)
        {
            rxCtrl->RxState = DOIP_RX_STATE_RECEIVED_HEADER;

            if (E_OK == DoIP_IsValidHeader(soadTxPduRef, txRxCtxIdx))
            {
                /*this is to aviod if the diagnostic message payload is too large,*/
                /*and in the first copy to pdur,may return with buffer not ok*/
                *bufferSizePtr = DOIP_MIN(
                                     rxCtrl->PayloadLen + (PduLengthType)DOIP_HEADER_LEN,
                                     (PduLengthType)DOIP_MAX_NON_DIAGNOSTIC_DATA_LENGTH)
                                 - rxCtrl->RxBufPos;

                rxCtrl->RxBufRemain = *bufferSizePtr;
            }
            else
            {
                rxCtrl->RxState = DOIP_RX_STATE_DISCARD;
            }
        }
        else
        {
            *bufferSizePtr      = (PduLengthType)DOIP_HEADER_LEN - rxCtrl->RxBufPos;
            rxCtrl->RxBufRemain = *bufferSizePtr;
        }
        /*how about RxBufPos > RxBufPos????*/
    } /*SduLength > DOIP_HEADER_LEN*/
    else
    {
        /*accident happen*/
        ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

/**
 * @brief Handle receive messages continuously.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
BufReq_ReturnType DoIP_HandleTpCFRx(
    DoIP_HandleDiagContextType* handleDiagCtx,
    const PduInfoType*          pduInfoPtr,
    uint16                      soadTxPduRef,
    PduLengthType*              bufferSizePtr)
/* PRQA S 1532 -- */
{
    BufReq_ReturnType ret = BUFREQ_OK;

    /* PRQA S 2814,2934,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr,VL_DoIP_DerefInvalidPtr */
    uint8                    txRxCtxIdx = handleDiagCtx->TxRxCtxIdx;
    DoIP_TcpTxRxContextType* ctx        = &DoIP_TcpTxRxContext[txRxCtxIdx];
    DoIP_TcpRxCtrlType*      rxCtrl     = &ctx->RxCtrl;

    if (pduInfoPtr->SduLength == 0u)
    {
        *bufferSizePtr = rxCtrl->RxBufRemain;
        /* PRQA S 2814,2934,2844 -- */
    }
    else
    {
        switch (rxCtrl->PayloadType)
        {
        case DOIP_MSG_TYPE_DIAG:

            handleDiagCtx->SoadTxPduRef = soadTxPduRef;
            ret                         = DoIP_HandleDiagMsg(pduInfoPtr, bufferSizePtr, handleDiagCtx);
            break;

        case DOIP_MSG_TYPE_ALIVE_CHECK_RSP:
        case DOIP_MSG_TYPE_ROUTING_ACTIVATION_REQ:

            if ((pduInfoPtr->SduLength + rxCtrl->RxBufPos) <= (PduLengthType)DOIP_RX_BUFFER_SIZE)
            {
                /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
                DoIP_MemCpy(&ctx->RxBuf[rxCtrl->RxBufPos], pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
                /* PRQA S 2934 -- */

                rxCtrl->RxBufPos += pduInfoPtr->SduLength;
                rxCtrl->RxBufRemain  = (PduLengthType)DOIP_RX_BUFFER_SIZE - rxCtrl->RxBufPos;
                rxCtrl->RxDataRemain = rxCtrl->PayloadLen - (rxCtrl->RxBufPos - (PduLengthType)DOIP_HEADER_LEN);

                /*Non-Diagnostic message has been completely received*/
                if (rxCtrl->RxDataRemain == 0u)
                {
                    DoIP_HandleTcpNonDiagMsg(soadTxPduRef, txRxCtxIdx, rxCtrl->PayloadType);
                }
                else
                {
                    *bufferSizePtr = rxCtrl->RxBufRemain;
                }
            }
            else
            {
                DoIP_ResetTcpRxContext(txRxCtxIdx);
                ret = BUFREQ_E_NOT_OK;
            }
            break;
        /* PRQA S 2016 ++ */ /* VL_DoIP_2016 */
        default:
            break;
            /* PRQA S 2016 -- */
        }
    }

    return ret;
}

/* ========================================== internal function definitions ========================================= */

/* ----------------------------------------- cfg ----------------------------------------- */

/**
 * @brief Gets the PC configuration for the specified connection.
 */
DOIP_LOCAL boolean DOIP_TCP_PCCON_CFG(uint8 id)
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return DoIP_PCCfgPtr->ConnsCfg->TcpRequestAddressAssignmentArray[id];
    /* PRQA S 2814,2824 -- */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
}

/**
 * @brief Get index of channel.
 */
DOIP_LOCAL Std_ReturnType DoIP_GetChannelIdxBySaTa(uint16 sa, uint16 ta, uint16* channelIdx)
{
    Std_ReturnType               ret = E_NOT_OK;
    const DoIP_PBChannelCfgType* channel;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint16 i = 0u; i < DoIP_PCCfgPtr->ChannelNum; i++)
    {
        channel = &DoIP_PBCfgPtr->ChannelCfgArray[i];
        if ((channel->ChannelSaRef->TesterSA == sa) && (channel->ChannelTa == ta))
        {
            *channelIdx = i;
            /* PRQA S 2814,2824 -- */
            ret = E_OK;

            break;
        }
    }

    return ret;
}

/* ----------------------------------------- pending ----------------------------------------- */

/**
 * @brief Clear all queue.
 */
DOIP_LOCAL_INLINE void DoIP_ResetQueue(DoIP_TcpTxRxContextType* ctx) /* PRQA S 1505 */ /* VL_DoIP_RefeOneFile */
{
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    ctx->TxTpQueue.Tail = 0u;
    ctx->TxTpQueue.Head = 0u;

    ctx->TxIfQueue.Tail = 0u;
    ctx->TxIfQueue.Head = 0u;

    ctx->TxNonDiagQueue.Tail = 0u;
    ctx->TxNonDiagQueue.Head = 0u;
    /* PRQA S 2814 -- */
}

/* ----------------------------------------- rcv ----------------------------------------- */

/**
 * @brief Whether it is a UDP message.
 */
DOIP_LOCAL boolean DoIP_IsUdpMsg(uint16 payloadType)
{
    boolean ret = FALSE;

    switch (payloadType)
    {
    case DOIP_MSG_TYPE_IDENTIFICATION_REQ:
    case DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_EID:
    case DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_VIN:
    case DOIP_MSG_TYPE_ANNOUNCEMENT_OR_IDENTIFICATION_RSP:
    case DOIP_MSG_TYPE_ENTITY_STATUS_REQ:
    case DOIP_MSG_TYPE_ENTITY_STATUS_RSP:
    case DOIP_MSG_TYPE_POWER_MODE_REQ:
    case DOIP_MSG_TYPE_POWER_MODE_RSP:

        ret = TRUE;
        break;

    default: /* PRQA S 2016 */ /* VL_DoIP_2016 */

        break;
    }

    return ret;
}

/**
 * @brief Whether the payloadlen field value of the message is correct.
 */
DOIP_LOCAL boolean
    DoIP_IsValidPayloadLen(uint16 soadTxPduRef, uint8 txRxCtxIdx, uint16 payloadType, PduLengthType payloadLen)
{
    boolean ret = FALSE;

    switch (payloadType)
    {
    case DOIP_MSG_TYPE_ROUTING_ACTIVATION_REQ:

        if ((payloadLen == DOIP_RA_REQ_MSG_PAYLOAD_LEN) || (payloadLen == DOIP_RA_REQ_MSG_PAYLOAD_LEN_WITH_OEM))
        {
            ret = TRUE;
        }

        break;

    case DOIP_MSG_TYPE_ALIVE_CHECK_RSP:

        if (payloadLen == DOIP_ALIVE_CHECK_RSP_MSG_LEN)
        {
            ret = TRUE;
        }

        break;

    case DOIP_MSG_TYPE_DIAG:

        if (payloadLen >= DOIP_MIN_DIAG_MSG_LEN)
        {
            ret = TRUE;
        }

        break;

    /* Invalid type */
    default:

        DoIP_SendTcpNack(soadTxPduRef, DOIP_HEADER_NACK_UNKNOWN_PAYLOAD_TYPE, txRxCtxIdx);

        ret = TRUE; /* Shoud't send DOIP_HEADER_NACK_INVALID_PAYLOAD_LENGTH message. */

        break;
    }

    return ret;
}

/**
 * @brief Check whether the payload of the message is valid.
 */
DOIP_LOCAL Std_ReturnType DoIP_CheckTcpMsgPayload(uint16 soadTxPduRef, uint8 txRxCtxIdx, SoAd_SoConIdType soConId)
{
    uint8          index;
    Std_ReturnType ret = DoIP_GetTcpConnStatusIdxBySoConId(soConId, &index);

    if (E_OK == ret)
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        uint16 payloadType = DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl.PayloadType;
        if (DoIP_IsUdpMsg(payloadType) == TRUE)
        {
            ret = E_NOT_OK;
        }
        else
        {
            PduLengthType payloadLen = DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl.PayloadLen;
            /* PRQA S 2844 -- */ /* VL_DoIP_DerefInvalidPtr */

            if ((payloadType == DOIP_MSG_TYPE_ALIVE_CHECK_RSP)
                && (DoIP_TcpConnStatus[index].RaState == DOIP_SOCKET_NONACTIVATED))
            {
                ret = E_NOT_OK;
            }
            else if (DoIP_IsValidPayloadLen(soadTxPduRef, txRxCtxIdx, payloadType, payloadLen) != TRUE)
            {
                DoIP_SendTcpNack(soadTxPduRef, DOIP_HEADER_NACK_INVALID_PAYLOAD_LENGTH, txRxCtxIdx);
                DoIP_TcpConnStatus[index].WaitCloseSocket = TRUE;
                ret                                       = E_NOT_OK;
            }
            else
            {
                // for qac
            }
        }
    }

    return ret;
}

/**
 * @brief Check whether the header of the message is valid.
 */
DOIP_LOCAL Std_ReturnType DoIP_IsValidHeader(uint16 soadTxPduRef, uint8 txRxCtxIdx)
{
    Std_ReturnType ret = E_OK;

    SoAd_SoConIdType soConId;
    (void)SoAd_GetSoConId(soadTxPduRef, &soConId);

    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
    DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl;
    if (DOIP_GENERICHEADER_CHECK(&DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[0], rxCtrl->RxBufPos) == TRUE)
    {
        rxCtrl->PayloadType =
            DOIP_U8_2_U16(&DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE]);

        rxCtrl->PayloadLen = (PduLengthType)DOIP_U8_2_U32(
            &DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN]);
        /* PRQA S 2814,2844 -- */

        /*The 0x03 error code should have been checked here, but ignored because of the code structure design. */
        if (rxCtrl->PayloadLen > DOIP_MAX_REQUEST_BYTES)
        {
            /*SWS_DoIP_00017*/
            DoIP_SendTcpNack(soadTxPduRef, DOIP_HEADER_NACK_MESSAGE_TO_LARGE, txRxCtxIdx);
            ret = E_NOT_OK;
        }
        else
        {
            ret = DoIP_CheckTcpMsgPayload(soadTxPduRef, txRxCtxIdx, soConId);
        }
    }
    else
    {
        /*SWS_DoIP_00014 Generic DoIP header synchronization pattern Check failed  */
        DoIP_SendTcpNack(soadTxPduRef, DOIP_HEADER_NACK_INCORRECT_PATTERN_FORMAT, txRxCtxIdx);

        uint8 index;
        if (E_OK == DoIP_GetTcpConnStatusIdxBySoConId(soConId, &index))
        {
            DoIP_TcpConnStatus[index].WaitCloseSocket = TRUE;
        }

        ret = E_NOT_OK;
    }

    return ret;
}

/* ----------------------------------------- send ----------------------------------------- */

/**
 * @brief Send TCP NACK message.
 */
DOIP_LOCAL void DoIP_SendTcpNack(PduIdType soadTxPduRef, uint8 nack, uint8 txRxCtxIdx)
{
    uint8 buf[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
    /* PRQA S 2844 -- */

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    /* payload type - Negative Acknowledge*/
    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_GENERIC_DOIP_HEADER_NACK);

    /* payload len*/
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_NACK_MSG_PAYLOAD_LEN);

    /* nack */
    buf[DOIP_NACK_MSG_FIELD_POSITION_CODE] = nack;

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_NACK_MSG_LEN;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
    /* PRQA S 2814 -- */
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        DoIP_MemCpy(DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf, buf, pduInfo.SduLength);
        (void)DoIP_TriggerTpTransmit(soadTxPduRef, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
        /* PRQA S 2844 -- */
    }
}
/* PRQA S 3415 -- */
#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
