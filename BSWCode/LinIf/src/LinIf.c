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
 **  @file        : LinIf.c
 **  @author      : HuRongbo
 **  @date        :
 **  @vendor      : isoft
 **  @description : API definitions of LinIf
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#include "LinIf_Cbk.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#endif
#if (STD_ON == LINIF_MASTER_SUPPORT)
#include "LinIf_Master.h"
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
#include "LinTp_Master.h"
#endif
#if ((LINIF_TP_SUPPORTED == STD_ON) && (STD_ON == LINTP_SLAVE_SUPPORT))
#include "LinTp_Slave.h"
#endif
#endif
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinIf_Slave.h"
#endif
#include "LinIf_PartitionVar.h"
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
#include "ComM.h"
#endif
#include "LinIf.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
LINIF_LOCAL_INLINE boolean LinIf_ValidateInitStatus(uint8 appId, ApplicationType applicationId);
LINIF_LOCAL_INLINE boolean LinIf_ValidateChannelNum(uint8 appId, uint8 ch);
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
LINIF_LOCAL_INLINE boolean LinIf_ValidateChannelPartition(uint8 appId, ApplicationType applicationId, uint8 ch);
LINIF_LOCAL_INLINE boolean
    LinIf_ValidateTxPduPartition(uint8 appId, ApplicationType applicationId, PduIdType LinTxPduId);
LINIF_LOCAL Std_ReturnType LinIf_CheckTxPduPatition(PduIdType TxPduId, ApplicationType applicationId);
#endif
LINIF_LOCAL_INLINE boolean LinIf_ValidateConfigNad(uint8 appId, uint8 Nad);
LINIF_LOCAL_INLINE boolean LinIf_ValidateLinTxPduId(uint8 appId, PduIdType LinTxPduId);
#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
LINIF_LOCAL_INLINE boolean LinIf_ValidateTrcvMode(uint8 appId, LinTrcv_TrcvModeType TransceiverMode);
LINIF_LOCAL_INLINE boolean LinIf_ValidateLinTrcvWakeupMode(uint8 appId, LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode);
LINIF_LOCAL_INLINE boolean LinIf_ValidateTrcvModeNormal(uint8 appId, uint8 ch);
#endif
LINIF_LOCAL_INLINE boolean LinIf_ValidatePidBufferLength(uint8 appId, uint8 PidBufferLength, uint8 ch);
LINIF_LOCAL_INLINE boolean LinIf_ValidateGetPidBufferLength(uint8 appId, uint8 PidBufferLength, uint8 ch);
LINIF_LOCAL_INLINE boolean LinIf_ValidatePointer(uint8 appId, const void* pointer);
#endif

LINIF_LOCAL const LinIf_SubstitutionFramesType* LinIf_GetSubstitution(PduIdType TxPduId);

LINIF_LOCAL NetworkHandleType LinIf_GetLinIfChannel(NetworkHandleType channel, ApplicationType applicationId);

#if (STD_ON == LINIF_MASTER_SUPPORT)
/**
 * @brief           The main processing function of the LinIf master node. This function process
                    master node wakeup and sleep, message transmit and update schedule table.
 * @param[in]       channel: Identification of the LinIf channel
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINIF_LOCAL void LinIf_MasterMainHandle(NetworkHandleType ch);
#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)
/**
 * @brief           LinIf slave main function
 * @param[in]       channel: Identification of the LinIf channel
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINIF_LOCAL void LinIf_SlaveMainHandle(NetworkHandleType ch);

LINIF_LOCAL NetworkHandleType
    LinIf_GetLinIfChannelByDriverChId(NetworkHandleType channel, ApplicationType applicationId);
#endif

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* ============================================ internal data definition ============================================ */
#define LINIF_START_SEC_VAR_INIT_PTR
#include "LinIf_MemMap.h"
/**
 * @brief Global configuration pointer of LINIF
 */
const LinIf_ConfigType*        LinIf_ConfigPtr = NULL_PTR;
static const LinIf_PbsMapType* LinIf_PbsMapPtr = NULL_PTR;
#define LINIF_STOP_SEC_VAR_INIT_PTR
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2889 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 1503,1532 ++ */      /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
/* ========================================== external function definition ========================================== */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Initialize LinIf module configuration, LinIf master node runtime variables,
 * LinIf slave node runtime variables
 */
void LinIf_Init(const LinIf_ConfigType* ConfigPtr)
{
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidatePointer(LINIF_INIT_ID, ConfigPtr))
#endif
    {
        /*@req <SWS_LinIf_00371>,<SWS_LinIf_00373>*/
        if (NULL_PTR == LinIf_ConfigPtr)
        {
            LinIf_ConfigPtr = ConfigPtr;
        }
        if (NULL_PTR == LinIf_PbsMapPtr)
        {
            LinIf_PbsMapPtr = LinIf_ConfigPtr->PbsMap;
        }
        ApplicationType applicationId = LinIf_GetApplicationID();
#if (STD_ON == LINIF_MASTER_SUPPORT)
        LinIf_MasterInit(applicationId);
#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)
        LinIf_SlaveInit(applicationId);
#endif

        /*@req <SWS_LinIf_00381>*/
        /* Set the status of LINIF */
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        if (LinIf_Status[applicationId] != NULL_PTR)
        {
            *LinIf_Status[applicationId] = LINIF_INIT;
        }
#else
        *LinIf_Status[applicationId] = LINIF_INIT;
#endif
    }
}

