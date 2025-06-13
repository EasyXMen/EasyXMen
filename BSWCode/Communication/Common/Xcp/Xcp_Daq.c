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
**  FILENAME    : Xcp_Daq.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation of the XCP_Daq command                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Internal.h"
#include "Xcp_Interface.h"
#include "SchM_Xcp.h"
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
/*=======[M A C R O S]========================================================*/
/* XCP start stop mode */
#define XCP_START_STOP_MOD_STOP   0x00u
#define XCP_START_STOP_MOD_START  0x01u
#define XCP_START_STOP_MOD_SELECT 0x02u

/* XCP start stop synch */
#define XCP_START_STOP_MOD_STOP_ALL     0x00u
#define XCP_START_STOP_MOD_START_SELECT 0x01u
#define XCP_START_STOP_MOD_STOP_SELECT  0x02u

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

/*=======[I N T E R N A L   D A T A]==========================================*/

#define XCP_START_SEC_VAR_INIT_16
#include "Xcp_MemMap.h"
/* DAQ Ptr ,for CMD:SET_DAQ_PTR*/
static VAR(uint16, XCP_VAR_INIT_16) Xcp_PtrDaq = 0xffffu;
#define XCP_STOP_SEC_VAR_INIT_16
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"
/* DAQ Ptr ,for CMD:SET_DAQ_PTR*/
static VAR(uint8, XCP_VAR_INIT_8) Xcp_PtrOdt = 0xffu;
static VAR(uint8, XCP_VAR_INIT_8) Xcp_PtrEntry = 0xffu;
#define XCP_STOP_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
/* dynamic config sequence statemachine */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
static VAR(uint8, XCP_VAR) Xcp_DynamicDaqFirstPIDCnt;
#endif
#endif
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

/* DAQ Queue */
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
static VAR(Xcp_DaqNumType, XCP_VAR) Xcp_DaqListQueuePos;
static VAR(Xcp_DaqNumType, XCP_VAR) Xcp_DaqListQueueLength;
static VAR(Xcp_DaqNumType, XCP_VAR) Xcp_MaxQueueLength;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
/* dynamic config sequence statemachine */
static VAR(Xcp_DaqCfgStatusType, XCP_VAR) Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_IDLE;
#endif
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"

#if (XCP_GET_DAQ_CLOCK == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(uint32, XCP_VAR) Xcp_RxTimeStamp;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
#endif /* XCP_GET_DAQ_CLOCK == STD_ON */

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
static VAR(uint8, XCP_VAR) Xcp_DaqBuffer[XCP_DTO_BUFFER_SIZE]; /*PRQA S 1533*/ /* PRQA S 3218 */ /* MISRA Rule 8.9 */
static VAR(uint8, XCP_VAR_NO_INIT_8)
    Xcp_EvChActivCnt[XCP_MAX_EVENT_CHANNEL]; /* counts of running daq in eventchannel */
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
/* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
static VAR(uint8, XCP_VAR) Xcp_DynDaqBuffer[XCP_DYNAMIC_DAQ_BUFFER_SIZE];
/* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC*/

#define XCP_START_SEC_VAR_CLEARED_16
#include "Xcp_MemMap.h"
static VAR(Xcp_DaqNumType, XCP_VAR) Xcp_DaqListQueue[XCP_MAX_DAQ];
#define XCP_STOP_SEC_VAR_CLEARED_16
#include "Xcp_MemMap.h"

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
static VAR(Xcp_DynamicalDaqQueueType, XCP_VAR) Xcp_DynamicalDaqQueue = {XCP_DYNAMIC_DAQ_BUFFER_SIZE, 0};
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC*/

#define XCP_START_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
static VAR(uint32, XCP_VAR) Xcp_EvChBufferUsage[XCP_MAX_EVENT_CHANNEL];
#define XCP_STOP_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static FUNC(void, XCP_CODE) Xcp_DaqResumeInit(void);
static FUNC(void, XCP_CODE) Xcp_DaqCfgBufferInit(void);
static FUNC(void, XCP_CODE) Xcp_ClearAllDaq(void);
static FUNC(void, XCP_CODE) Xcp_ClearDaq(Xcp_DaqNumType daqNum);
static FUNC(void, XCP_CODE) Xcp_ClearDaqOdt(Xcp_DaqNumType daqNum);

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(void, XCP_CODE) Xcp_FillEventBuffer(P2VAR(Xcp_DaqType, AUTOMATIC, AUTOMATIC) daqPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
static FUNC(void, XCP_CODE) Xcp_EventChannelDaqHal(uint16 eventNum, const Xcp_DaqNumType daqNum);
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(Std_ReturnType, XCP_CODE) Xcp_SampleAndTransmit(P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) daqPtr);
static FUNC(Std_ReturnType, XCP_CODE) Xcp_SendDaqFrame(P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) daqPtr, boolean overLoad);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
static FUNC(Std_ReturnType, XCP_CODE) Xcp_StartDaq(Xcp_DaqNumType daqNum);
static FUNC(void, XCP_CODE) Xcp_StopDaq(Xcp_DaqNumType daqNum);
static FUNC(void, XCP_CODE) Xcp_WriteDaqHal(void);
static FUNC(void, XCP_CODE) Xcp_SetDaqListModeHal(void);
static FUNC(void, XCP_CODE) Xcp_StartStopDaqListHal(void);
static FUNC(void, XCP_CODE) Xcp_StartStopSynchHal(void);

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
static FUNC(void, XCP_CODE) Xcp_AllocDaqHal(void);
static FUNC(void, XCP_CODE) Xcp_AllocOdtHal(void);
static FUNC(void, XCP_CODE) Xcp_AllocOdtEntryHal(void);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE)
Xcp_DAQInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if (0u != Xcp_SessionCfgId)
    {
        /*Resume action*/
        Xcp_DaqResumeInit();
    }
    else
    {
        Xcp_DAQReinit();
    }
    return;
}

/*Help processing resume action*/
static FUNC(void, XCP_CODE) Xcp_DaqResumeInit(void)
{
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_APPL_DATA) daqPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
    uint32 ts; /*Current slave Timestamp*/
#endif         /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/
    uint16 daqInx;
    uint16 evchNum;

    Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
    Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_RESUME_MODE;
    Xcp_CopyU2ToU1Buffer(Xcp_SessionCfgId, &(Xcp_EvBuffer[XCP_EV_RESUME_SSID_OFFEST]), CPU_BYTE_ORDER);
    Xcp_EvLength = 4u;
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
    ts = Xcp_ReadOutMS();
    Xcp_CopyU4ToU1Buffer((uint32)ts, &(Xcp_EvBuffer[XCP_EV_RESUME_TS_OFFEST]), CPU_BYTE_ORDER);
    Xcp_EvLength = 8u;
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/
    Xcp_SendEv();
    for (daqInx = 0u; daqInx < XCP_MAX_DAQ; daqInx++)
    {
        daqPtr = &(Xcp_Daq[daqInx]);
        if (XCP_DAQ_MOD_RESUME == (daqPtr->mode & XCP_DAQ_MOD_RESUME))
        {
            daqPtr->mode |= XCP_DAQ_MOD_RUNNING;
            evchNum = daqPtr->eventChannelNum;
            Xcp_EvChActivCnt[evchNum]++;
        }
    }
}

FUNC(void, XCP_CODE)
Xcp_DAQReinit(void)
{
    /* initlization the daq odt Ptr */
    Xcp_DaqCfgBufferInit();
    /* clear all DAQ */
    Xcp_ClearAllDaq();
    return;
}

static FUNC(void, XCP_CODE) Xcp_DaqCfgBufferInit(void)
{
    P2CONST(Xcp_DaqListConfigType, AUTOMATIC, XCP_CONST_PBCFG) daqCfgRef = Xcp_GlobalCfgPtr->XcpDaqList;
    uint16 Num;
    /* clear event usage counter */
    for (Num = 0; Num < XCP_MAX_EVENT_CHANNEL; Num++)
    {
        Xcp_EvChBufferUsage[Num] = 0;
        Xcp_EvChActivCnt[Num] = 0;
    }
    for (Num = 0; Num < XCP_MAX_DAQ; Num++)
    {
        Xcp_Daq[Num].odt = daqCfgRef[Num].XcpOdt;
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
        Xcp_Daq[Num].daqListSize = daqCfgRef[Num].XcpMaxOdt;
#else
        Xcp_Daq[Num].daqListSize = daqCfgRef[Num].XcpDaqSize; /*will be init 0*/
#endif
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
        Xcp_Daq[Num].firstPid = daqCfgRef[Num].XcpDto->XcpDtoPid;
#else
        Xcp_Daq[Num].firstPid = 0u;
#endif /*XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC*/
#else
        Xcp_Daq[Num].firstPid = 0;
#endif /*XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE*/
    }
    return;
}

static FUNC(void, XCP_CODE) Xcp_ClearAllDaq(void)
{
    Xcp_DaqNumType daqNum;
    Xcp_PtrDaq = 0xffffu; /*set the ptr is invalid*/
    Xcp_PtrOdt = 0xffu;
    Xcp_PtrEntry = 0xffu;

    Xcp_DaqListQueueLength = 0;
    Xcp_DaqListQueuePos = 0;
    Xcp_MaxQueueLength = 0;
    for (daqNum = 0; daqNum < XCP_MAX_DAQ; daqNum++)
    {
        Xcp_ClearDaq(daqNum);
    }
    /* initial Xcp_SessionStatus and keep the info of the DAQ Store Request */
    Xcp_SessionStatus &= (uint8)(~XCP_SESSION_STATE_RESUME);

#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_IDLE;
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
    Xcp_DynamicDaqFirstPIDCnt = 0;
#endif
    Xcp_DynamicalDaqQueue.bufferPos = 0; /* Record the location where the array is used */
#endif
}

