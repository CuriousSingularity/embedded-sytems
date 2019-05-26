/**
* \file <watchdog.c>
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
#include "watchdog.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/
static uint32_t WD_Alive_BitField = 0;

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
* Activate the Watchdog Trigger
* \param WDT_TimeOut_t timeout - [IN] Timeout Period
* @return RC_SUCCESS
*/
RC_t WD_Start(WDT_TimeOut_t timeout)
{
    CyWdtStart(timeout, CYWDT_LPMODE_MAXINTER);
    return RC_SUCCESS;
}


/**
* Service the Watchdog Trigger
* @return RC_SUCCESS
*/
RC_t WD_Trigger()
{
    CyWdtClear();
    return RC_SUCCESS;
}

/**
* Checks the watchdog bit
* @return TRUE if watchdog reset bit was set
*/
boolean_t WD_CheckResetBit()
{
    uint8_t reg = CyResetStatus;
    
    return (reg & WDT_RESET_BIT_MASK) ? TRUE : FALSE;
}

/**
* Set the bit to track the runnables
* @return RC_SUCCESS if successful
*/
RC_t WD_Alive(uint8_t myBitPosition)
{
    RC_t ret = RC_SUCCESS;
    
    WD_Alive_BitField |= 1 << myBitPosition;
    
    return ret;
}

/**
* clear the bits to track the runnables
* @return RC_SUCCESS if successful
*/
RC_t WD_Alive_ClearBit()
{
    RC_t ret = RC_SUCCESS;
    
    WD_Alive_BitField = 0;
    
    return ret;
}


/**
* status of the bits to track the runnables
* @return RC_SUCCESS if successful
*/
uint32_t WD_Alive_GetStatus()
{
    return WD_Alive_BitField;
}