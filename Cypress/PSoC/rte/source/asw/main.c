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
#include "rte_signalpool.h"

#include "adc.h"
#include "pwm.h"
#include "gpio.h"

ISR(systick_handler)
{
    CounterTick(cnt_systick);
}

typedef RC_t (*RC_tFunctPtrv)(void);

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static RC_tFunctPtrv fptr_init_functions[] = 
{
    // Add all the initialization functions here
    ADC_Init,
    PWM_Init,
    GPIO_Init,
};

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
    // MCAL - hardware initialization 
    for (unsigned int Index = 0; Index < sizeof(fptr_init_functions)/sizeof(RC_tFunctPtrv); Index++)
    {
        if (RC_SUCCESS != fptr_init_functions[Index]())
        {
            // Hardware Fault - Take necessary action
            while(1) __asm("NOP");
        }
    }
    
    // RTE Initializations
    RTE_ADC_init(&JOYSTICK_signal, &ADC_INIT_DATA);
    RTE_PWM_init(&ENGINE_signal, &PWM_INIT_DATA);
    RTE_GPIO_init(&BRAKELIGHT_signal, &GPIO_INIT_DATA);
    
    // OS initialization - This will override and reconfigures the interrupts by OS parameters
    EE_system_init();
    
    // Activate Tasks here
    ActivateTask(tsk_Background);
    ActivateTask(tsk_EventDispatcher);
    
#define TSK_CYCLICDISPATCHER_CYCLIC_TIME    10
    // Set Alarms here
    SetRelAlarm(alrm_10ms, 10, TSK_CYCLICDISPATCHER_CYCLIC_TIME);
    
    // Terminate the init task and let the schedular do its thing!
    TerminateTask();
}

TASK(tsk_CyclicDispatcher)
{
    uint16_t        Index = 0;
    
    for (Index = 0; Index < RTE_cyclicActivation_size; Index++)
    {
        RTE_cyclicActivationTable[Index].run();
    }
    
    TerminateTask();
}


TASK(tsk_EventDispatcher)
{
    uint16_t        Index = 0;
    EventMaskType   EventsToMonitor = 0, EventsReceived = 0;
    
    for (Index = 0; Index < RTE_eventActivation_size; Index++)
    {
        EventsToMonitor = EventsToMonitor | RTE_eventActivationTable[Index].event;
    }
    
    while (1)
    {
        WaitEvent(EventsToMonitor);
        GetEvent(tsk_EventDispatcher, &EventsReceived);
        ClearEvent(EventsReceived);
        
        for (Index = 0; Index < RTE_eventActivation_size; Index++)
        {
            if (RTE_eventActivationTable[Index].event & EventsReceived)
            {
                RTE_eventActivationTable[Index].run();
            }
        }
    }
}


TASK(tsk_Background)
{
    while(1);
    TerminateTask();
}
/* [] END OF FILE */
