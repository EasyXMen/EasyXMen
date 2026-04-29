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
 **  @file               : Xcp_Daq.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Implementation of the XCP_Daq command
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"
#include "SchM_Xcp.h"
#if (XCP_SET_REQUEST == STD_ON)
#include "Nvm.h"
#endif

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
/* ===================================================== macros ===================================================== */
/* XCP start stop mode */
#define XCP_START_STOP_MOD_STOP   0x00u
#define XCP_START_STOP_MOD_START  0x01u
#define XCP_START_STOP_MOD_SELECT 0x02u

/* XCP start stop synch */
#define XCP_START_STOP_MOD_STOP_ALL     0x00u
#define XCP_START_STOP_MOD_START_SELECT 0x01u
#define XCP_START_STOP_MOD_STOP_SELECT  0x02u

/*Overload*/
#define XCP_PID_OVFLOW 0x80u

/* DAQ-DTO data position offset */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
#define XCP_DATA_OFFSET 0x01u
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_BYTE)
#define XCP_DAQ_NUM_OFFSET 0x01u
#define XCP_DATA_OFFSET    0x02u
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_WORD)
#define XCP_DAQ_NUM_OFFSET 0x01u
#define XCP_DATA_OFFSET    0x03u
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_WORD_ALIGNED)
#define XCP_DAQ_NUM_OFFSET 0x02u
#define XCP_DATA_OFFSET    0x04u
#endif /*XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE*/

#define XCP_RESUME_FLAG_OFFSET 0X0U
#define XCP_RESUME_SEID_OFFSET 0X1U
#define XCP_RESUME_SIZE_OFFSET 0X3U
#define XCP_RESUME_DATA_OFFSET 0X5uL

/* ========================================== internal function declarations ======================================== */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
XCP_LOCAL void Xcp_ClearAllDaq(void);
XCP_LOCAL void Xcp_ClearDaq(Xcp_DaqNumType daqNum);
XCP_LOCAL void Xcp_ClearDaqOdt(Xcp_DaqNumType daqNum);

XCP_LOCAL void Xcp_FillEventBuffer(Xcp_DaqType* daqPtr);

XCP_LOCAL void           Xcp_EventChannelDaqHal(uint16 eventNum, const Xcp_DaqNumType daqNum);
XCP_LOCAL Std_ReturnType Xcp_PreSendDaqFrame(Xcp_DaqType* daqPtr, Xcp_DaqNumType daqNum);
XCP_LOCAL Std_ReturnType Xcp_SendDaqFrame(Xcp_DaqType* daqPtr, Xcp_DaqNumType daqNum, boolean overLoad);
XCP_LOCAL Std_ReturnType Xcp_StartDaq(Xcp_DaqNumType daqNum);
XCP_LOCAL void           Xcp_StopDaq(Xcp_DaqNumType daqNum);
XCP_LOCAL void           Xcp_WriteDaqHal(void);
XCP_LOCAL void           Xcp_SetDaqListModeHal(void);
XCP_LOCAL void           Xcp_StartStopDaqListHal(void);
XCP_LOCAL void           Xcp_StartStopSynchHal(void);

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
XCP_LOCAL void Xcp_AllocDaqHal(void);
XCP_LOCAL void Xcp_AllocOdtHal(void);
XCP_LOCAL void Xcp_AllocOdtEntryHal(void);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Xcp_3218 */
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
XCP_LOCAL uint8 Xcp_DynDaqBuffer[XCP_DYNAMIC_DAQ_BUFFER_SIZE];
#endif
XCP_LOCAL uint8 Xcp_DaqBuffer[XCP_MAX_DTO];
/* PRQA S 3218 -- */

#if (XCP_RESUME_SUPPORT == STD_ON)
/* PRQA S 3218,3678,3233 ++ */ /* VL_Xcp_3218, VL_Xcp_3678, VL_Xcp_3233 */
XCP_LOCAL uint8 Xcp_ResumeData[XCP_CFG_RESUME_BUFSIZE];
/* PRQA S 3218,3678,3233 -- */
#endif
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

/* ============================================ external data definitions =========================================== */
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
Xcp_ChannelDaqType  Xcp_ChannelDaqData;
Xcp_ChannelEvChType Xcp_ChannelEvChData;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

/* ========================================== external function definitions ========================================= */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
/**
 * Initializes the XCP Data Acquisition (DAQ) system. This function checks if
 * resume support is enabled and processes the resume action if necessary. It
 * restores the DAQ lists from non-volatile memory and sends an `EV_RESUME_MODE`
 * event to the master. If resume is not required, it reinitializes the DAQ
 * system.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef*/
void Xcp_DAQInit(void)
{
#if (XCP_RESUME_SUPPORT == STD_ON)
    uint16 resumeTotalSize;
    Xcp_CopyU1BufferToU2(
        &(Xcp_ResumeData[XCP_RESUME_SEID_OFFSET]),
        &Xcp_ChannelDaqData.SessionCfgId,
        (uint8)CPU_BYTE_ORDER);
    Xcp_CopyU1BufferToU2(&(Xcp_ResumeData[XCP_RESUME_SIZE_OFFSET]), &resumeTotalSize, (uint8)CPU_BYTE_ORDER);
    if ((Xcp_ChannelDaqData.SessionCfgId != 0u) || (resumeTotalSize > 0u))
    {
        /*Processing resume action*/
        boolean resumeFlag;

        /*Copy DAQ configuration from non-volatile memory.*/
        (void)IStdLib_MemCmp(
            &(Xcp_ChannelDaqData.DaqInfo[0]),
            &(Xcp_ResumeData[XCP_RESUME_DATA_OFFSET]),
            (uint32)resumeTotalSize - XCP_RESUME_DATA_OFFSET);
        /* PRQA S 4440 ++ */ /* VL_Xcp_4440 */
        resumeFlag = Xcp_ResumeData[XCP_RESUME_FLAG_OFFSET];
        /* PRQA S 4440 -- */

        if (resumeFlag)
        {
            /* Power-up data transfer (RESUME mode) action.*/
            Xcp_DaqType* daqPtr;
            uint16       daqInx;
            uint16       evchNum;

            /*1.Send EVENT(EV_RESUME_MODE):
             * On each power up, the slave has to restore the DAQ lists and send an EV_RESUME_MODE to the master.*/
            Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_RESUME_MODE;
            Xcp_CopyU2ToU1Buffer(
                Xcp_ChannelDaqData.SessionCfgId,
                &(Xcp_EvBuffer[XCP_EV_RESUME_SSID_OFFEST]),
                (uint8)CPU_BYTE_ORDER);
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
            uint32 ts = Xcp_ReadOutMS();
            Xcp_CopyU4ToU1Buffer((uint32)ts, &(Xcp_EvBuffer[XCP_EV_RESUME_TS_OFFEST]), (uint8)CPU_BYTE_ORDER);
            Xcp_ChannelCommonData.EvLength = XCP_CONST_8;
#else
            Xcp_ChannelCommonData.EvLength = XCP_CONST_4;
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP*/
            Xcp_SendEv();
            /*End of EV_RESUME_MODE*/

            /*2. Power-up data transfer.*/
            for (daqInx = 0u; daqInx < XCP_MAX_DAQ; daqInx++)
            {
                daqPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqInx]);
                if (XCP_DAQ_MOD_RESUME == (daqPtr->mode & XCP_DAQ_MOD_RESUME))
                {
                    daqPtr->mode |= XCP_DAQ_MOD_RUNNING;
                    evchNum = daqPtr->eventChannelNum;
                    Xcp_ChannelEvChData.EvChActivCnt[evchNum]++;
                }
            }
        }
    }
    else
#endif /*XCP_RESUME_SUPPORT == STD_ON*/
    {
        Xcp_DAQReinit();
    }
    return;
}

#if (XCP_SET_REQUEST == STD_ON)
/**
 * Handles the XCP `SET_REQUEST` command for storing or clearing DAQ configurations.
 * This function checks the session status and performs the appropriate action:
 * - Stores calibration data to non-volatile memory (NVM) and sends an `EV_STORE_CAL` event.
 * - Stores DAQ configurations to NVM and sends an `EV_STORE_DAQ` event.
 * - Clears DAQ configurations and sends an `EV_CLEAR_DAQ` event.
 * If none of these conditions are met, the function does nothing.
 */
void Xcp_SetRequestHandler(void)
{
    /*There are two ways to store to NVM:
     * 1.Utilize the hardware mechanism and realize it through MemMap mechanism,(SAVE_ZONE).
     * 2.Use NvmRam interface.
     * */
    if (XCP_SESSION_STATE_STORE_CAL == (Xcp_ChannelCommonData.SessionStatus & XCP_SESSION_STATE_STORE_CAL))
    {
        Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_STORE_CAL);

        /*TODO*/
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_STORE_CAL;
        Xcp_ChannelCommonData.EvLength   = XCP_CONST_2;
        Xcp_SendEv();
    }
#if (XCP_RESUME_SUPPORT == STD_ON)
    else if (XCP_SESSION_STATE_STORE_DAQ == (Xcp_ChannelCommonData.SessionStatus & XCP_SESSION_STATE_STORE_DAQ))
    {
        uint16  resumeDaqSize   = (uint16)(sizeof(Xcp_DaqType) * XCP_MAX_DAQ);
        uint16  resumeTotalSize = resumeDaqSize + XCP_RESUME_DATA_OFFSET;
        boolean isPowerOnFlag   = FALSE;
        /*Resume mode is active or not.*/
        if (XCP_SESSION_STATE_RESUME == (Xcp_ChannelCommonData.SessionStatus & XCP_SESSION_STATE_RESUME))
        {
            isPowerOnFlag = TRUE;
        }
        if (resumeTotalSize <= XCP_CFG_RESUME_BUFSIZE)
        {
            /*Start:Store data through NVM(by SET_REQUEST(0xF9)).*/
            /*1.Power-up data transfer flag.*/
            Xcp_ResumeData[XCP_RESUME_FLAG_OFFSET] = isPowerOnFlag;
            /*2.Session configuration id.*/
            Xcp_CopyU2ToU1Buffer(
                Xcp_ChannelDaqData.SessionCfgId,
                &(Xcp_ResumeData[XCP_RESUME_SEID_OFFSET]),
                (uint8)CPU_BYTE_ORDER);
            /*3.Total length of save data(non-volatile memory)*/
            Xcp_CopyU2ToU1Buffer(resumeTotalSize, &(Xcp_ResumeData[XCP_RESUME_SIZE_OFFSET]), (uint8)CPU_BYTE_ORDER);
            /*4.Copy whole DAQ configuration to non-volatile memory.*/
            (void)IStdLib_MemCmp(
                &(Xcp_ResumeData[XCP_RESUME_DATA_OFFSET]),
                &(Xcp_ChannelDaqData.DaqInfo[0]),
                resumeDaqSize);
            /*5.Call Nvm APi.*/
            (void)NvM_WriteBlock(Xcp_PbCfgPtr->nvmBlkIndex, &Xcp_ResumeData[0]);
            /*End:Store data.*/
        }
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_STORE_DAQ;
        Xcp_ChannelCommonData.EvLength   = XCP_CONST_2;
        Xcp_SendEv();
        Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_STORE_DAQ);
    }
    else if (XCP_SESSION_STATE_CLEAR_DAQ == (Xcp_ChannelCommonData.SessionStatus & XCP_SESSION_STATE_CLEAR_DAQ))
    {
        Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_CLEAR_DAQ);
        (void)IStdLib_MemSet(Xcp_ResumeData, 0, (uint32)XCP_CFG_RESUME_BUFSIZE);
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_CLEAR_DAQ;
        Xcp_ChannelCommonData.EvLength   = XCP_CONST_2;
        Xcp_SendEv();
    }
