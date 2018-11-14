/**
* \file <led.c>
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
#include "Pin_LedRed.h"
#include "Pin_LedGreen.h"
#include "Pin_LedYellow.h"
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



/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * A Generic function which changes the state of an led between ON and OFF.
 * @param eLEDTypes_t Pin       : IN Led Type (Red, Yellow, Green or all)
 * @param int Value             : IN ON or OFF
 * @return the changed state of the requested LED/s.
 */
void LEDStateChange(eLEDTypes_t Pin, uint8_t Value)
{
    /* If the Value is non-zero turn ON else OFF*/
    Value = Value ? 1 : 0;
    
    switch (Pin)
    {
        case LED_RED:
            Pin_LedRed_Write(Value);
        break;
        
        case LED_GREEN:
            Pin_LedGreen_Write(Value);
        break;
        
        case LED_YELLOW:
            Pin_LedYellow_Write(Value);
        break;
        
        case LED_ALL:
            Pin_LedRed_Write(Value);
            Pin_LedGreen_Write(Value);
            Pin_LedYellow_Write(Value);
        default:
        break;    
    }
}