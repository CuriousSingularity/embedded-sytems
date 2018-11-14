/**
* \file <ReactionGame.c>
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
#include "console.h"
#include "Timer.h"
#include "seven.h"
#include "Button.h"
#include <stdlib.h>

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define REACTIONGAME_TIMEOUT    1
#define MAX_RANDOM_TIME         3
#define MAX_RANDOM_NUMBER       2
#define NIBBLE_SHIFT            4
#define MASK_BIT0_BIT0          0x01
#define MASK_BIT1_BIT0          0x03
#define MASK_BIT2_BIT0          0x07

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/


/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/
enum eReactionStates
{
    REACTION_NO,
    REACTION_CORRECT,
    REACTION_WRONG,
};

enum eReactionStateTypes
{
    RG_START_BUTTON_PRESSED,
    RG_START_SCREEN,
    RG_GENERATE_RANDOM_DELAY,
    RG_GENERATE_RANDOM_NO_TO_DISPLAY,
    RG_UPDATE_SCREEN_WITH_RANDOM_NO,
    RG_ENABLE_KEY_INTERRUPTS,
    RG_START_REACTION_MEASUREMENT,
    RG_DISABLE_KEY_INTERRUPTS,
    RG_GET_REACTION_MEASUREMENT,
    RG_GET_BUTTON_PRESSED,
    RG_DISPLAY_RESULTS,
};

typedef enum eReactionStates eReactionStates_t;
typedef enum eReactionStateTypes eReactionStateTypes_t;

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint32_t ReactionCorrectCounter = 0, ReactionWrongCounter = 0, ReactionNoCounter = 0;
static const char *PredefinedMessages[] = 
{
    "Missed ",
    "Correct",
    "Wrong  "
};


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/
/**
 * A function to generate a random number.
 * @param uint8_t NumRange      : IN Maximum value of a random number
 * @return a random value.
 */
static uint8_t GetRandomNumber(uint8_t NumRange)
{
    uint32_t RandNum = 0;
    
    RandNum = rand();

    RandNum &= NumRange;
    
    return RandNum;
}

/**
 * A function to start the reaction time measurement and reset the button pressed.
 * @param void
 * @return timer is started to measure the reaction time.
 */
static void StartReactionMeasurement(void)
{
    StartTimerMeasurement();
    
    UpdateButtonPressed(BUTTON_UNPRESSED);
    
    return;
}

/**
 * A function to start the reaction time measurement and reset the button pressed.
 * @param void
 * @return timer is started to measure the reaction time.
 */
static uint32_t GetReactionMeasurement(eButtonPressStates_t *ButtonState)
{
    uint32_t MeasuredReactionTime = 0;
    
    MeasuredReactionTime = GetTimerMeasurement();
    *ButtonState = ReadCurrentButtonPressed();
    
    return MeasuredReactionTime;
}
   
/**
 * A function to check whether a button is pressed and validate it.
 * @param sint32_t Number                   : IN Number displayed
 * @param eButtonPressStates_t ButtonState  : IN Button pressed
 * @return validate the button check with the number displayed.
 */
static eReactionStates_t CheckButtonPressMatch(sint32_t Number, eButtonPressStates_t ButtonState)
{
    eReactionStates_t result = REACTION_NO;
   
    if (ReadCurrentButtonPressed() == BUTTON_UNPRESSED)
    {
        result = REACTION_NO;
        ReactionNoCounter++;
    }
    else if (Number == ButtonState)
    {
        ReactionCorrectCounter++;
        result = REACTION_CORRECT;
    }
    else
    {
        ReactionWrongCounter++;
        result = REACTION_WRONG;
    }
    
    return result;
}

/**
 * A function to display the reaction results.
 * @param uint32_t MeasuredTime
 * @return the appropriate message is displayed.
 */
static void DisplayReactionResults(uint32_t MeasuredTime, eReactionStates_t ReactionIndex)
{
    PrintOnConsole(CONSOLE_MSG, "%s Button Press | The reaction time is %ums | Correct %u | Wrong %u | Missed %u\n", 
        PredefinedMessages[ReactionIndex], MeasuredTime, ReactionCorrectCounter, ReactionWrongCounter, ReactionNoCounter);
    return;
}

/**
 * A function starts the reaction game.
 * @param void
 * @return Starts the game.
 */
void ReactionGameRun(void)
{
    eButtonPressStates_t ButtonState = BUTTON_UNPRESSED;
    sint32_t RandNum = 0;
    uint32_t MeasuredTime = 0;
    eReactionStates_t ReactionButton = REACTION_NO;
    eReactionStateTypes_t RG_CurrentState = RG_START_BUTTON_PRESSED;
    
    PrintOnConsole(CONSOLE_MSG, "Welcome to the Reaction Game\n\nPress a button to start...\n");
    
    do
    {
        switch (RG_CurrentState)
        {
            case RG_START_BUTTON_PRESSED:
                while (ReadCurrentButtonPressed() == BUTTON_UNPRESSED);
                RG_CurrentState = RG_START_SCREEN;
            break;
            
            case RG_START_SCREEN:
                /* Clear the screen & button */
                SEVEN_clear();
                RG_CurrentState = RG_GENERATE_RANDOM_DELAY;
            break;
                
            case RG_GENERATE_RANDOM_DELAY:
                /* Generate the random delay for blank screen */
                RandNum = GetRandomNumber(MASK_BIT1_BIT0) + 1;        
                DelayGenerator(RandNum);
                RG_CurrentState = RG_GENERATE_RANDOM_NO_TO_DISPLAY;
            break;
                
            case RG_GENERATE_RANDOM_NO_TO_DISPLAY:
                /* Generate a random number to display - 0 or 1 is returned here */
                RandNum = GetRandomNumber(MASK_BIT0_BIT0) + 1;
                RG_CurrentState = RG_UPDATE_SCREEN_WITH_RANDOM_NO;
            break;
                
            case RG_UPDATE_SCREEN_WITH_RANDOM_NO:
                /* Update the screen with the random number */
                SEVEN_Write(RandNum << NIBBLE_SHIFT | RandNum);
                RG_CurrentState = RG_ENABLE_KEY_INTERRUPTS;
            break;
                
            case RG_ENABLE_KEY_INTERRUPTS:
                /* Enable Interrupts */
                ISR_Button3_Enable();
                ISR_Button4_Enable();
                RG_CurrentState = RG_START_REACTION_MEASUREMENT;
            break;
                
            case RG_START_REACTION_MEASUREMENT:
                StartReactionMeasurement();
                DelayGenerator(REACTIONGAME_TIMEOUT);
                RG_CurrentState = RG_DISABLE_KEY_INTERRUPTS;
            break;
                
            case RG_DISABLE_KEY_INTERRUPTS:
                /* Disable Interrupts */
                ISR_Button3_Disable();
                ISR_Button4_Disable();
                RG_CurrentState = RG_GET_REACTION_MEASUREMENT;
            break;
                
            case RG_GET_REACTION_MEASUREMENT:
                MeasuredTime = GetReactionMeasurement(&ButtonState);
                RG_CurrentState = RG_GET_BUTTON_PRESSED;
            break;
                
            case RG_GET_BUTTON_PRESSED:
                ReactionButton = CheckButtonPressMatch(RandNum, ButtonState);
                RG_CurrentState = RG_DISPLAY_RESULTS;
            break;
                
            case RG_DISPLAY_RESULTS:
                /* Display Information of reaction time */
                DisplayReactionResults(MeasuredTime, ReactionButton);
                RG_CurrentState = RG_START_SCREEN;
            break;
                
            default:
            break;
        }
    } while (1);
    return;
}