static FUNC(void, XCP_CODE) Xcp_ClearDaq(Xcp_DaqNumType daqNum)
{
    /* clear send state machine */
    Xcp_Daq[daqNum].sendStatus = 0x00;
    /* reinit. mode and if the DAQ list only support STIM mode shall be 0x02 */
    Xcp_Daq[daqNum].mode = 0x00;
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
    if (Xcp_GlobalCfgPtr->XcpDaqList[daqNum].XcpDaqListType == STIM)
    {
        Xcp_Daq[daqNum].mode |= XCP_DAQ_MOD_DIR_STIM;
    }
#endif
    /* init currentPid */
    Xcp_Daq[daqNum].currentPid = Xcp_Daq[daqNum].firstPid;
/* init priority */
#if (XCP_DAQ_PRIORITY_SUPPORT == STD_ON)
    Xcp_Daq[daqNum].priority = 0x00;
#endif
/* init prescaler and cnt */
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
    Xcp_Daq[daqNum].prescaler = 0x01;
    Xcp_Daq[daqNum].prescalerCnt = 0x00;
#endif
    /* set eventchannel Num to a invaild value */
    Xcp_Daq[daqNum].eventChannelNum = XCP_MAX_EVENT_CHANNEL;
    /* init event buffer */
    Xcp_Daq[daqNum].eventBufferPos = 0x00;
    Xcp_Daq[daqNum].eventBufferPtr = NULL_PTR;

/* for configurable DAQ clear ODT and ODT Entries */
#if (0u != XCP_MIN_DAQ)
    if (daqNum >= XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
    {
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
        Xcp_Daq[daqNum].DynaCfgStatus = XCP_DAQ_CFG_FREE;
#endif
        Xcp_ClearDaqOdt(daqNum);
    }
#if (0u != XCP_MIN_DAQ)
    else
    {
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
        Xcp_Daq[daqNum].DynaCfgStatus = XCP_DAQ_CFG_DONE;
#endif
    }
#endif /*0u != XCP_MIN_DAQ*/
    return;
}

static FUNC(void, XCP_CODE) Xcp_ClearDaqOdt(Xcp_DaqNumType daqNum)
{
    uint8 odtNum;
    uint8 entryNum;
    P2VAR(Xcp_OdtType, AUTOMATIC, XCP_VAR) odtRef = Xcp_Daq[daqNum].odt; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(Xcp_EntryType, AUTOMATIC, XCP_VAR) entryRef; /* PRQA S 3432 */                   /* MISRA Rule 20.7 */
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    if (NULL_PTR != odtRef)
#endif
    {
        for (odtNum = 0; odtNum < Xcp_Daq[daqNum].daqListSize; odtNum++)
        {
            entryRef = odtRef[odtNum].XcpOdtEntry;
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
            if (entryRef != NULL_PTR)
#endif
            {
                for (entryNum = 0; entryNum < odtRef[odtNum].XcpOdtEntryCount; entryNum++)
                {
                    entryRef[entryNum].XcpOdtEntryNumber = 0xFF; /*set entry index is invalid*/
                    entryRef[entryNum].XcpOdtEntryAddr = NULL_PTR;
                    entryRef[entryNum].XcpOdtEntryLength = 0;
                }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
                odtRef[odtNum].XcpOdtEntry = NULL_PTR;
#endif
            }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
            odtRef[odtNum].XcpOdtEntryCount = 0;
#endif
        }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
        Xcp_Daq[daqNum].odt = NULL_PTR;
#endif
    }
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_DYNAMIC)
    Xcp_Daq[daqNum].daqListSize = 0x00;
#endif
    return;
}

/* DAQ-DAQ proceessor */
/******************************************************************************/
/*
 * @brief               <fill event Buffer for event consistency purpose >
 *
 * < fill event Buffer for event consistency purpose >.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <daqPtr pointer to the daq that shall be put into a buffer>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
static FUNC(void, XCP_CODE) Xcp_FillEventBuffer(P2VAR(Xcp_DaqType, AUTOMATIC, AUTOMATIC) daqPtr)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
    P2CONST(Xcp_EntryType, AUTOMATIC, AUTOMATIC) entryPtr;
    uint8 odtNum;           /* odt counter */
    uint8 entryNum;         /* entry conter */
    uint8 elementSizeCnt;   /* element counter */
    uint32 eventBufPos = 0; /* dest. counter */

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
            if (0x00u == entryPtr[entryNum].XcpOdtEntryLength)
            {
                break;
            }
            /* copy element to event buffer */
            for (elementSizeCnt = 0u; elementSizeCnt < entryPtr[entryNum].XcpOdtEntryLength; elementSizeCnt++)
            {
                daqPtr->eventBufferPtr[eventBufPos] = entryPtr[entryNum].XcpOdtEntryAddr[elementSizeCnt];
                eventBufPos++;
            }
        }
    }
/* update timestamp if needed */
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
    daqPtr->timeStamp = Xcp_ReadOutMS();
#endif
    return;
}

/******************************************************************************/
/*
 * @brief      <package a DAQ pdu and send>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <daqPtr     pointer to the DAQ that shall be sent
 *                       overload   flag to indicated wheather the first bit of the
                                    PID shall be set to reportting the overload>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <Std_ReturnType>
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(Std_ReturnType, XCP_CODE) Xcp_SendDaqFrame(P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) daqPtr, boolean overLoad)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    P2CONST(Xcp_OdtType, AUTOMATIC, AUTOMATIC) odtPtr = &(daqPtr->odt[daqPtr->currentPid - daqPtr->firstPid]);
    P2CONST(Xcp_EntryType, AUTOMATIC, AUTOMATIC) entryPtr;

    Xcp_DaqNumType DaqNum = Xcp_DaqListQueue[Xcp_DaqListQueuePos];

    uint16 pos; /* DTO buffer Position */
    P2CONST(Xcp_AGType, AUTOMATIC, AUTOMATIC) sourPtr;
    P2VAR(Xcp_AGType, AUTOMATIC, XCP_VAR) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 entryNum;                                                  /* entry counter */
    uint8 elementCnt;                                                /* element counter */
    uint16 destCnt = 0u;                                             /* dest Position counter */
    PduIdType pduId;

    SchM_Enter_Xcp_Exclusive_Daq();
    /* set PID Num*/
    if ((boolean)TRUE == overLoad)
    { /* to pid overflow indication flag */
        Xcp_DaqBuffer[XCP_PID_OFFSET] = ((daqPtr->currentPid) | ((uint8)XCP_PID_OVFLOW));
    }
    else
    {
        Xcp_DaqBuffer[XCP_PID_OFFSET] = daqPtr->currentPid;
    }

#if (XCP_PID_OFF_SUPPORT == STD_ON)
    if (XCP_DAQ_MOD_PIDOFF == (XCP_DAQ_MOD_PIDOFF & Xcp_Daq[DaqNum].mode))
    {
        pos = 0u;
    }
    else
#endif
    {
/* set DAQ Num */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_BYTE)
        Xcp_DaqBuffer[XCP_DAQ_NUM_OFFSET] = DaqNum;
#elif (XCP_IDENTIFICATION_FIELD_TYPE != XCP_PID_ABSOLUTE)
        Xcp_CopyU2ToU1Buffer(DaqNum, &Xcp_DaqBuffer[XCP_DAQ_NUM_OFFSET], CPU_BYTE_ORDER);
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
        if (XCP_EVENT_CONSIST_DAQ == Xcp_GlobalCfgPtr->XcpEvCh[daqPtr->eventChannelNum].XcpEvChConsistency)
        {
            Xcp_FillEventBuffer(daqPtr);
        }
/*add timestamp ( if needed )*/
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
        if (XCP_DAQ_MOD_TS == (XCP_DAQ_MOD_TS & (daqPtr->mode)))
        {
            if (XCP_EVENT_CONSIST_ODT == Xcp_GlobalCfgPtr->XcpEvCh[daqPtr->eventChannelNum].XcpEvChConsistency)
            {
                daqPtr->timeStamp = Xcp_ReadOutMS();
            }
#if (XCP_TIMESTAMP_TYPE == XCP_TS_ONE_BYTE)
            Xcp_DaqBuffer[XCP_DATA_OFFSET] = (Xcp_TSType)(daqPtr->timeStamp);
#elif (XCP_TIMESTAMP_TYPE == XCP_TS_TWO_BYTE)
            Xcp_CopyU2ToU1Buffer((Xcp_TSType)(daqPtr->timeStamp), &Xcp_DaqBuffer[XCP_DATA_OFFSET], CPU_BYTE_ORDER);
#elif (XCP_TIMESTAMP_TYPE == XCP_TS_FOUR_BYTE)
            Xcp_CopyU4ToU1Buffer((Xcp_TSType)(daqPtr->timeStamp), &Xcp_DaqBuffer[XCP_DATA_OFFSET], CPU_BYTE_ORDER);
#endif
            /* update the pos */
            pos += XCP_TIMESTAMP_TYPE;
        }
