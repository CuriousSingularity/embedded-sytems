/**
* \file <adc.c>
* \author <Bharath Ramachandraiah>
* \date <02/01/2019>
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
#include "adc.h"
#include "ADC_1.h"
#include "ADC_2.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/
extern sint8_t Offset_V, Offset_H;

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
 * A function initializes the ADC
 * @param void
 * @return RC_SUCCESS on successful initialization
 */
RC_t ADC_Init(void)
{
    ADC_1_Start();
    ADC_1_StartConvert();
    
    ADC_2_Start();
    ADC_2_StartConvert();
    
    	//Read data from the MCAL driver
    ADC_1_IsEndConversion(ADC_1_WAIT_FOR_RESULT);
    ADC_2_IsEndConversion(ADC_2_WAIT_FOR_RESULT);
    
	//Scale it to the application type
    Offset_V = (sint8_t)(~ADC_1_GetResult8() - 127);
    Offset_H = (sint8_t)(~ADC_2_GetResult8() - 127);
    
    return RC_SUCCESS;
}

RC_t ADC_1_Read(sint8_t * const pdata)
{
    ADC_1_IsEndConversion(ADC_1_WAIT_FOR_RESULT);
    
    *pdata = ADC_1_GetResult8();
    
    return RC_SUCCESS;
}

RC_t ADC_2_Read(sint8_t * const pdata)
{
    ADC_2_IsEndConversion(ADC_2_WAIT_FOR_RESULT);
    
    *pdata = ADC_2_GetResult8();
    
    return RC_SUCCESS;
}