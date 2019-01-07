
#include "project.h"
#include "rte_signalpool.h"

#include "adc_signal.h"

sint8_t Offset_V = 0, Offset_H = 0;

void run_readJoystick(void)
{
    RC_t ret = RC_SUCCESS;
    
    ret = RTE_ADC_pullPort(&JOYSTICK_signal);
    
    if (ret != RC_SUCCESS)
    {
        // Notify error during adc read
    }
}

void run_calculateControl(void)
{
    // check if the data is new
    if (RTE_ADC_getStatus(&JOYSTICK_signal) == RTE_SIGNALSTATUS_VALID &&
        RTE_ADC_getAge(&JOYSTICK_signal) == 0)
    {
        ADC_data_t JoyStickValues = RTE_ADC_get(&JOYSTICK_signal);
        
        // check if the acceleration is pressed
        if ((JoyStickValues.Channel1 - Offset_V) > 0 && (JoyStickValues.Channel2 - Offset_H) > 0)
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
    }
}

void run_setEngine(void)
{
    RC_t ret = RC_SUCCESS;
    
    ret = RTE_PWM_pushPort(&ENGINE_signal);
    
    if (ret != RC_SUCCESS)
    {
        // Notify error to push data
    }
}

void run_setBrakeLight(void)
{
    RC_t ret = RC_SUCCESS;
    
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
    }
}