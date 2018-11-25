/**
* \file <tsk_ReactionGame.c>
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
#include "seven.h"
#include "console.h"
#include "button.h"
#include "tsk_Timer.h"
#include "tsk_ReactionGame.h"
#include <stdlib.h>

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define MAX_TIMEOUT                 1
#define GAME_TIMEOUT                10
#define MAX_RANDOM_NUMBER           2
#define MAX_RANDOM_SECONDS          3
#define MAX_ROUNDS_IN_GAME          10

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
 * A function to generate a random number.
 * @param uint8_t NumRange      : IN Maximum value of a random number
 * @return a random value.
 */
static uint8_t GetRandomNumber(uint8_t NumberRange)
{
    uint32_t Num = 0;
    
    Num = rand();

    Num %= NumberRange;
        
    return Num;
}


static RC_t REACTIONGAME__WelcomeScreen(REACTIONGAME_t *Game)
{
    CONSOLE_Write("=========================================================\n");
    CONSOLE_Write("Welcome to The Reaction Game!\n");
    CONSOLE_Write("=========================================================\n\n");
    CONSOLE_Write("Press one of the two buttons to start the game...\n\n");
    
    // Wait for the any button to be pressed
    WaitEvent(ev_Button1 | ev_Button2 | ev_Timeout);
    GetEvent(tsk_ReactionGame, &Game->Event);
    ClearEvent(Game->Event);
    
    Game->Status = RG__STATUS_START;
    
    return RC_SUCCESS;
}


static RC_t REACTIONGAME__GetUserReaction(REACTIONGAME_t *Game)
{
    // Start a timeout for 1 second to read the user reaction
    SetRelAlarm(alrm_Timeout, MAX_TIMEOUT * MILLISECONDS, 0);
    
    // Start Time measurement
    TIMER_StartTimeMeasurement();
    
    // Wait until the user's reaction or timeout
    WaitEvent(ev_Button1 | ev_Button2 | ev_Timeout);
    
    // Cancel the Alarm and Clear event
    CancelAlarm(alrm_Timeout);
    GetEvent(tsk_ReactionGame, &Game->Event);
    ClearEvent(Game->Event);
    
    // Get the elapsed time
    TIMER_ElapsedTimeMeasurement(&Game->MeasureReactionTime);
    
    return RC_SUCCESS;
}


static RC_t REACTIONGAME__ProcessReaction(REACTIONGAME_t *Game, uint8_t RandomNumber)
{
    /**
    * Outcome:
    * 1. Timeout - Missed the button press
    * 2. Valid button press
    * 3. Invalid button press
    */
    
    if (Game->Event & ev_Timeout)
    {
        // Missed the button press
        CONSOLE_Write("Too slow\n\n");
        
        Game->MissedPresses++;
    }
    else
    {
        // Some button pressed; check valid button pressed or not
        if (((Game->Event & ev_Button1) && (RandomNumber == BUTTON_1)) ||
            ((Game->Event & ev_Button2) && (RandomNumber == BUTTON_2)))
        {
            // Valid button pressed
            CONSOLE_Write("Correct button pressed | Reaction Time is ");
            CONSOLE_WriteNumber(Game->MeasureReactionTime);
            CONSOLE_Write("ms\n\n");
            
            Game->CorrectButtonPresses++;
            Game->AvgReactionTime   += Game->MeasureReactionTime;
            Game->TotalReactionTime += Game->MeasureReactionTime;
        }
        else
        {
            // Invalid button pressed
            CONSOLE_Write("Wrong button pressed\n\n");
            
            Game->IncorrectButtonPresses++;
        }
    }
    
    return RC_SUCCESS;
}


static RC_t REACTIONGAME__Summary(REACTIONGAME_t *Game)
{
    CONSOLE_Write("=========================================================\n");
    CONSOLE_Write("Game Summary:\n");
    if (Game->Status == RG__STATUS_TIMEOUT)
    {
        CONSOLE_Write("Timeout!\n");    
    }
    CONSOLE_Write("Correct button presses : ");
    CONSOLE_WriteNumber(Game->CorrectButtonPresses);
    CONSOLE_Write("\n");
    CONSOLE_Write("Wrong   button presses : ");
    CONSOLE_WriteNumber(Game->IncorrectButtonPresses);
    CONSOLE_Write("\n");
    CONSOLE_Write("Missed  button presses : ");
    CONSOLE_WriteNumber(Game->MissedPresses);
    CONSOLE_Write("\n");
    CONSOLE_Write("Average  Reaction Time : ");
    CONSOLE_WriteNumber(Game->AvgReactionTime / Game->CorrectButtonPresses);
    CONSOLE_Write("ms\n");
    CONSOLE_Write("Total    Reaction Time : ");
    CONSOLE_WriteNumber(Game->TotalReactionTime);
    CONSOLE_Write("ms\n");
    CONSOLE_Write("=========================================================\n\n\n");
    
    // Reset Game parameters
    Game->Rounds                 = 0;
    Game->AvgReactionTime        = 0;
    Game->TotalReactionTime      = 0;
    Game->MeasureReactionTime    = 0;
    Game->MissedPresses          = 0;
    Game->CorrectButtonPresses   = 0;
    Game->IncorrectButtonPresses = 0;
    Game->Status                 = RG__STATUS_END;
    
    return RC_SUCCESS;
}


