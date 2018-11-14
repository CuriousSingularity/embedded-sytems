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
#include <ISR_1.h>
#include <ISR_2.h>

typedef enum
{
    RED,
    GREEN,
    YELLOW,
} LED_e;


static void LedStateChange(LED_e Pin, int Value)
{
    switch (Pin)
    {
        case RED:
            Pin_LedRed_Write(Value);
        break;
        
        case GREEN:
            Pin_LedGreen_Write(Value);
        break;
        
        case YELLOW:
            Pin_LedYellow_Write(Value);
        break;
        
        default:
        break;
        
    }
}

void ISR1_Callback(void)
{
    LedStateChange(RED, 1);
}

void ISR2_Callback(void)
{
    LedStateChange(RED, 0);
}

static void CheckToToggleYellow(void)
{
    static int Flag = 0;
    while (Pin_Button2_Read())
    {
        Flag = ~Flag;
    }
    
    Pin_LedYellow_Write(Flag);
    
    return;
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ISR_1_Start();
    ISR_2_Start();

    for(;;)
    {
        /* Place your application code here. */
        if (Pin_Button1_Read())
        {
            Pin_LedGreen_Write(1);
        }
        else
        {
            Pin_LedGreen_Write(0);
        }
        
        CheckToToggleYellow();
    }
}

/* [] END OF FILE */