/**
 * Requests transmission of a PDU.
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_LinIf_STMIF,VL_MTR_LinIf_STCAL*/
Std_ReturnType LinIf_Transmit(PduIdType LinTxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6030,6070 -- */
{
    Std_ReturnType ret = E_NOT_OK;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    ApplicationType applicationId = LinIf_GetApplicationID();

    if (LinIf_ValidateInitStatus(LINIF_TRANSMIT_ID, applicationId)
        && LinIf_ValidatePointer(LINIF_TRANSMIT_ID, PduInfoPtr)
        && LinIf_ValidateLinTxPduId(LINIF_TRANSMIT_ID, LinTxPduId)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateTxPduPartition(LINIF_TRANSMIT_ID, applicationId, LinTxPduId)
#endif
    )
#endif
    {
        if (LinTxPduId < LinIf_ConfigPtr->PbsMap->TxPduMapLen)
        {
            PduIdType txPduIdReal                      = (PduIdType)LinIf_ConfigPtr->PbsMap->TxPduMapList[LinTxPduId];
            const LinIf_SubstitutionFramesType* subPtr = LinIf_GetSubstitution(txPduIdReal);

            if (NULL_PTR == subPtr)
            {
                /*@req <SWS_LinIf_00700>*/
                /*
                 * The Pdu belongs to a non-sporadic frame or non-event-triggered
                 * frame,shall return E_OK.
                 */
                ret = E_OK;
            }
            else
            {
#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON || LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
                uint16 frameIdx = subPtr->SubstitutionFrameRef;

                for (uint8 ch = 0u; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
                {
                    if ((frameIdx >= LinIf_GetFrameOfs(ch))
                        && (frameIdx < (LinIf_GetFrameNum(ch) + LinIf_GetFrameOfs(ch))))
                    {
#if (LINIF_MASTER_SPORADIC_FRAME_SUPPORT == STD_ON)
                        if (LINIF_MASTER == LinIf_GetNodeType(ch))
                        {
#if (STD_ON == LINIF_MASTER_SUPPORT)
                            ret = LinIf_MasterTransmit(ch, frameIdx);
#endif
                        }
                        else
#endif
                        {
#if (STD_ON == LINIF_SLAVE_SUPPORT && STD_ON == LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT)
                            ret = LinIf_SlaveTransmit(frameIdx);
#endif
                        }

                        break;
                    }
                    else
                    {
                        /* Only for MISRA-C */
                    }
                }
#endif
            }
        }
    }
    LINIF_NOUSED(PduInfoPtr);

    return ret;
}

/**
 * Requests a schedule table to be executed.Only used for LIN master nodes.
 */
#if (STD_ON == LINIF_MASTER_SUPPORT)
/* PRQA S 3334 ++ */ /* VL_LinIf_3334 */
Std_ReturnType LinIf_ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType Schedule)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_SCHEDULEREQUEST_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_SCHEDULEREQUEST_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_SCHEDULEREQUEST_ID, applicationId, ch)
#endif
    )
#endif
    {
        if (Schedule < LinIf_PbsMapPtr->ScheduleTableMapLen)
        {
            LinIf_SchHandleType actualSchedule = (LinIf_SchHandleType)LinIf_PbsMapPtr->ScheduleTableMapList[Schedule];
            ret                                = LinIf_MasterScheduleRequest(ch, actualSchedule);
        }
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        else
        {
            /*@req <SWS_LinIf_00567>*/
            LinIf_DetReportError(LINIF_SCHEDULEREQUEST_ID, LINIF_E_SCHEDULE_REQUEST_ERROR);
        }
#endif
    }

    return ret;
}
/* PRQA S 3334 -- */
#endif

/**
 * Initiates a transition into the Sleep Mode on the selected channel.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
Std_ReturnType LinIf_GotoSleep(NetworkHandleType Channel)
/* PRQA S 6070 -- */
{
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);
    Std_ReturnType    ret           = E_NOT_OK;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_GOTOSLEEP_ID, applicationId) && LinIf_ValidateChannelNum(LINIF_GOTOSLEEP_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_GOTOSLEEP_ID, applicationId, ch)
#endif
    )
#endif
    {
        if (LINIF_MASTER == LinIf_GetNodeType(ch))
        {
#if (STD_ON == LINIF_MASTER_SUPPORT)
            LinIf_MasterGotoSleep(ch);
#endif
        }
        else
        {
#if (STD_ON == LINIF_SLAVE_SUPPORT)
            LinIf_SlaveGotoSleep(ch);
#endif
        }

        ret = E_OK;
    }

    return ret;
}

