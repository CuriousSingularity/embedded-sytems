
#include "project.h"
#include "rte_signalpool.h"

#include "uart.h"
#include "watchdog.h"
#include "adc_signal.h"
#include "swc_application.h"

sint8_t Offset_V = 0, Offset_H = 0;

void run_readJoystick(void)
{
    RC_t ret = RC_SUCCESS;
    
    WD_Alive(RUN_READJOYSTICK);
    
    ret = RTE_ADC_pullPort(&JOYSTICK_signal);
    
    if (ret != RC_SUCCESS)
    {
        UART_Write("ERROR: Reading ADC Value.\n");
    }
}

void run_calculateControl(void)
{
    WD_Alive(RUN_CALCULATECONTROL);
    
    // check if the data is new
    if (RTE_ADC_getStatus(&JOYSTICK_signal) == RTE_SIGNALSTATUS_VALID &&
        RTE_ADC_getAge(&JOYSTICK_signal) == 0)
    {
        ADC_data_t JoyStickValues = RTE_ADC_get(&JOYSTICK_signal);
        
        // Offset the values
        JoyStickValues.Channel1 -= Offset_V;
        JoyStickValues.Channel2 -= Offset_H;
        
        UART_Write("JOYSTICK H : ");UART_WriteNumber(JoyStickValues.Channel1);UART_Write("\n");
        UART_Write("JOYSTICK V : ");UART_WriteNumber(JoyStickValues.Channel2);UART_Write("\n\n");
        
        // check if the acceleration is pressed
        if ((JoyStickValues.Channel1) > 0 && (JoyStickValues.Channel2) > 0)
        {
            sint8_t Vertical    = JoyStickValues.Channel1;
            sint8_t Horizontal  = JoyStickValues.Channel2;
            volatile PWM_data_t NewEngineValue = 
            {
                .Channel1 = 2 * (Vertical < Horizontal ? Vertical : Horizontal),
            };
            
            RTE_PWM_set(&ENGINE_signal, NewEngineValue);
        }
        else
        {
            // set the value to a safe value
            RTE_PWM_set(&ENGINE_signal, PWM_INIT_DATA);
        }
    }
    else
    {
        // set the value to a safe value
        RTE_PWM_set(&ENGINE_signal, PWM_INIT_DATA);
        UART_Write("ERROR: Old Joystick Value.\n");
    }
}

void run_setEngine(void)
{
    RC_t ret = RC_SUCCESS;
    
    WD_Alive(RUN_SETENGINE);
    
    ret = RTE_PWM_pushPort(&ENGINE_signal);
    
    if (ret != RC_SUCCESS)
    {
        // Notify error to push data
        UART_Write("ERROR: Set Engine value failed.\n");
    }
}

void run_setBrakeLight(void)
{
    RC_t ret = RC_SUCCESS;
    
    WD_Alive(RUN_SETBRAKELIGHT);
    
    PWM_data_t value = {0};
    GPIO_data_t NewBrakeLightValue = {0};
    
    value = RTE_PWM_get(&ENGINE_signal);
    
    if (value.Channel1 > 0)
    {
        NewBrakeLightValue.state = FALSE;
    }
    else
    {
        NewBrakeLightValue.state = TRUE;
    }
    
    RTE_GPIO_set(&BRAKELIGHT_signal, NewBrakeLightValue);
    
    ret = RTE_GPIO_pushPort(&BRAKELIGHT_signal);
    
    if (ret != RC_SUCCESS)
    {
        // Notify error to push data
        UART_Write("ERROR: Brake Light push data failed.\n");
    }
}

void run_systemShutdown(void)
{
    WD_Alive(RUN_SYSTEMSHUTDOWN);
    
    ShutdownOS(E_OK);
}