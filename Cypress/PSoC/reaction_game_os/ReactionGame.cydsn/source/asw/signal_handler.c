/**
* \file <signal_handler.c>
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
#include "signal_handler.h"
#include "button.h"

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
 * The function which sets button 1 press event to the tsk_ReactionGame.
 * @param   void
 * @return  void
 */
static void SIGNAL__from_button1(void)
{
    // Events for button 1 press
    SetEvent(tsk_ReactionGame, ev_Button1);
}

/**
 * The function which sets button 2 press event to the tsk_ReactionGame.
 * @param   void
 * @return  void
 */
static void SIGNAL__from_button2(void)
{
    // Events for button 2 press
    SetEvent(tsk_ReactionGame, ev_Button2);
}

/**
 * The button press interrupt - Category 2
 * Corresponding event function will be invoked.
 */
ISR2(isr_BUTTON)
{
    if (BUTTON_IsPressed(BUTTON_1)) SIGNAL__from_button1();
    if (BUTTON_IsPressed(BUTTON_2)) SIGNAL__from_button2();
}