#endif
    else
    {
        /*Do nothing*/
    }
}
#endif

/**
 * Reinitializes the XCP Data Acquisition (DAQ) system. This function clears
 * event usage counters, initializes DAQ list information, sets the dynamic buffer
 * pointer, and clears all DAQ configurations. The initialization process depends
 * on whether the DAQ configuration is static or dynamic.
 */
void Xcp_DAQReinit(void)
{
    /* Step1:Initial the daq odt Ptr */
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
    const Xcp_DaqListConfigType* daqCfgRef = Xcp_PConfig.XcpDaqList;
#endif
    Xcp_ChannelDaqPtr  daqDataPtr  = &Xcp_ChannelDaqData;
    Xcp_ChannelEvChPtr evChDataPtr = &Xcp_ChannelEvChData;
    uint16             Num;
    /* clear event usage counter */
    for (Num = 0u; Num < XCP_MAX_EVENT_CHANNEL; Num++)
    {
        evChDataPtr->EvChBufferUsage[Num] = 0u;
        evChDataPtr->EvChActivCnt[Num]    = 0u;
        evChDataPtr->EvChActiveFlag[Num]  = FALSE;
    }
    for (Num = 0u; Num < XCP_MAX_DAQ; Num++)
    {
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
        daqDataPtr->DaqInfo[Num].odt         = daqCfgRef[Num].XcpOdt;
        daqDataPtr->DaqInfo[Num].daqListSize = daqCfgRef[Num].XcpMaxOdt;
#else
        daqDataPtr->DaqInfo[Num].daqListSize = 0u;
#endif
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
        daqDataPtr->DaqInfo[Num].firstPid = daqCfgRef[Num].XcpFirstPid;
#else
        daqDataPtr->DaqInfo[Num].firstPid = 0u;
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC*/
#else
        daqDataPtr->DaqInfo[Num].firstPid = 0;
#endif /*XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE*/
    }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    /*Set dynamic buffer point.*/
    daqDataPtr->DynDaqBufPtr  = &Xcp_DynDaqBuffer[0];
    daqDataPtr->DynDaqBufSize = XCP_DYNAMIC_DAQ_BUFFER_SIZE;
#endif
    /* clear all DAQ */
    Xcp_ClearAllDaq();
    return;
}

/* ========================================== internal function definitions ========================================= */
/**
 * Clears all DAQ configurations. This function invalidates the DAQ pointers,
 * clears each DAQ list, and resets the session status to ensure no resume state
 * is active. For dynamic DAQ configurations, it also resets the sequence status
 * and buffer position.
 */
XCP_LOCAL void Xcp_ClearAllDaq(void)
{
    Xcp_DaqNumType daqNum;

    Xcp_ChannelDaqData.PtrDaq   = XCP_U16_MAX; /*set the ptr is invalid*/
    Xcp_ChannelDaqData.PtrOdt   = XCP_U8_MAX;
    Xcp_ChannelDaqData.PtrEntry = XCP_U8_MAX;
    for (daqNum = 0u; daqNum < XCP_MAX_DAQ; daqNum++)
    {
        Xcp_ClearDaq(daqNum);
    }
    /* initial Xcp_ChannelCommonData.SessionStatus and keep the info of the DAQ Store Request */
    Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_RESUME);

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_IDLE;
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
    Xcp_ChannelDaqData.DynDaqFirstPIDCnt = 0u;
#endif
    Xcp_ChannelDaqData.DynDaqBufPos = 0u; /* Record the location where the array is used */
#endif
}

/**
 * Clears a specific DAQ list. This function resets the send state machine, mode,
 * current PID, prescaler, event channel number, and event buffer. For dynamic
 * DAQ configurations, it also sets the configuration status and clears the ODT
 * entries.
 */
XCP_LOCAL void Xcp_ClearDaq(Xcp_DaqNumType daqNum)
{
    Xcp_DaqType* daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);

    /* clear send state machine */
    daqInfoPtr->sendStatus = 0u;
    /* reinit. mode and if the DAQ list only support STIM mode shall be 0x02 */
    daqInfoPtr->mode = 0u;
    /* init currentPid */
    daqInfoPtr->currentPid = daqInfoPtr->firstPid;
/* init prescaler and cnt */
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
    daqInfoPtr->prescaler    = 0x01;
    daqInfoPtr->prescalerCnt = 0x00;
#endif
    /* set eventchannel Num to a invaild value */
    daqInfoPtr->eventChannelNum = XCP_MAX_EVENT_CHANNEL;
    /* init event buffer */
    daqInfoPtr->eventBufferPos = 0u;
    daqInfoPtr->eventBufferPtr = NULL_PTR;
    daqInfoPtr->totalEntryLen  = 0u;

/* for configurable DAQ clear ODT and ODT Entries */
#if (0u != XCP_MIN_DAQ)
    if (daqNum >= XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
    {
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
        daqInfoPtr->DynaCfgStatus = XCP_DAQ_CFG_FREE;
#endif
        Xcp_ClearDaqOdt(daqNum);
    }
#if (0u != XCP_MIN_DAQ)
    else
    {
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
        daqInfoPtr->DynaCfgStatus = XCP_DAQ_CFG_DONE;
#endif
    }
#endif /*0u != XCP_MIN_DAQ*/
    return;
}

/**
 * Clears the ODT (Object Descriptor Table) entries for a specific DAQ list.
 * This function iterates through each ODT and its entries, setting the entry
 * index to invalid, the address to NULL, and the length to zero. For dynamic
 * DAQ configurations, it also resets the ODT entry count and the ODT pointer.
 */
XCP_LOCAL void Xcp_ClearDaqOdt(Xcp_DaqNumType daqNum)
{
    uint8          odtNum;
    uint8          entryNum;
    Xcp_DaqType*   daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);
    Xcp_OdtType*   odtRef     = daqInfoPtr->odt;
    Xcp_EntryType* entryRef;
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    if (NULL_PTR != odtRef)
#endif
    {
        for (odtNum = 0u; odtNum < daqInfoPtr->daqListSize; odtNum++)
        {
            entryRef = odtRef[odtNum].XcpOdtEntry;
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
            if (entryRef != NULL_PTR)
#endif
            {
                for (entryNum = 0u; entryNum < odtRef[odtNum].XcpOdtEntryCount; entryNum++)
                {
                    entryRef[entryNum].XcpOdtEntryNumber = XCP_U8_MAX; /*set entry index is invalid*/
                    entryRef[entryNum].XcpOdtEntryAddr   = NULL_PTR;
                    entryRef[entryNum].XcpOdtEntryLength = 0u;
                }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
                odtRef[odtNum].XcpOdtEntry = NULL_PTR;
#endif
            }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
            odtRef[odtNum].XcpOdtEntryCount = 0u;
#endif
        }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
        daqInfoPtr->odt = NULL_PTR;
#endif
    }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    daqInfoPtr->daqListSize = 0u;
#endif
    return;
}

/**
 * Fills the event buffer with data from the DAQ list entries. This function
 * iterates through each ODT and its entries, copying the data to the event
 * buffer if the entry length is valid and there is enough space in the buffer.
 * It also updates the timestamp if timestamping is enabled.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
XCP_LOCAL void Xcp_FillEventBuffer(Xcp_DaqType* daqPtr)
/* PRQA S 3673 -- */
{
    const Xcp_EntryType* entryPtr;
    uint8                odtNum;           /* odt counter */
    uint8                entryNum;         /* entry conter */
    uint32               eventBufPos = 0u; /* dest. counter */

    /* traverse all odt */
    for (odtNum = 0u; odtNum < daqPtr->daqListSize; odtNum++)
    {
        /* if length of the first entry of the current odt is 0 goto next daq ,ASAM_XCP_Part1-page32*/
        if (0x00u == daqPtr->odt[odtNum].XcpOdtEntry[0].XcpOdtEntryLength)
        {
            break;
        }
        /* update entry ptr */
        entryPtr = daqPtr->odt[odtNum].XcpOdtEntry;
        /* traverse all entry */
        for (entryNum = 0u; entryNum < daqPtr->odt[odtNum].XcpOdtEntryCount; entryNum++)
        {
            /* if length of current entry is 0, goto next odt */
            if ((0x00u == entryPtr[entryNum].XcpOdtEntryLength)
                || ((eventBufPos + entryPtr->XcpOdtEntryLength)
                    >= Xcp_PConfig.XcpEvCh[daqPtr->eventChannelNum].XcpEvChBuffDpth))
            {
                break;
            }
            /* copy element to event buffer */
            uint8*       destPtr = (uint8*)&daqPtr->eventBufferPtr[eventBufPos];
            const uint8* srcPtr  = (uint8*)entryPtr[entryNum].XcpOdtEntryAddr;
            Xcp_BufferCopy(srcPtr, destPtr, entryPtr->XcpOdtEntryLength);
            eventBufPos += entryPtr->XcpOdtEntryLength;
        }
    }
/* update timestamp if needed */
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
    daqPtr->timeStamp = Xcp_ReadOutMS();
#endif
    return;
}

/**
 * Sends a DAQ frame for a specific DAQ list. This function checks if the DAQ
 * list is running, sets the PID number, and fills the DAQ buffer with data
 * from the ODT entries. It updates the event buffer if necessary and adds a
 * timestamp if configured. The function then transmits the DAQ frame using the
 * appropriate PDU ID. If the DAQ list is not running, it returns an error.
 */
