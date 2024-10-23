/*============================================================================*/
/*  Copyright (C) iSOFT   (2022), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Os_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:RISC-V>
 *  
 *  @author     <>
 *  @date       <2024-04-24 13:52:07>
 */
/*============================================================================*/
/*=======[I N C L U D E S]====================================================*/
#include "Os_Cfg.h"
#include "Os_CfgData.h"
#include "Os.h"
#include "Arch_Processor.h"
#include "Os_Internal.h"
/*=======[V E R S I O N   I N F O R M A T I O N]===============================*/
#define     OS_CFG_C_AR_MAJOR_VERSION              19U
#define     OS_CFG_C_AR_MINOR_VERSION              11U
#define     OS_CFG_C_AR_PATCH_VERSION              0U
#define     OS_CFG_C_SW_MAJOR_VERSION              2U
#define     OS_CFG_C_SW_MINOR_VERSION              0U
#define     OS_CFG_C_SW_PATCH_VERSION              0U
/*=======[V E R S I O N  C H E C K]===========================================*/
#if (OS_CFG_C_AR_MAJOR_VERSION != OS_CFG_H_AR_MAJOR_VERSION)
    #error "Os_Cfg.c:Mismatch in Specification Major Version"
#endif
#if (OS_CFG_C_AR_MINOR_VERSION != OS_CFG_H_AR_MINOR_VERSION)
    #error "Os_Cfg.c:Mismatch in Specification Minor Version"
#endif
#if (OS_CFG_C_AR_PATCH_VERSION != OS_CFG_H_AR_PATCH_VERSION)
    #error "Os_Cfg.c:Mismatch in Specification Patch Version"
#endif
#if (OS_CFG_C_SW_MAJOR_VERSION != OS_CFG_H_SW_MAJOR_VERSION)
    #error "Os_Cfg.c:Mismatch in Specification Major Version"
#endif
#if (OS_CFG_C_SW_MINOR_VERSION != OS_CFG_H_SW_MINOR_VERSION)
    #error "Os_Cfg.c:Mismatch in Specification Minor Version"
