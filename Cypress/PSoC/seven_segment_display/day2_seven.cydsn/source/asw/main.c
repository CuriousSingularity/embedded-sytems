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
#include "global.h"
#include "ISR_Button1.h"
#include "ISR_Button2.h"
#include "ISR_Button3.h"
#include "ISR_Button4.h"
#include "seven.h"
#include "UART_1.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ISR_Button1_Start();
    ISR_Button2_Start();
    ISR_Button3_Start();
    ISR_Button4_Start();
    SEVEN_init();
    UART_1_Start();
    
    for(;;)
    {
        /* Place your application code here. */
        if (UART_1_ReadRxStatus())
        {
            uint8 data = 0;
            
            data = UART_1_ReadRxData();
            
            UART_1_WriteTxData(data);
        }
    }
}

/* [] END OF FILE */
