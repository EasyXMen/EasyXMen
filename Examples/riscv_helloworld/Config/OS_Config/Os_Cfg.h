/*============================================================================*/
/*  Copyright (C) iSOFT   (2022), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Os_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:RISC-V>
 *  
 *  @author     <>
 *  @date       <2024-04-24 13:52:07>
 */
/*============================================================================*/

/*============================================================================*/
#ifndef OS_CFG_H
#define OS_CFG_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"

/*=======[F I L E  V E R S I O N   I N F O R M A T I O N]===============================*/
#define     OS_CFG_H_AR_MAJOR_VERSION              19U
#define     OS_CFG_H_AR_MINOR_VERSION              11U
#define     OS_CFG_H_AR_PATCH_VERSION              0U
#define     OS_CFG_H_SW_MAJOR_VERSION              2U
#define     OS_CFG_H_SW_MINOR_VERSION              0U
#define     OS_CFG_H_SW_PATCH_VERSION              0U

/*=======[M A C R O S]========================================================*/

/* --------------------Core-------------------- */
/* Core Definition*/
#define     CFG_CORE_MAX                           3U
#define     OS_CORE_ID_MASTER                      ((Os_CoreIdType)0U)
#define     OS_CORE_ID_0                           ((Os_CoreIdType)0U)
#define     OS_NONAUTOSARCORE_ID_0                     ((Os_CoreIdType)1U)
#define     OS_NONAUTOSARCORE_ID_1                     ((Os_CoreIdType)2U)
/* OsNumberOfCores: number of cores that are controlled by the AUTOSAR OS*/
#define     OS_AUTOSAR_CORES                       1U

#define     CFG_CORE0_AUTOSAROS_ENABLE             TRUE
#define     CFG_CORE1_AUTOSAROS_ENABLE             FALSE
#define     CFG_CORE2_AUTOSAROS_ENABLE             FALSE

/* core start address. */
#define     CFG_CORE0_START_ADDRESS                    &_start
#define     CFG_CORE1_START_ADDRESS                    &_start
#define     CFG_CORE2_START_ADDRESS                    &_start
/* ---------------OsPeripheral Definition-------------------- */
#define     CFG_PERIPHERAL_MAX                      1U
#define     OsPeripheralArea_0  0U
/* --------------------system-------------------- */
/* Scalability Class */
#define     CFG_SC                                  OS_SC1

/* Conformance  Class */
#define     CFG_CC                                  OS_ECC2

/* System Status: stardard, extended */
#define     CFG_STATUS                              OS_STATUS_EXTENDED

/* App Mode Definition */
#define     DONOTCARE                              ((Os_AppModeType)0x0U)
#define     OSDEFAULTAPPMODE                        ((Os_AppModeType)0x1U)

/* Csa Management */
#define     CFG_CSA_MAX_CORE0                       64U
#define     CFG_CSA_MAX_CORE1                       64U
#define     CFG_CSA_MAX_CORE2                       64U

/* system timer */
#define     CFG_SYSTEM_TIMER_ENABLE               TRUE

#define     CFG_SYSTEM_TIMER_ENABLE_CORE0           TRUE
#define     CFG_SYSTEM_TIMER_ENABLE_CORE1           FALSE
#define     CFG_SYSTEM_TIMER_ENABLE_CORE2           FALSE
/* System timer register value define */
#define     CFG_REG_OSTIMER_VALUE_CORE0             10000U
#define     CFG_REG_OSTIMER_VALUE_CORE1             10000U
#define     CFG_REG_OSTIMER_VALUE_CORE2             10000U

/* Timing Protection */
#define     CFG_TIMING_PROTECTION_ENABLE            FALSE
#define     CFG_TIMING_PROTECTION_ENABLE_CORE1         FALSE
#define     CFG_TIMING_PROTECTION_ENABLE_CORE2         FALSE

/* timing protection timer register value */
#define     CFG_REG_TP_TIMER_VALUE_CORE0            100000U
#define     CFG_REG_TP_TIMER_VALUE_CORE1            100000U
#define     CFG_REG_TP_TIMER_VALUE_CORE2            100000U

#define     CFG_SYSTEM_STACK_MAX                    1U

/* wether support trace function */
#define     CFG_TRACE_ENABLE                        FALSE

/* wether check stack overflow */
#define     CFG_STACK_CHECK                         TRUE

#define     CFG_CRITICAL_ZONE_MAX                   2U

/* Hooks */
#define     CFG_ERRORHOOK                           TRUE
#define     CFG_PRETASKHOOK                         FALSE
#define     CFG_POSTTASKHOOK                        FALSE
#define     CFG_STARTUPHOOK                         TRUE
#define     CFG_SHUTDOWNHOOK                        TRUE
#define     CFG_USEGETSERVICEID                     TRUE
#define     CFG_USEPARAMETERACCESS                  TRUE
#define     CFG_PROTECTIONHOOK                      FALSE
#define     CFG_READYTASKHOOK                       FALSE