#endif
    }

    destPtr = (Xcp_AGType*)&Xcp_DaqBuffer[pos];

    /* if ODT consistent copy data directly from source*/
    if (XCP_EVENT_CONSIST_ODT == Xcp_GlobalCfgPtr->XcpEvCh[daqPtr->eventChannelNum].XcpEvChConsistency)
    {
        entryPtr = odtPtr->XcpOdtEntry;
        for (entryNum = 0u; entryNum < odtPtr->XcpOdtEntryCount; entryNum++)
        {
            if (0u == entryPtr[entryNum].XcpOdtEntryLength)
            {
                break;
            }
            sourPtr = entryPtr[entryNum].XcpOdtEntryAddr;
            for (elementCnt = 0u; elementCnt < entryPtr[entryNum].XcpOdtEntryLength; elementCnt++)
            {
                if (XCP_MAX_DTO >= (((destCnt + 1u) * XCP_AG) + pos))
                {
                    destPtr[destCnt] = sourPtr[elementCnt];
                    destCnt++;
                }
            }
        }
    }
    /* for other consistent copy data from eventBuffer */
    else
    {
        entryPtr = odtPtr->XcpOdtEntry;
        for (entryNum = 0; entryNum < odtPtr->XcpOdtEntryCount; entryNum++)
        {
            if (0u == entryPtr[entryNum].XcpOdtEntryLength)
            {
                break;
            }
            sourPtr = &(daqPtr->eventBufferPtr[daqPtr->eventBufferPos]);
            for (elementCnt = 0; elementCnt < entryPtr[entryNum].XcpOdtEntryLength; elementCnt++)
            {
                if (XCP_MAX_DTO >= (((destCnt + 1u) * XCP_AG) + pos))
                {
                    destPtr[destCnt] = sourPtr[elementCnt];
                    destCnt++;
                }
            }
            /* update the BufferPos */
            daqPtr->eventBufferPos += elementCnt;
        }
    }
    SchM_Exit_Xcp_Exclusive_Daq();
    if (Xcp_GlobalCfgPtr->XcpDaqList[DaqNum].XcpDto != NULL_PTR)
    {
        pduId = (PduIdType)Xcp_GlobalCfgPtr->XcpDaqList[DaqNum].XcpDto->XcpDto2PduMapping->XcpTxPdu->LowLayerTxPduId;
    }
    else
    {
        pduId = (PduIdType)Xcp_PbCfgPtr->XcpPduRef->XcpTxPdu->LowLayerTxPduId;
    }
    return (Xcp_Transmit(pduId, (PduLengthType)((destCnt * XCP_AG) + pos), Xcp_DaqBuffer));
}

/******************************************************************************/
/*
 * @brief      <call SendDaqframe according to the overload state>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <Std_ReturnType>
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(Std_ReturnType, XCP_CODE) Xcp_SampleAndTransmit(P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) daqPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType result;

    if (XCP_DAQ_SS_OVL == (XCP_DAQ_SS_OVL & (daqPtr->sendStatus)))
    {
        /* if consistency of the associated event channel is DAQ,
         * send the DAQ List from the beginning.
         */
        if (XCP_EVENT_CONSIST_DAQ == (Xcp_GlobalCfgPtr->XcpEvCh[daqPtr->eventChannelNum].XcpEvChConsistency))
        {
            daqPtr->currentPid = daqPtr->firstPid;
        }
/* indicated by event */
#if (XCP_DAQ_OVL_EV == XCP_DAQ_OVL_INDICATION)
        result = Xcp_SendDaqFrame(daqPtr, FALSE);
        Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_DAQ_OVERLOAD;
        Xcp_EvLength = 2;
        Xcp_SendEv();
        daqPtr->sendStatus &= (uint8)(~(XCP_DAQ_SS_OVL));
/* indicated by pid */
#elif (XCP_DAQ_OVL_PID == XCP_DAQ_OVL_INDICATION)
        result = Xcp_SendDaqFrame(daqPtr, TRUE);
        if (E_OK == result)
        {
            daqPtr->sendStatus &= (uint8)(~(XCP_DAQ_SS_OVL));
        }
/* not indicated */
#else
        result = Xcp_SendDaqFrame(daqPtr, FALSE);
        daqPtr->sendStatus &= (uint8)(~(XCP_DAQ_SS_OVL));
#endif
    }
    else
    {
        result = Xcp_SendDaqFrame(daqPtr, FALSE);
    }
    return result;
}

/******************************************************************************/
/*
 * @brief      <this function is used to send a DAQ frame >
 *
 * <>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_SendDaq(void)
{
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) currentDaqPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */

    Xcp_SendStatus |= XCP_DAQ_REQUEST;

    /* if daq processor disabled */
    if (0u == Xcp_DaqListQueueLength)
    {
        Xcp_SendStatus &= (uint8)(~XCP_DAQ_REQUEST);
    }
    else
    {
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
        if (XCP_DAQ_PENDING != (XCP_DAQ_PENDING & Xcp_SendStatus))
        {
#endif
            currentDaqPtr = &Xcp_Daq[Xcp_DaqListQueue[Xcp_DaqListQueuePos]];

#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
            if ((Std_ReturnType)E_OK == Xcp_SampleAndTransmit(currentDaqPtr))
#else
        while ((Std_ReturnType)E_OK == Xcp_SampleAndTransmit(currentDaqPtr))
#endif
            {
                /* send successed  */
                (currentDaqPtr->currentPid)++;
                Xcp_SendStatus |= XCP_DAQ_PENDING;
                Xcp_TransferTimeOutCnt = 0;
                if (XCP_DAQ_SS_AL == (XCP_DAQ_SS_AL & currentDaqPtr->sendStatus))
                {
                    (currentDaqPtr->sendStatus) &= (uint8)(~(XCP_DAQ_SS_SENDING));
                    (currentDaqPtr->sendStatus) &= (uint8)(~(XCP_DAQ_SS_QUEUE));
                    /*
                     * updata the DAQ queue length and position
                     */

                    Xcp_DaqListQueueLength--;
                    Xcp_DaqListQueuePos++;

                    if ((Xcp_DaqListQueuePos >= Xcp_MaxQueueLength) || (0u == Xcp_DaqListQueueLength))
                    {
                        Xcp_DaqListQueuePos = 0u;
                    }
                    if (((currentDaqPtr->daqListSize + currentDaqPtr->firstPid) <= (currentDaqPtr->currentPid))
                        /* size of new entry is zero */
                        || (0u
                            == (currentDaqPtr->odt[currentDaqPtr->currentPid - currentDaqPtr->firstPid]
                                    .XcpOdtEntry[0u]
                                    .XcpOdtEntryLength)))
                    {
                        currentDaqPtr->eventBufferPos = 0u;
                        currentDaqPtr->currentPid = currentDaqPtr->firstPid;
                    }
#if (XCP_BUS_TX_POLLING_MODE == STD_ON)
                    break;
#endif
                }
#if (XCP_BUS_TX_POLLING_MODE == STD_ON)
                if (
                    /* Reach the end of the daq List */
                    ((currentDaqPtr->daqListSize + currentDaqPtr->firstPid) < (currentDaqPtr->currentPid))
                    /* size of new entry is zero */
                    || (0u
                        == (currentDaqPtr->odt[currentDaqPtr->currentPid - currentDaqPtr->firstPid]
                                .XcpOdtEntry[0]
                                .XcpOdtEntryLength)))
                {
                    /* clear the DAQ sending ongoing flag */
                    (currentDaqPtr->sendStatus) &= (uint8)(~(XCP_DAQ_SS_SENDING));
                    break;
                }
#endif
            }
            /* update the queque if needed */
            if (
                /* DAQ List in Queue has been stoped */
                (0u == (XCP_DAQ_MOD_RUNNING & (currentDaqPtr->mode)))
                /* Reach the end of the daq List */
                || ((currentDaqPtr->daqListSize + currentDaqPtr->firstPid) <= (currentDaqPtr->currentPid))
                /* size of new entry is zero */
                || (0u
                    == (currentDaqPtr->odt[currentDaqPtr->currentPid - currentDaqPtr->firstPid]
                            .XcpOdtEntry[0]
                            .XcpOdtEntryLength)))
            {
                /* clear the DAQ sending ongoing flag */
                (currentDaqPtr->sendStatus) &= (uint8)(~(XCP_DAQ_SS_SENDING));
                /*
                 * updata the DAQ queue length and position
                 */
                currentDaqPtr->eventBufferPos = 0u;
                currentDaqPtr->currentPid = currentDaqPtr->firstPid;

                Xcp_DaqListQueueLength--;
                Xcp_DaqListQueuePos++;
                if (Xcp_DaqListQueuePos >= Xcp_MaxQueueLength)
                {
                    Xcp_DaqListQueuePos = 0x00u;
                }
            }
#if (XCP_BUS_TX_POLLING_MODE == STD_OFF)
        }
        else
        {
            /* Do Nothing */
        }
#endif
    }
    return;
}
#if (XCP_PL_STIM == (XCP_PL_STIM & XCP_RESOURCE))
/******************************************************************************/
/*
 * @brief               <Xcp Module STIM Rx Handeler>
 *
 * <This function copy stim data into associated buffer>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * @Edit by qinchun.yang
 * Modify STIM function
 * 2020/01/19
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_RxStimHal(P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) dataPtr)
{
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) daqPtr;
    P2VAR(Xcp_OdtType, AUTOMATIC, XCP_VAR) odtPtr;
    P2VAR(Xcp_EntryType, AUTOMATIC, XCP_VAR) entryPtr;
    P2CONST(Xcp_EvChConfigType, AUTOMATIC, XCP_VAR) evchCfgPtr;
    P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED_8) srcPtr;
    P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED_8) destPtr;
    uint16 evchNum;
    uint16 pos;
    Xcp_DaqNumType daqNum;
    uint8 odtNum;
    uint8 entryIdx;
    uint8 pid = dataPtr[XCP_PID_OFFSET];
    boolean errFlag = FALSE;

    /* Find daq num */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
    for (daqNum = 0; daqNum < XCP_MAX_DAQ; daqNum++)
    {
        if ((pid >= Xcp_Daq[daqNum].firstPid) && (pid < (Xcp_Daq[daqNum].firstPid + Xcp_Daq[daqNum].daqListSize)))
        {
            break;
        }
    }
    pos = XCP_DATA_OFFSET;
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_RELATIVE_BYTE)
    daqNum = dataPtr[XCP_DAQ_NUM_OFFSET];
    pos = XCP_DATA_OFFSET;
