/*============================================================================*/
/*  Copyright (C) iSOFT   (2022), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Os_UserInf.c>
 *  @brief      <>
 *  
 *  <MCU:RISC-V>
 *  
 *  @author     <>
 *  @date       <2024-04-24 13:52:07>
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#include "Os.h"
#include "Arch_Extend.h"
#include "Arch_Processor.h"

/*=======[V E R S I O N   I N F O R M A T I O N]===============================*/
#define     OS_USERAPP_C_AR_MAJOR_VERSION              19U
#define     OS_USERAPP_C_AR_MINOR_VERSION              11U
#define     OS_USERAPP_C_AR_PATCH_VERSION              0U
#define     OS_USERAPP_C_SW_MAJOR_VERSION              2U
#define     OS_USERAPP_C_SW_MINOR_VERSION              0U
#define     OS_USERAPP_C_SW_PATCH_VERSION              0U

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (OS_USERAPP_C_AR_MAJOR_VERSION != OS_CFG_H_AR_MAJOR_VERSION)
    #error "Os_Userapp.c:Mismatch in Specification Major Version"
#endif
#if (OS_USERAPP_C_AR_MINOR_VERSION != OS_CFG_H_AR_MINOR_VERSION)
    #error "Os_Userapp.c:Mismatch in Specification Minor Version"
#endif
#if (OS_USERAPP_C_AR_PATCH_VERSION != OS_CFG_H_AR_PATCH_VERSION)
    #error "Os_Userapp.c:Mismatch in Specification Patch Version"
#endif
#if (OS_USERAPP_C_SW_MAJOR_VERSION != OS_CFG_H_SW_MAJOR_VERSION)
    #error "Os_Userapp.c:Mismatch in Specification Major Version"
#endif
#if (OS_USERAPP_C_SW_MINOR_VERSION != OS_CFG_H_SW_MINOR_VERSION)
    #error "Os_Userapp.c:Mismatch in Specification Minor Version"
#endif

/*=======[T A S K S]==========================================*/