XCP_LOCAL Std_ReturnType Xcp_SendDaqFrame(Xcp_DaqType* daqPtr, Xcp_DaqNumType daqNum, boolean overLoad)
{
    const Xcp_OdtType*   odtPtr = &(daqPtr->odt[daqPtr->currentPid - daqPtr->firstPid]);
    const Xcp_EntryType* entryPtr;
    uint16               evChNum = daqPtr->eventChannelNum;
    uint16               pos; /* DTO buffer Position */
    const Xcp_AGType*    sourPtr;
    Xcp_AGType*          destPtr;
    uint8                entryNum; /* entry counter */
    PduIdType            pduId;
    Std_ReturnType       ret = E_OK;
    (void)daqNum;

    SchM_Enter_Xcp_Exclusive_Daq();
    if (XCP_DAQ_MOD_RUNNING != (XCP_DAQ_MOD_RUNNING & (daqPtr->mode)))
    {
        ret = E_NOT_OK;
    }
    else
    {
        uint8 respPid = daqPtr->currentPid;
        /* set PID Num*/
        if (overLoad)
        { /* to pid overflow indication flag */
            respPid |= ((uint8)XCP_PID_OVFLOW);
        }
        Xcp_DaqBuffer[XCP_PID_OFFSET] = respPid;

#if (XCP_PID_OFF_SUPPORT == STD_ON)
        if (XCP_DAQ_MOD_PIDOFF == (XCP_DAQ_MOD_PIDOFF & (Xcp_ChannelDaqData.DaqInfo)[daqNum].mode))
        {
            pos = 0u;
        }
        else
#endif
        {
/* set DAQ Num */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_BYTE)
            Xcp_DaqBuffer[XCP_DAQ_NUM_OFFSET] = daqNum;
#elif (XCP_IDENTIFICATION_FIELD_TYPE != XCP_PID_ABSOLUTE)
            Xcp_CopyU2ToU1Buffer(daqNum, &Xcp_DaqBuffer[XCP_DAQ_NUM_OFFSET], (uint8)CPU_BYTE_ORDER);
#endif
            pos = XCP_DATA_OFFSET;
        }
        /* if this is the first odt of a DAQ   */
        if (daqPtr->currentPid == daqPtr->firstPid)
        {
            /* update send Status */
            daqPtr->sendStatus |= (XCP_DAQ_SS_SENDING);
            daqPtr->sendStatus &= (uint8)(~(XCP_DAQ_SS_QUEUE));

            /* if event consistent is daq then update the event buffer */
            if (XCP_EVENT_CONSIST_DAQ == Xcp_PConfig.XcpEvCh[evChNum].XcpEvChConsistency)
            {
                Xcp_FillEventBuffer(daqPtr);
            }
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
            /*Add time stamp ( if needed )*/
            if (XCP_DAQ_MOD_TS == (XCP_DAQ_MOD_TS & (daqPtr->mode)))
            {
                if (XCP_EVENT_CONSIST_ODT == Xcp_PConfig.XcpEvCh[evChNum].XcpEvChConsistency)
                {
                    daqPtr->timeStamp = Xcp_ReadOutMS();
                }
#if (XCP_TIMESTAMP_TYPE == XCP_TS_ONE_BYTE)
                Xcp_DaqBuffer[XCP_DATA_OFFSET] = (uint8)(daqPtr->timeStamp);
#elif (XCP_TIMESTAMP_TYPE == XCP_TS_TWO_BYTE)
                Xcp_CopyU2ToU1Buffer(
                    (Xcp_TSType)(daqPtr->timeStamp),
                    &Xcp_DaqBuffer[XCP_DATA_OFFSET],
                    (uint8)CPU_BYTE_ORDER);
#elif (XCP_TIMESTAMP_TYPE == XCP_TS_FOUR_BYTE)
                Xcp_CopyU4ToU1Buffer(
                    (Xcp_TSType)(daqPtr->timeStamp),
                    &Xcp_DaqBuffer[XCP_DATA_OFFSET],
                    (uint8)CPU_BYTE_ORDER);
#endif
                /* update the pos */
                pos += XCP_TIMESTAMP_TYPE;
            }
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP*/
        }

        entryPtr = odtPtr->XcpOdtEntry;
        /*Odt-Entry Copy.*/
        for (entryNum = 0u; entryNum < odtPtr->XcpOdtEntryCount; entryNum++)
        {
            /*Copy Condition:
             * 1.Valid Odt Entry;
             * 2.Having sufficient buffer space.
             * */
            if ((0u == entryPtr[entryNum].XcpOdtEntryLength)
                || ((pos + entryPtr[entryNum].XcpOdtEntryLength) > XCP_MAX_DTO))
            {
                break;
            }
            destPtr = (Xcp_AGType*)&Xcp_DaqBuffer[pos];
            /* if ODT consistent copy data directly from source*/
            if (XCP_EVENT_CONSIST_ODT == Xcp_PConfig.XcpEvCh[evChNum].XcpEvChConsistency)
            {
                sourPtr = entryPtr[entryNum].XcpOdtEntryAddr;
            }
            else
            {
                sourPtr = &(daqPtr->eventBufferPtr[daqPtr->eventBufferPos]);
                /* update the BufferPos */
                daqPtr->eventBufferPos += entryPtr[entryNum].XcpOdtEntryLength;
            }
            Xcp_BufferCopy(sourPtr, destPtr, entryPtr[entryNum].XcpOdtEntryLength);
            pos += entryPtr[entryNum].XcpOdtEntryLength;
        }
    }
    SchM_Exit_Xcp_Exclusive_Daq();
    if (ret == E_OK)
    {
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
        if (Xcp_PConfig.XcpDaqList[daqNum].XcpDto != NULL_PTR)
        {
            pduId = (PduIdType)Xcp_PConfig.XcpDaqList[daqNum].XcpDto->LowLayerPduId;
        }
        else
#endif
        {
            pduId = (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId;
        }
        ret = Xcp_Transmit(pduId, (PduLengthType)pos, Xcp_DaqBuffer);
    }
    return ret;
}

/**
 * Prepares and sends a DAQ frame, handling overload conditions. This function
 * checks if an overload condition is active and adjusts the current PID if
 * necessary. It then calls `Xcp_SendDaqFrame` to transmit the DAQ frame, either
 * indicating the overload via an event or by setting a flag in the PID. If no
 * overload is present, it simply sends the DAQ frame.
 */
XCP_LOCAL Std_ReturnType Xcp_PreSendDaqFrame(Xcp_DaqType* daqPtr, Xcp_DaqNumType daqNum)
{
    Std_ReturnType result;

    if (XCP_DAQ_SS_OVL == (XCP_DAQ_SS_OVL & (daqPtr->sendStatus)))
    {
        /* if consistency of the associated event channel is DAQ,
         * send the DAQ List from the beginning.
         */
        if (XCP_EVENT_CONSIST_DAQ == (Xcp_PConfig.XcpEvCh[daqPtr->eventChannelNum].XcpEvChConsistency))
        {
            daqPtr->currentPid = daqPtr->firstPid;
        }
#if (XCP_DAQ_OVL_EV == XCP_DAQ_OVL_INDICATION) /* indicated by event */
        result                           = Xcp_SendDaqFrame(daqPtr, daqNum, FALSE);
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_DAQ_OVERLOAD;
        Xcp_ChannelCommonData.EvLength   = 2;
        Xcp_SendEv();
#elif (XCP_DAQ_OVL_PID == XCP_DAQ_OVL_INDICATION) /* indicated by pid */
        result = Xcp_SendDaqFrame(daqPtr, daqNum, TRUE);
#else                                             /* not indicated */
        result = Xcp_SendDaqFrame(daqPtr, daqNum, FALSE);
#endif
        daqPtr->sendStatus &= (uint8)(~(XCP_DAQ_SS_OVL));
    }
    else
    {
        result = Xcp_SendDaqFrame(daqPtr, daqNum, FALSE);
    }
    return result;
}

/**
 * Sends a DAQ frame for a specific event channel. This function checks if the
 * request is enabled and marks the event channel as active. It then retrieves
 * the next DAQ list from the queue, prepares and sends the DAQ frame using
 * `Xcp_PreSendDaqFrame`. If the send is successful, it updates the current PID
 * and sets the pending flag. If the DAQ list is stopped, reaches the end, or
 * has a zero-length entry, it resets the DAQ sending flags and updates the
 * queue position. If all DAQ lists in the queue have been processed, it marks
 * the event channel as inactive.
 */
void Xcp_SendDaq(uint16 evChNum, boolean rqstEnable)
{
    Xcp_DaqType* currentDaqPtr;

    if (rqstEnable)
    {
        Xcp_ChannelEvChData.EvChActiveFlag[evChNum] = TRUE;
    }
    if (XCP_DAQ_PENDING != (XCP_DAQ_PENDING & Xcp_ChannelCommonData.SendStatus))
    {
        uint8*         daqPos = &Xcp_ChannelEvChData.DaqListQueuePos[evChNum];
        Xcp_DaqNumType daqNum = (Xcp_ChannelEvChData.DaqListQueue)[evChNum][*daqPos];
        currentDaqPtr         = &(Xcp_ChannelDaqData.DaqInfo)[daqNum];

        if ((Std_ReturnType)E_OK == Xcp_PreSendDaqFrame(currentDaqPtr, daqNum))
        {
            /* send successed  */
            (currentDaqPtr->currentPid)++;
            /*Flag clear in txconfirmation.*/
            Xcp_ChannelCommonData.SendStatus |= XCP_DAQ_PENDING;
        }
        if (
            /* DAQ List in Queue has been stoped */
            (0u == (XCP_DAQ_MOD_RUNNING & (currentDaqPtr->mode)))
            /* Reach the end of the daq List */
            || ((currentDaqPtr->daqListSize + currentDaqPtr->firstPid) == (currentDaqPtr->currentPid))
            /* size of new entry is zero */
            || (0u
                == (currentDaqPtr->odt[currentDaqPtr->currentPid - currentDaqPtr->firstPid]
                        .XcpOdtEntry[0]
                        .XcpOdtEntryLength)))
        {
            /* clear the DAQ sending ongoing flag and reset pid.*/
            (currentDaqPtr->sendStatus) &= (uint8)(~(XCP_DAQ_SS_SENDING));
            currentDaqPtr->eventBufferPos = 0u;
            currentDaqPtr->currentPid     = currentDaqPtr->firstPid;

            /*updata the DAQ queue position*/
            (*daqPos)++;

            /*Have all DAQs in the queue been processed?*/
            if ((*daqPos) == Xcp_ChannelEvChData.DaqListQueueLength[evChNum])
            {
                Xcp_ChannelEvChData.EvChActiveFlag[evChNum] = FALSE;
            }
        }
    }
    return;
}
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
/**
 * Processes incoming STIM (Stimulation) data. This function checks if the STIM
 * protection is locked and finds the corresponding DAQ list based on the PID.
 * It then verifies the DAQ mode and event channel type, processes the STIM
 * data by copying it to the appropriate memory locations, and handles any
 * errors by setting the appropriate error code and sending a response.
 */
void Xcp_RxStimHal(const uint8* dataPtr)
{
    const Xcp_DaqType*        daqPtr;
    const Xcp_OdtType*        odtPtr;
    const Xcp_EntryType*      entryPtr;
    const Xcp_EvChConfigType* evchCfgPtr;
    uint16                    pos;
    Xcp_DaqNumType            daqNum;
    uint8                     odtNum;
    uint8                     entryIdx;
    uint8                     pid     = dataPtr[XCP_PID_OFFSET];
    boolean                   errFlag = TRUE;

    if (XCP_PL_STIM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_STIM))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);

        /* PRQA S 2001,2015 ++ */ /* VL_Xcp_2001,VL_Xcp_2015 */
        goto XCP_PROCESS_STIM_EXIT;
        /* PRQA S 2001,2015 -- */
    }

    /* Find daq num */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
    for (daqNum = 0u; daqNum < XCP_MAX_DAQ; daqNum++)
    {
        daqPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);

        if ((pid >= daqPtr->firstPid) && (pid < (daqPtr->firstPid + daqPtr->daqListSize)))
        {
            break;
        }
    }
    pos = XCP_DATA_OFFSET;
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_BYTE)
    daqNum = dataPtr[XCP_DAQ_NUM_OFFSET];
    pos    = XCP_DATA_OFFSET;
#else                    /*XCP_PID_RELATIVE_WORD & XCP_PID_RELATIVE_WORD_ALIGNED*/
    Xcp_CopyU1BufferToU2(&(dataPtr[XCP_DAQ_NUM_OFFSET]), &daqNum, (uint8)CPU_BYTE_ORDER);
    pos = XCP_DATA_OFFSET;