/**
 * Initiates the wake up process.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
Std_ReturnType LinIf_Wakeup(NetworkHandleType Channel)
/* PRQA S 6070 -- */
{
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);
    Std_ReturnType    ret           = E_NOT_OK;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_WAKEUP_ID, applicationId) && LinIf_ValidateChannelNum(LINIF_WAKEUP_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_WAKEUP_ID, applicationId, ch)
#endif
    )
#endif
    {
        if (LINIF_MASTER == LinIf_GetNodeType(ch))
        {
#if (STD_ON == LINIF_MASTER_SUPPORT)
            ret = LinIf_MasterWakeUp(ch);
#endif
        }
        else
        {
#if (STD_ON == LINIF_SLAVE_SUPPORT)
            ret = LinIf_SlaveWakeUp(ch);
#endif
        }
    }

    return ret;
}

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
/**
 * Set the given LIN transceiver to the given mode.
 */
Std_ReturnType LinIf_SetTrcvMode(NetworkHandleType Channel, LinTrcv_TrcvModeType TransceiverMode)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_SETTRCVMODE_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_SETTRCVMODE_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_SETTRCVMODE_ID, applicationId, ch)
#endif
        && LinIf_ValidateTrcvMode(LINIF_SETTRCVMODE_ID, TransceiverMode))
#endif
    {
        /*@req <SWS_LinIf_00536>,<SWS_LinIf_00537>,<SWS_LinIf_00538>*/
        uint8 linChannel = LinIf_GetLinChannelId(ch);
        ret              = LinTrcv_SetOpMode(linChannel, TransceiverMode);
    }

    return ret;
}

/**
 * Returns the actual state of a LIN Transceiver Driver.
 */
Std_ReturnType LinIf_GetTrcvMode(NetworkHandleType Channel, LinTrcv_TrcvModeType* TransceiverModePtr)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_GETTRCVMODE_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_GETTRCVMODE_ID, ch)
        && LinIf_ValidatePointer(LINIF_GETTRCVMODE_ID, TransceiverModePtr)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_GETTRCVMODE_ID, applicationId, ch)
#endif
    )
#endif
    {
        /*@req <SWS_LinIf_00541>*/
        uint8 linChannel = LinIf_GetLinChannelId(ch);
        ret              = LinTrcv_GetOpMode(linChannel, TransceiverModePtr);
    }

    return ret;
}

/**
 * Returns the reason for the wake up that has been detected by the LIN Transceiver Driver.
 */
Std_ReturnType LinIf_GetTrcvWakeupReason(NetworkHandleType Channel, LinTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr)
{
    Std_ReturnType       ret           = E_NOT_OK;
    ApplicationType      applicationId = LinIf_GetApplicationID();
    NetworkHandleType    ch            = LinIf_GetLinIfChannel(Channel, applicationId);
    uint8                linChannel;
    LinTrcv_TrcvModeType transceiverMode = LINTRCV_TRCV_MODE_SLEEP;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_GETTRCVWAKEUPREASON_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_GETTRCVWAKEUPREASON_ID, ch)
        && LinIf_ValidatePointer(LINIF_GETTRCVWAKEUPREASON_ID, TrcvWuReasonPtr)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_GETTRCVWAKEUPREASON_ID, applicationId, ch)
#endif
        && LinIf_ValidateTrcvModeNormal(LINIF_GETTRCVWAKEUPREASON_ID, ch))
#endif
    {
        /*@req <SWS_LinIf_00548>*/
        linChannel = LinIf_GetLinChannelId(ch);
        ret        = LinTrcv_GetBusWuReason(linChannel, TrcvWuReasonPtr);
    }

    return ret;
}

/**
 * This API enables, disables and clears the notification for wakeup events on the addressed
 * network
 */
Std_ReturnType LinIf_SetTrcvWakeupMode(NetworkHandleType Channel, LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_SETTRCVWAKEUPREASON_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_SETTRCVWAKEUPREASON_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_SETTRCVWAKEUPREASON_ID, applicationId, ch)
#endif
        && LinIf_ValidateLinTrcvWakeupMode(LINIF_SETTRCVWAKEUPREASON_ID, LinTrcvWakeupMode))
#endif
    {
        /*@req <SWS_LinIf_00551>*/
        uint8 linChannel = LinIf_GetLinChannelId(ch);
        ret              = LinTrcv_SetWakeupMode(linChannel, LinTrcvWakeupMode);
    }

    return ret;
}
#endif

/**
 * The main processing function of the LIN Interface.
 */
