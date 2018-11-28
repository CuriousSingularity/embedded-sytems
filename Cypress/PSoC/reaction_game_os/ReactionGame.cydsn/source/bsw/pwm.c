/**
* \file <pwm.c>
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
#include "pwm.h"

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
RC_t PWM_Init(void)
{
    PWM_Red_Start();
    PWM_Green_Start();
    PWM_Blue_Start();
    
    // Turn off all RGB leds
    PWM_Red_WriteCompare1(0);
    PWM_Green_WriteCompare1(0);
    PWM_Blue_WriteCompare1(0);
    
    // Turn off all leds
    PWM_Red_WriteCompare2(0);
    PWM_Green_WriteCompare2(0);
    PWM_Blue_WriteCompare2(0);
    
    return RC_SUCCESS;
}


/**
 * A function which sets the Duty cycle of Red RGB led
 * @param uint8 value       : IN 0 - 0xFF
 * @return RC_SUCCESS on successful
 */
RC_t PWM_RGB_SetRed(uint8_t value)
{
    PWM_Red_WriteCompare1(value);
    return RC_SUCCESS;
}


/**
 * A function which sets the Duty cycle of Green RGB led
 * @param uint8 value       : IN 0 - 0xFF
 * @return RC_SUCCESS on successful
 */
RC_t PWM_RGB_SetGreen(uint8_t value)
{
    PWM_Green_WriteCompare1(value);
    return RC_SUCCESS;
}


/**
 * A function which sets the Duty cycle of Blue RGB led
 * @param uint8 value       : IN 0 - 0xFF
 * @return RC_SUCCESS on successful
 */
RC_t PWM_RGB_SetBlue(uint8_t value)
{
    PWM_Blue_WriteCompare1(value);
    return RC_SUCCESS;
}


/**
 * A function which sets the Duty cycle of Red led
 * @param uint8 value       : IN 0 - 0xFF
 * @return RC_SUCCESS on successful
 */
RC_t PWM_SetRed(uint8_t value)
{
    PWM_Red_WriteCompare2(value);
    return RC_SUCCESS;
}


/**
 * A function which sets the Duty cycle of Green led
 * @param uint8 value       : IN 0 - 0xFF
 * @return RC_SUCCESS on successful
 */
RC_t PWM_SetGreen(uint8_t value)
{
    PWM_Green_WriteCompare2(value);
    return RC_SUCCESS;
}


/**
 * A function which sets the Duty cycle of Yellow led
 * @param uint8 value       : IN 0 - 0xFF
 * @return RC_SUCCESS on successful
 */
RC_t PWM_SetYellow(uint8_t value)
{
    PWM_Blue_WriteCompare2(value);
    return RC_SUCCESS;
}