#endif                   /*XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE*/
    /* PRQA S 2995 ++ */ /* VL_Xcp_2995 */
    if ((daqNum < XCP_MAX_DAQ) && (NULL_PTR != daqPtr))
    /* PRQA S 2995 -- */
    {
        evchCfgPtr = &(Xcp_PConfig.XcpEvCh[daqPtr->eventChannelNum]);
        if ((XCP_DAQ_MOD_DIR_STIM != (daqPtr->mode & XCP_DAQ_MOD_DIR_STIM)) || (DAQ == evchCfgPtr->XcpEvChType))
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
        else
        {
            odtNum = pid - daqPtr->firstPid;
            odtPtr = &(daqPtr->odt[odtNum]);
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
            /* Judge whether need to store time stump,TS only applicable in first ODT of STIM*/
            if ((0u == odtNum) && (0u != (XCP_DAQ_MOD_TS & daqPtr->mode)))
            {
                pos += XCP_TIMESTAMP_TYPE;
            }
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP*/
            for (entryIdx = 0u; entryIdx < odtPtr->XcpOdtEntryCount; entryIdx++)
            {
                entryPtr = &(odtPtr->XcpOdtEntry[entryIdx]);
                if ((0x00u == entryPtr->XcpOdtEntryLength) || ((pos + entryPtr->XcpOdtEntryLength) > XCP_MAX_CTO))
                {
                    /* PRQA S 2001,2015 ++ */ /* VL_Xcp_2001,VL_Xcp_2015 */
                    goto XCP_PROCESS_STIM_EXIT;
                    /* PRQA S 2001,2015 -- */
                }
                const uint8* srcPtr  = &dataPtr[pos];
                uint8*       destPtr = (uint8*)entryPtr->XcpOdtEntryAddr;
                Xcp_BufferCopy(srcPtr, destPtr, entryPtr->XcpOdtEntryLength);
                pos += entryPtr->XcpOdtEntryLength;
            }
            errFlag = FALSE;
        }
    }
    else
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
/* PRQA S 2001,2015 ++ */ /* VL_Xcp_2001,VL_Xcp_2015 */
XCP_PROCESS_STIM_EXIT:
    /* PRQA S 2001,2015 -- */
    if (TRUE == errFlag)
    {
        Xcp_SendResp();
    }
    return;
}
#endif /*XCP_PL_STIM == XCP_PL_STIM & XCP_RESOURCE*/
/**
 * Handles the event channel for a specific DAQ list. This function checks if
 * the DAQ list is currently sending or queued. If so, it marks the DAQ list as
 * overloaded and resets the send status. Otherwise, it queues the DAQ list for
 * sending. If the event channel consistency is set to EVENT, it fills the event
 * buffer. Finally, it adds the DAQ list to the event channel's queue.
 */
XCP_LOCAL void Xcp_EventChannelDaqHal(uint16 eventNum, const Xcp_DaqNumType daqNum)
{
    Xcp_DaqType* daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);

    /* Overload*/
    if ((XCP_DAQ_SS_SENDING == (XCP_DAQ_SS_SENDING & daqInfoPtr->sendStatus))
        || (XCP_DAQ_SS_QUEUE == (XCP_DAQ_SS_QUEUE & daqInfoPtr->sendStatus)))
    {
        /* clear the DAQ sending ongoing flag */
        daqInfoPtr->sendStatus &= (uint8)(~(XCP_DAQ_SS_SENDING | XCP_DAQ_SS_QUEUE));
        daqInfoPtr->sendStatus |= XCP_DAQ_SS_OVL;
        daqInfoPtr->eventBufferPos = 0u;
        daqInfoPtr->currentPid     = daqInfoPtr->firstPid;
    }
    else
    {
        daqInfoPtr->sendStatus |= XCP_DAQ_SS_QUEUE;
    }
    if (XCP_EVENT_CONSIST_EVENT == (Xcp_PConfig.XcpEvCh[eventNum].XcpEvChConsistency))
    {
        Xcp_FillEventBuffer(daqInfoPtr);
    }
    /*Put the pending DAQ into the queue.*/
    (Xcp_ChannelEvChData.DaqListQueue[eventNum])[Xcp_ChannelEvChData.DaqListQueueLength[eventNum]] = daqNum;
    Xcp_ChannelEvChData.DaqListQueueLength[eventNum]++;
    return;
}

/**
 * Handles an event indication for a specific event channel. This function
 * clears the queue position and length, then iterates through the DAQ lists
 * associated with the event channel. It checks if each DAQ list is running and
 * matches the event channel. If so, it handles the DAQ list using
 * `Xcp_EventChannelDaqHal`. If any DAQ lists are marked for sending, it calls
 * `Xcp_SendDaq` to transmit the DAQ frames.
 */
/* PRQA S 2889, 6060 ++ */ /* VL_QAC_MultiReturn, VL_MTR_Xcp_STM19 */
void Xcp_EventIndication(uint16 eventNum)
/* PRQA S 2889, 6060 -- */
{
    uint16         daqCnt;
    Xcp_DaqNumType cnt; /* loop counter */
    Xcp_DaqNumType daqNum;
    boolean        SenddaqFlag = FALSE;
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
    const Xcp_EvChConfigType* eventPtr;
#endif
    Xcp_DaqType* daqInfoPtr; /* PRQA S 3678 */ /* VL_Xcp_3678 */

    if ((!Xcp_ModuleInit) || (eventNum >= XCP_MAX_EVENT_CHANNEL))
    {
        return;
    }
    /*Clear queue position information */
    Xcp_ChannelEvChData.DaqListQueuePos[eventNum]    = 0u;
    Xcp_ChannelEvChData.DaqListQueueLength[eventNum] = 0u;
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
    eventPtr = &Xcp_PConfig.XcpEvCh[eventNum];
    daqCnt   = eventPtr->XcpEvChMaxDaqList;
#else
    daqCnt = Xcp_ChannelDaqData.DynAllocDaqNum;
#endif
    for (cnt = 0u; cnt < daqCnt; cnt++)
    {
        /* get daq Num */
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
        daqNum = eventPtr->XcpEvChTrigDaqListRef[cnt];
#else
        daqNum = cnt;
#endif
        daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);
        /* Find the DAQ that is running and matching eventNum */
        if ((XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & daqInfoPtr->mode))
            && (eventNum == daqInfoPtr->eventChannelNum))
        {
            SchM_Enter_Xcp_Exclusive_Daq();
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
            daqInfoPtr->prescalerCnt++; /* to prescaler */
            if (daqInfoPtr->prescalerCnt >= daqInfoPtr->prescaler)
            {
                daqInfoPtr->prescalerCnt = 0;
#endif /*XCP_PRESCALER_SUPPORTED == STD_ON*/
                /*Handle DAQ*/
                if (XCP_DAQ_MOD_DIR_DAQ == (daqInfoPtr->mode & XCP_DAQ_MOD_DIR_BIT))
                {
                    SenddaqFlag = TRUE;
                    Xcp_EventChannelDaqHal(eventNum, daqNum);
                }
                /*STIM processing in Rxindication.*/
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
            }
#endif
            SchM_Exit_Xcp_Exclusive_Daq();
        }
    }
    if (SenddaqFlag)
    {
        Xcp_SendDaq(eventNum, TRUE);
    }
    return;
}
/**
 * Starts a specific DAQ list. This function initializes the DAQ list if it is
 * not already running and checks if the event channel is valid and has space
 * for the DAQ list. It calculates the total length of the DAQ entries and
 * sets the event buffer pointer based on the event channel consistency. If
 * the event buffer has enough space, it marks the DAQ list as running and
 * updates the session and event channel status. If the event buffer overflows
 * or the DAQ list is already running, it returns an error.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
XCP_LOCAL Std_ReturnType Xcp_StartDaq(Xcp_DaqNumType daqNum)
/* PRQA S 6030 -- */
{
    Xcp_DaqType*              daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);
    uint16                    evChNum    = daqInfoPtr->eventChannelNum;
    uint16                    len        = 0u;
    uint8                     odtCnt;
    uint8                     entryCnt;
    Std_ReturnType            result          = E_OK;
    const Xcp_EvChConfigType* eventChannelPtr = Xcp_PConfig.XcpEvCh;

    if (XCP_DAQ_MOD_RUNNING != (XCP_DAQ_MOD_RUNNING & (daqInfoPtr->mode)))
    {
        daqInfoPtr->eventBufferPos = 0u;
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
        daqInfoPtr->prescalerCnt = 0u;
#endif
        daqInfoPtr->currentPid = daqInfoPtr->firstPid;
        /* evChNum is valid and running daq in eventchannel < maxdaqlist */
        if ((evChNum < XCP_MAX_EVENT_CHANNEL)
            && (Xcp_ChannelEvChData.EvChActivCnt[evChNum] <= Xcp_PConfig.XcpEvCh[evChNum].XcpEvChMaxDaqList))
        {
            for (odtCnt = 0u; odtCnt < daqInfoPtr->daqListSize; odtCnt++)
            {
                for (entryCnt = 0u; entryCnt < daqInfoPtr->odt[odtCnt].XcpOdtEntryCount; entryCnt++)
                {
                    len += daqInfoPtr->odt[odtCnt].XcpOdtEntry[entryCnt].XcpOdtEntryLength;
                }
            }
            if ((len <= eventChannelPtr[evChNum].XcpEvChBuffDpth)
                || (XCP_EVENT_CONSIST_ODT == (eventChannelPtr[evChNum].XcpEvChConsistency)))
            {
                daqInfoPtr->totalEntryLen = len;
                /* set buffer ptr */
                if (XCP_EVENT_CONSIST_EVENT == (eventChannelPtr[evChNum].XcpEvChConsistency))
                {
                    /* for event consistency,put all daq to the Xcp_EvChxBuffer[] */
                    daqInfoPtr->eventBufferPtr =
                        &(eventChannelPtr[evChNum].XcpEvChBuffPtr[Xcp_ChannelEvChData.EvChBufferUsage[evChNum]]);
                    Xcp_ChannelEvChData.EvChBufferUsage[evChNum] += len;
                }
                else if (XCP_EVENT_CONSIST_DAQ == (eventChannelPtr[evChNum].XcpEvChConsistency))
                {
                    /* for DAQ consist and DAQ direction,put the daq buffer in the end.*/
                    len                        = eventChannelPtr[evChNum].XcpEvChBuffDpth - len;
                    daqInfoPtr->eventBufferPtr = &(eventChannelPtr[evChNum].XcpEvChBuffPtr[len]);
                }
                else /* odt consistency,do nothing */
                {
                    /* Do Nothing */
                }
                daqInfoPtr->mode |= XCP_DAQ_MOD_RUNNING;
                Xcp_ChannelCommonData.SessionStatus |= XCP_SESSION_STATE_DAQ_RUNNING;
                Xcp_ChannelEvChData.EvChActivCnt[evChNum]++;
            }
            else
            {
                /* event buffer memory overflow */
                result = E_NOT_OK;
            }
        }
    }
    else
    {
        result = E_NOT_OK;
    }
    return result;
}

/**
 * Stops a specific DAQ list. This function checks if the DAQ list is running
 * and, if so, clears the running status and send status flags. It decrements
 * the active DAQ count for the associated event channel. If the event channel
 * consistency is set to EVENT, it reduces the event buffer usage by the total
 * length of the DAQ entries.
 */
XCP_LOCAL void Xcp_StopDaq(Xcp_DaqNumType daqNum)
{
    Xcp_DaqType* daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqNum]);
    uint16       evChNum    = daqInfoPtr->eventChannelNum;

    if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & (daqInfoPtr->mode)))
    {
        /* clear Status flag */
        daqInfoPtr->mode &= (uint8)(~XCP_DAQ_MOD_RUNNING);
        daqInfoPtr->sendStatus &= (uint8)(~XCP_DAQ_SS_AL);
        Xcp_ChannelEvChData.EvChActivCnt[evChNum]--;
        if (XCP_EVENT_CONSIST_EVENT == (Xcp_PConfig.XcpEvCh[evChNum].XcpEvChConsistency))
        {
            Xcp_ChannelEvChData.EvChBufferUsage[evChNum] -= daqInfoPtr->totalEntryLen;
        }
    }
    return;
}

