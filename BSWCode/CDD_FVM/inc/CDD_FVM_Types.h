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
 **  @file               : CDD_FVM_Types.h
 **  @author             : Jian.Jiang
 **  @date               : 2023/10/20
 **  @vendor             : isoft
 **  @description        : The configuration type of FVM
 **
 ***********************************************************************************************************************/

#ifndef CDD_FVM_TYPES_H
#define CDD_FVM_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "CDD_FVM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief    Define the initialization state of the FVM module.
 */
typedef enum CDD_FVM_StateTag
{
    CDD_FVM_UNINIT, /**<  Initialization state @range 0 */
    CDD_FVM_INIT    /**<  Initialization state @range 1 */
} CDD_FVM_StateType;

/**
 * @brief    Define the mode of the FVM module.
 */
typedef enum CDD_FVM_ModeTag
{
    CDD_FVM_MODE_SINGLE_COUNTER,             /**<  Single counter mode @range 0 */
    CDD_FVM_MODE_SINGLE_TIMESTAMP,           /**<  Single timestamp mode @range 1 */
    CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER, /**<  Truncated multiple counter mode @range 2 */
    CDD_FVM_MODE_COMPLETE_MULTIPLE_COUNTER,  /**<  Complete multiple counter mode @range 3 */
    CDD_FVM_MODE_UNDEFINED,                  /**<  Undefined mode @range 4 */
} CDD_FVM_ModeType;

/**
 * @brief    Define the type of the API.
 */
typedef enum CDD_FVM_PduTag
{
    CDD_FVM_IFPDU, /**<  Interface descriptor descriptor @range 0 */
    CDD_FVM_TPPDU  /**<   Transport protocol descriptor @range 1 */
} CDD_FVM_PduType;

/**
 * @brief    Define the configuration of the freshness value in Tx mode.
 */
typedef struct CDD_FVM_TxFreshnessConfigTag
{
    uint16 FvmFreshnessId; /**<  Indicate the freshness management ID from the SecOCFreshnessValueId configuration item.
                              @range 0 ~ 0xFFFF  */
    CDD_FVM_ModeType mode; /**<  Used to record being in that freshness. @range 0 ~ 3 */
    uint16 freshnessIndex; /**<  Used to record the position in the relevant configuration. @range 0 ~ 0xFFFF */
} CDD_FVM_TxFreshnessConfigType;

/**
 * @brief    Define the configuration of the freshness value in Rx mode.
 */
typedef struct CDD_FVM_RxFreshnessConfigTag
{
    uint16 FvmFreshnessId; /**<  Indicate the freshness management ID from the SecOCFreshnessValueId configuration item.
                              @range 0 ~ 0xFFFF */
    CDD_FVM_ModeType mode; /**<  Used to record being in that freshness. @range 0 ~ 3 */
    uint16 freshnessIndex; /**<  Used to record the position in the relevant configuration. @range 0 ~ 0xFFFF */
} CDD_FVM_RxFreshnessConfigType;

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/**
 * @brief    Define a single counter send configuration.
 */
typedef struct CDD_FVM_TxSingleFreshnessCounterConfigTag
{
    uint8 FvmFreshnessLength; /**<  Define the length of freshness, in bits, that needs to be sent. @range 0 ~ 64 */
} CDD_FVM_TxSingleFreshnessCounterConfigType;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/**
 * @brief    Define a single counter receive configuration.
 */
typedef struct CDD_FVM_RxSingleFreshnessCounterConfigTag
{
    uint8 FvmFreshnessLength; /**<  Define the freshness length of the actual freshness value, in bits. @range 0 ~ 64 */
    uint16 MaxDeltaCounter;   /**<  Delta counter. @range 0 ~ 0xFFFF */
} CDD_FVM_RxSingleFreshnessCounterConfigType;
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u) || (CDD_FVM_MASTER_ECU_ENABLED == STD_ON)
/**
 * @brief    Define the PDU corresponding to the synchronization message.
 */
typedef struct CDD_FVM_SyncMsgCtrlPudTag
{
    PduIdType FvMIfTxPduHandleId;   /**<  Indicate the PDU identifier assigned by FVM module. Used by PduR for
                                       PduR_CDD_FVMTransmit. @range 0 ~ 0xFFFF */
    PduLengthType FvMIfTxPduLength; /**<  Indicate the Length of the PDU identifier assigned by FVM module, Used by PduR
                                       for PduR_CDD_FVMTransmit. @range 0 ~ 0xFFFF */
    PduIdType FvMIfRxPduHandleId;   /**<  Indicate the PDU identifier assigned by CDD_FVM module. Used by PduR for
                                       CDD_FVM_IfRxIndication. @range 0 ~ 0xFFFF */
    PduLengthType FvMIfRxPduLength; /**<  Indicate the PDU identifier assigned by CDD_FVM module. Used by PduR for
                                       CDD_FVM_IfRxIndication. @range 0 ~ 0xFFFF */
} CDD_FVM_SyncMsgCtrlPudType;
#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON || \
CDD_FVM_MASTER_ECU_ENABLED == STD_ON */

