/**
* \file <console.c>
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
#include "console.h"
#include "UART_LOG.h"

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
 * A function initializes the UART port used to transmit the log messages.
 * @param void
 * @return RC_SUCCESS on successful transmission
 */
RC_t CONSOLE_Init(void)
{
    UART_LOG_Start();
    
    return RC_SUCCESS;
}

/**
 * A function transmits the string characters over serial console.
 * @param const sint8_t *pMessaage      : IN String
 * @return RC_SUCCESS on successful transmission
 */
RC_t CONSOLE_Write(const char *pMessage)
{
    UART_LOG_PutString(pMessage);
    
    return RC_SUCCESS;
}


/**
 * A function transmits the numberover serial console.
 * @param const sint32_t Number      : IN Number
 * @return RC_SUCCESS on successful transmission
 */
RC_t CONSOLE_WriteNumber(sint32_t Number)
{
    uint8_t NumberString[sizeof(Number) * sizeof(BYTE_L)] = {0};
    sint32_t temp = 0;
    uint8_t count = 0, Index, SignCount = 0;
    
    // check if the number is negative
    if(Number >> ((sizeof(Number) * sizeof(BYTE_L)) - 1))
    {
        /*CONVERTING 2's complement value to normal value*/    
        Number = ~Number + 1;
        
        NumberString[count++] = '-';
        
        SignCount = 1;
    }
    
    if (Number)
    {
        // non-zero
        for(temp = Number; temp != 0; temp /= 10, count++);
        
        for(Index = count - 1, temp = Number; Index >= SignCount && temp; Index--)
        {
            NumberString[Index] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    else
    {
        // zero
        NumberString[count++] = '0';
    }
    
    return CONSOLE_Write((const char *)NumberString);
}