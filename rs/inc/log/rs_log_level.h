#ifndef RS_LOG_LEVEL_H
#define RS_LOG_LEVEL_H

#include "rs_log_level_private.h"
#include <cstdint>

namespace rs {
namespace log {

    enum class level : uint8_t {
        DISABLED = 0,
        ERROR,
        WARN,
        INFO,
        TRACE,

        N_LEVELS,
        ALL = TRACE,
    };

} // rs::log
} // rs

using rs_log_level = rs::log::level;

#endif // RS_LOG_LEVEL_H