#endif
/*=======[M A C R O S]========================================================*/
#define  OS_STACK_TOP(stack)    ((Os_StackPtrType)((&(stack)[0]) + (sizeof(stack) / sizeof((stack)[0]))))
#define  OS_STACK_BOTTOM(stack) ((Os_StackPtrType)(stack))
/*=======[I N T E R N A L   D A T A]==========================================*/
/* Ready Queue Manager */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST (OsPeripheralAreaCfgType, OS_CONST) Os_PeriAreaCfg[CFG_PERIPHERAL_MAX] =
{
    {
        0x00002000, //OsPeripheralAreaStartAddress
        0x00004000, //OsPeripheralAreaEndAddress
    },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_TaskStack_Idle_Core0[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_CounterCfgType, OS_CONST) Os_CounterCfgCore0[CFG_COUNTER_MAX_CORE0] =
{    {
        65535,    /*osCounterMaxAllowedValue*/
        1,    /*osCounterMinCycle*/
        1,    /*osCounterTicksPerBase*/
        COUNTER_HARDWARE, /*osCounterType*/
        1000, /*osSecondsPerTick*/
    },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CODE) Os_ArchSystemTimerCore0(void)
{
    (void)Os_IncrementHardCounter(SystemTimer_Core0);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) AlarmCallback_OsAlarm_100ms(void)
{
    (void)ActivateTask(OsTask_100ms);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) AlarmCallback_OsAlarm_10ms(void)
{
    (void)ActivateTask(OsTask_10ms);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) AlarmCallback_OsAlarm_1ms(void)
{
    (void)ActivateTask(OsTask_1ms);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) AlarmCallback_OsAlarm_1s(void)
{
    (void)ActivateTask(OsTask_1s);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) AlarmCallback_OsAlarm_5ms(void)
{
    (void)ActivateTask(OsTask_5ms);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_AlarmCfgType, OS_CONST) Os_AlarmCfgCore0[CFG_ALARM_MAX_CORE0] =
{
    {
        Os_GetObjLocalId(SystemTimer_Core0),  /*osAlarmCounter*/
        NULL_PTR,   /*osAlarmAutostartRef*/
        &AlarmCallback_OsAlarm_100ms,   /*osAlarmCallback*/
    },
    {
        Os_GetObjLocalId(SystemTimer_Core0),  /*osAlarmCounter*/
        NULL_PTR,   /*osAlarmAutostartRef*/
        &AlarmCallback_OsAlarm_10ms,    /*osAlarmCallback*/
    },
    {
        Os_GetObjLocalId(SystemTimer_Core0),  /*osAlarmCounter*/
        NULL_PTR,   /*osAlarmAutostartRef*/
        &AlarmCallback_OsAlarm_1ms, /*osAlarmCallback*/
    },
    {
        Os_GetObjLocalId(SystemTimer_Core0),  /*osAlarmCounter*/
        NULL_PTR,   /*osAlarmAutostartRef*/
        &AlarmCallback_OsAlarm_1s,  /*osAlarmCallback*/
    },
    {
        Os_GetObjLocalId(SystemTimer_Core0),  /*osAlarmCounter*/
        NULL_PTR,   /*osAlarmAutostartRef*/
        &AlarmCallback_OsAlarm_5ms, /*osAlarmCallback*/
    },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_AlarmCfgType, AUTOMATIC, OS_CONST) Os_AlarmCfg_Inf[OS_AUTOSAR_CORES] =
{
    Os_AlarmCfgCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_AlarmType, OS_CONST) Os_CfgAlarmMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_ALARM_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_CounterCfgType, AUTOMATIC, OS_VAR) Os_CounterCfg_Inf[OS_AUTOSAR_CORES] =
{
    Os_CounterCfgCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_CounterType, OS_CONST) Os_CfgCounterMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_COUNTER_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* ++ OsOs Configuration ++ */
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_SysStack_Core0[1024];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_StackType, OS_CONST) Os_SystemStackCore0 =
{
    OS_STACK_TOP(Os_SysStack_Core0), OS_STACK_BOTTOM(Os_SysStack_Core0)/* PRQA S 0488 */ /* MISRA Rule 18.4 */
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/* -- OsOs Configuration -- */

/* ++ OsIoc Configuration ++ */

/* PRQA S 1840 ++*/ /* MISRA Rule 10.4 */

/* PRQA S 1840 --*/ /* MISRA Rule 10.4 */

/* -- OsIoc Configuration -- */

/* ++ OsIsr Configuration ++ */
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_SysTimer_Stack_Core0[512];/*system timer*/
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_1_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_2_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_4_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_5_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_6_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_8_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OS_INT_CPU_9_IRQn_Stack_Core0[256];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_StackType, OS_CONST) Os_ISR2StackCore0[CFG_ISR2_MAX_CORE0] =
{
    /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */
    {OS_STACK_TOP(Os_SysTimer_Stack_Core0), OS_STACK_BOTTOM(Os_SysTimer_Stack_Core0)},/*system timer*/
    {OS_STACK_TOP(Os_OS_INT_CPU_1_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_1_IRQn_Stack_Core0)},
    {OS_STACK_TOP(Os_OS_INT_CPU_2_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_2_IRQn_Stack_Core0)},
    {OS_STACK_TOP(Os_OS_INT_CPU_4_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_4_IRQn_Stack_Core0)},
    {OS_STACK_TOP(Os_OS_INT_CPU_5_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_5_IRQn_Stack_Core0)},
    {OS_STACK_TOP(Os_OS_INT_CPU_6_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_6_IRQn_Stack_Core0)},
    {OS_STACK_TOP(Os_OS_INT_CPU_8_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_8_IRQn_Stack_Core0)},
    {OS_STACK_TOP(Os_OS_INT_CPU_9_IRQn_Stack_Core0), OS_STACK_BOTTOM(Os_OS_INT_CPU_9_IRQn_Stack_Core0)},
    /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_IsrCfgType, OS_CONST) Os_IsrCfgCore0[CFG_ISR_MAX_CORE0] =
{
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        FALSE,  /*OsNestedEnable*/
        OS_INT_CPU_MTI_IRQn,    /*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },

    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_1_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_2_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_4_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_5_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_6_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_8_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY2,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_9_IRQn,/*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY1,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_12_IRQn, /*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY1,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_13_IRQn, /*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY1,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_14_IRQn, /*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
    {
        OS_ISR_CATEGORY1,   /*OsIsrCatType*/
        TRUE,               /*OsNestedEnable*/
        OS_INT_CPU_15_IRQn, /*OsIsrSrc*/
        OS_ARCH_INT_CPU0, /*OsIsrSrcType*/
    },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_IsrType, OS_CONST) Os_CfgIsrMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_ISR_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_IsrType, OS_CONST) Os_CfgIsr2Max_Inf[OS_AUTOSAR_CORES] =
{
    CFG_ISR2_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_StackType, AUTOMATIC, OS_VAR) Os_Isr2Stack_Inf[OS_AUTOSAR_CORES] =
{
    Os_ISR2StackCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_IsrCfgType, AUTOMATIC, OS_VAR) Os_IsrCfg_Inf[OS_AUTOSAR_CORES] =
{
    Os_IsrCfgCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_IPLType, OS_CONST) Os_CfgIsr2TplMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_ISR2_IPL_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/* -- OsIsr Configuration -- */

