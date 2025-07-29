#ifndef RS_CONFIG_H
#define RS_CONFIG_H



///////////////////
//    LOGGING    //
///////////////////

// Enable/disable logging module (0: disabled, 1: enabled).
#define RS_ENABLE_LOG   1
#if RS_ENABLE_LOG
    // Logging function used to print to stdout (must have signature `int (const char*, ...)`).
    #define RS_LOG_FUNC             printf

    // Enable colored text when printing to stdout (0: disabled, 1: enabled).
    #define RS_LOG_ENABLE_COLOR     1

    // Set the maximum log level (ERROR, WARN, INFO, TRACE, NONE).
    #define RS_GLOBAL_LOG_LEVEL     level_t::TRACE
#endif // RS_ENABLE_LOG



#endif // RS_CONFIG_H
