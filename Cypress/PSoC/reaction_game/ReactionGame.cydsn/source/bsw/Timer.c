/**
* \file <Timer.c>
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
#include "Timer.h"
#include "Button.h"

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
static volatile uint32_t SYS_TimeMeasure_1s = 0;
static volatile uint32_t SYS_TimeMeasure_1ms = 0, CurrentTimeMeasure_ms = 0;


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * This is a callback function for Timer overrun. The process of the ISR is
 * done here.
 * @param void
 * @return void
 */
void BSW_TimeCallBack(void)
{
    static volatile uint32_t SYS_TickCounter_1ms = 0;
    
    if (SYS_TickCounter_1ms >= 1000)
    {
        SYS_TickCounter_1ms = 0;
        SYS_TimeMeasure_1s++;
    }
    
    /* Increment Counters */
    SYS_TickCounter_1ms++;
    SYS_TimeMeasure_1ms++;
    
    return;
}

/**
 * A function to produce a random delay.
 * done here.
 * @param uint8_t Delay         : IN Delay in seconds
 * @return provides a random delay.
 */
void DelayGenerator(uint8_t Delay)
{
    SYS_TimeMeasure_1s = 0;
    
    /* Produce Random Delay */
    while (SYS_TimeMeasure_1s < Delay);
    
    return;
}

/**
 * A function to start the timer measurement.
 * @param void
 * @return internal timer counter is reset.
 */
void StartTimerMeasurement(void)
{
    SYS_TimeMeasure_1ms = 0;
    CurrentTimeMeasure_ms = 0;
    
    return;
}

/**
 * A function to stop the timer measurement.
 * @param void
 * @return internal timer counter is read.
 */
void StopTimerMeasurement(void)
{
    CurrentTimeMeasure_ms = SYS_TimeMeasure_1ms;
    
    return;
}

/**
 * A function to get the timer measurement.
 * @param void
 * @return the time measured between start and stop.
 */
uint32_t GetTimerMeasurement(void)
{
    return CurrentTimeMeasure_ms;
}