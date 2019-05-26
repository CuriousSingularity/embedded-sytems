/**
 * \file 	XXX.h
 * \author 	Thomas Barth	- Hochschule Darmstadt thomas.barth@h-da.de
 * \date 	XXXX
 * \version 0.1
 *
 * \brief XXX
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

#include "logging.h"

//UART string buffer
char __uart_buf[LOGGING_L_T];

/**
 * \brief Stringification of debug levels
 */
static const char __level_str[][CFG_LOGGING_LEVEL_L+1]={
    "Debug",
    "Info",
    "Warning",
    "Error"
};

RC_t vLOG_init(){
    LOG_MODULE(Start)();
    return RC_SUCCESS;
}

RC_t vLOG_send(logging_t* __message){
             
#if(CFG_LOGGING_DT_T==LOGGING_USE_N_T)            
            sprintf(                                //Assemble output string
                __uart_buf,                         //output buffer for message      
                "%*.*s from %*.*s : %s\n",          //sprintf message template
                CFG_LOGGING_LEVEL_L,                //Level str-length
                CFG_LOGGING_LEVEL_L,                //Level str-length
                __level_str[__message->level],      //Level str
                CFG_LOGGING_SENDER_L,               //Sender ID str-length
                CFG_LOGGING_SENDER_L,               //Sender ID str-length
                __message->sender,                  //Sender ID str
                __message->message                  //Message
            );
#elif(CFG_LOGGING_DT_T==LOGGING_USE_T_T)
            if(vRTC_TS_VALID_HAS(__message.timestamp)){                    
                sprintf(                            //Assemble output string
                    __uart_buf,                     //output buffer for message      
                    "%02d:%02d:%02d %*.*s from %*.*s : %s\r\n",    //sprintf message template
                    __message.timestamp[vRTC_TIME_T_HO_I],
                    __message.timestamp[vRTC_TIME_T_MI_I],
                    __message.timestamp[vRTC_TIME_T_SE_I],
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    __level_str[__message.level],   //Level str
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    __message.sender,               //Sender ID str
                    __message.message               //Message
                );
            }
            else{
                sprintf(                            //Assemble output string
                    __uart_buf,                     //output buffer for message      
                    "no RTC   %*.*s from %*.*s : %s\r\n",    //sprintf message template
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    __level_str[__message.level],   //Level str
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    __message.sender,               //Sender ID str
                    __message.message               //Message
                    );
            }
#elif(CFG_LOGGING_DT_T==LOGGING_USE_DT_T)
            if(vRTC_TS_VALID_HAS(__message.timestamp)){                    
                sprintf(                            //Assemble output string
                    __uart_buf,                     //output buffer for message      
                    "%02d:%02d:%02d %02d.%02d.%02d %*.*s from %*.*s : %s\r\n",    //sprintf message template
                    __message.timestamp[vRTC_TIME_DT_HO_I],
                    __message.timestamp[vRTC_TIME_DT_MI_I],
                    __message.timestamp[vRTC_TIME_DT_SE_I],
                    __message.timestamp[vRTC_TIME_DT_DY_I],
                    __message.timestamp[vRTC_TIME_DT_MO_I],
                    __message.timestamp[vRTC_TIME_DT_YE_I],
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    __level_str[__message.level],   //Level str
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    __message.sender,               //Sender ID str
                    __message.message               //Message
                );
            }
            else{
                sprintf(                            //Assemble output string
                    __uart_buf,                     //output buffer for message      
                    "no RTC            %*.*s from %*.*s : %s\r\n",    //sprintf message template
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    CFG_LOGGING_LEVEL_L,            //Level str-length
                    __level_str[__message.level],   //Level str
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    CFG_LOGGING_SENDER_L,           //Sender ID str-length
                    __message.sender,               //Sender ID str
                    __message.message               //Message
                    );
            }
#endif

    //write str to UART
    LOG_MODULE(PutString)(__uart_buf);
    
    return RC_SUCCESS;
}

/* [] END OF FILE */
