/**
* \file <seven.c>
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
#include "seven.h"
#include "S7D_Select.h"
#include "S7D_Display.h"
#include "S7D_ControlReg.h"

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
static const uint8 LookUpForDisplay[] = 
{
    0x3F,
    0x06,
    0x5b,
    0x4f,
    0x66,
    0x6d,
    0x7d,
    0x07,
    0x7f,
    0x6f,
    0x77,
    0x7c,
    0x39,
    0x5e,
    0x79,
    0x71,
    0x00,
};

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * The function takes the number as an input and updates on to the display.
 * @param uint8 Value       : IN 0 - 0xF
 * @return Updates the seven segment display with the values.
 */
static RC_t S7D_Set(eSegments_t DisplayNo, uint8 Value)
{
    uint8 NoToDisplay = 0;
    /* Select the Display */
    S7D_Select_Write(DisplayNo);
    
    NoToDisplay = LookUpForDisplay[Value & MASK_MSB_UINT8];

    S7D_ControlReg_Write(NoToDisplay);

    return RC_SUCCESS;
}


/**
 * The function initializes the display to default settings.
 * @param void
 * @return the displays are reset.
 */
RC_t S7D_init(void)
{
    S7D_Display_Write(SEGMENT_ON);
    
    S7D_Write(LookUpForDisplay[(sizeof(LookUpForDisplay) / sizeof(uint8)) - 1]);
    
    return RC_SUCCESS;
}


/**
 * The function clears the displays.
 * @param void
 * @return the displays are reset.
 */
RC_t S7D_clear(void)
{   
    return (S7D_init());
}


/**
 * The function takes the number as an input and updates on to the displays.
 * @param uint8 Value       : IN 0 - 0xFF
 * @return Updates the seven segment display with the values.
 */
RC_t S7D_Write(uint8 Value)
{
    /* Update 0th display */
    S7D_Set(S7D_DISPLAY0, Value & MASK_MSB_UINT8);
    
    /* Update 1st display */
    S7D_Set(S7D_DISPLAY1, (Value & MASK_LSB_UINT8) >> SHIFT_NIBBLE);
    
    return RC_SUCCESS;
}