#if (CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON)
/**
 * @brief    Define the configuration of the freshness counter.
 */
typedef struct CDD_FVM_FreshnessCounterConfigTag
{
    uint8 CounterPos; /**<  Position of the Counter. @range 0 ~ 64 */
    uint8 CounterLen; /**<  Length of the Counter. @range 0 ~ 64 */
} CDD_FVM_FreshnessCounterConfigType;

/**
 * @brief    Define the structure of the freshness value in the multiple freshness value mode.
 */
typedef struct CDD_FVM_MultipleFreshnessValueStructureTag
{
    CDD_FVM_FreshnessCounterConfigType tirpCnt;     /**<  Indicate the trip counter. @range NA */
    CDD_FVM_FreshnessCounterConfigType resetCnt;    /**<  Indicate the reset counter. @range NA */
    CDD_FVM_FreshnessCounterConfigType msgCntUpper; /**<  Indicate the higher bits of the message counter. @range NA
                                                     */
    CDD_FVM_FreshnessCounterConfigType msgCntLower; /**<  Indicate the lower bits of the message counter. @range NA */
    CDD_FVM_FreshnessCounterConfigType resetFlag;   /**<  Indicate the lower bits of the reset counter. @range NA */
} CDD_FVM_MultipleFreshnessValueStructureType;

#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
/**
 * @brief    Define the configuration of the PDU corresponding to the synchronization message from the slave ECU.
 */
typedef struct CDD_FVM_SlaveSyncMsgPduTag
{
    CDD_FVM_PduType pduType;         /**<  Indicate the API type to use for communication with PduR. @range 0 ~ 1 */
    PduIdType       RxPduHandleId;   /**<  Indicate the PDU identifier assigned by FVM module. Used by PduR for
                                        CDD_FVM__[If|Tp]RxIndication. @range 0 ~ 0xFFFF */
    PduLengthType RxPduBufferLength; /**<  Indicate received sync message Frame Length. @range 0 ~ 0xFFFF */
} CDD_FVM_SlaveSyncMsgPduType;

/**
 * @brief    Define slave ECU configurations related to synchronization.
 */
typedef struct CDD_FVM_SlaveECUSyncConfigTag
{
    uint16 FvMSyncMsgId;      /**<  Message ID associated with this message. @range 0 ~ 0xFFFF */
    uint8  FvMTripCntLength;  /**<  Indicate the length of trip counter, in bits. @range 0 ~ 64 */
    uint8  FvMResetCntLength; /**<  Indicate the length of reset counter, in bits. @range 0 ~ 64 */
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
    const CDD_FVM_SyncMsgCtrlPudType*
        FvMCtrlPduRef; /**<  Indicate the configuration of the PDU for synchronization messages. @range NA */
#endif                 /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
    const CDD_FVM_SlaveSyncMsgPduType*
        FvMSyncMsgRef; /**<  Indicate the configuration for synchronization messages. @range NA
                        */
#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)
    /**<  Master index associated with this message, Generate 0XFFFF when it doesn't exist, and the index of the
     * corresponding master when it does exist. @range 0 ~ 0xFFFF */
    uint16 FvMCommonMasterIndex;
#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */
    /**<  Permissible range for a counter initialization when the trip counter reaches the maximum value.
     * Under the erroneous situation such as miss-synchronous counter between FV master and slave around upper limit of
     * trip counter, this window parameter would work effectively to recover the situation as a robustness. @range 0 ~
     * 0xFFFF
     */
    uint16 FvMClearAcceptanceWindow;
#if (CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0U)
    const NvM_BlockIdType*
        blkIdRef; /**<  The NvM block descriptor reference, is used to load and store the trip counter. @range NA */
#endif            /* CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0U */
} CDD_FVM_SlaveECUSyncConfigType;
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

#if (CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON)
#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/**
 * @brief    Define the configuration in Tx Multiple Freshness Truncated Counter mode.
 */
typedef struct CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigTag
{
    /**<  Indicate the ID of the synchronised message frame, the multicounter freshness value
     * for truncation to get reset counter and trip counter information. @range 0 ~ 0xFFFF */
    uint16 syncMsgIndex;
    uint8 FvmFreshnessLength; /**<  Define the freshness length of the actual freshness value, in bits. @range 0 ~ 64 */
    const CDD_FVM_MultipleFreshnessValueStructureType*
        freshnessConfigRef; /**<  Point to a structure describing the freshness value. @range NA  */
} CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/**
 * @brief    Define the configuration in Rx Multiple Freshness Truncated Counter mode.
 */
