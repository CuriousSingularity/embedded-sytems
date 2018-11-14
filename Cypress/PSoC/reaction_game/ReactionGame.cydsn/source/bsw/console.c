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
#include "global.h"
#include "UART_1.h"
#include "console.h"
#include <stdarg.h>
#include <stdio.h>

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
 * A function transmits the string characters over serial console.
 * @param const sint8_t *pMessaage      : IN String
 * @return Transmits the messsage character by character over the UART.
 */
static void ConsoleWrite(const char *pMessage)
{
    UART_1_PutString(pMessage);
}

/**
 * A function which takes variable number of args to be printed on console.
 * @param eConsoleMsgTypes_t MessageType      : IN Message Type
 * @param char *msg                           : IN String to be printed
 * @return fomats the string into printable data over console.
 */
void PrintOnConsole(eConsoleMsgTypes_t MessageType, char *msg, ...)
{
    char MessageData[100] = {0};
    va_list valist; 

    va_start(valist, msg); 
    vsprintf((char *)MessageData, (const char *)msg, valist); 
  
    va_end(valist);
    
    ConsoleWrite(MessageData);
    
    (void)MessageType;
    return;
}