#else  /*XCP_PID_RELATIVE_WORD & XCP_PID_RELATIVE_WORD_ALIGNED*/
    Xcp_CopyU1BufferToU2(&(dataPtr[XCP_DAQ_NUM_OFFSET]), &daqNum, CPU_BYTE_ORDER);
    pos = XCP_DATA_OFFSET;
#endif /*XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE*/
    if (XCP_MAX_DAQ >= daqNum)
    {
        daqPtr = &(Xcp_Daq[daqNum]);
        evchNum = daqPtr->eventChannelNum;
        evchCfgPtr = &(Xcp_GlobalCfgPtr->XcpEvCh[evchNum]);
        if (XCP_DAQ_MOD_DIR_STIM != (daqPtr->mode & XCP_DAQ_MOD_DIR_STIM)
            || XCP_DAQ_MOD_RUNNING != (daqPtr->mode & XCP_DAQ_MOD_RUNNING)
            || DAQ == (evchCfgPtr->XcpEvChType & DAQ_STIM))
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            errFlag = TRUE;
        }
        else
        {
            odtNum = pid - Xcp_Daq[daqNum].firstPid;
            odtPtr = &(daqPtr->odt[odtNum]);
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
            /* Judge whether need to store time stump*/
            if ((0x00 == odtNum) && (0x00 != (XCP_DAQ_MOD_TS & daqPtr->mode)))
            {
                pos += XCP_TIMESTAMP_TYPE;
            }
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/
            for (entryIdx = 0; entryIdx < odtPtr->XcpOdtEntryCount; entryIdx++)
            {
                entryPtr = &(odtPtr->XcpOdtEntry[entryIdx]);
                if ((0x00u == entryPtr->XcpOdtEntryLength) || (XCP_MAX_CTO <= pos))
                {
                    goto XCP_PROCESS_STIM_EXIT;
                }
                srcPtr = (uint8*)&dataPtr[pos];
                destPtr = (uint8*)entryPtr->XcpOdtEntryAddr;
                Xcp_BufferCopy((uint32)srcPtr, (uint32)destPtr, entryPtr->XcpOdtEntryLength);
                pos += entryPtr->XcpOdtEntryLength;
            }
        }
    }
    else
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        errFlag = TRUE;
    }
XCP_PROCESS_STIM_EXIT:
    if (TRUE == errFlag)
    {
        Xcp_SendResp();
    }
    return;
}
#endif /*XCP_PL_STIM == XCP_PL_STIM & XCP_RESOURCE*/
/******************************************************************************/
/*
 * @brief      <begin a DAQ list sending trigered by fixed eventchannel>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <eventNum, daqNum>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_EventChannelDaqHal(uint16 eventNum, const Xcp_DaqNumType daqNum)
{
    Xcp_DaqNumType queueIndex;

    if (0u == Xcp_DaqListQueueLength)
    {
        Xcp_MaxQueueLength = Xcp_DaqListQueueLength;
    }
    /* Overload*/
    if ((XCP_DAQ_SS_SENDING == (XCP_DAQ_SS_SENDING & Xcp_Daq[daqNum].sendStatus))
        || (XCP_DAQ_SS_QUEUE == (XCP_DAQ_SS_QUEUE & Xcp_Daq[daqNum].sendStatus)))
    {
        /* clear the DAQ sending ongoing flag */
        Xcp_Daq[daqNum].sendStatus &= (uint8)(~(XCP_DAQ_SS_SENDING));
        Xcp_Daq[daqNum].sendStatus |= XCP_DAQ_SS_OVL;
        Xcp_Daq[daqNum].eventBufferPos = 0u;
        Xcp_Daq[daqNum].currentPid = Xcp_Daq[daqNum].firstPid;
    }
    else
    {
        Xcp_Daq[daqNum].sendStatus |= XCP_DAQ_SS_QUEUE;
    }
    if (XCP_EVENT_CONSIST_EVENT == (Xcp_GlobalCfgPtr->XcpEvCh[eventNum].XcpEvChConsistency))
    {
        Xcp_FillEventBuffer(&Xcp_Daq[daqNum]);
    }
    /* Regardless of whether the priority is opened or not, the daq unified into the array, followed by the priority for
     * processing */
    queueIndex = (Xcp_DaqListQueueLength + Xcp_DaqListQueuePos) % (XCP_MAX_DAQ);
    Xcp_DaqListQueue[queueIndex] = daqNum;
    Xcp_DaqListQueueLength++;
    if ((Xcp_DaqListQueueLength > Xcp_MaxQueueLength) && (Xcp_DaqListQueueLength <= XCP_MAX_DAQ))
    {
        Xcp_MaxQueueLength = Xcp_DaqListQueueLength;
    }
    return;
}

/******************************************************************************/
/*
 * @brief      < Event Channel triggered indication >
 *
 * <This function inform the stack that a event channel is triggered>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <eventNum>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_EventIndication(uint16 eventNum) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 cnt; /* loop counter */
    Xcp_DaqNumType daqNum;
    boolean SenddaqFlag = FALSE;
    const Xcp_EvChConfigType* eventPtr;

    if (Xcp_Status == XCP_UINIT)
    {
        return;
    }
    eventPtr = &Xcp_GlobalCfgPtr->XcpEvCh[eventNum];
    if (0u == (Xcp_SendStatus & XCP_DAQ_REQUEST))
    {
        Xcp_DaqListQueuePos = 0;
        Xcp_DaqListQueueLength = 0;
    }
    for (cnt = 0; cnt < eventPtr->XcpEvChMaxDaqList; cnt++)
    {
        /* get daq Num */
        daqNum = eventPtr->XcpEvChTrigDaqListRef[cnt];
        /* Find the DAQ that is running and matching eventNum */
        if ((XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & Xcp_Daq[daqNum].mode))
            && (eventNum == Xcp_Daq[daqNum].eventChannelNum))
        {
            SchM_Enter_Xcp_Exclusive_Daq();
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
            Xcp_Daq[daqNum].prescalerCnt++; /* to prescaler */
            if (Xcp_Daq[daqNum].prescalerCnt >= Xcp_Daq[daqNum].prescaler)
            {
                Xcp_Daq[daqNum].prescalerCnt = 0;
#endif /*XCP_PRESCALER_SUPPORTED == STD_ON*/
                if (XCP_DAQ_MOD_DIR_DAQ == (Xcp_Daq[daqNum].mode & XCP_DAQ_MOD_DIR_BIT))
                {
                    SenddaqFlag = TRUE;
                    Xcp_EventChannelDaqHal(eventNum, daqNum);
                }
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
            }
#endif
            SchM_Exit_Xcp_Exclusive_Daq();
        }
    }
    if (((boolean)TRUE == SenddaqFlag) && (0u == (Xcp_SendStatus & XCP_DAQ_REQUEST)))
    {
        Xcp_SendDaq();
    }
    return;
}
/******************************************************************************/
/*
 * @brief      <Start a DAQ list>
 *
 * <>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <daqNum>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <Std_ReturnType>
 */
