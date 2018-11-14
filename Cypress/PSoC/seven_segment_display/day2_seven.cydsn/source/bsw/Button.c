/**
* \file <ISR_Handler.c>
* \author <Bharath Ramachandraiah>
* \date <25/09/2018>
*
* \brief <Symbolic File name>
*
* \copyright Copyright ©2016
* Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
* Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
* (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
* this paragraph and the following paragraph appear in all copies, modifications, and distributions.
* Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
*
* \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
* The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
* This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
*/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "global.h"
#include "Button.h"
#include "seven.h"
#include "led.h"
/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint8 ButtonClickCounter = 0;


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * This is a callback function for Button 1 press. The process of the ISR is
 * done here.
 * @param void
 * @return void
 */
void ISR_Button1_Callback(void)
{
    LEDStateChange(LED_GREEN, LED_ON);
    return;
}

/**
 * This is a callback function for Button 2 press. The process of the ISR is
 * done here.
 * @param void
 * @return void
 */
void ISR_Button2_Callback(void)
{
    LEDStateChange(LED_YELLOW, LED_ON);
    return;
}

/**
 * This is a callback function for Button 3 press. The process of the ISR is
 * done here.
 * @param void
 * @return void
 */
void ISR_Button3_Callback(void)
{
    LEDStateChange(LED_RED, LED_ON);
    
    if (ButtonClickCounter++ == 0xFF)
        ButtonClickCounter = 0;
    
    SEVEN_Write(ButtonClickCounter);
    
    return;
}

/**
 * This is a callback function for Button 4 press. The process of the ISR is
 * done here.
 * @param void
 * @return void
 */
void ISR_Button4_Callback(void)
{
    LEDStateChange(LED_GREEN, LED_OFF);
    LEDStateChange(LED_YELLOW, LED_OFF);
    LEDStateChange(LED_RED, LED_OFF);
    
    if (ButtonClickCounter-- == 0x00)
        ButtonClickCounter = 0;
    
    SEVEN_Write(ButtonClickCounter);
    
    return;
}