/* ++ OsResource Configuration ++ */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_ResourceCfgType, OS_CONST) Os_ResourceCfgCore0[CFG_STD_RESOURCE_MAX_CORE0] =
{
    {
     7U,    /*ceiling*/
     OCCUPIED_BY_TASK,  /*resourceOccupyType*/
     0U,/*rsv1*/
     },
    {
     6U,    /*ceiling*/
     OCCUPIED_BY_TASK,  /*resourceOccupyType*/
     0U,/*rsv1*/
     },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_ResourceType, OS_CONST) Os_ResScheduler_Inf[OS_AUTOSAR_CORES] =
{
    RES_SCHEDULER_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_ResourceCfgType, AUTOMATIC, OS_VAR) Os_ResourceCfg_Inf[OS_AUTOSAR_CORES] =
{
    Os_ResourceCfgCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_ResourceType, OS_CONST) Os_CfgResourceMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_RESOURCE_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_ResourceType, OS_CONST) Os_CfgStdResourceMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_STD_RESOURCE_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/* -- OsResource Configuration -- */

/* ++ OsScheduleTable Configuration ++ */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_EPSetEventAction, OS_CONST) Os_OsScheduleTable_0OsScheduleTableExpiryPoint_0EventList[1] =
{
    {OsEvent_0, OsTask_event},
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_SchedTblEP, OS_CONST) Os_OsScheduleTable_0EPList[1] =
{
    {
        0,  /*osSchedTblEPOffset*/
        1,  /*osSetEventListSize*/
        0,  /*osActivateTaskListSize*/
        Os_OsScheduleTable_0OsScheduleTableExpiryPoint_0EventList,  /*osSetEventList*/
        NULL_PTR,   /*osActivateTaskList*/
    },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_SchedTblCfgType, OS_CONST) Os_SchedTblCfgCore0[CFG_SCHEDTBL_MAX_CORE0] =
{
    {
        2000,   /*osSchedTblDuration*/
        TRUE,   /*osSchedTblRepeating*/
        Os_GetObjLocalId(SystemTimer_Core0), /*osSchedTblCounterRef*/
        NULL_PTR, /*osSchedTblAutostartRef*/
        Os_OsScheduleTable_0EPList, /*osSchedTblEP*/
        1,  /*osSchedTblEPsize*/
    },
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_SchedTblCfgType, AUTOMATIC, OS_CONST) Os_SchedTblCfg_Inf[OS_AUTOSAR_CORES] =
{
    Os_SchedTblCfgCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_ScheduleTableType, OS_CONST) Os_CfgSchedTblMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_SCHEDTBL_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/* -- OsScheduleTable Configuration -- */