void LinIf_ChannelMainFunction(NetworkHandleType ch)
{
    ApplicationType applicationId = LinIf_GetApplicationID();
    if ((LINIF_INIT == *LinIf_Status[applicationId])
#if (LINIF_TP_SUPPORTED == STD_ON)
        && (LINTP_INIT == *LinTp_Status[applicationId])
#endif
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && (LinIf_GetChannelApplicationId(ch) == applicationId)
#endif
    )
    {
        if (LINIF_MASTER == LinIf_GetNodeType(ch))
        {
#if (STD_ON == LINIF_MASTER_SUPPORT)
            LinIf_MasterMainHandle(ch);
#endif
        }
        else
        {
#if (STD_ON == LINIF_SLAVE_SUPPORT)
            LinIf_SlaveMainHandle(ch);
#endif
        }
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_WAKEUP_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Will be called when the EcuM has been notified about a wakeup on a specific LIN channel.
 */
Std_ReturnType LinIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType ret = E_NOT_OK;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    boolean         customWakeupSource = FALSE;
    boolean         detNoErr           = TRUE;
    ApplicationType applicationId      = LinIf_GetApplicationID();
    if (LinIf_ValidateInitStatus(LINIF_CHECKWAKEUP_ID, applicationId))
    {
        for (uint8 ch = 0u; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
        {
            if (WakeupSource == LinIf_GetLinDriverWakeupSource(ch))
            {
                customWakeupSource = TRUE;
                break;
            }
        }
    }
    if ((!customWakeupSource) && (ECUM_WKSOURCE_POWER != WakeupSource) && (ECUM_WKSOURCE_RESET != WakeupSource)
        && (ECUM_WKSOURCE_INTERNAL_RESET != WakeupSource) && (ECUM_WKSOURCE_INTERNAL_WDG != WakeupSource)
        && (ECUM_WKSOURCE_EXTERNAL_WDG != WakeupSource))
    {
        /*@req <SWS_LinIf_00566>*/
        LinIf_DetReportError(LINIF_CHECKWAKEUP_ID, LINIF_E_PARAM_WAKEUPSOURCE);
        detNoErr = FALSE;
    }
    if (detNoErr)
#endif
    {
        EcuM_WakeupSourceType wakeupSource;

#if (LINIF_WAKEUP_SUPPORT == STD_ON)
        /*check all Lin driver channel to find the wake-up source*/
        for (uint8 ch = 0u; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
        {
#if (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON)
            const LinIf_ChannelType* chCfgPtr = LinIf_GetChannel(ch);
            if (NULL_PTR != chCfgPtr->TransceiverDrvConfig)
            {
                wakeupSource = LinIf_GetLinTrcvWakeupSource(ch);
                if (0u != (WakeupSource & wakeupSource))
                {
                    /*@req <SWS_LinIf_00503>*/
                    ret = LinTrcv_CheckWakeup(ch);
                    break;
                }
            }
            else
#endif
            {
                wakeupSource = LinIf_GetLinDriverWakeupSource(ch);
                if (0u != (WakeupSource & wakeupSource))
                {
                    /*@req <SWS_LinIf_00503>*/
                    uint8 linDriver  = LinIf_GetLinDriverId(ch);
                    uint8 linChannel = LinIf_GetLinChannelId(ch);
                    ret              = Lin_DriverApi[linDriver].LinCheckWakeup(linChannel);
                    break;
                }
            }
        }
#endif
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (STD_ON == LINIF_VERSION_INFO_API)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

void LinIf_GetVersionInfo(Std_VersionInfoType* versionInfo)
{
#if (STD_ON == LINIF_DEV_ERROR_DETECT)
    if (NULL_PTR == versionInfo)
    {
        Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID, LINIF_GETVERSIONINFO_ID, LINIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        versionInfo->vendorID         = LINIF_VENDOR_ID;
        versionInfo->moduleID         = LINIF_MODULE_ID;
        versionInfo->sw_major_version = LINIF_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = LINIF_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = LINIF_SW_PATCH_VERSION;
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CALLOUT_CODE
#include "LinIf_MemMap.h"

/**
 * The LIN Driver or LIN Transceiver Driver will call this function to report the wake up source after
 * the successful wakeup detection during CheckWakeup or after power on by bus.
 */
void LinIf_WakeupConfirmation(EcuM_WakeupSourceType WakeupSource)
{
    ApplicationType applicationId = LinIf_GetApplicationID();
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    boolean detNoErr           = TRUE;
    boolean customWakeupSource = FALSE;
    if (LinIf_ValidateInitStatus(LINIF_WAKEUPCONFIRMATION_ID, applicationId))
    {
        for (uint8 ch = 0u; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
        {
            if (WakeupSource == LinIf_GetLinDriverWakeupSource(ch))
            {
                customWakeupSource = TRUE;
                break;
            }
        }
    }
    if ((!customWakeupSource) && (ECUM_WKSOURCE_POWER != WakeupSource) && (ECUM_WKSOURCE_RESET != WakeupSource)
        && (ECUM_WKSOURCE_INTERNAL_RESET != WakeupSource) && (ECUM_WKSOURCE_INTERNAL_WDG != WakeupSource)
        && (ECUM_WKSOURCE_EXTERNAL_WDG != WakeupSource))
    {
        /*@req <SWS_LinIf_00717>*/
        LinIf_DetReportError(LINIF_WAKEUPCONFIRMATION_ID, LINIF_E_PARAM_WAKEUPSOURCE);
        detNoErr = FALSE;
    }
    if (detNoErr)
#endif
    {
#if (STD_ON == LINIF_MASTER_SUPPORT)
        LinIf_MasterWakeupConfirmation(WakeupSource, applicationId);
#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)
        LinIf_SlaveWakeupConfirmation(WakeupSource, applicationId);
#endif
    }
}
#define LINIF_STOP_SEC_CALLOUT_CODE
#include "LinIf_MemMap.h"
#endif

#if (STD_ON == LINIF_SLAVE_SUPPORT)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Retrieves all assigned PID values. The order is congruent to the LIN frame index.
 * Only applicable for LIN slave nodes.
 */
Std_ReturnType LinIf_GetPIDTable(NetworkHandleType Channel, Lin_FramePidType* PidBuffer, uint8* PidBufferLength)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_GETPIDTABLE_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_GETPIDTABLE_ID, ch) && LinIf_ValidatePointer(LINIF_GETPIDTABLE_ID, PidBuffer)
        && LinIf_ValidatePointer(LINIF_GETPIDTABLE_ID, PidBufferLength)
        && LinIf_ValidateGetPidBufferLength(LINIF_GETPIDTABLE_ID, *PidBufferLength, ch))
#endif
    {
        LinIf_SlaveGetPIDTable(ch, PidBuffer, PidBufferLength);
        ret = E_OK;
    }

    return ret;
}

/**
 * Sets all assigned PID values. The order is congruent to the LIN frame index.
 * Only applicable for LIN slave nodes.
 */
/* PRQA S 3673,6070 ++ */ /* VL_QAC_3673,VL_MTR_LinIf_STCAL*/
Std_ReturnType LinIf_SetPIDTable(NetworkHandleType Channel, Lin_FramePidType* PidBuffer, uint8 PidBufferLength)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_SETPIDTABLE_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_SETPIDTABLE_ID, ch) && LinIf_ValidatePointer(LINIF_SETPIDTABLE_ID, PidBuffer)
        && LinIf_ValidatePidBufferLength(LINIF_SETPIDTABLE_ID, PidBufferLength, ch))
#endif
    {
        /* Locked */
        SchM_Enter_LinIf_ExclusiveArea_Channel();

        /*@req <SWS_LinIf_00823>*/
        LinIf_SlaveSetPIDTable(ch, PidBuffer);

        /* Unlocked */
        SchM_Exit_LinIf_ExclusiveArea_Channel();
        ret = E_OK;
    }
    LINIF_NOUSED(PidBufferLength);

    return ret;
}
/* PRQA S 3673,6070 -- */

/**
 * Reports the current configured NAD.Only applicable for LIN slave nodes.
 */
Std_ReturnType LinIf_GetConfiguredNAD(NetworkHandleType Channel, uint8* Nad)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_GETCONFIGUREDNAD_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_GETCONFIGUREDNAD_ID, ch)
        && LinIf_ValidatePointer(LINIF_GETCONFIGUREDNAD_ID, Nad))