/**
 * Sets the DAQ pointer to a specified DAQ list, ODT, and ODT entry. This function
 * checks the command length, program status, and protection status. It retrieves
 * the DAQ list, ODT, and ODT entry numbers from the command buffer and validates
 * them. If the DAQ list is already running, it sets an error code. Otherwise, it
 * sets the DAQ pointer to the specified values. If any checks fail, it sets an
 * appropriate error code. Finally, it sends a response.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
void Xcp_SetDaqPtr(void)
/* PRQA S 6030 -- */
{
    uint16 daqListNo = XCP_U8_MAX;
    uint8  odtNo;
    uint8  odtEntryNo;
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_6) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
    {
#endif
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            /* get the daq List No from CommandRxObject[2-3] buffer */
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
            /* get the Odt number from CommandRxObject[4] buffer */
            odtNo = Xcp_CmdBuffer[XCP_CONST_4];
            /* get the Odt Entry number from CommandRxObject[5] buffer */
            odtEntryNo = Xcp_CmdBuffer[XCP_CONST_5];
            /* check the daq/odt/entry num */
            if ((daqListNo < XCP_MAX_DAQ)
#if (XCP_MIN_DAQ != 0u)
                && (daqListNo >= XCP_MIN_DAQ)
#endif
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
                && (0u != (Xcp_ChannelDaqData.DaqInfo)[daqListNo].daqListSize)
#endif
                && (odtNo < (Xcp_ChannelDaqData.DaqInfo)[daqListNo].daqListSize)
                && (odtEntryNo < (Xcp_ChannelDaqData.DaqInfo)[daqListNo].odt[odtNo].XcpOdtEntryCount))
            {
                /* check whether DAQ is running at this moment */
                if (XCP_DAQ_MOD_RUNNING == ((Xcp_ChannelDaqData.DaqInfo)[daqListNo].mode & XCP_DAQ_MOD_RUNNING))
                {
                    Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
                }
                else
                {
                    Xcp_ChannelDaqData.PtrDaq   = daqListNo; /* set daq point */
                    Xcp_ChannelDaqData.PtrOdt   = odtNo;
                    Xcp_ChannelDaqData.PtrEntry = odtEntryNo;
                }
            }
            else /* DAQ/ODT/entry num check failed */
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            }
        }
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    }
#endif
    Xcp_SendResp();
    return;
}

/**
 * Writes data to a specified DAQ entry. This function checks the command length,
 * program status, and protection status. It verifies that the DAQ list number
 * is within the valid range. If any checks fail, it sets an appropriate error
 * code. Otherwise, it calls `Xcp_WriteDaqHal` to perform the write operation.
 * Finally, it sends a response.
 */
void Xcp_WriteDaq(void)
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    /* PRQA S 2995 ++ */ /* VL_Xcp_2995 */
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_8) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    /* PRQA S 2995 -- */
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* Daq List Number used in the previous SET_DAQ_PTR has to be in the range */
        else if (
            (Xcp_ChannelDaqData.PtrDaq >= XCP_MAX_DAQ)
#if (0u != XCP_MIN_DAQ)
            || (Xcp_ChannelDaqData.PtrDaq < XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
        )
        {
            Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
        }
        else
        {
            Xcp_WriteDaqHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Writes data to a specified DAQ entry. This function retrieves the size of
 * the DAQ element, address extension, and address of the DAQ element from the
 * command buffer. It checks if the ODT and entry indices are valid, the DAQ
 * list type, and the memory address. It also verifies that the DAQ list is not
 * running and that the total length of the DAQ DTO frame does not exceed the
 * maximum allowed size. If all checks pass, it updates the ODT entry with the
 * provided data and increments the DAQ list pointer. If any checks fail, it
 * sets an appropriate error code.
 */
XCP_LOCAL void Xcp_WriteDaqHal(void)
{
    /* Note: BitOffSet don't support.*/
    uint8  sizeOfDaqElement;
    uint8  addressExtension;
    uint32 addressOfDaqElement = 0u;
    uint32 addressOfOdtEntry;

    uint8  entryNum;
    uint16 len = 0u;

    sizeOfDaqElement = Xcp_CmdBuffer[XCP_CONST_2];
    addressExtension = Xcp_CmdBuffer[XCP_CONST_3];
    /* get the addressOfDaqElement from CommandRxObject[4-7] buffer */
    Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[XCP_CONST_4], &addressOfDaqElement, (uint8)CPU_BYTE_ORDER);
    /* get the address */
    addressOfOdtEntry = Xcp_Mta2Ptr(addressExtension, addressOfDaqElement);
    if ((Xcp_ChannelDaqData.PtrOdt >= (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq].daqListSize)
        || (Xcp_ChannelDaqData.PtrEntry
            >= (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq].odt[Xcp_ChannelDaqData.PtrOdt].XcpOdtEntryCount))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
    }
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
    else if (
        ((DAQ == Xcp_PConfig.XcpDaqList[Xcp_ChannelDaqData.PtrDaq].XcpDaqListType)
         && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_DAQ))
        || ((STIM == Xcp_PConfig.XcpDaqList[Xcp_ChannelDaqData.PtrDaq].XcpDaqListType)
            && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_STIM))
        || ((DAQ_STIM == Xcp_PConfig.XcpDaqList[Xcp_ChannelDaqData.PtrDaq].XcpDaqListType)
            && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_DAQ) && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_STIM)))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
#endif
#if (XCP_CHECK_MEA_ADDR == STD_ON)
    /*Check DAQ address.*/
    else if (!Xcp_CheckMeaMem(addressOfOdtEntry, sizeOfDaqElement))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
#endif /*XCP_CHECK_MEA_ADDR == STD_ON*/
    /* check ptr whether valid */
    /* check whether DAQ Running at this moment */
    else if (
        XCP_DAQ_MOD_RUNNING == ((Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq].mode & XCP_DAQ_MOD_RUNNING))
    {
        Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
    }
    else
    {
        for (entryNum = 0u; entryNum < Xcp_ChannelDaqData.PtrEntry; entryNum++)
        {
            len += (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                       .odt[Xcp_ChannelDaqData.PtrOdt]
                       .XcpOdtEntry[entryNum]
                       .XcpOdtEntryLength;
        }
        len += sizeOfDaqElement;
        /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
        len *= XCP_AG;
        /* PRQA S 2985 -- */
        /* check length of this daq-dto frame */
        if (XCP_MAX_WRITEDAQ_SIZE >= len)
        {
            (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                .odt[Xcp_ChannelDaqData.PtrOdt]
                .XcpOdtEntry[Xcp_ChannelDaqData.PtrEntry]
                .XcpOdtEntryLength = sizeOfDaqElement;
            /* PRQA S 0306 ++ */ /* VL_Xcp_0306 */
            (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                .odt[Xcp_ChannelDaqData.PtrOdt]
                .XcpOdtEntry[Xcp_ChannelDaqData.PtrEntry]
                .XcpOdtEntryAddr = (Xcp_AGType*)addressOfOdtEntry;
            /* PRQA S 0306 -- */
            (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                .odt[Xcp_ChannelDaqData.PtrOdt]
                .XcpOdtEntry[Xcp_ChannelDaqData.PtrEntry]
                .XcpOdtEntryNumber = Xcp_ChannelDaqData.PtrEntry;
            /* The DAQ list pointer is auto post incremented*/
            Xcp_ChannelDaqData.PtrEntry++;
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_DAQ_CONFIG);
        }
    }
}

/**
 * Sets the mode of a DAQ list. This function checks the command length, program
 * status, and protection status. If any checks fail, it sets an appropriate error
 * code. Otherwise, it calls `Xcp_SetDaqListModeHal` to set the DAQ list mode.
 * Finally, it sends a response.
 */
void Xcp_SetDaqListMode(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    /* PRQA S 2995 ++ */ /* VL_Xcp_2995 */
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_8) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    /* PRQA S 2995 -- */
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection*/
        if ((XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
            || (XCP_PL_STIM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_STIM)))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_SetDaqListModeHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Sets the mode of a DAQ list. This function retrieves the mode, DAQ list number,
 * event channel number, and transmission rate prescaler from the command buffer.
 * It checks the input parameters, the legality of the mode settings, and whether
 * the DAQ list is running. If any checks fail, it sets an appropriate error code.
 * Otherwise, it updates the DAQ list mode and send status. If the configuration
 * is static, it also verifies the event channel number.
 */
XCP_LOCAL void Xcp_SetDaqListModeHal(void)
{
    uint8  mode;
    uint16 daqListNo = 0u;
    uint16 evChNo    = 0u;
    uint8  transRatePrescaler;

#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
    uint16 staticDaqCnt;
    /* Daq Priority don't support */
    const Xcp_EvChConfigType* evchPtr = Xcp_PConfig.XcpEvCh;
#endif
    Xcp_DaqType* daqInfoPtr;

    /* get the detail parmeters from CommandRxObject[1] buffer*/
    mode = Xcp_CmdBuffer[XCP_CONST_1];
    /* get the mode from CommandRxObject[2-3] buffer */
    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
    daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqListNo]);
    /* get the mode from CommandRxObject[4-5] buffer */
    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_4], &evChNo, (uint8)CPU_BYTE_ORDER);
    transRatePrescaler = Xcp_CmdBuffer[XCP_CONST_6];
    if (/* check input parameter */
        (evChNo >= XCP_MAX_EVENT_CHANNEL) || (daqListNo >= XCP_MAX_DAQ)
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
        || (0u == daqInfoPtr->daqListSize) /* dynamic,the daqlistsize can't eaqul 0 */
#endif
#if (STD_OFF == XCP_PRESCALER_SUPPORTED)
        || (0x01u != transRatePrescaler) /*without prescaler,the Prescaler must = 1*/
#else
        || (0u == transRatePrescaler) /*with prescaler,the Prescaler must >= 1*/
#endif
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else if (/* check if alternating is illegal */
             /* 1. only  DIRECTION=DAQ ,can set alternating flag*/
             ((XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_DIR_STIM) == (mode & (XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_DIR_BIT)))
             /* can not allowed to set the ALTERNATING flag and the TIMESTAMP flag at the same time. */
             || ((XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_TS) == (mode & (XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_TS)))
/* illegal timestamp disable */
#if (XCP_TS_NO_TIME_STAMP != XCP_TIMESTAMP_TYPE)
#if (XCP_TIMESTAMP_FIXED == STD_ON) /* If the Tinmestump fixed is on,so the mode must set XCP_DAQ_MOD_TS*/
             || (XCP_DAQ_MOD_TS != (XCP_DAQ_MOD_TS & mode))
#endif
#endif
/* illegal timestamp enable */
#if (XCP_TS_NO_TIME_STAMP == XCP_TIMESTAMP_TYPE)
             || (XCP_DAQ_MOD_TS
                 == (XCP_DAQ_MOD_TS & mode)) /* when ts_type is no_ts,so the mode can't set XCP_DAQ_MOD_TS */
#endif
/* illegal pid_off enable */
#if (STD_OFF == XCP_PID_OFF_SUPPORT)
             || (XCP_DAQ_MOD_PIDOFF == (XCP_DAQ_MOD_PIDOFF & mode))
#endif
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
             /* illegal direction */
             || ((XCP_DAQ_MOD_DIR_STIM
                  == (mode & XCP_DAQ_MOD_DIR_BIT)) /* DaqListtype in mode parameter is not match to XcpDaqList*/
                 && (DAQ == (Xcp_PConfig.XcpDaqList[daqListNo].XcpDaqListType)))
             || ((XCP_DAQ_MOD_DIR_DAQ == (mode & XCP_DAQ_MOD_DIR_BIT))
                 && (STIM == (Xcp_PConfig.XcpDaqList[daqListNo].XcpDaqListType)))
#endif
    )
    {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
    }
    /* check whether DAQ Running at this moment */
    else if (XCP_DAQ_MOD_RUNNING == (daqInfoPtr->mode & XCP_DAQ_MOD_RUNNING))
    {
        Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
    }
    else
    {
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
        /* check if the eventchannel Num is valid */
        for (staticDaqCnt = 0; staticDaqCnt < evchPtr[evChNo].XcpEvChMaxDaqList; staticDaqCnt++)
        {
            if (evchPtr[evChNo].XcpEvChTrigDaqListRef[staticDaqCnt] == daqListNo)
            {
                break;
            }
        }
        if (staticDaqCnt != evchPtr[evChNo].XcpEvChMaxDaqList)
#endif
        {
            daqInfoPtr->eventChannelNum = evChNo;
            /* first clear these flag */
            daqInfoPtr->sendStatus &= (uint8)(~XCP_DAQ_SS_AL);
            daqInfoPtr->sendStatus |= (mode & XCP_DAQ_SS_AL);
            daqInfoPtr->mode &= (uint8)(~(XCP_DAQ_MOD_DIR_BIT | XCP_DAQ_MOD_TS | XCP_DAQ_MOD_PIDOFF));
            daqInfoPtr->mode |= (mode & (XCP_DAQ_MOD_DIR_BIT | XCP_DAQ_MOD_TS | XCP_DAQ_MOD_PIDOFF));
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
            daqInfoPtr->prescaler = transRatePrescaler;
#endif
        }
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
#endif
    }
}