#define     CFG_APPL_STARTUPHOOK                   FALSE
#define     CFG_APPL_ERRORHOOK                     FALSE
#define     CFG_APPL_SHUTDOWNHOOK                  FALSE

/* --------------------Task Definition-------------------- */
/* Scheduling policy: full preemptive, no preemptive, mixed preemptive */
#define     CFG_SCHED_POLICY                        OS_PREEMPTIVE_MIXED
/* number of task and resource priority in system */
#define     CFG_PRIORITY_MAX_CORE0                  (8U)
/* group of priority */
#define     CFG_PRIORITY_GROUP_CORE0                (1U)
/* All of the tasks */
#define     CFG_TASK_MAX                           (8U)
#define     CFG_EXTENDED_TASK_MAX                  (1U)
/* Core0 */
#define     CFG_TASK_MAX_CORE0                      (8U)
#define     CFG_EXTENDED_TASK_MAX_CORE0             (1U)
#define     OsTask_event                                ((Os_TaskType)0x0000U)
#define     OsTask_100ms                                ((Os_TaskType)0x0001U)
#define     OsTask_10ms                                 ((Os_TaskType)0x0002U)
#define     OsTask_1ms                                  ((Os_TaskType)0x0003U)
#define     OsTask_1s                               ((Os_TaskType)0x0004U)
#define     OsTask_5ms                                  ((Os_TaskType)0x0005U)
#define     OsTask_init                                 ((Os_TaskType)0x0006U)
#define     OS_TASK_IDLE_CORE0                      ((Os_TaskType)0x0007U)
/* Core1 */
#define     CFG_TASK_MAX_CORE1                      (0U)
#define     CFG_EXTENDED_TASK_MAX_CORE1             (0U)
/* Core2 */
#define     CFG_TASK_MAX_CORE2                      (0U)
#define     CFG_EXTENDED_TASK_MAX_CORE2             (0U)
/* --------------------ISR Definition-------------------- */
#define     CFG_ISR_MAX                             12U
#define     CFG_ISR2_MAX                            8U
/* wether support interrupt nest */
#define     CFG_INT_NEST_ENABLE                     TRUE
/* highest ISR2 interrupt priority */
#define     CFG_ISR2_IPL_MAX_CORE0                  10U
#define     CFG_ISR2_IPL_MAX_CORE1                  0U
#define     CFG_ISR2_IPL_MAX_CORE2                  0U

/* Core0*/
#define     CFG_ISR_MAX_CORE0                       12U
#define     CFG_ISR2_MAX_CORE0                      8U
#define     CFG_SYS_TIMER_CORE0_ID                  0x0000U       /*system timer*/
#define     CFG_ISR_OS_INT_CPU_1_IRQn_ID                        0x0001U
#define     CFG_ISR_OS_INT_CPU_2_IRQn_ID                        0x0002U
#define     CFG_ISR_OS_INT_CPU_4_IRQn_ID                        0x0003U
#define     CFG_ISR_OS_INT_CPU_5_IRQn_ID                        0x0004U
#define     CFG_ISR_OS_INT_CPU_6_IRQn_ID                        0x0005U
#define     CFG_ISR_OS_INT_CPU_8_IRQn_ID                        0x0006U
#define     CFG_ISR_OS_INT_CPU_9_IRQn_ID                        0x0007U
#define     CFG_ISR_OS_INT_CPU_12_IRQn_ID                       0x0008U
#define     CFG_ISR_OS_INT_CPU_13_IRQn_ID                       0x0009U
#define     CFG_ISR_OS_INT_CPU_14_IRQn_ID                       0x000aU
#define     CFG_ISR_OS_INT_CPU_15_IRQn_ID                       0x000bU
/* Core1*/
#define     CFG_ISR_MAX_CORE1                       0U
#define     CFG_ISR2_MAX_CORE1                      0U
/* Core2*/
#define     CFG_ISR_MAX_CORE2                       0U
#define     CFG_ISR2_MAX_CORE2                      0U