#endif
    {
        /*@req <SWS_LinIf_00830>*/
        (void)LinIf_SlaveGetConfiguredNAD(ch, Nad);

        ret = E_OK;
    }

    return ret;
}

/**
 * Sets the current configured NAD.Only applicable for LIN slave nodes.
 */
Std_ReturnType LinIf_SetConfiguredNAD(NetworkHandleType Channel, uint8 Nad)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannel(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_SETCONFIGUREDNAD_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_SETCONFIGUREDNAD_ID, ch)
        && LinIf_ValidateConfigNad(LINIF_SETCONFIGUREDNAD_ID, Nad))
#endif
    {
        /*@req <SWS_LinIf_00834>*/
        LinIf_SlaveSetConfiguredNAD(ch, Nad);
        ret = E_OK;
    }

    return ret;
}

/* PRQA S 1503,1532 -- */

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CALLOUT_CODE
#include "LinIf_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */

/**
 * The LIN Driver will call this function to report a received LIN header. This function is only
 * applicable for LIN slave nodes (available only if the ECU has any LIN slave channel).
 */
Std_ReturnType LinIf_HeaderIndication(NetworkHandleType Channel, Lin_PduType* PduPtr)
{
    Std_ReturnType    ret           = E_NOT_OK;
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannelByDriverChId(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_HEADERINDICATION_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_HEADERINDICATION_ID, ch)
        && LinIf_ValidatePointer(LINIF_HEADERINDICATION_ID, PduPtr)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_HEADERINDICATION_ID, applicationId, ch)
#endif
    )
#endif
    {
        ret = LinIf_SlaveHeaderIndication(ch, PduPtr);
    }

    return ret;
}

/**
 * The LIN Driver will call this function to report a successfully received response and provides the
 * reception data to the LIN Interface. This function is only applicable for LIN slave nodes
 * (available only if the ECU has any LIN slave channel).
 */
