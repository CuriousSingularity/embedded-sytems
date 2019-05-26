/*
 * adc_type.h
 *
 * Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm
 */



#ifndef ADC_TYPE_H_
#define ADC_TYPE_H_

#include "global.h"

/*****************************************************************************************
 *************** Type Definition for ADC signal                     **************************
 *****************************************************************************************/


#error "Provide your data structure"
typedef struct
{

} ADC_data_t;

#error "Provide a sensible init value"
#define ADC_INIT_DATA ((ADC_data_t){0})


/**
 * Default IN driver API - may be deleted if not required
 * \param ADC_data_t *const data : [OUT] scaled data read from the driver
 * \return RC_SUCCESS is all was ok, error code otherwise
 */
RC_t ADC_driverIn(ADC_data_t *const data);


/**
 * Default IN driver API - may be deleted if not required
 * \param const ADC_data_t  *const data : [IN] signal data for the driver, will be scaled in this function
 * \return RC_SUCCESS is all was ok, error code otherwise
 */
RC_t ADC_driverOut(const ADC_data_t  *const data);



#endif /* ADC_TYPE_H_ */

