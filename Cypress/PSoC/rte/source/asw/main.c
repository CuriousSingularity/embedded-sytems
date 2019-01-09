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

#include <math.h>
#include "adc.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "watchdog.h"
#include "swc_application.h"

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
    UART_Init,
    ADC_Init,
    PWM_Init,
    GPIO_Init,
};

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    WD_CheckResetBit();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
    EE_systick_enable_int();
    EE_systick_start();

        /* Place your application code here. */
    StartOS(OSDEFAULTAPPMODE);
    
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
    SetRelAlarm(alrm_10ms, TSK_CYCLICDISPATCHER_CYCLIC_TIME, TSK_CYCLICDISPATCHER_CYCLIC_TIME);
    
    //Start the watch dog
    if (WD_CheckResetBit())
    {
        UART_Write("INFO: System Reboot due to Watchdog.\n");
    }
    else
    {
        UART_Write("INFO: System Reboot due to Power on Reset.\n");
    }
    
    WD_Start(WDT_2048_TO_3072_MS);
    
    // Terminate the init task and let the schedular do its thing!
    TerminateTask();
}

TASK(tsk_CyclicDispatcher)
{
    uint16_t        Index = 0;
    static uint16_t TimeToMonitor = 0;
    
    TimeToMonitor += TSK_CYCLICDISPATCHER_CYCLIC_TIME;
    
    for (Index = 0; Index < RTE_cyclicActivation_size; Index++)
    {
        if (TimeToMonitor % RTE_cyclicActivationTable[Index].cycleTime == 0)
        {
            RTE_cyclicActivationTable[Index].run();
        }
    }
    
#define TIME_1_SEC      1000
    // reset monitor time
    if (TimeToMonitor >= TIME_1_SEC)
    {
        TimeToMonitor = 0;
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
    
    TerminateTask();
}


TASK(tsk_Background)
{
    while (1)
    {
        if (UART_DataInBuffer() == RC_SUCCESS)
        {
            UART_Flush();
        }
        
        if (WD_Alive_GetStatus() == (pow(2, RUN_MAX) - 1))
        {
            WD_Alive_ClearBit();
            WD_Trigger();
        }
    }
    
    TerminateTask();
}

/**
 * The function which sets button 1 press event to the tsk_ReactionGame.
 * @param   void
 * @return  void
 */
static void SIGNAL__from_button1(void)
{
    // Events for button 1 press
    SetEvent(tsk_EventDispatcher, ev_shutdown);
}
/**
 * The button press interrupt - Category 2
 * Corresponding event function will be invoked.
 */
ISR2(ISR_Shutdown)
{
    if (BUTTON_IsPressed(BUTTON_1)) SIGNAL__from_button1();
}
/* [] END OF FILE */
