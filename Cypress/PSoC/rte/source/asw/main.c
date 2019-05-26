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
#include "uart.h"
#include "watchdog.h"
#include "swc_application.h"

#define TSK_CYCLICDISPATCHER_CYCLIC_TIME    10
#define DEADLINE_MONITORING_RUNNABLE        5

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
    ActivateTask(tsk_Errorhandler);
    ActivateTask(tsk_EventDispatcher);
    ActivateTask(tsk_Background);
        
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


/**
 * The Task declaration
 * The tsk_CyclicDispatcher is a cyclic task which is activated
 * every TSK_CYCLICDISPATCHER_CYCLIC_TIME miliseconds.
 * The task invokes all the runnable with its appropriate time.
 */
TASK(tsk_CyclicDispatcher)
{
    uint16_t        Index = 0;
    TickType        RemainingTicks = 0;
    static uint16_t TimeToMonitor = 0;
    
    TimeToMonitor += TSK_CYCLICDISPATCHER_CYCLIC_TIME;
    
    RTE_timertick();
    
    for (Index = 0; Index < RTE_cyclicActivation_size; Index++)
    {
        if (TimeToMonitor % RTE_cyclicActivationTable[Index].cycleTime == 0)
        {
            SetRelAlarm(alrm_DM_cyclicDispatcherTsk, DEADLINE_MONITORING_RUNNABLE, 0);
            RTE_cyclicActivationTable[Index].run();
            GetAlarm(alrm_DM_cyclicDispatcherTsk, &RemainingTicks);
            CancelAlarm(alrm_DM_cyclicDispatcherTsk);

#if (defined(VERBOSE) && (VERBOSE))            
            UART_Write("CRunnable ");
            UART_WriteNumber(Index);
            UART_Write(" : ");
            UART_WriteNumber(DEADLINE_MONITORING_RUNNABLE - RemainingTicks);
            UART_Write("\n\n");
#endif
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


/**
 * The Task declaration
 * The tsk_EventDispatcher is an event handler task which is activated
 * when an event corresponding the task is activated, the corresponding 
 * runnables will be invoked.
 */
TASK(tsk_EventDispatcher)
{
    TickType        RemainingTicks = 0;
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
                SetRelAlarm(alrm_DM_eventDispatcherTsk, DEADLINE_MONITORING_RUNNABLE, 0);
                RTE_eventActivationTable[Index].run();
                GetAlarm(alrm_DM_eventDispatcherTsk, &RemainingTicks);
                CancelAlarm(alrm_DM_eventDispatcherTsk);

#if (defined(VERBOSE) && (VERBOSE))
                UART_Write("ERunnable ");
                UART_WriteNumber(Index);
                UART_Write(" : ");
                UART_WriteNumber(DEADLINE_MONITORING_RUNNABLE - RemainingTicks);
                UART_Write("\n\n");
#endif
            }
        }
    }
    
    TerminateTask();
}


/**
 * The Task declaration
 * The tsk_Background is a background task which is used to feed watchdog,
 * alive monitoring of all the runnables. It is used to flush the log messages
 * to console.
 */
TASK(tsk_Background)
{
    while (1)
    {
        if (UART_DataInBuffer() == RC_SUCCESS)
        {
            UART_Flush();
        }
        
        if (WD_Alive_GetStatus() == ALL_RUNNABLES_ALIVE)
        {
            WD_Alive_ClearBit();
            WD_Trigger();
        }
    }
    
    TerminateTask();
}


/**
 * The Task declaration
 * The tsk_Errorhandler is used to handle shutdown event and deadline monitoring
 * events.
 */
TASK(tsk_Errorhandler)
{
    EventMaskType   EventsReceived = 0;
    
    WaitEvent(ev_shutdown | ev_DM_cyclicDispatcherTsk | ev_DM_eventDispatcherTsk);
    GetEvent(tsk_Errorhandler, &EventsReceived);
    ClearEvent(EventsReceived);

    if (EventsReceived & ev_shutdown)
    {
        ShutdownOS(E_OK);
    }
    
    if (EventsReceived & ev_DM_cyclicDispatcherTsk)
    {
        // disable a few runnable
    }
    
    if (EventsReceived & ev_DM_eventDispatcherTsk)
    {
        // disable a few runnable
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
    SetEvent(tsk_Errorhandler, ev_shutdown);
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