/******************************************************************************/
static FUNC(Std_ReturnType, XCP_CODE) Xcp_StartDaq(Xcp_DaqNumType daqNum)
{
    uint16 evChNum = Xcp_Daq[daqNum].eventChannelNum;
    uint8 odtCnt;
    uint8 entryCnt;
    uint32 len = 0;
    Std_ReturnType result = E_OK;
    const Xcp_EvChConfigType* eventChannelPtr = Xcp_GlobalCfgPtr->XcpEvCh;

    if (XCP_DAQ_MOD_RUNNING != (XCP_DAQ_MOD_RUNNING & Xcp_Daq[daqNum].mode))
    {
        Xcp_Daq[daqNum].eventBufferPos = 0;
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
        Xcp_Daq[daqNum].prescalerCnt = 0u;
#endif
        Xcp_Daq[daqNum].currentPid = Xcp_Daq[daqNum].firstPid;
        if ((evChNum < XCP_MAX_EVENT_CHANNEL) /* evChNum is valid and running daq in eventchannel < maxdaqlist */
            && (Xcp_EvChActivCnt[evChNum] <= Xcp_GlobalCfgPtr->XcpEvCh[evChNum].XcpEvChMaxDaqList))
        {
            for (odtCnt = 0; odtCnt < Xcp_Daq[daqNum].daqListSize; odtCnt++)
            {
                for (entryCnt = 0; entryCnt < Xcp_Daq[daqNum].odt[odtCnt].XcpOdtEntryCount; entryCnt++)
                {
                    len += Xcp_Daq[daqNum].odt[odtCnt].XcpOdtEntry[entryCnt].XcpOdtEntryLength;
                }
            }
            /* set buffer ptr */
            if ((XCP_DAQ_MOD_DIR_STIM == (Xcp_Daq[daqNum].mode & XCP_DAQ_MOD_DIR_BIT))
                || (XCP_EVENT_CONSIST_EVENT == (eventChannelPtr[evChNum].XcpEvChConsistency)))
            { /* for event consistency,put all daq to the Xcp_EvChxBuffer[] */
                Xcp_Daq[daqNum].eventBufferPtr =
                    &(eventChannelPtr[evChNum].XcpEvChBuffPtr[Xcp_EvChBufferUsage[evChNum]]);
                Xcp_EvChBufferUsage[evChNum] += len;
            }
            /* for DAQ consist and DAQ direction */
            else if (XCP_EVENT_CONSIST_DAQ == (eventChannelPtr[evChNum].XcpEvChConsistency))
            {

                len = eventChannelPtr[evChNum].XcpEvChBuffDpth - len; /* put the daq buffer in the end */
                Xcp_Daq[daqNum].eventBufferPtr = &(eventChannelPtr[evChNum].XcpEvChBuffPtr[len]);
            }
            /* event buffer memory overflow */

            else /* odt consistency,do nothing */
            {
                /* Do Nothing */
            }
            Xcp_Daq[daqNum].mode |= XCP_DAQ_MOD_RUNNING;
            Xcp_SessionStatus |= XCP_SESSION_STATE_DAQ_RUNNING;
            Xcp_EvChActivCnt[evChNum]++;
        }
    }
    else
    {
        result = E_NOT_OK;
    }
    return result;
}
static FUNC(void, XCP_CODE) Xcp_StopDaq(Xcp_DaqNumType daqNum)
{
    uint16 evChNum = Xcp_Daq[daqNum].eventChannelNum;

    if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & (Xcp_Daq[daqNum].mode)))
    {
        Xcp_Daq[daqNum].mode &= (uint8)(~XCP_DAQ_MOD_RUNNING);
        Xcp_Daq[daqNum].sendStatus &= (uint8)(~XCP_DAQ_SS_AL);
        ; /* clear DAQ sendStatus flag */
        Xcp_Daq[daqNum].eventBufferPtr = NULL_PTR;
        Xcp_EvChActivCnt[evChNum]--;
        if (0u == Xcp_EvChActivCnt[evChNum])
        {
            Xcp_EvChBufferUsage[evChNum] = 0;
        }
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_SetDaqPtr>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_SetDaqPtr(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 daqListNo = 0xff;
    uint8 odtNo;
    uint8 odtEntryNo;
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 6u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
#endif
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            /* get the daq List No from CommandRxObject[2-3] buffer */
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &daqListNo, CPU_BYTE_ORDER);
            /* get the Odt number from CommandRxObject[4] buffer */
            odtNo = Xcp_CmdBuffer[4u];
            /* get the Odt Entry number from CommandRxObject[5] buffer */
            odtEntryNo = Xcp_CmdBuffer[5u];
            /* check the daq/odt/entry num */
            if ((daqListNo < XCP_MAX_DAQ)
#if (XCP_MIN_DAQ != 0u)
                && (daqListNo >= XCP_MIN_DAQ)
#endif
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
                && (0u != Xcp_Daq[daqListNo].daqListSize)
#endif
                && (odtNo < Xcp_Daq[daqListNo].daqListSize)
                && (odtEntryNo < Xcp_Daq[daqListNo].odt[odtNo].XcpOdtEntryCount))
            {
                /* check whether DAQ is running at this moment */
                if (XCP_DAQ_MOD_RUNNING == (Xcp_Daq[daqListNo].mode & XCP_DAQ_MOD_RUNNING))
                {
                    Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
                    Xcp_RespLength = 0x02u;
                }
                else
                {
                    Xcp_PtrDaq = daqListNo; /* set daq point */
                    Xcp_PtrOdt = odtNo;
                    Xcp_PtrEntry = odtEntryNo;
                }
            }
            else /* DAQ/ODT/entry num check failed */
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
            }
        }
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    }
#endif
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_WriteDaq>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_WriteDaq(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 0x08u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* Daq List Number used in the previous SET_DAQ_PTR has to be in the range */
        else if (
            (Xcp_PtrDaq >= XCP_MAX_DAQ)
#if (0u != XCP_MIN_DAQ)
            || (Xcp_PtrDaq < XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
        )
        {
            Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_WriteDaqHal();
        }
    Xcp_SendResp();
    return;
}
static FUNC(void, XCP_CODE) Xcp_WriteDaqHal(void)
{
#if 0
    uint8 bitOffSet;
#endif /* don't support bit temporary */

    uint8 sizeOfDaqElement;
    uint8 addressExtension;
    uint32 addressOfDaqElement = 0;
    uint32 addressOfOdtEntry;

    uint8 entryNum;
    uint16 len = 0;

#if 0
    bitOffSet = Xcp_CmdBuffer[1u];
#endif /* we don't support bit_stim in this version */
    sizeOfDaqElement = Xcp_CmdBuffer[2u];
    addressExtension = Xcp_CmdBuffer[3u];
    /* get the addressOfDaqElement from CommandRxObject[4-7] buffer */
    Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &addressOfDaqElement, CPU_BYTE_ORDER);
    /* get the address */
    addressOfOdtEntry = Xcp_Mta2Ptr(addressExtension, addressOfDaqElement);
    if (((DAQ == Xcp_GlobalCfgPtr->XcpDaqList[Xcp_PtrDaq].XcpDaqListType)
         && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_DAQ))
        || ((STIM == Xcp_GlobalCfgPtr->XcpDaqList[Xcp_PtrDaq].XcpDaqListType)
            && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_STIM))
        || ((DAQ_STIM == Xcp_GlobalCfgPtr->XcpDaqList[Xcp_PtrDaq].XcpDaqListType)
            && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_DAQ) && (sizeOfDaqElement > XCP_ODT_ENTRY_SIZE_STIM)))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
#if (XCP_CHECK_MEA_ADDR == STD_ON)
    /*Check DAQ address.*/
    else if ((addressOfOdtEntry < Xcp_MeaStartAddr) || (addressOfOdtEntry > (Xcp_MeaStartAddr + Xcp_MeaLen)))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
#endif /*XCP_CHECK_MEA_ADDR == STD_ON*/
    /* check ptr whether valid */
    else if (
        (Xcp_PtrOdt >= Xcp_Daq[Xcp_PtrDaq].daqListSize)
        || (Xcp_PtrEntry >= Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntryCount))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
        Xcp_RespLength = 0x02u;
    }
    /* check whether DAQ Running at this moment */
    else if (XCP_DAQ_MOD_RUNNING == (Xcp_Daq[Xcp_PtrDaq].mode & XCP_DAQ_MOD_RUNNING))
    {
        Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        for (entryNum = 0; entryNum < Xcp_PtrEntry; entryNum++)
        {
            len += Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntry[entryNum].XcpOdtEntryLength;
        }
        len += sizeOfDaqElement;
        len *= XCP_AG;
        /* check length of this daq-dto frame */
        if (XCP_MAX_WRITEDAQ_SIZE >= len)
        {
            Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntry[Xcp_PtrEntry].XcpOdtEntryLength = sizeOfDaqElement;
            /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
            Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntry[Xcp_PtrEntry].XcpOdtEntryAddr =
                (Xcp_AGType*)addressOfOdtEntry;
            /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
            Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntry[Xcp_PtrEntry].XcpOdtEntryNumber = Xcp_PtrEntry;
            /* The DAQ list pointer is auto post incremented*/
            Xcp_PtrEntry++;
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_DAQ_CONFIG);
            Xcp_RespLength = 0x02u;
        }
    }
}

