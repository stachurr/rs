#ifndef RS_LOG_LEVEL_H
#define RS_LOG_LEVEL_H

#include <cstdint>

namespace rs {
namespace log {

    enum class level_t : uint8_t { NONE = 0, ERROR, WARN, INFO, TRACE };

} // rs::log
} // rs

#endif // RS_LOG_LEVEL_H
