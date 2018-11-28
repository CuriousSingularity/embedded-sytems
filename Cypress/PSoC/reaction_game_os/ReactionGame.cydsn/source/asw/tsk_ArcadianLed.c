/**
* \file <tsk_ArcadianLed.c>
* \author <Bharath Ramachandraiah>
* \date <13/11/2018>
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
#include "tsk_ArcadianLed.h"
#include "pwm.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define ARCADIANLED__FADER_TIME_DURATION    100

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/


/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static const ARCADIANLED__Fader_t  Led_fadertable[] = {
    
    //Red    Yellow Green   TimeInMS
    //{0,      0,     0,      ARCADIANLED__FADER_TIME_DURATION},
    {127,    0,     0,      ARCADIANLED__FADER_TIME_DURATION},
    {255,    0,     0,      ARCADIANLED__FADER_TIME_DURATION},
    {127,    127,   0,      ARCADIANLED__FADER_TIME_DURATION},
    {0,      255,   0,      ARCADIANLED__FADER_TIME_DURATION},
    {0,      127,   127,    ARCADIANLED__FADER_TIME_DURATION},
    {0,      0,     255,    ARCADIANLED__FADER_TIME_DURATION},
    {0,      0,     127,    ARCADIANLED__FADER_TIME_DURATION},
};


static const ARCADIANLED__Glower_t RGB_glowtable[] = {
    
    //Red   Green   Blue    TimeInMS
    {255,   0,      0,      500},
    {0,     255,    0,      500},
    {0,     0,      255,    500},
    {0,     0,      0,      100},
    {255,   255,    255,    100},
    {0,     0,      0,      100},
    {255,   255,    255,    100},
    {0,     0,      0,      100},
    {255,   255,    255,    100},
};

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/
/**
 * A function which provides the arcadian style fader functionality to the leds
 * @param   void
 * @return  RC_SUCCESS
 */
static RC_t ARCADIANLED__Fader(void)
{
    static uint8_t Index = 0, TimeCount = 0;
    
    if (Index < (sizeof(Led_fadertable) / sizeof(ARCADIANLED__Fader_t)))
    {
        PWM_SetRed   (Led_fadertable[Index].rgb_R_pwm);
        PWM_SetYellow(Led_fadertable[Index].rgb_B_pwm);
        PWM_SetGreen (Led_fadertable[Index].rgb_G_pwm);
        
        // Has the time elapsed ?
        if (Led_fadertable[Index].TimeDuration <= (++TimeCount * TSK_ARCADIANLED_CYCLIC_TIME))
        {
            Index++;
            TimeCount = 0;
        }
    }
    else
    {
        Index       = 0;
        TimeCount   = 0;
    }
    
    return RC_SUCCESS;
}


/**
 * A function which provides the arcadian style glower functionality to the RGB led
 * @param   void
 * @return  RC_SUCCESS
 */
static RC_t ARCADIANLED__Glower(void)
{
    static uint8_t Index = 0, TimeCount = 0;
    
    if (Index < (sizeof(RGB_glowtable) / sizeof(ARCADIANLED__Glower_t)))
    {
        PWM_RGB_SetRed  (RGB_glowtable[Index].rgb_R_pwm);
        PWM_RGB_SetGreen(RGB_glowtable[Index].rgb_G_pwm);
        PWM_RGB_SetBlue (RGB_glowtable[Index].rgb_B_pwm);
        
        // Has the time elapsed ?
        if (RGB_glowtable[Index].TimeDuration <= (TimeCount++ * TSK_ARCADIANLED_CYCLIC_TIME))
        {
            Index++;
            TimeCount = 0;
        }
    }
    else
    {
        Index       = 0;
        TimeCount   = 0;
    }
    
    return RC_SUCCESS;
}


/**
 * The Task definition
 * The tsk_ArcadianLed is a cyclic task which is activated every 100ms.
 * The task is activated via an alarm.
 */
TASK(tsk_ArcadianLed)
{
    // call fader
    ARCADIANLED__Fader();
    
    // call glower
    ARCADIANLED__Glower();
    
    // Terminate the init task and let the schedular do its thing!
    TerminateTask();
}