/**
 * Starts or stops a DAQ list. This function checks the command length, program
 * status, and protection status. If any checks fail, it sets an appropriate error
 * code. Otherwise, it calls `Xcp_StartStopDaqListHal` to handle the start or stop
 * operation of the DAQ list. Finally, it sends a response.
 */
void Xcp_StartStopDaqList(void)
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_4) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_StartStopDaqListHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Starts, stops, or selects a DAQ list. This function retrieves the mode and
 * DAQ list number from the command buffer. It checks if the DAQ list number
 * is valid and, if so, performs the requested operation (start, stop, or select).
 * If the operation is to stop the DAQ list, it checks if all DAQ lists are stopped
 * and updates the session status accordingly. If the operation is to start the DAQ
 * list, it calls `Xcp_StartDaq` and checks the result. If the mode is invalid,
 * it sets an appropriate error code.
 */
XCP_LOCAL void Xcp_StartStopDaqListHal(void)
{
    Std_ReturnType result;
    uint8          tMode;
    uint16         daqListNo = 0u;
    tMode                    = Xcp_CmdBuffer[XCP_CONST_1];
    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
    Xcp_DaqType* daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqListNo]);
    if (/* check daqListNo  */
        (daqListNo >= XCP_MAX_DAQ)
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
        || (0u == daqInfoPtr->daqListSize) || (daqInfoPtr->DynaCfgStatus != XCP_DAQ_CFG_DONE)
#endif
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else
    {
        Xcp_RespBuffer[XCP_CONST_1]      = daqInfoPtr->firstPid;
        Xcp_ChannelCommonData.RespLength = XCP_CONST_2;
        switch (tMode)
        {
        case XCP_START_STOP_MOD_STOP:
        {
            Xcp_StopDaq((Xcp_DaqNumType)daqListNo);
            /* Check that whether all DAQs are not running */
            for (daqListNo = 0u; daqListNo < XCP_MAX_DAQ; daqListNo++)
            {
                if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & daqInfoPtr->mode))
                {
                    break;
                }
            }
            if (daqListNo == XCP_MAX_DAQ) /* mean no daq is running */
            {
                Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
            }
            break;
        }
        case XCP_START_STOP_MOD_START:
        {
            result = Xcp_StartDaq((Xcp_DaqNumType)daqListNo);
            if ((Std_ReturnType)E_NOT_OK == result)
            {
                Xcp_SetErrorCode(XCP_ERR_DAQ_CONFIG);
            }
            break;
        }
        case XCP_START_STOP_MOD_SELECT:
        {
            daqInfoPtr->mode |= XCP_DAQ_MOD_SELE;
            break;
        }
        default:
        {
            Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);

            break;
        }
        }
    }
}

/**
 * Starts or stops synchronization. This function checks the command length, program
 * status, and protection status. If any checks fail, it sets an appropriate error
 * code. Otherwise, it calls `Xcp_StartStopSynchHal` to handle the synchronization
 * start or stop operation. Finally, it sends a response.
 */
void Xcp_StartStopSynch(void)
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_2) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_StartStopSynchHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Starts or stops synchronization for all or selected DAQ lists. This function
 * retrieves the mode from the command buffer. If the mode is to stop all DAQ
 * lists, it clears the running status for all DAQ lists and resets the session
 * status and event channel buffer usage. If the mode is to start or stop selected
 * DAQ lists, it iterates through the DAQ lists, applies the requested operation
 * to those marked for selection, and clears the selection flag. If the mode is
 * invalid, it sets an appropriate error code.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
XCP_LOCAL void Xcp_StartStopSynchHal(void)
/* PRQA S 6030 -- */
{
    uint16 cnt;
    uint8  tMode = Xcp_CmdBuffer[1u];

    if (tMode == XCP_START_STOP_MOD_STOP_ALL)
    {
        for (cnt = 0u; cnt < XCP_MAX_DAQ; cnt++)
        {
            (Xcp_ChannelDaqData.DaqInfo)[cnt].mode &= (uint8)(~XCP_DAQ_MOD_RUNNING);
        }
        Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
        for (cnt = 0u; cnt < XCP_MAX_EVENT_CHANNEL; cnt++)
        {
            Xcp_ChannelEvChData.EvChBufferUsage[cnt] = 0u;
            Xcp_ChannelEvChData.EvChActivCnt[cnt]    = 0u;
        }
    }
    else if (tMode <= XCP_START_STOP_MOD_STOP_SELECT)
    {
        for (cnt = 0u; cnt < XCP_MAX_DAQ; cnt++)
        {
            if (XCP_DAQ_MOD_SELE == (XCP_DAQ_MOD_SELE & (Xcp_ChannelDaqData.DaqInfo)[cnt].mode))
            {
                if (tMode == XCP_START_STOP_MOD_START_SELECT)
                {
                    (void)Xcp_StartDaq((Xcp_DaqNumType)cnt);
                }
                else
                {
                    Xcp_StopDaq((Xcp_DaqNumType)cnt);
                }
                (Xcp_ChannelDaqData.DaqInfo)[cnt].mode &= (uint8)(~XCP_DAQ_MOD_SELE);
            }
        }
    }
    else
    {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
    }
}

/* CMD:WRITE_DAQ_MULTIPLE:now don't support */

#if (STD_ON == XCP_READ_DAQ)
/**
 * Reads data from a specified DAQ entry. This function checks the command length,
 * program status, and protection status. It verifies that the DAQ list, ODT, and
 * entry indices are within the valid range. If any checks fail, it sets an
 * appropriate error code. Otherwise, it retrieves the size, address extension,
 * and address of the DAQ element, and prepares the response buffer. Finally, it
 * increments the entry pointer and sends the response.
 */