void LinIf_RxIndication(NetworkHandleType Channel, uint8* Lin_SduPtr)
{
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannelByDriverChId(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_RXINDICATION_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_RXINDICATION_ID, ch)
        && LinIf_ValidatePointer(LINIF_RXINDICATION_ID, Lin_SduPtr)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_RXINDICATION_ID, applicationId, ch)
#endif
    )
#endif
    {
        LinIf_SlaveRxIndication(ch, Lin_SduPtr);
    }
}

/**
 * The LIN Driver will call this function to report a successfully transmitted response. This function
 * is only applicable for LIN slave nodes (available only if the ECU has any LIN slave channel).
 */
void LinIf_TxConfirmation(NetworkHandleType Channel)
{
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannelByDriverChId(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if (LinIf_ValidateInitStatus(LINIF_TXCONFIRMATION_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_TXCONFIRMATION_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_TXCONFIRMATION_ID, applicationId, ch)
#endif
    )
#endif
    {
        LinIf_SlaveTxConfirmation(ch);
    }
}

/**
 * The LIN Driver will call this function to report a detected error event during header or response
 * processing. This function is only applicable for LIN slave nodes (available only if the ECU has
 * any LIN slave channel).
 */
void LinIf_LinErrorIndication(NetworkHandleType Channel, Lin_SlaveErrorType ErrorStatus)
{
    ApplicationType   applicationId = LinIf_GetApplicationID();
    NetworkHandleType ch            = LinIf_GetLinIfChannelByDriverChId(Channel, applicationId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)

    if (LinIf_ValidateInitStatus(LINIF_LINERRORINDICATION_ID, applicationId)
        && LinIf_ValidateChannelNum(LINIF_LINERRORINDICATION_ID, ch)
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        && LinIf_ValidateChannelPartition(LINIF_LINERRORINDICATION_ID, applicationId, ch)
#endif
    )
#endif
    {
        LinIf_SlaveLinErrorIndication(ch, ErrorStatus);
    }
}
/* PRQA S 1503 -- */
#define LINIF_STOP_SEC_CALLOUT_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Get LinIf channel id by Lin Driver channel Id
 */
/* PRQA S 6060 ++ */ /* VL_MTR_LinIf_STM19*/
LINIF_LOCAL NetworkHandleType
    LinIf_GetLinIfChannelByDriverChId(NetworkHandleType channel, ApplicationType applicationId)
/* PRQA S 6060 -- */
{
    NetworkHandleType idx = LINIF_NUMBER_OF_CHANNELS;

    if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        (NULL_PTR != LinIf_Status[applicationId]) &&
#endif
        (LINIF_INIT == *LinIf_Status[applicationId]))
    {
        for (idx = 0u; idx < LINIF_NUMBER_OF_CHANNELS; idx++)
        {
            if (LinIf_GetLinChannelId(idx) == channel)
            {
                return idx;
            }
        }
    }

    return idx;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif

/* ========================================== internal function definition ========================================== */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_DEV_ERROR_DETECT == STD_ON)

/**
 * @brief Validate linif init status
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    applicationId  LinIf partition id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateInitStatus(uint8 appId, ApplicationType applicationId)
{
    boolean ret = TRUE;
    if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        (NULL_PTR != LinIf_Status[applicationId]) &&
#endif
        (LINIF_UNINIT == *LinIf_Status[applicationId]))
    {
        /*@req <SWS_LinIf_00535>*/
        LinIf_DetReportError(appId, LINIF_E_UNINIT);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate linif channel validity
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    ch  LinIf channel id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateChannelNum(uint8 appId, uint8 ch)
{
    boolean ret = TRUE;
    if (LINIF_NUMBER_OF_CHANNELS <= ch)
    {
        /*@req <SWS_LinIf_00855>*/
        LinIf_DetReportError(appId, LINIF_E_NONEXISTENT_CHANNEL);
        ret = FALSE;
    }
    return ret;
}

#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
/**
 * @brief Validate linif partition validity
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    applicationId  LinIf partition id
 * @param[in]    ch  LinIf channel id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateChannelPartition(uint8 appId, ApplicationType applicationId, uint8 ch)
{
    boolean ret = TRUE;
    if (LinIf_GetChannelApplicationId(ch) != applicationId)
    {
        LinIf_DetReportError(appId, LINIF_E_ERROR_PARTITION);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate linif txpdu partition validity
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    applicationId  LinIf partition id
 * @param[in]    LinTxPduId  LinIf tx pdu id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean
    LinIf_ValidateTxPduPartition(uint8 appId, ApplicationType applicationId, PduIdType LinTxPduId)
{
    boolean ret = TRUE;
    if (E_NOT_OK == LinIf_CheckTxPduPatition(LinTxPduId, applicationId))
    {
        LinIf_DetReportError(appId, LINIF_E_ERROR_PARTITION);
        ret = FALSE;
    }
    return ret;
}
#endif

/**
 * @brief Validate linif configura nad
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    Nad  LinIf configura nad
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateConfigNad(uint8 appId, uint8 Nad)
{
    boolean ret = TRUE;
    if (0u == Nad)
    {
        /*@req <SWS_LinIf_00836>*/
        LinIf_DetReportError(appId, LINIF_E_PARAMETER);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate linif tx pdu id
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    LinTxPduId  LinIf tx pdu id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateLinTxPduId(uint8 appId, PduIdType LinTxPduId)
{
    boolean ret = TRUE;
    if (LinTxPduId >= LinIf_GetTxPduNum())
    {
        /*@req <SWS_LinIf_00575>*/
        LinIf_DetReportError(appId, LINIF_E_PARAMETER);
        ret = FALSE;
    }
    return ret;
}