static RC_t REACTIONGAME__EndRound(REACTIONGAME_t *Game)
{
    // Game ended, cleanup
    // Clear the Display
    S7D_Clear();

    // Summary of the Game
    REACTIONGAME__Summary(Game);

    CONSOLE_Write("press one of the two buttons to exit!\n\n");

    // Wait for the any button to be pressed
    WaitEvent(ev_Button1 | ev_Button2);
    GetEvent(tsk_ReactionGame, &Game->Event);
    ClearEvent(Game->Event);
    
    Game->Status = RG__STATUS_IDLE;
    
    return RC_SUCCESS;
}


static RC_t REACTIONGAME__GameRound(REACTIONGAME_t *Game)
{
    uint8_t RandomNumber = 0;
    
    CONSOLE_Write("Reaction Game - Round ");
    CONSOLE_WriteNumber(Game->Rounds + 1);
    CONSOLE_Write("\n");
    CONSOLE_Write("press one of the two buttons to start...\n");
    
    Game->Status = RG__STATUS_RUNNING;
    
    // Clear all the alarms and events before we start the round
    CancelAlarm(alrm_Timeout);
    GetEvent(tsk_ReactionGame, &Game->Event);
    ClearEvent(Game->Event);
    
    // Set an alarm for a timeout 
    SetRelAlarm(alrm_Timeout, GAME_TIMEOUT * MILLISECONDS, 0);
    
    // Wait for the any button to be pressed
    WaitEvent(ev_Button1 | ev_Button2 | ev_Timeout);
    
    // Cancel the alarm - maybe the user is still playing!
    CancelAlarm(alrm_Timeout);
    GetEvent(tsk_ReactionGame, &Game->Event);
    ClearEvent(Game->Event);
    
    if (Game->Event & ev_Timeout)
    {
        // User has left in middle of the game, cleanup
        Game->Status = RG__STATUS_TIMEOUT;
        
        REACTIONGAME__EndRound(Game);
        
        // change the state 
        Game->NextState = RG__STATE_STANDBY;
    }
    else
    {
        // User is still playing, cancel the alarm before timeout
        // Display dot to indicate the game is in progress
        S7D_SetDot();
        
        // Set a timer alarm for random time and wait until it expires
        RandomNumber = GetRandomNumber(MAX_RANDOM_SECONDS) + 1;
        SetRelAlarm(alrm_Timeout, RandomNumber * MILLISECONDS, 0);
        
        WaitEvent(ev_Timeout);
        
        // During this time a button might be pressed as well, so clear all the events
        // Since we are waiting for time expiry, no need to explicitly cancel the alarm
        GetEvent(tsk_ReactionGame, &Game->Event);
        ClearEvent(Game->Event);
        
        // change the state 
        Game->NextState = RG__STATE_DISPLAY_NUMBER;
    }
    
    return RC_SUCCESS;
}


/**
 * The tsk_ReactionGame is an extended task which is used to implement the game
 * The priority must be highest in the system.
 */
TASK(tsk_ReactionGame)
{
    uint8_t RandomNumber = 0;
    
    REACTIONGAME_t Game = {
        .Rounds                 = 0,
        .CorrectButtonPresses   = 0,
        .IncorrectButtonPresses = 0,
        .MissedPresses          = 0,
        .AvgReactionTime        = 0,
        .TotalReactionTime      = 0,
        .MeasureReactionTime    = 0,
        .Event                  = 0,
        .CurrentState           = RG__STATE_STANDBY, 
        .NextState              = RG__STATE_STANDBY,
        .Status                 = RG__STATUS_IDLE,
    };
    
    while (1)
    {
        switch (Game.CurrentState)
        {
            case RG__STATE_STANDBY:
                
                // Clear the Display
                S7D_Clear();
                
                // check if it is the begining of new game
                if (!Game.Rounds)
                {
                    REACTIONGAME__WelcomeScreen(&Game);
                }
                
                // Start the a Game Round
                REACTIONGAME__GameRound(&Game);
                
            break;
            
            case RG__STATE_DISPLAY_NUMBER:
                
                // Generate Random number and update it on Display 
                RandomNumber = GetRandomNumber(MAX_RANDOM_NUMBER) + 1;
                S7D_IdenticalWrite(RandomNumber);
                
                REACTIONGAME__GetUserReaction(&Game);
                
                // change the state 
                Game.NextState = RG__STATE_PROCESS_REACTION;
                
            break;
            
            case RG__STATE_PROCESS_REACTION:
                
                // Process the events
                REACTIONGAME__ProcessReaction(&Game, RandomNumber);
                
                // Game ended ?
                if (++Game.Rounds >= MAX_ROUNDS_IN_GAME)
                {
                    REACTIONGAME__EndRound(&Game);
                }
                else
                {
                    Game.Status = RG__STATUS_IDLE;
                }
                
                Game.NextState = RG__STATE_STANDBY;
                
            break;
                
            default:
                
                // nothing to do here
                
            break;
        }
        
        Game.CurrentState = Game.NextState;
    }
    
    // Terminate the task
    // Ideally the control must not come here for the task
    TerminateTask();
}