void Xcp_ReadDaq(void)
{
    uint32 elementaddress;

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 0x01u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* Daq List Number used in the previous SET_DAQ_PTR has to be in the range */
        else if (
            (Xcp_ChannelDaqData.PtrDaq >= XCP_MAX_DAQ)
            || (Xcp_ChannelDaqData.PtrOdt >= (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq].daqListSize)
            || (Xcp_ChannelDaqData.PtrEntry >= (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                                                   .odt[Xcp_ChannelDaqData.PtrOdt]
                                                   .XcpOdtEntryCount))
        {
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        }
        else
        {
            /* Position of bit in 32-bit variable */
            Xcp_RespBuffer[XCP_CONST_1] = XCP_U8_MAX;

            /* Size of DAQ element [AG] */
            Xcp_RespBuffer[XCP_CONST_2] = (Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                                              .odt[Xcp_ChannelDaqData.PtrOdt]
                                              .XcpOdtEntry[Xcp_ChannelDaqData.PtrEntry]
                                              .XcpOdtEntryLength;

            /* Address extension of DAQ element */
            Xcp_RespBuffer[XCP_CONST_3] = 0u;

            /* Address of DAQ element */
            /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
            elementaddress = (uint32)((Xcp_ChannelDaqData.DaqInfo)[Xcp_ChannelDaqData.PtrDaq]
                                          .odt[Xcp_ChannelDaqData.PtrOdt]
                                          .XcpOdtEntry[Xcp_ChannelDaqData.PtrEntry]
                                          .XcpOdtEntryAddr);
            /* PRQA S 0306,0309 -- */

            /* Put the ElementAddress to RespBuffer[4]-[7] buffer */
            Xcp_CopyU4ToU1Buffer(elementaddress, &Xcp_RespBuffer[XCP_CONST_4], (uint8)CPU_BYTE_ORDER);

            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;

            Xcp_ChannelDaqData.PtrEntry++;
        }
    Xcp_SendResp();

    return;
}
#endif /*STD_ON == XCP_READ_DAQ*/

/**
 * Retrieves the DAQ clock value. This function checks the command length, program
 * status, and protection status. If any checks fail, it sets an appropriate error
 * code. Otherwise, it retrieves the receive timestamp and prepares the response
 * buffer. Finally, it sends the response.
 */
#if ((STD_ON == XCP_GET_DAQ_CLOCK) && (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP))
void Xcp_GetDaqClock(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    else
    {
        /* Put the Receive Timestamp to RespBuffer[4]-[7] buffer */
        Xcp_CopyU4ToU1Buffer(
            (uint32)Xcp_ChannelDaqData.RxTimeStamp,
            &(Xcp_RespBuffer[XCP_CONST_4]),
            (uint8)CPU_BYTE_ORDER);
        Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
    }
    Xcp_SendResp();
    return;
}
#endif

#if (STD_ON == XCP_GET_DAQ_PROCESSOR_INFO)
/**
 * Retrieves DAQ processor information. This function checks the command length,
 * program status, and protection status. If any checks fail, it sets an appropriate
 * error code. Otherwise, it prepares the response buffer with DAQ properties,
 * maximum DAQ lists, maximum event channels, minimum DAQ list size, identification
 * field type, address extension flag, and optimization type. Finally, it sends
 * the response.
 */
void Xcp_GetDaqProcessorInfo(void)
{
    uint8 daqProperties;
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            /* daq properties depends on the macros configured in xcp_cfg.h */
            daqProperties =
                (XCP_DAQ_CONFIG_TYPE
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
                 | (XCP_CONST_2)
#endif /*XCP_PRESCALER_SUPPORTED == STD_ON*/
#if (XCP_RESUME_SUPPORT == STD_ON)
                 | (XCP_CONST_4)
#endif /*XCP_RESUME_SUPPORT == STD_ON*/
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
                 | (XCP_CONST_8)
#endif
#if (XCP_PID_OFF_SUPPORT == STD_ON)
                 | (0x20u)
#endif /*XCP_PID_OFF_SUPPORT == STD_ON*/
                 | ((uint8)(XCP_DAQ_OVL_INDICATION << XCP_CONST_6)));
/* TIMESTAMP_SUPPORTED */
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
            daqProperties |= XCP_CONST_16;
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP*/

            Xcp_RespBuffer[XCP_CONST_1] = daqProperties;
            Xcp_CopyU2ToU1Buffer((uint16)XCP_MAX_DAQ, &Xcp_RespBuffer[XCP_CONST_2], (uint8)CPU_BYTE_ORDER);
            Xcp_CopyU2ToU1Buffer((uint16)XCP_MAX_EVENT_CHANNEL, &Xcp_RespBuffer[XCP_CONST_4], (uint8)CPU_BYTE_ORDER);

            Xcp_RespBuffer[XCP_CONST_6] = XCP_MIN_DAQ;

            Xcp_RespBuffer[XCP_CONST_7] =
                (((uint8)(XCP_IDENTIFICATION_FIELD_TYPE << XCP_CONST_6)) /**< Identification Field Type*/
                 | ((uint8)(XCP_CONST_3 << XCP_CONST_4))                 /**< The ADDR EXTENSION flag */
                 | (0u));                                                /**< The Optimisation Type */

            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_DAQ_PROCESSOR_INFO*/

#if (STD_ON == XCP_GET_DAQ_RESOLUTION_INFO)
/**
 * Retrieves DAQ resolution information. This function checks the command length,
 * program status, and protection status. If any checks fail, it sets an appropriate
 * error code. Otherwise, it prepares the response buffer with the address granularity
 * for DAQ and STIM entries, and timestamp information if supported. Finally, it sends
 * the response.
 */
void Xcp_GetDaqResolutionInfo(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_RespBuffer[XCP_CONST_1] = XCP_AG;
            Xcp_RespBuffer[XCP_CONST_2] = XCP_ODT_ENTRY_SIZE_DAQ;
            Xcp_RespBuffer[XCP_CONST_3] = XCP_AG;
            Xcp_RespBuffer[XCP_CONST_4] = XCP_ODT_ENTRY_SIZE_STIM;
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TIME_STAMP)
            Xcp_RespBuffer[XCP_CONST_5] =
                (XCP_TIMESTAMP_TYPE | (uint8)(XCP_TIMESTAMP_FIXED << XCP_CONST_3)
                 | (uint8)((uint8)XCP_TIMESTAMP_UNIT << XCP_CONST_4));

            Xcp_CopyU2ToU1Buffer(XCP_TIMESTAMP_TICKS, &Xcp_RespBuffer[XCP_CONST_6], (uint8)CPU_BYTE_ORDER);
#else
        Xcp_RespBuffer[XCP_CONST_5] = 0u;
        Xcp_RespBuffer[XCP_CONST_6] = 0u;
        Xcp_RespBuffer[XCP_CONST_7] = 0u;
#endif
            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_DAQ_RESOLUTION_INFO*/

#if (STD_ON == XCP_GET_DAQ_LIST_MODE)
/**
 * Retrieves the mode of a specified DAQ list. This function checks the command length,
 * program status, and protection status. If any checks fail, it sets an appropriate
 * error code. Otherwise, it retrieves the DAQ list mode, current event channel number,
 * and prescaler (if supported), and prepares the response buffer. Finally, it sends
 * the response.
 */
void Xcp_GetDaqListMode(void)
{
    uint16 daqListNo = XCP_U8_MAX;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_4) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
            if (daqListNo < XCP_MAX_DAQ)
            {
                Xcp_RespBuffer[XCP_CONST_1] = (Xcp_ChannelDaqData.DaqInfo)[daqListNo].mode;

                /* Put the currentEventChannelNo to RespBuffer[4]-[5] buffer */
                Xcp_CopyU2ToU1Buffer(
                    (Xcp_ChannelDaqData.DaqInfo)[daqListNo].eventChannelNum,
                    &Xcp_RespBuffer[XCP_CONST_4],
                    (uint8)CPU_BYTE_ORDER);

#if (XCP_PRESCALER_SUPPORTED == STD_ON)
                Xcp_RespBuffer[XCP_CONST_6] = (Xcp_ChannelDaqData.DaqInfo)[daqListNo].prescaler;
#else
            Xcp_RespBuffer[XCP_CONST_6] = 0u;
#endif
                /* Daq Priority don't support */
                Xcp_RespBuffer[XCP_CONST_7] = 0u;
                /* response length is 8 */
                Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            }
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_DAQ_LIST_MODE*/

#if (STD_ON == XCP_GET_DAQ_EVENT_INFO)
/**
 * Retrieves information about a specified event channel. This function checks the
 * command length, program status, and protection status. If any checks fail, it
 * sets an appropriate error code. Otherwise, it retrieves the event channel type,
 * consistency, maximum DAQ lists, name length, name pointer, time cycle, time unit,
 * and priority, and prepares the response buffer. Finally, it sends the response.
 */
void Xcp_GetDaqEventInfo(void)
{
    uint16 eventChannelNo = XCP_U16_MAX;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_4) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &eventChannelNo, (uint8)CPU_BYTE_ORDER);

            if (eventChannelNo < XCP_MAX_EVENT_CHANNEL)
            {
                Xcp_RespBuffer[XCP_CONST_1] =
                    (uint8)(((uint8)((uint8)Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChType << XCP_CONST_2))
                            | ((uint8)((uint8)Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChConsistency << XCP_CONST_6)));
                Xcp_RespBuffer[XCP_CONST_2] = Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChMaxDaqList;

                /* EVENT_CHANNEL_NAME_LENGTH in bytes ,0 If not available */
                Xcp_RespBuffer[XCP_CONST_3] = Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChNameLength;

                /* set MTA for upload the event channel name */
                /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
                Xcp_ChannelCommonData.MTA = (uint32)(Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChNamePtr);
                /* PRQA S 0306,0309 -- */
                Xcp_ChannelCommonData.MTAExtension = 0x0u;

                Xcp_RespBuffer[XCP_CONST_4] = Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChTimeCycle;
                Xcp_RespBuffer[XCP_CONST_5] = (uint8)Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChTimeUnit;
                Xcp_RespBuffer[XCP_CONST_6] = Xcp_PConfig.XcpEvCh[eventChannelNo].XcpEvChPriority;

                Xcp_ChannelCommonData.RespLength = XCP_CONST_7;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            }
        }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_DAQ_EVENT_INFO == STD_ON*/

#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
/**
 * Clears a specified DAQ list. This function checks the command length, program
 * status, and protection status. If any checks fail, it sets an appropriate error
 * code. Otherwise, it stops and clears the specified DAQ list, checks if any DAQ
 * lists are still running, and updates the session status if necessary. Finally,
 * it sends the response.
 */
void Xcp_ClearDaqList(void)
{
    uint16 daqListNo;
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 4u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    else
    {
        Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
        if (daqListNo < XCP_MAX_DAQ)
        {
            Xcp_StopDaq((Xcp_DaqNumType)daqListNo);
            Xcp_ClearDaq((Xcp_DaqNumType)daqListNo);
            /* Check if there is no daq is running */
            for (daqListNo = 0; daqListNo < XCP_MAX_DAQ; daqListNo++)
            {
                if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & (Xcp_ChannelDaqData.DaqInfo)[daqListNo].mode))
                {
                    break;
                }
            }
            /* if no daq is running,reset the Running flag */
            if (daqListNo == XCP_MAX_DAQ)
            {
                Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
            }
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
    }
    Xcp_SendResp();
    return;
}

#if (XCP_GET_DAQ_LIST_INFO == STD_ON)
/**
 * Retrieves information about a specified DAQ list. This function checks the command
 * length, program status, and protection status. If any checks fail, it sets an
 * appropriate error code. Otherwise, it finds the event channel number associated
 * with the DAQ list, determines the DAQ list properties, and prepares the response
 * buffer with the maximum ODT count, maximum ODT entries, DAQ direction, and fixed
 * event channel status. Finally, it sends the response.
 */
void Xcp_GetDaqListInfo(void)
{
    uint8          daqproperty = 0;
    uint16         daqListNo;
    Xcp_DaqNumType daqNum;
    uint16         evChNum;
    uint16         fixedEvent  = 0;
    uint16         findEVCount = 0;

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
    if ((daqListNo < XCP_MIN_DAQ) || (daqListNo >= XCP_MAX_DAQ))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else
    {
        /* find event channel Number for this Daq */
        for (evChNum = 0u; evChNum < XCP_MAX_EVENT_CHANNEL; evChNum++)
        {
            for (daqNum = 0u; daqNum < (Xcp_PConfig.XcpEvCh[evChNum].XcpEvChMaxDaqList); daqNum++)
            {
                if (daqListNo == Xcp_PConfig.XcpEvCh[evChNum].XcpEvChTrigDaqListRef[daqNum])
                {
                    /* get the No of the Event Channel for the current daq list No */
                    fixedEvent = Xcp_PConfig.XcpEvCh[evChNum].XcpEvChNumber;
                    findEVCount++;
                    break;
                }
            }
            if (findEVCount
                >= XCP_CONST_2) /* Indicating that this DAQ has no fixed event channel,so the 4th response is null */
            {
                break;
            }
        }
    }
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 4u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else if (daqListNo < XCP_MAX_DAQ)
        {
/* max_odt & max_odt_entries*/
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
            Xcp_RespBuffer[XCP_CONST_2] = Xcp_PConfig.XcpDaqList[daqListNo].XcpMaxOdt;
            Xcp_RespBuffer[XCP_CONST_3] = Xcp_PConfig.XcpDaqList[daqListNo].XcpMaxOdtEntries;
#endif
            /* get daq direction */
            daqproperty = (uint8)((uint8)Xcp_PConfig.XcpDaqList[daqListNo].XcpDaqListType << XCP_CONST_2);
            /* predef of properties */
            if (daqListNo < XCP_MIN_DAQ)
            {
                daqproperty |= 0x01u;
            }
            Xcp_RespBuffer[XCP_CONST_1] = daqproperty;
            /* Bit fixed event */
            switch (findEVCount)
            {
            /* No event channel can be asigned to this daq */
            case 0x00u:
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

                break;
            }
            case 0x01u:
            { /* EVENT_FIXED */
                Xcp_RespBuffer[XCP_CONST_1] |= 0x02u;
                Xcp_ChannelCommonData.RespLength = 0x06u;
                break;
            }
            /* not fixed */
            default:
            {
                Xcp_ChannelCommonData.RespLength = 0x06u;
                break;
            }
            }
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_DAQ_LIST_INFO == STD_ON*/
/*
 * DYNAMIC CONFIG COMMAND
 */
#else
/**
 * Frees all DAQ lists. This function checks the command length, program status,
 * and protection status. If any checks fail, it sets an appropriate error code.
 * Otherwise, it stops all DAQ lists, resets the session status, clears all event
 * channel buffer usage, and frees all DAQ resources. Finally, it updates the DAQ
 * configuration state and sends the response.
 */
void Xcp_FreeDaq(void)
{
    uint16 cnt;

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            SchM_Enter_Xcp_Exclusive_Daq();
            for (cnt = 0u; cnt < XCP_MAX_DAQ; cnt++)
            {
                (Xcp_ChannelDaqData.DaqInfo)[cnt].mode &= (uint8)(~XCP_DAQ_MOD_RUNNING);
            }
            Xcp_ChannelCommonData.SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
            for (cnt = 0u; cnt < XCP_MAX_EVENT_CHANNEL; cnt++)
            {
                Xcp_ChannelEvChData.EvChBufferUsage[cnt] = 0u;
                Xcp_ChannelEvChData.EvChActivCnt[cnt]    = 0u;
            }
            Xcp_ClearAllDaq();
            /* The XCP_DAQ_CFG_FREE state must be modified after FREE_DAQ is successful*/
            Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_FREE;
            Xcp_ChannelDaqData.DynAllocDaqNum  = 0u;
            SchM_Exit_Xcp_Exclusive_Daq();
        }
    Xcp_SendResp();
    return;
}

/**
 * Allocates DAQ resources. This function checks the command length, program status,
 * protection status, and DAQ configuration sequence. If any checks fail, it sets an
 * appropriate error code. Otherwise, it allocates DAQ resources using the hardware
 * abstraction layer. Finally, it sends the response.
 */