/* ++ OsTask Configuration ++ */
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_0[1];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_1[2];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_2[2];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_3[2];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_4[2];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_5[2];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_6[3];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_TaskType, OS_VAR) Os_ActivateQueue_Core0_7[3];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_ReadyQueueType, OS_VAR)  Os_ReadyQueueMark_Core0[CFG_PRIORITY_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONSTP2VAR(Os_TaskType, OS_CONST, OS_VAR) Os_ReadyQueue_Core0[CFG_PRIORITY_MAX_CORE0] =
{
    Os_ActivateQueue_Core0_0,
    Os_ActivateQueue_Core0_1,
    Os_ActivateQueue_Core0_2,
    Os_ActivateQueue_Core0_3,
    Os_ActivateQueue_Core0_4,
    Os_ActivateQueue_Core0_5,
    Os_ActivateQueue_Core0_6,
    Os_ActivateQueue_Core0_7,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_ActivateQueueSize_Core0[CFG_PRIORITY_MAX_CORE0] =
{
    1,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_PriorityType, OS_CONST) Os_PrioGroupCore0[CFG_PRIORITY_MAX_CORE0] =
{
    0U,
    0U,
    0U,
    0U,
    0U,
    0U,
    0U,
    0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_PriorityType, OS_CONST) Os_PrioMaskCore0[CFG_PRIORITY_MAX_CORE0] =
{
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x0010,
    0x0020,
    0x0040,
    0x0080,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_event_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_100ms_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_10ms_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_1ms_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_1s_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_5ms_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"
static VAR(Os_StackDataType, OS_VAR) Os_OsTask_init_Stack[512];
#define OS_STOP_SEC_VAR_STACK_CORE0
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_StackType, OS_CONST) Os_TaskStackCore0[CFG_TASK_MAX_CORE0] =
{
    /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ 
    {OS_STACK_TOP(Os_OsTask_event_Stack), OS_STACK_BOTTOM(Os_OsTask_event_Stack)},
    {OS_STACK_TOP(Os_OsTask_100ms_Stack), OS_STACK_BOTTOM(Os_OsTask_100ms_Stack)},
    {OS_STACK_TOP(Os_OsTask_10ms_Stack), OS_STACK_BOTTOM(Os_OsTask_10ms_Stack)},
    {OS_STACK_TOP(Os_OsTask_1ms_Stack), OS_STACK_BOTTOM(Os_OsTask_1ms_Stack)},
    {OS_STACK_TOP(Os_OsTask_1s_Stack), OS_STACK_BOTTOM(Os_OsTask_1s_Stack)},
    {OS_STACK_TOP(Os_OsTask_5ms_Stack), OS_STACK_BOTTOM(Os_OsTask_5ms_Stack)},
    {OS_STACK_TOP(Os_OsTask_init_Stack), OS_STACK_BOTTOM(Os_OsTask_init_Stack)},
    {OS_STACK_TOP(Os_TaskStack_Idle_Core0), OS_STACK_BOTTOM(Os_TaskStack_Idle_Core0)}
    /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(Os_TaskCfgType, OS_CONST) Os_TaskCfgCore0[CFG_TASK_MAX_CORE0] =
{
    {
        &Os_TaskEntry_OsTask_event, /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_event), /*osTaskStackId*/
        1,  /*osTaskActivation*/
        7,  /*osTaskPriority*/
        OS_PREEMPTIVE_FULL, /*osTaskSchedule*/
        0U,/*osRsv1*/
        OS_NULL_APPMODE,    /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_OsTask_100ms,     /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_100ms),   /*osTaskStackId*/
        1,                              /*osTaskActivation*/
        3,                              /*osTaskPriority*/
        OS_PREEMPTIVE_FULL,             /*osTaskSchedule*/
        0U,                             /*osRsv1*/
        OS_NULL_APPMODE,                /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_OsTask_10ms,      /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_10ms),   /*osTaskStackId*/
        1,                              /*osTaskActivation*/
        4,                              /*osTaskPriority*/
        OS_PREEMPTIVE_FULL,             /*osTaskSchedule*/
        0U,                             /*osRsv1*/
        OS_NULL_APPMODE,                /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_OsTask_1ms,       /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_1ms),   /*osTaskStackId*/
        1,                              /*osTaskActivation*/
        6,                              /*osTaskPriority*/
        OS_PREEMPTIVE_FULL,             /*osTaskSchedule*/
        0U,                             /*osRsv1*/
        OS_NULL_APPMODE,                /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_OsTask_1s,    /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_1s),   /*osTaskStackId*/
        1,                              /*osTaskActivation*/
        2,                              /*osTaskPriority*/
        OS_PREEMPTIVE_FULL,             /*osTaskSchedule*/
        0U,                             /*osRsv1*/
        OS_NULL_APPMODE,                /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_OsTask_5ms,       /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_5ms),   /*osTaskStackId*/
        1,                              /*osTaskActivation*/
        5,                              /*osTaskPriority*/
        OS_PREEMPTIVE_FULL,             /*osTaskSchedule*/
        0U,                             /*osRsv1*/
        OS_NULL_APPMODE,                /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_OsTask_init,      /*osTaskEntry*/
        Os_GetObjLocalId(OsTask_init),   /*osTaskStackId*/
        1,                              /*osTaskActivation*/
        1,                              /*osTaskPriority*/
        OS_PREEMPTIVE_NON,              /*osTaskSchedule*/
        0U,                             /*osRsv1*/
        OSDEFAULTAPPMODE,               /*osTaskAutoStartMode*/
    },
    {
        &Os_TaskEntry_IdleCore0,            /*osTaskEntry*/
        Os_GetObjLocalId(OS_TASK_IDLE_CORE0), /*osTaskStackId*/
        1,                                        /*osTaskActivation*/
        0,                                        /*osTaskPriority*/
        OS_PREEMPTIVE_FULL,                       /*osTaskSchedule*/
        0U,                                       /*osRsv1*/
        OS_ALL_APPMODE,                           /*osTaskAutoStartMode*/

    }
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_TaskType, OS_CONST) Os_CfgExtendTaskMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_EXTENDED_TASK_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(uint32, AUTOMATIC, OS_VAR) Os_ActivateQueueSize_Inf[OS_AUTOSAR_CORES] =
{
    Os_ActivateQueueSize_Core0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_TaskRefType, AUTOMATIC, OS_VAR) Os_ReadyQueue_Inf[OS_AUTOSAR_CORES] =
{
    Os_ReadyQueue_Core0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_PriorityType, AUTOMATIC, OS_VAR) Os_PrioGroup_Inf[OS_AUTOSAR_CORES] =
{
    Os_PrioGroupCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_PriorityType, AUTOMATIC, OS_VAR) Os_PrioMask_Inf[OS_AUTOSAR_CORES] =
{
    Os_PrioMaskCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(uint16, OS_CONST) Os_CfgPriorityMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_PRIORITY_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2VAR(Os_ReadyQueueType, AUTOMATIC, OS_VAR) Os_ReadyQueueMark_Inf[OS_AUTOSAR_CORES] = /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    Os_ReadyQueueMark_Core0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(uint8, OS_CONST) Os_CfgPriorityGroup_Inf[OS_AUTOSAR_CORES] =
{
    CFG_PRIORITY_GROUP_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_StackType, AUTOMATIC, OS_VAR) Os_SystemStack_Inf[OS_AUTOSAR_CORES] =
{
    &Os_SystemStackCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_TaskCfgType, AUTOMATIC, OS_CONST) Os_TaskCfg_Inf[OS_AUTOSAR_CORES] =
{
    Os_TaskCfgCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(Os_StackType, AUTOMATIC, OS_CONST) Os_TaskStack_Inf[OS_AUTOSAR_CORES] =
{
    Os_TaskStackCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(uint16, OS_CONST) Os_CfgTaskMax_Inf[OS_AUTOSAR_CORES] =
{
    CFG_TASK_MAX_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(uint16, OS_CONST) Os_TASK_IDLE_Inf[OS_AUTOSAR_CORES] =
{
    OS_TASK_IDLE_CORE0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/* -- OsTask Configuration -- */

/* ++ ServiceProtection Configuration ++ */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_event[1] =
{

    0x1U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_100ms[1] =
{
    0x1U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_10ms[1] =
{
    0x3U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_1ms[1] =
{
    0x3U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_1s[1] =
{
    0x1U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_5ms[1] =
{
    0x3U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OsTask_init[1] =
{
    0x1U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_OS_TASK_IDLE_CORE0[1] =
{
    0x0000U
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONSTP2CONST(uint16, OS_VAR, OS_CONST) Os_TaskResourceAccessMaskCore0[CFG_TASK_MAX_CORE0] =
{
    Os_ResourceAccessMask_OsTask_event,
    Os_ResourceAccessMask_OsTask_100ms,
    Os_ResourceAccessMask_OsTask_10ms,
    Os_ResourceAccessMask_OsTask_1ms,
    Os_ResourceAccessMask_OsTask_1s,
    Os_ResourceAccessMask_OsTask_5ms,
    Os_ResourceAccessMask_OsTask_init,
    Os_ResourceAccessMask_OS_TASK_IDLE_CORE0,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(uint16* const, AUTOMATIC, OS_VAR)Os_TaskResourceAccessMask_Inf[OS_AUTOSAR_CORES] =
{
    Os_TaskResourceAccessMaskCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_STM0_SR0[1] =
{
    0x1U
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_1[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_2[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_4[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_5[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_6[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_8[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint16, OS_CONST) Os_ResourceAccessMask_ISR_CPU_9[1] =
{

    0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONSTP2CONST(uint16, OS_VAR, OS_CONST) Os_IsrResourceAccessMaskCore0[CFG_ISR2_MAX_CORE0] =
{
    Os_ResourceAccessMask_ISR_STM0_SR0,
    Os_ResourceAccessMask_ISR_CPU_1,
    Os_ResourceAccessMask_ISR_CPU_2,
    Os_ResourceAccessMask_ISR_CPU_4,
    Os_ResourceAccessMask_ISR_CPU_5,
    Os_ResourceAccessMask_ISR_CPU_6,
    Os_ResourceAccessMask_ISR_CPU_8,
    Os_ResourceAccessMask_ISR_CPU_9,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(uint16* const, AUTOMATIC, OS_VAR)Os_IsrResourceAccessMask_Inf[OS_AUTOSAR_CORES] =
{
    Os_IsrResourceAccessMaskCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_event[1] =
{

    0x1U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_100ms[1] =
{
     0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_10ms[1] =
{
     0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_1ms[1] =
{
     0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_1s[1] =
{
     0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_5ms[1] =
{
     0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OsTask_init[1] =
{
     0x0U,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) Os_EventAccessMask_OS_TASK_IDLE_CORE0[1] =
{
    0x0000U
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONSTP2CONST(uint32, OS_VAR, OS_CONST) Os_TaskEventAccessMaskCore0[CFG_TASK_MAX_CORE0] =
{
    Os_EventAccessMask_OsTask_event,
    Os_EventAccessMask_OsTask_100ms,
    Os_EventAccessMask_OsTask_10ms,
    Os_EventAccessMask_OsTask_1ms,
    Os_EventAccessMask_OsTask_1s,
    Os_EventAccessMask_OsTask_5ms,
    Os_EventAccessMask_OsTask_init,
    Os_EventAccessMask_OS_TASK_IDLE_CORE0,
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONSTP2CONST(uint32* const, AUTOMATIC, OS_VAR)Os_TaskEventAccessMask_Inf[OS_AUTOSAR_CORES] =
{
    Os_TaskEventAccessMaskCore0
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/* -- ServiceProtection Configuration -- */
/*=======[E N D   O F   F I L E]==============================================*/