/*OsTask_init: Core0(CPU0),Type = BASIC, Priority = 1*/
TASK(OsTask_init)
{
    /* please insert your code here ... */
    myprintf("                                                                                   \n");
    myprintf("-----------------------------------------------------------------------------------\n");
    myprintf("                                                                                   \n");
    myprintf("Hello, EasyXMen!                                                                   \n");
    myprintf("                                                                                   \n");
#if __riscv_xlen == 32
    myprintf("ISA: rv32                                                                          \n");
#else
    myprintf("ISA: rv64                                                                          \n");
#endif
#if __SIZEOF_POINTER__ == 8
    myprintf("ABI: lp64                                                                          \n");
#else
    myprintf("ABI: ilp32                                                                         \n");
#endif

    myprintf("                                                                                   \n");
    myprintf("  ________                         __       __ _         _                         \n");
    myprintf(" |  ______|                        \\ \\     / /| \\       / |                     \n");
    myprintf(" | |         ____    ____   _    _  \\ \\   / / |  \\     /  |  ____   ______      \n");
    myprintf(" | |______  / _  \\  / ___| | |  | |  \\ \\ / /  | \\ \\   / / | / __ \\ |  __  |  \n");
    myprintf(" |  ______|| / \\  || (___  | |  | |   > _ <   | |\\ \\ / /| || (__) || |  | |     \n");
    myprintf(" | |       ||   | | \\ __ \\ | |__| |  / / \\ \\  | | \\_v_/ | ||  ___/ | |  | |   \n");
    myprintf(" | |______ | \\__| |  ___) | \\___, | / /   \\ \\ | |       | || (____ | |  | |    \n");
    myprintf(" |________| \\__/|_| |___ /      | |/_/     \\ \\|_|       |_| \\_____||_|  |_|    \n");
    myprintf("                            __ /  |                                                \n");
    myprintf("                           |_____/                                                 \n");
    myprintf("                                                                                   \n");
    myprintf("                                                                                   \n");
    myprintf("-----------------------------------------------------------------------------------\n");
    myprintf("                                                                                   \n");
    myprintf("                                                                                   \n");




    /*activate task*/
    ActivateTask(OsTask_100ms);
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*OsTask_1ms: Core0(CPU0),Type = BASIC, Priority = 6*/
TASK(OsTask_1ms)
{
    /* please insert your code here ... */
    myprintf("| | | Enter task D\n");
    myprintf("| | | Exit  task D\n");
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*OsTask_5ms: Core0(CPU0),Type = BASIC, Priority = 5*/
TASK(OsTask_5ms)
{
    /* please insert your code here ... */
    myprintf("| | Enter task C\n");
    ActivateTask(OsTask_1ms);
    myprintf("| | Exit  task C\n");
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*OsTask_10ms: Core0(CPU0),Type = BASIC, Priority = 4*/
TASK(OsTask_10ms)
{
    /* please insert your code here ... */
    myprintf("| Enter task B\n");
    ActivateTask(OsTask_5ms);
    myprintf("| Exit  task B\n");
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*OsTask_100ms: Core0(CPU0),Type = BASIC, Priority = 3*/
TASK(OsTask_100ms)
{
    /* please insert your code here ... */
    myprintf("Start task switch\n");
    myprintf("Enter task A\n");
    ActivateTask(OsTask_10ms);
    myprintf("Exit  task A\n");
    myprintf("End   task switch\n\n");

    myprintf("Start periodic task\n");
    SetRelAlarm(OsAlarm_1s,1000,2000);
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*OsTask_1s: Core0(CPU0),Type = BASIC, Priority = 2*/
TASK(OsTask_1s)
{
    /* please insert your code here ... */
    myprintf("Hello, EasyXMen! \n");
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*OsTask_event: Core0(CPU0),Type = EXTEND, Priority = 8*/
TASK(OsTask_event)
{
    /* please insert your code here ... */
    if (E_OK != TerminateTask())
    {
        while (1)
        {
            /* dead loop */
        }
    }
}

/*=======[H O O K S]================================================*/
FUNC(void, OS_APPL_CODE) IdleHook_Core0(void)
{
    /* please insert your code here ... */
}

/*=======[ALARM CALL BACK]==========================================*/

FUNC(void, OS_APPL_CODE) ErrorHook(StatusType Error)
{
    /***The code below is just for deleting the complier warning.***
    ***Please remove it and use your own code****/
    (void)Error;
    /***The code above is just for deleting the complier warning*/
    /* please insert your code here ... */
}

FUNC(void, OS_APPL_CODE) StartupHook(void)
{
    /* please insert your code here ... */
}

FUNC(void, OS_APPL_CODE) ShutdownHook(StatusType Error)
{
    /***The code below is just for deleting the complier warning.***
    ***Please remove it and use your own code****/
    (void)Error;
    /***The code above is just for deleting the complier warning*/
    /* please insert your code here ... */
}

/*=======[P A N I C H A N D L E R]================================*/
FUNC(void, OS_CODE) Arch_PanicHandler(void)
{
    /* please insert your code here ... */
}

/*=======[I S R S]================================================*/
/*
 *ISR(ISR_CPU_1: Core0(CPU0))
 */
ISR(ISR_CPU_1)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_2: Core0(CPU0))
 */
ISR(ISR_CPU_2)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_4: Core0(CPU0))
 */
ISR(ISR_CPU_4)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_5: Core0(CPU0))
 */
ISR(ISR_CPU_5)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_6: Core0(CPU0))
 */
ISR(ISR_CPU_6)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_8: Core0(CPU0))
 */
ISR(ISR_CPU_8)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_9: Core0(CPU0))
 */
ISR(ISR_CPU_9)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_12: Core0(CPU0))
 */
ISR(ISR_CPU_12)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_13: Core0(CPU0))
 */
ISR(ISR_CPU_13)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_14: Core0(CPU0))
 */
ISR(ISR_CPU_14)
{
    /* please insert your code here ... */
}

/*
 *ISR(ISR_CPU_15: Core0(CPU0))
 */
ISR(ISR_CPU_15)
{
    /* please insert your code here ... */
}

/*=======[E N D   O F   F I L E]==============================================*/

