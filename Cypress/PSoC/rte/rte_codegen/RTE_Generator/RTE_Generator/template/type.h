/*
 * $Name/$_type.h
 *
 * Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm
 */



#ifndef $NAME/$_TYPE_H_
#define $NAME/$_TYPE_H_

#include "global.h"

/*****************************************************************************************
 *************** Type Definition for $NAME/$ signal                     **************************
 *****************************************************************************************/


#error "Provide your data structure"
typedef struct
{

} $NAME/$_data_t;

#error "Provide a sensible init value"
#define $NAME/$_INIT_DATA (($NAME/$_data_t){0})


/**
 * Default IN driver API - may be deleted if not required
 * \param $NAME/$_data_t *const data : [OUT] scaled data read from the driver
 * \return RC_SUCCESS is all was ok, error code otherwise
 */
RC_t $NAME/$_driverIn($NAME/$_data_t *const data);


/**
 * Default IN driver API - may be deleted if not required
 * \param const $NAME/$_data_t  *const data : [IN] signal data for the driver, will be scaled in this function
 * \return RC_SUCCESS is all was ok, error code otherwise
 */
RC_t $NAME/$_driverOut(const $NAME/$_data_t  *const data);



#endif /* $NAME/$_TYPE_H_ */