/* --------------------Counter Definition-------------------- */
#define     CFG_COUNTER_MAX                         1U
/* Core0*/
#define     CFG_COUNTER_MAX_CORE0                   1U
#define     SystemTimer_Core0                       ((Os_CounterType)0x0000U)
#define     OS_TICKS2NS_SystemTimer_Core0(ticks)    ((ticks)*1000*1000)
#define     OS_TICKS2US_SystemTimer_Core0(ticks)    ((ticks)*1000)
#define     OS_TICKS2MS_SystemTimer_Core0(ticks)    ((ticks)*1000/1000)
#define     OS_TICKS2SEC_SystemTimer_Core0(ticks)   ((ticks)*1000/1000000)
#define     OS_NS2TICKS_SystemTimer_Core0(ns)       ((ns)/1000/1000)
#define     OS_US2TICKS_SystemTimer_Core0(us)       ((us)/1000)
#define     OS_MS2TICKS_SystemTimer_Core0(ms)       ((ms)*1000/1000)
#define     OS_SEC2TICKS_SystemTimer_Core0(sec)     ((sec)*1000000/1000)
/* Core1*/
#define     CFG_COUNTER_MAX_CORE1                   0U
/* Core2*/
#define     CFG_COUNTER_MAX_CORE2                   0U
/* --------------------Alarm Definition-------------------- */
#define     CFG_ALARM_MAX                           5U
#define     CFG_AUTO_ALARM_MAX                      0U
/* Core0*/
#define     CFG_ALARM_MAX_CORE0                     5U
#define     CFG_AUTO_ALARM_MAX_CORE0                0U
#define     OsAlarm_100ms                               ((Os_AlarmType)0x0000U)
#define     OsAlarm_10ms                                ((Os_AlarmType)0x0001U)
#define     OsAlarm_1ms                                 ((Os_AlarmType)0x0002U)
#define     OsAlarm_1s                                  ((Os_AlarmType)0x0003U)
#define     OsAlarm_5ms                                 ((Os_AlarmType)0x0004U)
/* Core1*/
#define     CFG_ALARM_MAX_CORE1                     0U
#define     CFG_AUTO_ALARM_MAX_CORE1                0U
/* Core2*/
#define     CFG_ALARM_MAX_CORE2                     0U
#define     CFG_AUTO_ALARM_MAX_CORE2                0U
/* --------------------Schedule Table Definition-------------------- */
#define     CFG_SCHEDTBL_MAX                        1U
/* Core0*/
#define     CFG_SCHEDTBL_MAX_CORE0                  1U
#define     OsScheduleTable_0                       ((Os_ScheduleTableType)0x0000U)
/* Core1*/
#define     CFG_SCHEDTBL_MAX_CORE1                  0U
/* Core2*/
#define     CFG_SCHEDTBL_MAX_CORE2                  0U
/* --------------------Event Definition-------------------- */
#define     CFG_EVENT_MAX                           1U
#define     OsEvent_0                               ((Os_EventType)0x1U)
/* -------------------Resource Definition------------------ */
#define     CFG_USERESSCHEDULER                     TRUE
#define     CFG_RESOURCE_MAX                         2U     /* STD + INTERNAL */
#define     CFG_STD_RESOURCE_MAX                    2U
#define     CFG_INTERNAL_RESOURCE_MAX               0U
/* Core0*/
#define     CFG_RESOURCE_MAX_CORE0                  2U     /* STD + INTERNAL */
#define     CFG_STD_RESOURCE_MAX_CORE0              2U
#define     CFG_INTERNAL_RESOURCE_MAX_CORE0         0U
#define     RES_SCHEDULER_CORE0                     ((Os_ResourceType)0x0000U)
#define     OsResource_0                            ((Os_ResourceType)0x0001U)
/* Core1*/
#define     CFG_RESOURCE_MAX_CORE1                  0U
#define     CFG_STD_RESOURCE_MAX_CORE1              0U
#define     CFG_INTERNAL_RESOURCE_MAX_CORE1         0U
/* Core2*/
#define     CFG_RESOURCE_MAX_CORE2                  0U
#define     CFG_STD_RESOURCE_MAX_CORE2              0U
#define     CFG_INTERNAL_RESOURCE_MAX_CORE2         0U
/* ---------------OsApplication Definition----------------- */
#define     CFG_OSAPPLICATION_MAX                   0U
/* ---------------OsAppEcucPartitionRef-------------------- */
/*Spinlock Definition*/
#define     CFG_SPINLOCK_MAX                       0U

/*  whether shell the os service*/
#define     CFG_SRV_SHELLOS                         FALSE

/* SC34: Memory Protection. */
#define     CFG_MEMORY_PROTECTION_ENABLE            FALSE

/* SC34: Service Protection. */
#define     CFG_SERVICE_PROTECTION_ENABLE           FALSE

#define     CFG_TRUSTED_SYSTEM_SERVICE_MAX          0U
/* ------------------------IOC Definition---------------------- */

#define CFG_IOC_MAX                              0U
#define OS_IOC_DATA_HEAD_SIZE                    sizeof(Os_IocU16Type)

#define     CFG_CPU_USAGE_ENABLE                   FALSE

#endif /* #define OS_CFG_H */
/*=======[E N D   O F   F I L E]==============================================*/