/******************************************************************************/
/*
 * @brief               <Xcp_SetDaqListMode>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_SetDaqListMode(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 8u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection*/
        if ((XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
            || (XCP_PL_STIM == (Xcp_ProtectionStatus & XCP_PL_STIM)))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_SetDaqListModeHal();
        }
    Xcp_SendResp();
    return;
}
static FUNC(void, XCP_CODE) Xcp_SetDaqListModeHal(void)
{
    uint8 mode;
    uint16 daqListNo = 0;
    uint16 evChNo = 0;
    uint8 transRatePrescaler;
    uint8 daqPriority;
    P2CONST(Xcp_EvChConfigType, AUTOMATIC, XCP_CONST_PBCFG) evchPtr = Xcp_GlobalCfgPtr->XcpEvCh;
    Xcp_DaqNumType cnt;
    /* get the detail parmeters from CommandRxObject[1] buffer*/
    mode = Xcp_CmdBuffer[1u];
    /* get the mode from CommandRxObject[2-3] buffer */
    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &daqListNo, CPU_BYTE_ORDER);
    /* get the mode from CommandRxObject[4-5] buffer */
    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[4u], &evChNo, CPU_BYTE_ORDER);
    transRatePrescaler = Xcp_CmdBuffer[6u];
    daqPriority = Xcp_CmdBuffer[7u];
    if (/* check input parameter */
        (evChNo >= XCP_MAX_EVENT_CHANNEL) || (daqListNo >= XCP_MAX_DAQ)
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
        || (0u == Xcp_Daq[daqListNo].daqListSize) /* dynamic,the daqlistsize can't eaqul 0 */
#endif
#if (STD_OFF == XCP_PRESCALER_SUPPORTED)
        || (0x01u != transRatePrescaler) /*without prescaler,the Prescaler must = 1*/
#else
        || (0u == transRatePrescaler) /*with prescaler,the Prescaler must >= 1*/
#endif
#if (STD_OFF == XCP_DAQ_PRIORITY_SUPPORT)
        || (daqPriority > 0u)
#endif /*STD_OFF == XCP_DAQ_PRIORITY_SUPPORT*/
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else if (/* check if alternating is illegal */
             /* 1. only  DIRECTION=DAQ ,can set alternating flag*/
             ((XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_DIR_STIM) == (mode & (XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_DIR_BIT)))
             /* can not allowed to set the ALTERNATING flag and the TIMESTAMP flag at the same time. */
             || ((XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_TS) == (mode & (XCP_DAQ_MOD_AL_BIT | XCP_DAQ_MOD_TS)))
/* illegal timestamp disable */
#if (XCP_TS_NO_TS != XCP_TIMESTAMP_TYPE)
#if (XCP_TIMESTAMP_FIXED == STD_ON) /* If the Tinmestump fixed is on,so the mode must set XCP_DAQ_MOD_TS*/
             || (XCP_DAQ_MOD_TS != (XCP_DAQ_MOD_TS & tMode))
#endif
#endif
/* illegal timestamp enable */
#if (XCP_TS_NO_TS == XCP_TIMESTAMP_TYPE)
             || (XCP_DAQ_MOD_TS
                 == (XCP_DAQ_MOD_TS & mode)) /* when ts_type is no_ts,so the mode can't set XCP_DAQ_MOD_TS */
#endif
/* illegal pid_off enable */
#if (STD_OFF == XCP_PID_OFF_SUPPORT)
             || (XCP_DAQ_MOD_PIDOFF == (XCP_DAQ_MOD_PIDOFF & mode))
#endif
             /* illegal direction */
             || ((XCP_DAQ_MOD_DIR_STIM
                  == (mode & XCP_DAQ_MOD_DIR_BIT)) /* DaqListtype in mode parameter is not match to XcpDaqList*/
                 && (DAQ == (Xcp_GlobalCfgPtr->XcpDaqList[daqListNo].XcpDaqListType)))
             || ((XCP_DAQ_MOD_DIR_DAQ == (mode & XCP_DAQ_MOD_DIR_BIT))
                 && (STIM == (Xcp_GlobalCfgPtr->XcpDaqList[daqListNo].XcpDaqListType))))
    {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
        Xcp_RespLength = 0x02u;
    }
    /* check whether DAQ Running at this moment */
    else if (XCP_DAQ_MOD_RUNNING == (Xcp_Daq[daqListNo].mode & XCP_DAQ_MOD_RUNNING))
    {
        Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        /* check if the eventchannel Num is valid */
        for (cnt = 0; cnt < evchPtr[evChNo].XcpEvChMaxDaqList; cnt++)
        {
            if (evchPtr[evChNo].XcpEvChTrigDaqListRef[cnt] == daqListNo)
            {
                break;
            }
        }
        if (cnt != evchPtr[evChNo].XcpEvChMaxDaqList)
        {
            Xcp_Daq[daqListNo].eventChannelNum = evChNo;
            /* first clear these flag */
            Xcp_Daq[daqListNo].sendStatus &= (uint8)(~XCP_DAQ_SS_AL);
            Xcp_Daq[daqListNo].sendStatus |= (mode & XCP_DAQ_SS_AL);
            Xcp_Daq[daqListNo].mode &= (uint8)(~(XCP_DAQ_MOD_DIR_BIT | XCP_DAQ_MOD_TS | XCP_DAQ_MOD_PIDOFF));
            Xcp_Daq[daqListNo].mode |= (mode & (XCP_DAQ_MOD_DIR_BIT | XCP_DAQ_MOD_TS | XCP_DAQ_MOD_PIDOFF));

#if (XCP_PRESCALER_SUPPORTED == STD_ON)
            Xcp_Daq[daqListNo].prescaler = transRatePrescaler;
#endif

#if (XCP_DAQ_PRIORITY_SUPPORT == STD_ON)
            Xcp_Daq[daqListNo].priority = daqPriority;
#endif
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
    }
}

/******************************************************************************/
/*
 * @brief               <Xcp_StartStopDaqList>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_StartStopDaqList(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_StartStopDaqListHal();
        }
    Xcp_SendResp();
    return;
}
static FUNC(void, XCP_CODE) Xcp_StartStopDaqListHal(void)
{
    Std_ReturnType result;
    uint8 tMode;
    uint16 daqListNo = 0;
    tMode = Xcp_CmdBuffer[0x01];
    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[0x02], &daqListNo, CPU_BYTE_ORDER);
    if (/* check daqListNo  */
        (daqListNo >= XCP_MAX_DAQ)
#if (XCP_DAQ_DYNAMIC == XCP_DAQ_CONFIG_TYPE)
        || (0u == Xcp_Daq[daqListNo].daqListSize) || (Xcp_Daq[daqListNo].DynaCfgStatus != XCP_DAQ_CFG_DONE)
#endif
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_RespBuffer[1u] = Xcp_Daq[daqListNo].firstPid;
        Xcp_RespLength = 0x02u;
        switch (tMode)
        {
        case XCP_START_STOP_MOD_STOP: {
            Xcp_StopDaq((Xcp_DaqNumType)daqListNo);
            /* Check that whether all DAQs are not running */
            for (daqListNo = 0; daqListNo < XCP_MAX_DAQ; daqListNo++)
            {
                if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & Xcp_Daq[daqListNo].mode))
                {
                    break;
                }
            }
            if (daqListNo == XCP_MAX_DAQ) /* mean no daq is running */
            {
                Xcp_SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
            }
            break;
        }
        case XCP_START_STOP_MOD_START: {
            result = Xcp_StartDaq((Xcp_DaqNumType)daqListNo);
            if ((Std_ReturnType)E_NOT_OK == result)
            {
                Xcp_SetErrorCode(XCP_ERR_DAQ_CONFIG);
                Xcp_RespLength = 0x02u;
            }
            break;
        }
        case XCP_START_STOP_MOD_SELECT: {
            Xcp_Daq[daqListNo].mode |= XCP_DAQ_MOD_SELE;
            break;
        }
        default: {
            Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
            Xcp_RespLength = 0x02u;
            break;
        }
        }
    }
}

/******************************************************************************/
/*
 * @brief               <Xcp_StartStopSynch>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_StartStopSynch(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 2u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_StartStopSynchHal();
        }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_StartStopSynchHal(void)
{
    Std_ReturnType result;
    boolean runningFlag = FALSE;
    boolean findSeleDaqFlag = FALSE;
    uint8 tMode;
    uint16 cnt;
    tMode = Xcp_CmdBuffer[1u];
    switch (tMode)
    {
    case XCP_START_STOP_MOD_STOP_ALL: {
        for (cnt = 0; cnt < XCP_MAX_DAQ; cnt++)
        {
            Xcp_Daq[cnt].mode &= (uint8)(~XCP_DAQ_MOD_RUNNING);
        }
        Xcp_SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
        for (cnt = 0; cnt < XCP_MAX_EVENT_CHANNEL; cnt++)
        {
            Xcp_EvChBufferUsage[cnt] = 0;
            Xcp_EvChActivCnt[cnt] = 0;
        }
        break;
    }

    case XCP_START_STOP_MOD_START_SELECT: {
        for (cnt = 0; cnt < XCP_MAX_DAQ; cnt++)
        {
            if (XCP_DAQ_MOD_SELE == (XCP_DAQ_MOD_SELE & Xcp_Daq[cnt].mode))
            {
                findSeleDaqFlag = TRUE;
                result = Xcp_StartDaq((Xcp_DaqNumType)cnt);
                if ((Std_ReturnType)E_OK == result)
                {
                    Xcp_Daq[cnt].mode &= (uint8)(~XCP_DAQ_MOD_SELE);
                }
                else
                {
                    Xcp_SetErrorCode(XCP_ERR_DAQ_CONFIG);
                    Xcp_RespLength = 0x02u;
                }
            }
        }
        if ((boolean)FALSE == findSeleDaqFlag)
        {
            Xcp_SetErrorCode(XCP_ERR_DAQ_CONFIG);
            Xcp_RespLength = 0x02u;
        }
        break;
    }
    case XCP_START_STOP_MOD_STOP_SELECT: {
        for (cnt = 0; cnt < XCP_MAX_DAQ; cnt++)
        {
            if (XCP_DAQ_MOD_SELE == (XCP_DAQ_MOD_SELE & Xcp_Daq[cnt].mode))
            {
                Xcp_StopDaq((Xcp_DaqNumType)cnt);
                Xcp_Daq[cnt].mode &= (uint8)(~XCP_DAQ_MOD_SELE);
            }
            /* Check if there is running daq which without selected */
            else if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & Xcp_Daq[cnt].mode))
            {
                runningFlag = TRUE;
            }
            else
            {
                /* Nothing to do */
            }
        }
        if ((boolean)FALSE == runningFlag)
        {
            Xcp_SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
        }
        break;
    }
    default: {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
        Xcp_RespLength = 0x02u;
        break;
    }
    }
}

/* CMD:WRITE_DAQ_MULTIPLE:now don't support */

#if (STD_ON == XCP_READ_DAQ)
/******************************************************************************/
/*
 * @brief               <Xcp_ReadDaq>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_ReadDaq(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint32 elementaddress;

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 0x01u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* Daq List Number used in the previous SET_DAQ_PTR has to be in the range */
        else if (
            (Xcp_PtrDaq >= XCP_MAX_DAQ) || (Xcp_PtrOdt >= Xcp_Daq[Xcp_PtrDaq].daqListSize)
            || (Xcp_PtrEntry >= Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntryCount))
        {
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            /* Position of bit in 32-bit variable */
            Xcp_RespBuffer[1u] = 0xFFu;

            /* Size of DAQ element [AG] */
            Xcp_RespBuffer[2u] = Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntry[Xcp_PtrEntry].XcpOdtEntryLength;

            /* Address extension of DAQ element */
            Xcp_RespBuffer[3u] = 0x0u;

            /* Address of DAQ element */
            /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
            elementaddress = (uint32)(Xcp_Daq[Xcp_PtrDaq].odt[Xcp_PtrOdt].XcpOdtEntry[Xcp_PtrEntry].XcpOdtEntryAddr);
            /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

            /* Put the ElementAddress to RespBuffer[4]-[7] buffer */
            Xcp_CopyU4ToU1Buffer(elementaddress, &Xcp_RespBuffer[4u], CPU_BYTE_ORDER);

            Xcp_RespLength = 0x08u;

            Xcp_PtrEntry++;
        }
    Xcp_SendResp();

    return;
}
#endif /*STD_ON == XCP_READ_DAQ*/

/******************************************************************************/
/*
 * @brief               <Xcp_GetDaqClock>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
#if ((STD_ON == XCP_GET_DAQ_CLOCK) && (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS))
FUNC(void, XCP_CODE)
Xcp_GetDaqClock(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        /* Put the Receive Timestamp to RespBuffer[4]-[7] buffer */
        Xcp_CopyU4ToU1Buffer((uint32)Xcp_RxTimeStamp, &(Xcp_RespBuffer[4u]), CPU_BYTE_ORDER);
        Xcp_RespLength = 0x08u;
    }
    Xcp_SendResp();
    return;
}
#endif

