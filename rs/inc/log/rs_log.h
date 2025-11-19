#ifndef RS_LOG_H
#define RS_LOG_H

#include "config/rs_config_private.h"
#if RS_ENABLE_LOG

#include "log/rs_log_private.h"
#include "log/rs_log_context.h"


namespace rs {
namespace log {
    /*
    template<typename... Args>
    void error (rs_source_context&& fmt, Args&&... args) {
        _private::log(rs_log_level::ERROR, std::move(fmt), args...);
    }

    template<typename... Args>
    void warn (rs_source_context&& fmt, Args&&... args) {
        _private::log(rs_log_level::WARN, std::move(fmt), args...);
    }

    template<typename... Args>
    void info (rs_source_context&& fmt, Args&&... args) {
        _private::log(rs_log_level::INFO, std::move(fmt), args...);
    }

    template<typename... Args>
    void trace (rs_source_context&& fmt, Args&&... args) {
        _private::log(rs_log_level::TRACE, std::move(fmt), args...);
    }
    */
} // rs::log
} // rs

#endif // RS_ENABLE_LOG
#endif // RS_LOG_H
