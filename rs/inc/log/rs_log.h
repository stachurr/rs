#ifndef RS_LOG_H
#define RS_LOG_H

#include "private/rs_config_private.h"
#if RS_ENABLE_LOG

#include "log/rs_log_private.h"


namespace rs {
namespace log {
    template<typename... Args>
    void error (rs_source_location_capture_cstring&& fmt, Args&&... args) {
        _private::log(level_t::ERROR, std::move(fmt), args...);
    }

    template<typename... Args>
    void warn (rs_source_location_capture_cstring&& fmt, Args&&... args) {
        _private::log(level_t::WARN, std::move(fmt), args...);
    }

    template<typename... Args>
    void info (rs_source_location_capture_cstring&& fmt, Args&&... args) {
        _private::log(level_t::INFO, std::move(fmt), args...);
    }

    template<typename... Args>
    void trace (rs_source_location_capture_cstring&& fmt, Args&&... args) {
        _private::log(level_t::TRACE, std::move(fmt), args...);
    }
} // rs::log
} // rs

#endif // RS_ENABLE_LOG
#endif // RS_LOG_H
