/**
* \file <tsk_Init.c>
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
#include "global.h"
#include "tsk_Init.h"
#include "tsk_ReactionGame.h"
#include "tsk_ArcadianLed.h"
#include "seven.h"
#include "console.h"
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
typedef RC_t (*RC_tFunctPtrv)(void);

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static RC_tFunctPtrv fptr_init_functions[] = 
{
    // Add all the initialization functions here
    CONSOLE_Init,
    S7D_Init,
    PWM_Init,
};

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * The Task declaration
 * The tsk_Init initializes all the necessary hardware components.
 * This is an autostart task which will be run once the OS starts.
 */
TASK(tsk_Init)
{
    // MCAL - hardware initialization 
    for (unsigned int Index = 0; Index < sizeof(fptr_init_functions)/sizeof(RC_tFunctPtrv); Index++)
    {
        if (RC_SUCCESS != fptr_init_functions[Index]())
        {
            // Hardware Fault - Take necessary action
            while(1) __asm("NOP");
        }
    }
    
    // OS initialization - This will override and reconfigures the interrupts by OS parameters
    EE_system_init();
    
    // Activate Tasks here
    ActivateTask(tsk_ReactionGame);
    
    // Set Alarms here
    SetRelAlarm(alrm_ArcadianLed, TSK_ARCADIANLED_CYCLIC_TIME, TSK_ARCADIANLED_CYCLIC_TIME);
    
    // Terminate the init task and let the schedular do its thing!
    TerminateTask();
}