#if (LINIF_TRCV_DRIVER_SUPPORTED == STD_ON)
/**
 * @brief Validate linif tx pdu id
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    LinTxPduId  LinIf tx pdu id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateTrcvMode(uint8 appId, LinTrcv_TrcvModeType TransceiverMode)
{
    boolean ret = TRUE;
    if ((LINTRCV_TRCV_MODE_NORMAL != TransceiverMode) && (LINTRCV_TRCV_MODE_STANDBY != TransceiverMode)
        && (LINTRCV_TRCV_MODE_SLEEP != TransceiverMode))
    {
        /*@req <SWS_LinIf_00540>*/
        LinIf_DetReportError(appId, LINIF_E_TRCV_INV_MODE);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate lintrcv wakeup mode
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    LinTrcvWakeupMode  LinTrcv wakeup mode
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateLinTrcvWakeupMode(uint8 appId, LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode)
{
    boolean ret = TRUE;
    if ((LINTRCV_WUMODE_ENABLE != LinTrcvWakeupMode) && (LINTRCV_WUMODE_DISABLE != LinTrcvWakeupMode)
        && (LINTRCV_WUMODE_CLEAR != LinTrcvWakeupMode))
    {
        /*@req <SWS_LinIf_00596>*/
        LinIf_DetReportError(appId, LINIF_E_PARAMETER);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate lintrcv work mode is normal
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    ch  LinIf channel id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateTrcvModeNormal(uint8 appId, uint8 ch)
{
    boolean              ret             = TRUE;
    uint8                linChannel      = LinIf_GetLinChannelId(ch);
    LinTrcv_TrcvModeType transceiverMode = LINTRCV_TRCV_MODE_SLEEP;
    (void)LinTrcv_GetOpMode(linChannel, &transceiverMode);
    if (LINTRCV_TRCV_MODE_NORMAL != transceiverMode)
    {
        /*@req <SWS_LinIf_00572>*/
        LinIf_DetReportError(appId, LINIF_E_TRCV_NOT_NORMAL);
        ret = FALSE;
    }
    return ret;
}
#endif

/**
 * @brief Validate linif slave node pid buffer length
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    PidBufferLength  Pid buffer length
 * @param[in]    ch LinIf channel id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidatePidBufferLength(uint8 appId, uint8 PidBufferLength, uint8 ch)
{
    boolean ret = TRUE;
    if (PidBufferLength < LinIf_GetFrameNum(ch))
    {
        /*@req <SWS_LinIf_00827>*/
        LinIf_DetReportError(appId, LINIF_E_PARAMETER);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate getPidBuffer interface pid buffer length
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    PidBufferLength  Pid buffer length
 * @param[in]    ch LinIf channel id
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidateGetPidBufferLength(uint8 appId, uint8 PidBufferLength, uint8 ch)
{
    boolean ret = TRUE;
    if ((PidBufferLength != 0u) && (PidBufferLength < LinIf_GetFrameNum(ch)))
    {
        /*@req <SWS_LinIf_00827>*/
        LinIf_DetReportError(appId, LINIF_E_PARAMETER);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Validate pointer
 * @param[in]    apiId  ID of API service in which error is detected
 * @param[in]    pointer  pointer
 * @return       TRUE   Development error not occurred
 * @return       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE boolean LinIf_ValidatePointer(uint8 appId, const void* pointer)
{
    boolean ret = TRUE;
    if (NULL_PTR == pointer)
    {
        LinIf_DetReportError(appId, LINIF_E_PARAM_POINTER);
        ret = FALSE;
    }
    return ret;
}
#endif

/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/**
 * @brief           LinIf slot timer handle
 * @param[in]       masterChRtDataPtr: Runtime data of linif channel
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           -
 */
LINIF_LOCAL_INLINE void LinIf_SlotTimer(LinIf_MasterRuntimeType* masterChRtDataPtr)
{
    LinIf_MasterRuntimeType* LinIf_MasterChRtData = masterChRtDataPtr;

    if (LinIf_MasterChRtData->Timer > 0u)
    {
        /* The current frame slot counter minus 1 */
        LinIf_MasterChRtData->Timer--;
    }
}
/* PRQA S 1532 -- */

/**
 * @brief           Get LinIf channel state
 * @param[in]       masterChRtDataPtr: Runtime data of linif channel
 * @return          ChannelState
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
LINIF_LOCAL_INLINE LinIf_ChannelStateType LinIf_GetChannelState(const LinIf_MasterRuntimeType* masterChRtDataPtr)
{
    const LinIf_MasterRuntimeType* LinIf_MasterChRtData = masterChRtDataPtr;
    return LinIf_MasterChRtData->ChannelState;
}

/**
 * @brief           Gets the frame reference by Tx PduId
 * @param[in]       TxPduId: LinIf tx pdu id
 * @return          subPtr: pointer of substitution frame
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
/* PRQA S 6060 ++ */ /* VL_MTR_LinIf_STM19*/
LINIF_LOCAL const LinIf_SubstitutionFramesType* LinIf_GetSubstitution(PduIdType TxPduId)
/* PRQA S 6060 -- */
{
    for (uint16 idx = 0u; idx < LinIf_GetSubNum(); idx++)
    {
        const LinIf_SubstitutionFramesType* subPtr   = LinIf_GetSubFrame(idx);
        uint16                              frameIdx = subPtr->SubstitutionFrameRef;
        const LinIf_TxPduType*              txPduPtr = LinIf_GetGlobalFrame(frameIdx)->PduDirection->TxPdu;

        if (txPduPtr->TxPduId == TxPduId)
        {
            return subPtr;
        }
    }

    return NULL_PTR;
}

#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)

/**
 * @brief           Validate tx pdu partition
 * @param[in]       TxPduId: LinIf tx pdu id
 * @param[in]       applicationId: Current partition id
 * @return          E_OK: Check pass
 * @return          E_NOT_OK: Check fail
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
/* PRQA S 6060 ++ */ /* VL_MTR_LinIf_STM19*/
LINIF_LOCAL Std_ReturnType LinIf_CheckTxPduPatition(PduIdType TxPduId, ApplicationType applicationId)
/* PRQA S 6060 -- */
{
    if (TxPduId < LinIf_GetTxPduNum())
    {
        const LinIf_TxPduType* txPduPtr = LinIf_GetTxPdu(TxPduId);
        if (applicationId == txPduPtr->PduApplicationId)
        {
            return E_OK;
        }
    }

    return E_NOT_OK;
}
#endif

/**
 * @brief           Get LinIf channel id by network(ComM Channel id)
 * @param[in]       channel: Identification of the ComM channel
 * @param[in]       applicationId: Current partition id
 * @return          idx: Identification of the LinIf channel
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace        -
 */
/* PRQA S 6060 ++ */ /* VL_MTR_LinIf_STM19*/
LINIF_LOCAL NetworkHandleType LinIf_GetLinIfChannel(NetworkHandleType channel, ApplicationType applicationId)
/* PRQA S 6060 -- */
{
    NetworkHandleType idx = LINIF_NUMBER_OF_CHANNELS;

    if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        (LinIf_Status[applicationId] != NULL_PTR) &&
#endif
        (LINIF_INIT == *LinIf_Status[applicationId]))
    {
        for (idx = 0u; idx < LINIF_NUMBER_OF_CHANNELS; idx++)
        {
            if (LinIf_GetComMNetwork(idx) == channel)
            {
                return idx;
            }
        }
    }

    return idx;
}

#if (STD_ON == LINIF_MASTER_SUPPORT)
/**
 * The main processing function of the LinIf master node. This function process
 * master node wakeup and sleep, message transmit and update schedule table.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
LINIF_LOCAL void LinIf_MasterMainHandle(NetworkHandleType ch)
/* PRQA S 6070 -- */
{
    LinIf_MasterRuntimeType* masterChRtDataPtr = LinIf_GetMasterRtDataPtr(ch);
    LinIf_SlotTimer(masterChRtDataPtr);
    LinIf_WakeUpProcess(masterChRtDataPtr, ch);
    LinIf_SleepProcess(masterChRtDataPtr, ch);

    if (LINIF_CHANNEL_OPERATIONAL == LinIf_GetChannelState(masterChRtDataPtr))
    {
        if (LinIf_IsEntryDelayTimeout(masterChRtDataPtr))
        {
            LinIf_PrevTransmit(masterChRtDataPtr, ch);
        }

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
        LinTp_MasterMainFunction(ch);
#endif

        if (LinIf_IsEntryDelayTimeout(masterChRtDataPtr))
        {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_MASTER_SUPPORT == STD_ON))
            if (!LinTp_IsWaitEventSet(ch))
#endif
            {
                LinIf_UpdateSchedule(masterChRtDataPtr, ch);
                LinIf_NextTransmit(masterChRtDataPtr, ch);
            }
        }
    }
}
#endif

/**
 * LinIf slave main function
 */
#if (STD_ON == LINIF_SLAVE_SUPPORT)
LINIF_LOCAL void LinIf_SlaveMainHandle(NetworkHandleType ch)
{
    LinIf_SlaveMainFunction(ch);
#if (LINTP_SLAVE_SUPPORT == STD_ON)
    LinTp_SlaveMainFunction(ch);
#endif
}
#endif

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2889 -- */