typedef struct CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigTag
{
    /**<  The ID of the synchronised message frame, the multicounter freshness value
     * for truncation to get reset counter and trip counter information. @range 0 ~ 0xFFFF */
    uint16 syncMsgIndex;
    uint8 FvmFreshnessLength; /**<  Define the freshness length of the actual freshness value, in bits. @range 0 ~ 64 */
    const CDD_FVM_MultipleFreshnessValueStructureType*
        freshnessConfigRef; /**<  Point to a structure describing the freshness value. @range NA  */
} CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType;
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON */

#if (CDD_FVM_MASTER_ECU_ENABLED == STD_ON)
/**
 * @brief    Define the configuration of the PDU corresponding to the synchronization message from the master ECU.
 */
typedef struct CDD_FVM_MasterSyncMsgPduTag
{
    CDD_FVM_PduType pduType;       /**<  Indicate the API type to use for communication with PduR. @range 0 ~ 1 */
    PduIdType       TxPduHandleId; /**<  Indicate the PDU identifier assigned by FVM module. Used by PduR for
                                      PduR_CDD_FVMTransmit. @range 0 ~ 0xFFFF  */
    PduLengthType
        TxPduBufferLength; /**<  Indicate the length of the outgoing synchronization message frame. @range 0 ~ 0xFFFF */
} CDD_FVM_MasterSyncMsgPduType;

/**
 * @brief    Define master ECU configurations related to synchronization.
 */
typedef struct CDD_FVM_MasterConfigTag
{
    uint16 FvMSyncMsgId;      /**<  Message ID associated with this message. @range 0 ~ 0xFFFF */
    uint8  FvMTripCntLength;  /**<  Indicate the length of trip counter, in bits. @range 0 ~ 64 */
    uint8  FvMResetCntLength; /**<  Indicate the length of reset counter, in bits. @range 0 ~ 64 */
    uint16 FvMResetCycle;     /**<  Indicate reset counter increment cycle. @range 0 ~ 0xFFFF */
    const CDD_FVM_MasterSyncMsgPduType*
        fvmSyncMsgPduRef; /**<  Indicate the configuration of the PDU for synchronization messages. @range NA */
#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
    const CDD_FVM_SyncMsgCtrlPudType*
        fvmCtrlPduRef; /**<  Indicate the configuration for synchronization messages. @range NA */
#endif                 /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
#if (CDD_FVM_NVM_BLOCK_ENABLED == STD_ON)
    NvM_BlockIdType
        blkId; /**<  Indicate identifier of NvM block, is used to load and store the trip counter. @range NA */
#endif         /* CDD_FVM_NVM_BLOCK_ENABLED == STD_ON */
} CDD_FVM_MasterConfigType;
#endif /* CDD_FVM_MASTER_ECU_ENABLE == STD_ON */

/**
 * @brief    Define configuration data structure of FVM module.
 */
typedef struct CDD_FVM_ConfigTag
{
#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
    const CDD_FVM_MasterConfigType* FvmMasterEcuConfig; /**<  Point to the structure of the master ECU configurations
                                                           related to synchronization. @range NA */
#endif                                                  /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
    const CDD_FVM_SlaveECUSyncConfigType* FvmSlaveEcuConfig; /**<  Point to the structure of the slave ECU
                                                                configurations related to synchronization. @range NA */
#endif                                                       /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

    const CDD_FVM_TxFreshnessConfigType* FvmTxFreshnessConfig; /**<  Point to the structure of the freshness value
                                                                  configurations in Tx mode. @range NA */

    const CDD_FVM_RxFreshnessConfigType* FvmRxFreshnessConfig; /**<  Point to the structure of the freshness value
                                                                  configurations in Rx mode. @range NA */
#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
    const CDD_FVM_TxSingleFreshnessCounterConfigType*
        FvmTxSingleFreshnessConfig; /**<  Point to the structure of the single counter send configuration. @range NA */
#endif                              /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */
#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
    const CDD_FVM_RxSingleFreshnessCounterConfigType*
        FvmRxSingleFreshnessConfig; /**<  Point to the structure of the single counter receive configuration. @range NA
                                     */
#endif                              /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
    /**<  Point to the structure of the configuration in Tx Multiple Freshness Truncated Counter mode. @range NA */
    const CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType* FvmTxMultipleFreshnessTruncateConfig;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
    /**<  Point to the structure of the configuration in Rx Multiple Freshness Truncated Counter mode. @range NA */
    const CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType* FvmRxMultipleFreshnessTruncateConfig;
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

} CDD_FVM_ConfigType;

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#ifdef __cplusplus
}
#endif
#endif /* CDD_FVM_TYPES_H */
