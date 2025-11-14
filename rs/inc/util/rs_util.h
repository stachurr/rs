#ifndef RS_SOURCE_LOCATION_CAPTURE_H
#define RS_SOURCE_LOCATION_CAPTURE_H

#include "util/rs_util_private.h"

#include <type_traits>

namespace rs {
namespace util {

    // A value wrapper which provides source code information.
    template<typename T>
    struct source_location_capture {
        consteval source_location_capture (const T& value, const std::source_location& loc = std::source_location::current())
            : m_value (value)
            , m_loc (loc)
            , m_short_file_name (_private::consteval_source_location_get_shortname(loc))
        {}

        consteval source_location_capture (void) = delete;
        consteval source_location_capture (const source_location_capture&) = delete;
        consteval source_location_capture (source_location_capture&&) = delete;


        // The line number.
        constexpr uint_least32_t line() const noexcept {
            return m_loc.line();
        }

        // The column number.
        constexpr uint_least32_t column() const noexcept {
            return m_loc.column();
        }

        // The file name (absolute path).
        constexpr const char* file_name() const noexcept {
            return m_loc.file_name();
        }

        // The function name.
        constexpr const char* function_name() const noexcept {
            return m_loc.function_name();
        }


        // The file name without the parent path.
        constexpr const char* short_file_name (void) const noexcept {
            return m_short_file_name;
        }

        // The wrapped value.
        constexpr std::remove_reference_t<T>& value (void) const noexcept {
            return m_value;
        }

    private:
        T                           m_value;
        const std::source_location  m_loc;
        const char* const           m_short_file_name;
    };

} // rs::util
} // rs


using rs_source_location_capture_cstring = rs::util::source_location_capture<const char* const>;

#endif // RS_SOURCE_LOCATION_CAPTURE_H