#if (STD_ON == XCP_GET_DAQ_PROCESSOR_INFO)
/******************************************************************************/
/*
 * @brief               <Xcp_GetDaqProcessorInfo>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetDaqProcessorInfo(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 daqProperties;
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            /* daq properties depends on the macros configured in xcp_cfg.h */
            daqProperties =
                (XCP_DAQ_CONFIG_TYPE
#if (XCP_PRESCALER_SUPPORTED == STD_ON)
                 | (0x02u)
#endif /*XCP_PRESCALER_SUPPORTED == STD_ON*/
#if (XCP_RESUME_SUPPORT == STD_ON)
                 | (0x04u)
#endif /*XCP_RESUME_SUPPORT == STD_ON*/
#if (XCP_BIT_STIM_SUPPORT == STD_ON)
                 | (0x08u)
#endif /*XCP_BIT_STIM_SUPPORT == STD_ON*/
#if (XCP_PID_OFF_SUPPORT == STD_ON)
                 | (0x20u)
#endif /*XCP_PID_OFF_SUPPORT == STD_ON*/
                 | ((uint8)(XCP_DAQ_OVL_INDICATION << 6u)));
/* TIMESTAMP_SUPPORTED */
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
            daqProperties |= 0x10u;
#endif /*XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS*/

            Xcp_RespBuffer[1u] = daqProperties;
            Xcp_CopyU2ToU1Buffer((uint16)XCP_MAX_DAQ, &Xcp_RespBuffer[2u], CPU_BYTE_ORDER);
            Xcp_CopyU2ToU1Buffer((uint16)XCP_MAX_EVENT_CHANNEL, &Xcp_RespBuffer[4u], CPU_BYTE_ORDER);

            Xcp_RespBuffer[6u] = XCP_MIN_DAQ;

            Xcp_RespBuffer[7u] =
                (((uint8)(XCP_IDENTIFICATION_FIELD_TYPE << 6u)) /**< Identification Field Type*/
                 | ((uint8)(0x3u << 4u))                        /**< The ADDR EXTENSION flag */
                 | (0x0u));                                     /**< The Optimisation Type */

            Xcp_RespLength = 0x08u;
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_DAQ_PROCESSOR_INFO*/

#if (STD_ON == XCP_GET_DAQ_RESOLUTION_INFO)
/******************************************************************************/
/*
 * @brief               <Xcp_GetDaqProcessorInfo>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetDaqResolutionInfo(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_RespBuffer[1u] = XCP_AG;
            Xcp_RespBuffer[2u] = XCP_ODT_ENTRY_SIZE_DAQ;
            Xcp_RespBuffer[3u] = XCP_AG;
            Xcp_RespBuffer[4u] = XCP_ODT_ENTRY_SIZE_STIM;
#if (XCP_TIMESTAMP_TYPE != XCP_TS_NO_TS)
            Xcp_RespBuffer[5u] = (XCP_TIMESTAMP_TYPE | (XCP_TIMESTAMP_FIXED << 3u) | (XCP_TIMESTAMP_UNIT << 4u));

            Xcp_CopyU2ToU1Buffer(XCP_TIMESTAMP_TICKS, &Xcp_RespBuffer[6u], CPU_BYTE_ORDER);
#else
        Xcp_RespBuffer[5u] = 0u;
        Xcp_RespBuffer[6u] = 0u;
        Xcp_RespBuffer[7u] = 0u;
#endif
            Xcp_RespLength = 0x08u;
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_DAQ_RESOLUTION_INFO*/

#if (STD_ON == XCP_GET_DAQ_LIST_MODE)
/******************************************************************************/
/*
 * @brief               <Xcp_GetDaqProcessorInfo>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetDaqListMode(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 daqListNo = 0xff;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &daqListNo, CPU_BYTE_ORDER);
            if (daqListNo < XCP_MAX_DAQ)
            {
                Xcp_RespBuffer[1u] = Xcp_Daq[daqListNo].mode;

                /* Put the currentEventChannelNo to RespBuffer[4]-[5] buffer */
                Xcp_CopyU2ToU1Buffer(Xcp_Daq[daqListNo].eventChannelNum, &Xcp_RespBuffer[4u], CPU_BYTE_ORDER);

#if (XCP_PRESCALER_SUPPORTED == STD_ON)
                Xcp_RespBuffer[6u] = Xcp_Daq[daqListNo].prescaler;
#else
            Xcp_RespBuffer[6u] = 0u;
#endif

#if (XCP_DAQ_PRIORITY_SUPPORT == STD_ON)
                Xcp_RespBuffer[7u] = Xcp_Daq[daqListNo].priority;
#else
            Xcp_RespBuffer[7u] = 0u;
#endif

                /* response length is 8 */
                Xcp_RespLength = 0x08u;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
            }
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_DAQ_LIST_MODE*/

#if (STD_ON == XCP_GET_DAQ_EVENT_INFO)
/******************************************************************************/
/*
 * @brief               <Xcp_GetDaqEventInfo>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetDaqEventInfo(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 eventChannelNo = 0xff;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &eventChannelNo, CPU_BYTE_ORDER);

            if (eventChannelNo < XCP_MAX_EVENT_CHANNEL)
            {
                Xcp_RespBuffer[1u] =
                    (uint8)(((uint8)((uint8)Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChType << 2u))
                            | ((uint8)((uint8)Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChConsistency << 6u)));
                Xcp_RespBuffer[2u] = Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChMaxDaqList;

                /* EVENT_CHANNEL_NAME_LENGTH in bytes ,0 If not available */
                Xcp_RespBuffer[3u] = Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChNameLength;

                /* set MTA for upload the event channel name */
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
                Xcp_MTA.transferAddr = (uint32)(Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChNamePtr);
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                Xcp_MTA.extensionAddr = 0x0u;
                Xcp_UploadInfoLen = ((((uint32)Xcp_RespBuffer[3u] - 1u) / XCP_AG) + 1u);

                Xcp_RespBuffer[4u] = Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChTimeCycle;
                Xcp_RespBuffer[5u] = (uint8)Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChTimeUnit;
                Xcp_RespBuffer[6u] = Xcp_GlobalCfgPtr->XcpEvCh[eventChannelNo].XcpEvChPriority;

                Xcp_RespLength = 0x07u;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
            }
        }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_DAQ_EVENT_INFO == STD_ON*/

#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
/******************************************************************************/
/*
 * @brief               <Xcp_ClearDaqList>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_ClearDaqList(void)
{
    uint16 daqListNo;
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2], &daqListNo, CPU_BYTE_ORDER);
        if (daqListNo < XCP_MAX_DAQ)
        {
            Xcp_StopDaq((Xcp_DaqNumType)daqListNo);
            Xcp_ClearDaq((Xcp_DaqNumType)daqListNo);
            /* Check if there is no daq is running */
            for (daqListNo = 0; daqListNo < XCP_MAX_DAQ; daqListNo++)
            {
                if (XCP_DAQ_MOD_RUNNING == (XCP_DAQ_MOD_RUNNING & Xcp_Daq[daqListNo].mode))
                {
                    break;
                }
            }
            /* if no daq is running,reset the Running flag */
            if (daqListNo == XCP_MAX_DAQ)
            {
                Xcp_SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
            }
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
    }
    Xcp_SendResp();
    return;
}

#if (XCP_GET_DAQ_LIST_INFO == STD_ON)
/******************************************************************************/
/*
 * @brief               <Xcp_GetDaqListInfo>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetDaqListInfo(void)
{
    uint8 daqproperty = 0;
    uint16 daqListNo;
    Xcp_DaqNumType daqNum;
    uint16 evChNum;
    uint16 fixedEvent = 0;
    uint16 findEVCount = 0;

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &daqListNo, CPU_BYTE_ORDER);
    if ((daqListNo < XCP_MIN_DAQ) || (daqListNo >= XCP_MAX_DAQ))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        /* find event channel Number for this Daq */
        for (evChNum = 0u; evChNum < XCP_MAX_EVENT_CHANNEL; evChNum++)
        {
            for (daqNum = 0u; daqNum < (Xcp_GlobalCfgPtr->XcpEvCh[evChNum].XcpEvChMaxDaqList); daqNum++)
            {
                if (daqListNo == Xcp_GlobalCfgPtr->XcpEvCh[evChNum].XcpEvChTrigDaqListRef[daqNum])
                {
                    /* get the No of the Event Channel for the current daq list No */
                    fixedEvent = Xcp_GlobalCfgPtr->XcpEvCh[evChNum].XcpEvChNumber;
                    findEVCount++;
                    break;
                }
            }
            if (findEVCount >= 2u) /* Indicating that this DAQ has no fixed event channel,so the 4th response is null */
            {
                break;
            }
        }
    }
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if (daqListNo < XCP_MAX_DAQ)
        {
/* max_odt & max_odt_entries*/
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC)
            Xcp_RespBuffer[2u] = Xcp_GlobalCfgPtr->XcpDaqList[daqListNo].XcpMaxOdt;
            Xcp_RespBuffer[3u] = Xcp_GlobalCfgPtr->XcpDaqList[daqListNo].XcpMaxOdtEntries;
#endif
            /* get daq direction */
            daqproperty = (uint8)((uint8)Xcp_GlobalCfgPtr->XcpDaqList[daqListNo].XcpDaqListType << 2u);
            /* predef of properties */
            if (daqListNo < XCP_MIN_DAQ)
            {
                daqproperty |= 0x01u;
            }
            Xcp_RespBuffer[1u] = daqproperty;
            /* Bit fixed event */
            switch (findEVCount)
            {

            /* No event channel can be asigned to this daq */
            case 0x00u: {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
                break;
            }
            case 0x01u: { /* EVENT_FIXED */
                Xcp_RespBuffer[1u] |= 0x02u;
                Xcp_RespLength = 0x06u;
                break;
            }
            /* not fixed */
            default: {
                Xcp_RespLength = 0x06u;
                break;
            }
            }
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_DAQ_LIST_INFO == STD_ON*/
/*
 * DYNAMIC CONFIG COMMAND
 */