void Xcp_AllocDaq(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_4) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        if (XCP_PL_DAQ == (XCP_PL_DAQ & Xcp_ChannelCommonData.ProtectionStatus))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    /* check daq cfg sequence */
    /* ALLOC_DAQ can use after Free_Daq or Alloc_daq */
    else if (
        (XCP_DAQ_CFG_FREE != Xcp_ChannelDaqData.DynDaqSeqStatus)
        && (XCP_DAQ_CFG_DAQ != Xcp_ChannelDaqData.DynDaqSeqStatus))

    {
        Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_IDLE;
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
    }
    else
    {
        Xcp_AllocDaqHal();
    }
    Xcp_SendResp();
    return;
}

/**
 * Allocates DAQ resources at the hardware level. This function checks the requested
 * DAQ count. If the count is out of range, it sets an appropriate error code.
 * Otherwise, it configures the specified number of DAQ lists as allocated and sets
 * the remaining DAQ lists to free. It also updates the DAQ configuration state and
 * the number of allocated DAQ lists. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_AllocDaqHal(void)
{
    uint16         daqCount = 0u;
    Xcp_DaqNumType daqNum;

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqCount, (uint8)CPU_BYTE_ORDER);
    /* check daqCount */
    if (daqCount > XCP_DAQ_COUNT)
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else
    {
        SchM_Enter_Xcp_Exclusive_Daq();
        /* PRQA S 2986 ++ */ /* VL_Xcp_2986 */
        uint16 allocDaqNum = XCP_MIN_DAQ + daqCount;
        /* PRQA S 2986 -- */
        for (daqNum = XCP_MIN_DAQ; daqNum < allocDaqNum; daqNum++)
        {
            (Xcp_ChannelDaqData.DaqInfo)[daqNum].DynaCfgStatus = XCP_DAQ_CFG_DAQ;
        }
        /* The remaining DAQ status is configured as XCP_DAQ_CFG_FREE */
        for (; daqNum < XCP_MAX_DAQ; daqNum++)
        {
            (Xcp_ChannelDaqData.DaqInfo)[daqNum].DynaCfgStatus = XCP_DAQ_CFG_FREE;
        }
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
        Xcp_ChannelDaqData.DynDaqFirstPIDCnt = 0u;
#endif
        Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_DAQ;
        Xcp_ChannelDaqData.DynAllocDaqNum  = daqCount;
        SchM_Exit_Xcp_Exclusive_Daq();
    }
    return;
}

/**
 * Allocates ODT (Object Descriptor Table) entries. This function checks the command
 * length, program status, protection status, and DAQ configuration sequence. If any
 * checks fail, it sets an appropriate error code. Otherwise, it allocates ODT entries
 * using the hardware abstraction layer. Finally, it sends the response.
 */
void Xcp_AllocOdt(void)
{

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_5) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection */
        if (XCP_PL_DAQ == (XCP_PL_DAQ & Xcp_ChannelCommonData.ProtectionStatus))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* check daq cfg sequence */
        /* ALLOC_ODT can use after ALLOC_DAQ or ALLOC_ODT */
        else if (
            (XCP_DAQ_CFG_DAQ != Xcp_ChannelDaqData.DynDaqSeqStatus)
            && (XCP_DAQ_CFG_ODT != Xcp_ChannelDaqData.DynDaqSeqStatus))
        {
            Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_IDLE;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        else
        {
            Xcp_AllocOdtHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Allocates ODT (Object Descriptor Table) entries at the hardware level. This function
 * checks the DAQ list number and ODT count for validity, ensures the correct DAQ
 * configuration sequence, and verifies sufficient buffer space. If any checks fail,
 * it sets an appropriate error code. Otherwise, it allocates the ODT entries, updates
 * the DAQ list size, and initializes the ODT entries. Finally, it updates the DAQ
 * configuration state. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_AllocOdtHal(void)
{
    uint16 daqListNo = 0u;
    uint8  odtCount  = Xcp_CmdBuffer[XCP_CONST_4];

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
    Xcp_DaqType* daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqListNo]);

    if ((daqListNo >= XCP_MAX_DAQ) || (odtCount > XCP_ODT_COUNT)
#if (0u != XCP_MIN_DAQ)
        || (daqListNo < XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else if ((XCP_DAQ_CFG_DAQ != daqInfoPtr->DynaCfgStatus) && (XCP_DAQ_CFG_ODT != daqInfoPtr->DynaCfgStatus))
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
    }
    else if (Xcp_ChannelDaqData.DynDaqBufSize < (Xcp_ChannelDaqData.DynDaqBufPos + (odtCount * sizeof(Xcp_OdtType))))
    {
        Xcp_SetErrorCode(XCP_ERR_MEMORY_OVERFLOW);
    }
    else
    {
        SchM_Enter_Xcp_Exclusive_Daq();
        daqInfoPtr->daqListSize = odtCount;

#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
        daqInfoPtr->firstPid = Xcp_ChannelDaqData.DynDaqFirstPIDCnt;
        Xcp_ChannelDaqData.DynDaqFirstPIDCnt += odtCount;
#endif
        /*Add by Qinchun.yang
         * 2020/05/08
         * Pointer access requires four-byte alignment
         * */
        /* PRQA S 0306,0309,1252 ++ */ /* VL_Xcp_0306,VL_Xcp_0309,VL_Xcp_1252 */
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
        while (((uint32)(&(Xcp_ChannelDaqData.DynDaqBufPtr)[Xcp_ChannelDaqData.DynDaqBufPos]) % XCP_CONST_4) != 0uL)
        /* PRQA S 0306,0309,1252 -- */
        {
            Xcp_ChannelDaqData.DynDaqBufPos++;
        }
#endif
        /*
         * Note:Ignore the warning, the problem has been solved by code.
         * */
        /* PRQA S 3305,0310,0759 ++ */ /* VL_Xcp_3305,VL_Xcp_0310,VL_Xcp_0759 */
        Xcp_OdtPtrUnion odtPtrUnion;
        odtPtrUnion.u8ptr = &(Xcp_ChannelDaqData.DynDaqBufPtr)[Xcp_ChannelDaqData.DynDaqBufPos];
        daqInfoPtr->odt   = odtPtrUnion.odtPtr;
        /* PRQA S 3305,0310,0759 -- */
        Xcp_ChannelDaqData.DynDaqBufPos += (uint16)(odtCount * sizeof(Xcp_OdtType));
        do
        {
            odtCount--;
            daqInfoPtr->odt[odtCount].XcpOdtEntryCount = 0u;
        } while (odtCount != 0x00u);
        daqInfoPtr->DynaCfgStatus          = XCP_DAQ_CFG_ODT;
        Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_ODT;
        SchM_Exit_Xcp_Exclusive_Daq();
    }
    return;
}

/**
 * Allocates ODT entry resources. This function checks the command length, program
 * status, protection status, and DAQ configuration sequence. If any checks fail, it
 * sets an appropriate error code. Otherwise, it allocates ODT entry resources using
 * the hardware abstraction layer. Finally, it sends the response.
 */
void Xcp_AllocOdtEntry(void)
{

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_6) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection */
        if (XCP_PL_DAQ == (XCP_PL_DAQ & Xcp_ChannelCommonData.ProtectionStatus))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* check daq cfg sequence */
        else if (
            (XCP_DAQ_CFG_ODT != Xcp_ChannelDaqData.DynDaqSeqStatus)
            && (XCP_DAQ_CFG_ENTRY != Xcp_ChannelDaqData.DynDaqSeqStatus))
        {
            Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_IDLE;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        else
        {
            Xcp_AllocOdtEntryHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Allocates ODT entry resources at the hardware level. This function checks the
 * DAQ list number, ODT number, and entry count for validity, ensures the correct
 * DAQ configuration sequence, and verifies sufficient buffer space. If any checks
 * fail, it sets an appropriate error code. Otherwise, it allocates the ODT entry
 * resources, updates the entry count, and initializes the ODT entries. It also
 * updates the DAQ configuration state to reflect the allocation. This function is
 * intended for internal use.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
XCP_LOCAL void Xcp_AllocOdtEntryHal(void)
/* PRQA S 6030 -- */
{
    uint16 daqListNo  = 0u;
    uint8  odtNum     = Xcp_CmdBuffer[XCP_CONST_4];
    uint8  entryCount = Xcp_CmdBuffer[XCP_CONST_5];
    uint8  daqListSize;

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &daqListNo, (uint8)CPU_BYTE_ORDER);
    Xcp_DaqType* daqInfoPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqListNo]);
    if ((daqListNo >= XCP_MAX_DAQ) || (entryCount > XCP_ODTENTRY_COUNT)
#if (0u != XCP_MIN_DAQ)
        || (daqListNo < XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else if ((XCP_DAQ_CFG_DAQ == daqInfoPtr->DynaCfgStatus) || (XCP_DAQ_CFG_FREE == daqInfoPtr->DynaCfgStatus))
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
    }
    /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
    else if (
        Xcp_ChannelDaqData.DynDaqBufSize < (Xcp_ChannelDaqData.DynDaqBufPos + (entryCount * sizeof(Xcp_EntryType))))
    /* PRQA S 2985 -- */
    {
        Xcp_SetErrorCode(XCP_ERR_MEMORY_OVERFLOW);
    }
    else
    {
        daqInfoPtr->odt[odtNum].XcpOdtEntryCount = entryCount;
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
        /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
        while (((uint32)(&(Xcp_ChannelDaqData.DynDaqBufPtr)[Xcp_ChannelDaqData.DynDaqBufPos]) % XCP_CONST_4L) != 0uL)
        /* PRQA S 0306,0309 -- */
        {
            Xcp_ChannelDaqData.DynDaqBufPos++;
        }
#endif
        /*
         * Note:Ignore the warning, the problem has been solved by code.
         * */
        /* PRQA S 3305,0310,0759 ++ */ /* VL_Xcp_3305,VL_Xcp_0310,VL_Xcp_0759 */
        Xcp_OdtEntryPtrUnion odtEntryPtrUnion;
        odtEntryPtrUnion.u8ptr              = &(Xcp_ChannelDaqData.DynDaqBufPtr)[Xcp_ChannelDaqData.DynDaqBufPos];
        daqInfoPtr->odt[odtNum].XcpOdtEntry = odtEntryPtrUnion.odtEntryPtr;
        /* PRQA S 3305,0310,0759 -- */
        /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
        Xcp_ChannelDaqData.DynDaqBufPos += (uint16)(entryCount * sizeof(Xcp_EntryType));
        /* PRQA S 2985 -- */
        do
        {
            if (entryCount > 0u)
            {
                entryCount--;
            }
            /* PRQA S 3305 ++ */ /* VL_Xcp_3305 */
            daqInfoPtr->odt[odtNum].XcpOdtEntry[entryCount].XcpOdtEntryLength = 0u;
            /* PRQA S 3305 -- */
        } while (entryCount != 0x00u);
        daqInfoPtr->DynaCfgStatus          = XCP_DAQ_CFG_ENTRY;
        Xcp_ChannelDaqData.DynDaqSeqStatus = XCP_DAQ_CFG_ENTRY;
        daqListSize                        = daqInfoPtr->daqListSize;
        for (odtNum = 0u; odtNum < daqListSize; odtNum++)
        {
            if (0x00u == daqInfoPtr->odt[odtNum].XcpOdtEntryCount)
            {
                break;
            }
        }
        /* When all ODTs in the DAQ are assigned ENTRY */
        if (odtNum == daqInfoPtr->daqListSize)
        {
            daqInfoPtr->DynaCfgStatus = XCP_DAQ_CFG_DONE;
        }
    }
    return;
}
/* PRQA S 1532 -- */

#endif /*XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE*/
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /*XCP_PL_DAQ == XCP_PL_DAQ&XCP_RESOURCE*/
