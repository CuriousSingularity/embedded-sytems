/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "rte_activation.h"

ISR(systick_handler)
{
    CounterTick(cnt_systick);
}

#if 0
typedef RC_t (*RC_tFunctPtrv)(void);

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static RC_tFunctPtrv fptr_init_functions[] = 
{
    // Add all the initialization functions here
    CONSOLE_Init,
    S7D_Init,
    PWM_Init,
};
#endif

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
    EE_systick_enable_int();
    EE_systick_start();

    for(;;)
    {
        /* Place your application code here. */
        StartOS(OSDEFAULTAPPMODE);
    }
    
    return 0;
}

/**
 * The Task declaration
 * The tsk_Init initializes all the necessary hardware components.
 * This is an autostart task which will be run once the OS starts.
 */
TASK(tsk_Init)
{
    #if 0
    // MCAL - hardware initialization 
    for (unsigned int Index = 0; Index < sizeof(fptr_init_functions)/sizeof(RC_tFunctPtrv); Index++)
    {
        if (RC_SUCCESS != fptr_init_functions[Index]())
        {
            // Hardware Fault - Take necessary action
            while(1) __asm("NOP");
        }
    }
    #endif
    
    // OS initialization - This will override and reconfigures the interrupts by OS parameters
    EE_system_init();
    
    // Activate Tasks here
    ActivateTask(tsk_EventDispatcher);
    ActivateTask(tsk_Background);

#define TSK_CYCLICDISPATCHER_CYCLIC_TIME    10
    // Set Alarms here
    SetRelAlarm(alrm_10ms, 10, TSK_CYCLICDISPATCHER_CYCLIC_TIME);
    
    // Terminate the init task and let the schedular do its thing!
    TerminateTask();
}

TASK(tsk_CyclicDispatcher)
{
    
    TerminateTask();
}


TASK(tsk_EventDispatcher)
{
    
    TerminateTask();
}


TASK(tsk_Background)
{
    TerminateTask();
}
/* [] END OF FILE */