#else
/******************************************************************************/
/*
 * @brief               <Xcp_FreeDaq>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_FreeDaq(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 cnt;

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_DAQ == (Xcp_ProtectionStatus & XCP_PL_DAQ))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            SchM_Enter_Xcp_Exclusive_Daq();
            for (cnt = 0; cnt < XCP_MAX_DAQ; cnt++)
            {
                Xcp_Daq[cnt].mode &= (uint8)(~XCP_DAQ_MOD_RUNNING);
            }
            Xcp_SessionStatus &= (uint8)(~XCP_SESSION_STATE_DAQ_RUNNING);
            for (cnt = 0; cnt < XCP_MAX_EVENT_CHANNEL; cnt++)
            {
                Xcp_EvChBufferUsage[cnt] = 0;
                Xcp_EvChActivCnt[cnt] = 0;
            }
            Xcp_ClearAllDaq();
            /* The XCP_DAQ_CFG_FREE state must be modified after FREE_DAQ is successful*/
            Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_FREE;
            SchM_Exit_Xcp_Exclusive_Daq();
        }
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_AllocDaq>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_AllocDaq(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if (XCP_PL_DAQ == (XCP_PL_DAQ & Xcp_ProtectionStatus))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    /* check daq cfg sequence */
    /* ALLOC_DAQ can use after Free_Daq or Alloc_daq */
    else if ((XCP_DAQ_CFG_FREE != Xcp_DaqCfgSeqStat) && (XCP_DAQ_CFG_DAQ != Xcp_DaqCfgSeqStat))

    {
        Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_IDLE;
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_AllocDaqHal();
    }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_AllocDaqHal(void)
{
    uint16 daqCount = 0;
    Xcp_DaqNumType daqNum;

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2], &daqCount, CPU_BYTE_ORDER);
    /* check daqCount */
    if (daqCount > XCP_DAQ_COUNT)
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        SchM_Enter_Xcp_Exclusive_Daq();
        uint16 allocDaqNum = XCP_MIN_DAQ + daqCount;
        for (daqNum = XCP_MIN_DAQ; daqNum < allocDaqNum; daqNum++)
        {
            Xcp_Daq[daqNum].DynaCfgStatus = XCP_DAQ_CFG_DAQ;
        }
        /* The remaining DAQ status is configured as XCP_DAQ_CFG_FREE */
        for (; daqNum < XCP_MAX_DAQ; daqNum++)
        {
            Xcp_Daq[daqNum].DynaCfgStatus = XCP_DAQ_CFG_FREE;
        }
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
        Xcp_DynamicDaqFirstPIDCnt = 0;
#endif
        Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_DAQ;
        SchM_Exit_Xcp_Exclusive_Daq();
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_AllocOdt>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_AllocOdt(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 5u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection */
        if (XCP_PL_DAQ == (XCP_PL_DAQ & Xcp_ProtectionStatus))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* check daq cfg sequence */
        /* ALLOC_ODT can use after ALLOC_DAQ or ALLOC_ODT */
        else if ((XCP_DAQ_CFG_DAQ != Xcp_DaqCfgSeqStat) && (XCP_DAQ_CFG_ODT != Xcp_DaqCfgSeqStat))
        {
            Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_IDLE;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_AllocOdtHal();
        }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_AllocOdtHal(void)
{
    uint16 daqListNo = 0x0;
    uint8 odtCount = Xcp_CmdBuffer[4u];

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &daqListNo, CPU_BYTE_ORDER);

    if ((daqListNo >= XCP_MAX_DAQ) || (odtCount > XCP_ODT_COUNT)
#if (0u != XCP_MIN_DAQ)
        || (daqListNo < XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else if (
        (XCP_DAQ_CFG_DAQ != Xcp_Daq[daqListNo].DynaCfgStatus) && (XCP_DAQ_CFG_ODT != Xcp_Daq[daqListNo].DynaCfgStatus))
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
    }
    else if (Xcp_DynamicalDaqQueue.bufferSize < (Xcp_DynamicalDaqQueue.bufferPos + (odtCount * sizeof(Xcp_OdtType))))
    {
        Xcp_SetErrorCode(XCP_ERR_MEMORY_OVERFLOW);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        SchM_Enter_Xcp_Exclusive_Daq();
        Xcp_Daq[daqListNo].daqListSize = odtCount;

#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_PID_ABSOLUTE)
        Xcp_Daq[daqListNo].firstPid = Xcp_DynamicDaqFirstPIDCnt;
        Xcp_DynamicDaqFirstPIDCnt += odtCount;
#endif
        /*Add by Qinchun.yang
         * 2020/05/08
         * Pointer access requires four-byte alignment
         * */
        /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
        while (((uint32)(&Xcp_DynDaqBuffer[Xcp_DynamicalDaqQueue.bufferPos]) % 4u) != 0u)
        /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
        {
            Xcp_DynamicalDaqQueue.bufferPos++;
        }
        /*
         * Note:Ignore the warning, the problem has been solved by code.
         * */
        /* PRQA S 3305,0310 ++ */ /* MISRA Rule 11.3 */
        Xcp_Daq[daqListNo].odt = (Xcp_OdtType*)&Xcp_DynDaqBuffer[Xcp_DynamicalDaqQueue.bufferPos];
        /* PRQA S 3305,0310 -- */ /* MISRA Rule 11.3 */
        Xcp_DynamicalDaqQueue.bufferPos += odtCount * sizeof(Xcp_OdtType);
        /* PRQA S 0310 -- */ /* MISRA Rule 20.7 */
        /* PRQA S 3305 -- */ /* MISRA Rule 20.7 */
        do
        {
            odtCount--;
            Xcp_Daq[daqListNo].odt[odtCount].XcpOdtEntryCount = 0u;
        } while (odtCount != 0x00u);
        Xcp_Daq[daqListNo].DynaCfgStatus = XCP_DAQ_CFG_ODT;
        Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_ODT;
        SchM_Exit_Xcp_Exclusive_Daq();
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_AllocOdtEntry>
 *
 * <> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_AllocOdtEntry(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 6u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection */
        if (XCP_PL_DAQ == (XCP_PL_DAQ & Xcp_ProtectionStatus))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* check daq cfg sequence */
        else if ((XCP_DAQ_CFG_ODT != Xcp_DaqCfgSeqStat) && (XCP_DAQ_CFG_ENTRY != Xcp_DaqCfgSeqStat))
        {
            Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_IDLE;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_AllocOdtEntryHal();
        }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_AllocOdtEntryHal(void)
{
    uint16 daqListNo = 0u;
    uint8 odtNum = Xcp_CmdBuffer[4u];
    uint8 entryCount = Xcp_CmdBuffer[5u];
    uint8 daqListSize;

    Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &daqListNo, CPU_BYTE_ORDER);
    if ((daqListNo >= XCP_MAX_DAQ) || (entryCount > XCP_ODTENTRY_COUNT)
#if (0u != XCP_MIN_DAQ)
        || (daqListNo < XCP_MIN_DAQ)
#endif /*0u != XCP_MIN_DAQ*/
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else if (
        (XCP_DAQ_CFG_DAQ == Xcp_Daq[daqListNo].DynaCfgStatus) || (XCP_DAQ_CFG_FREE == Xcp_Daq[daqListNo].DynaCfgStatus))
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
    }
    else if (
        Xcp_DynamicalDaqQueue.bufferSize < (Xcp_DynamicalDaqQueue.bufferPos + (entryCount * sizeof(Xcp_EntryType))))
    {
        Xcp_SetErrorCode(XCP_ERR_MEMORY_OVERFLOW);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_Daq[daqListNo].odt[odtNum].XcpOdtEntryCount = entryCount;

        /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
        while (((uint32)(&Xcp_DynDaqBuffer[Xcp_DynamicalDaqQueue.bufferPos]) % 4u) != 0u)
        /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
        {
            Xcp_DynamicalDaqQueue.bufferPos++;
        }
        /* PRQA S 3305,0310 ++ */ /* MISRA Rule 11.3 */
        Xcp_Daq[daqListNo].odt[odtNum].XcpOdtEntry = (Xcp_EntryType*)&Xcp_DynDaqBuffer[Xcp_DynamicalDaqQueue.bufferPos];
        /* PRQA S 3305,0310 -- */ /* MISRA Rule 11.3 */
        Xcp_DynamicalDaqQueue.bufferPos += entryCount * sizeof(Xcp_EntryType);
        do
        {
            entryCount--;
            /* PRQA S 3305 ++ */ /* MISRA Rule 11.3 */
            Xcp_Daq[daqListNo].odt[odtNum].XcpOdtEntry[entryCount].XcpOdtEntryLength = 0u;
            /* PRQA S 3305 -- */ /* MISRA Rule 11.3 */
        } while (entryCount != 0x00u);
        Xcp_Daq[daqListNo].DynaCfgStatus = XCP_DAQ_CFG_ENTRY;
        Xcp_DaqCfgSeqStat = XCP_DAQ_CFG_ENTRY;
        daqListSize = Xcp_Daq[daqListNo].daqListSize;
        for (odtNum = 0; odtNum < daqListSize; odtNum++)
        {
            if (0x00u == Xcp_Daq[daqListNo].odt[odtNum].XcpOdtEntryCount)
            {
                break;
            }
        }
        /* When all ODTs in the DAQ are assigned ENTRY */
        if (odtNum == Xcp_Daq[daqListNo].daqListSize)
        {
            Xcp_Daq[daqListNo].DynaCfgStatus = XCP_DAQ_CFG_DONE;
        }
    }
    return;
}
#endif /*XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE*/
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /*XCP_PL_DAQ == XCP_PL_DAQ&XCP_RESOURCE*/
