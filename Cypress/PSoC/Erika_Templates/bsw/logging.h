/**
 * \file 	logging.h
 * \author 	Thomas Barth	- Hochschule Darmstadt thomas.barth@h-da.de
 * \date 	21.09.2016
 * \version	0.1
 *
 * \brief Logging mechanism without queue
 *
 *
 *
 *  Changelog:\n
 *  - 0.1   21.09.2016  Barth
 *            - Initial documentation
 *
 * \copyright Copyright ©2017
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

#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>
#include "global.h"
#include "system_config.h"

#define LOGGING_USE_N_T     0               /**< \brief Do not add date or time to logging messages*/
#define LOGGING_USE_T_T     1               /**< \brief Add time to logging messages*/
#define LOGGING_USE_DT_T    2               /**< \brief Add date and time to logging messages*/
    
/** \brief Number of Characters, the assembled string can have. Characters used in the assembly function have to be added manually. */
#define LOGGING_L_T                         ((CFG_LOGGING_L_M+CFG_LOGGING_LEVEL_L+CFG_LOGGING_SENDER_L)+CFG_LOGGING_FR_L) 
    
/** \brief Module API concationation with #CFG_LOGGING_MODULE. */
#define LOG_MODULE(x)                       PPCAT(CFG_LOGGING_MODULE, x)
    
 /** 
 * \brief Macro for sending messages to the logging system
 * \param x     Logging level, see #logging_level_t
 * \param y     Sender identification string
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 */
#define LOG(x,y,...)                                \
{if(x>=CFG_LOGGING_LEVEL){                          \
logging_t msg;                                      \
msg.level=x;                                        \
sprintf (msg.sender, "%s",y);                       \
sprintf (msg.message, __VA_ARGS__);                 \
vLOG_send(&msg);                                    \
}}                                                  \

 /** 
 * \brief Macro for sending debug messages to the logging system
 * \param y     Sender identification string
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 */
#define LOG_D(y,...)    LOG(LOG_DEBUG,y, __VA_ARGS__)
    
 /** 
 * \brief Macro for sending information messages to the logging system
 * \param y     Sender identification string
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 */
#define LOG_I(y,...)    LOG(LOG_INFO,y, __VA_ARGS__)
    
 /** 
 * \brief Macro for sending warning messages to the logging system
 * \param y     Sender identification string
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 */
#define LOG_W(y,...)    LOG(LOG_WARN,y, __VA_ARGS__)
    
 /** 
 * \brief Macro for sending error messages to the logging system
 * \param y     Sender identification string
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 */
#define LOG_E(y,...)    LOG(LOG_ERR,y, __VA_ARGS__)
    
 /** 
 * \brief Macro for  reading bytes from the logging UART module
 * \return see 'GetChar' of the UART Module
 */
#define LOG_READ()      LOG_MODULE(GetChar)()
    
 /** 
 * \brief Macro for sending a string directly to the logging UART module
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 */
#define LOG_PRINT(...)                              \
{sprintf (__uart_buf, __VA_ARGS__);                 \
LOG_MODULE(PutString)(__uart_buf);                  \
}                                                   \
    
/**
 * \brief Debugging Level.
 *
 * The debugging level defines the character of an event and is used to define which events are send to the log using #LOGGING_LEVEL
 */
typedef enum{
    LOG_DEBUG,                              /**< \brief Low Level Information */
    LOG_INFO,                               /**< \brief High level information during normal operation */
    LOG_WARN,                               /**< \brief abnormal operation, which might be fixed */
    LOG_ERR,                                /**< \brief abnormal operation, which can not be fixed */
    LOG_OFF                                 /**< \brief Event logging is switched off */
}logging_level_t;


/**
 * \brief Logging structure
 *
 * Each entry in the log has the following structure
 */
typedef struct{
#if(CFG_LOGGING_DT_T==LOGGING_USE_N_T)
    //no timestamp for log
#elif(CFG_LOGGING_DT_T==LOGGING_USE_T_T)
    vRTC_time_arr_t     timestamp;                          /**< \brief Timestamp with time only*/
#elif(CFG_LOGGING_DT_T==LOGGING_USE_DT_T)
    vRTC_datetime_arr_t timestamp[vRTC_TIME_DT_L];          /**< \brief Timestamp with date and time */
#endif
    logging_level_t     level;                              /**< \brief Logging level, see #logging_level_t */
    char                sender [CFG_LOGGING_SENDER_L+1];    /**< \brief sender of the message */
    char                message[CFG_LOGGING_L_M+1];         /**< \brief message content */
}logging_t;

/**
 * \brief Reference to the UART string buffer
 */
extern char __uart_buf[];

/**
 * \brief   Start the UART logging module
 * \return  #RC_SUCCESS
 */
RC_t vLOG_init();

/**
 * \brief   Send a new logging message. It is recommended to use #LOGGING_SEND instead of this function directly.
 * \param   entry   Logging message, see #logging_t.
 * \return  #RC_SUCCESS
 */
RC_t vLOG_send(logging_t* __message);

#endif /* LOGGING_H_ */

/* [] END OF FILE */