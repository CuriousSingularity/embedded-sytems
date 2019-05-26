/**
* \file <gpio.c>
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
#include "gpio.h"

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



/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * A function initializes the PWM
 * @param void
 * @return RC_SUCCESS on successful initialization
 */
RC_t GPIO_Init(void)
{
    return RC_SUCCESS;
}

/**
 * A function which sets the Red led
 * @param uint8 value       : IN 
 * @return RC_SUCCESS on successful
 */
RC_t GPIO_LedRed_Write(uint8_t value)
{
    LED_Red_Write(value);
    return RC_SUCCESS;
}

/**
 * A function which sets the Green led
 * @param uint8 value       : IN 
 * @return RC_SUCCESS on successful
 */
RC_t GPIO_LedGreen_Write(uint8_t value)
{
    LED_Green_Write(value);
    return RC_SUCCESS;
}

/**
 * A function which sets the Yellow led
 * @param uint8 value       : IN 
 * @return RC_SUCCESS on successful
 */
RC_t GPIO_LedYellow_Write(uint8_t value)
{
    LED_Yellow_Write(value);
    return RC_SUCCESS;
}

/**
 * The function returns if the corresponding button is pressed or not.
 * @param BUTTON_id_t Button        : IN Button number
 * @return boolean_t                : TRUE if button is pressed, else FALSE
 */
boolean_t BUTTON_IsPressed(BUTTON_id_t Button)
{
    boolean_t result = FALSE;
    
    switch (Button)
    {
        case BUTTON_1:  if (Shutdown_Read()) result = TRUE; break;
        break;
        
        default:
        break;
    }
    return result;
}