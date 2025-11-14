#ifndef RS_LOG_PRIVATE_H
#define RS_LOG_PRIVATE_H

#include "log/rs_log_level.h"
#include "ansi/rs_ansi.h"
#include "util/rs_util.h"

#include RS_LOG_FUNC_INC

#include <type_traits>
#include <memory>
#include <source_location>


namespace rs {
namespace log {
namespace _private {

#pragma region "Push & Define Log Level Colors"
    #pragma push_macro("ERROR_COLOR")
    #pragma push_macro("WARN_COLOR")
    #pragma push_macro("INFO_COLOR")
    #pragma push_macro("TRACE_COLOR")
    #pragma push_macro("DEFAULT_COLOR")
    #if RS_LOG_ENABLE_COLOR
        #define ERROR_COLOR     RS_TEXT_COLOR_RGB(239, 71, 111)
        #define WARN_COLOR      RS_TEXT_COLOR_RGB(255, 209, 102)
        #define INFO_COLOR      RS_TEXT_COLOR_RGB(17, 138, 178)
        #define TRACE_COLOR     RS_TEXT_COLOR_RGB(100, 100, 100) //RS_TEXT_COLOR_GRAY //RS_RGB(7, 59, 76)
        #define DEFAULT_COLOR   RS_TEXT_COLOR_DEFAULT
    #else
        #define ERROR_COLOR     ""
        #define WARN_COLOR      ""
        #define INFO_COLOR      ""
        #define TRACE_COLOR     ""
        #define DEFAULT_COLOR   ""
    #endif
#pragma endregion
    static constexpr const char* LOG_LEVEL_TITLES[] = {
        ERROR_COLOR "[ERROR]" DEFAULT_COLOR,
        WARN_COLOR  "[WARN] " DEFAULT_COLOR,
        INFO_COLOR  "[INFO] " DEFAULT_COLOR,
        TRACE_COLOR "[TRACE]" DEFAULT_COLOR
    };
#pragma region "Undefine & Pop Log Level Colors"
    #undef ERROR_COLOR
    #undef WARN_COLOR
    #undef INFO_COLOR
    #undef TRACE_COLOR
    #undef DEFAULT_COLOR
    #pragma pop_macro("ERROR_COLOR")
    #pragma pop_macro("WARN_COLOR")
    #pragma pop_macro("INFO_COLOR")
    #pragma pop_macro("TRACE_COLOR")
    #pragma pop_macro("DEFAULT_COLOR")
#pragma endregion

    static constexpr const char *const get_level_string (level_t level) {
        return LOG_LEVEL_TITLES[static_cast<std::underlying_type_t<level_t>>(level) - 1];
    }

    class maybe_string {
    public:
        maybe_string (std::nullptr_t)
            : m_data_unowned (nullptr)
            , m_length       (0)
            , m_owned        (false)
        {}

        maybe_string (const char* str)
            : m_data_unowned (str)
            , m_length       (strnlen_s(str, RS_LOG_MAX_STRLEN))
            , m_owned        (false)
        {}

        maybe_string (std::unique_ptr<char[]> str)
            : m_data_owned  (std::move(str))
            , m_length      (strnlen_s(m_data_owned.get(), RS_LOG_MAX_STRLEN))
            , m_owned       (true)
        {}

        ~maybe_string (void) {
            if (m_owned) {
                m_data_owned.reset();
            }
        }

        maybe_string (void)                 = delete;
        maybe_string (const maybe_string&)  = delete;
        maybe_string (maybe_string&&)       = delete;


        constexpr const char* c_str (void) const noexcept {
            return (m_length == RS_LOG_MAX_STRLEN)
                ? nullptr       // strnlen_s did not find null terminator
                : (m_owned)
                    ? m_data_owned.get()
                    : m_data_unowned;
        }

        constexpr size_t length (void) const noexcept {
            return (m_length == RS_LOG_MAX_STRLEN)
                ? 0     // strnlen_s did not find null terminator
                : m_length;
        }

        constexpr operator bool (void) const {
            return static_cast<bool>(this->c_str());
        }

    private:
        union {
            const char* const       m_data_unowned;
            std::unique_ptr<char[]> m_data_owned;
        };
        const size_t m_length;
        const bool m_owned;
    };

    template<typename... ArgTypes>
    static maybe_string format_string (const char *const RS_RESTRICT format, ArgTypes&&... args) {
        if constexpr (sizeof...(ArgTypes) > 0) {
            // Check format validity.
            // https://en.cppreference.com/w/cpp/io/c/snprintf
            const int size_req = 1 + std::snprintf(nullptr, 0, format, args...);    // +1 for null-terminator

            if (size_req > 0) {
                auto s = std::unique_ptr<char[]>(new char[size_req]);
                (void) std::snprintf(s.get(), size_req, format, args...);
                return maybe_string(std::move(s));
            }
            else {
                return maybe_string(nullptr);
            }
        }
        else {
            // No formatting required.
            return maybe_string(format);
        }
    }

    template<typename... ArgTypes>
    static void log (level_t level, rs_source_location_capture_cstring&& loc, ArgTypes&&... args) {
        // Enabled logging if global log level is not NONE.
        if constexpr (RS_GLOBAL_LOG_LEVEL > level_t::NONE) {
            // Don't log anything above the global log level.
            if (level <= RS_GLOBAL_LOG_LEVEL) {
                // Format and print.
                const maybe_string maybe_str = format_string(loc.value(), args...);
                const char* message;

                if (maybe_str) {
                    message = maybe_str.c_str();
                }
                else {
                    level = level_t::ERROR;
                    message = "<cannot format string>";
                }

                RS_LOG_FUNC(RS_TEXT_COLOR_DEFAULT "%s %s\t" RS_TEXT_COLOR_GRAY "%s: %u" RS_TEXT_COLOR_DEFAULT "\n",
                    get_level_string(level),
                    message,
                    loc.short_file_name(),
                    loc.line());
            }
        }
    }

} // rs::log::_private
} // rs::log
} // rs

#endif // RS_LOG_PRIVATE_H
