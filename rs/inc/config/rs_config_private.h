#ifndef RS_CONFIG_PRIVATE_H
#define RS_CONFIG_PRIVATE_H

#include "rs_config.h"



///////////////////
//    LOGGING    //
///////////////////

#ifndef RS_ENABLE_LOG
    #define RS_ENABLE_LOG   0
#endif

#if RS_ENABLE_LOG
    // Default values for all options.
    #ifndef RS_LOG_FUNC
        #define RS_LOG_FUNC             printf
        #define RS_LOG_FUNC_INC         "stdio.h"
    #endif

    #ifndef RS_LOG_ENABLE_COLOR
        #define RS_LOG_ENABLE_COLOR     0
    #endif

    #ifndef RS_GLOBAL_LOG_LEVEL
        #define RS_GLOBAL_LOG_LEVEL     level_t::WARN
    #endif
#endif



#endif // RS_CONFIG_PRIVATE_H
