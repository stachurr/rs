#ifndef RS_LOG_PRIVATE_H
#define RS_LOG_PRIVATE_H

#include "log/rs_log_level.h"
#include "util/rs_util.h"

#include RS_LOG_FUNC_INC

#include <type_traits>
#include <memory>
#include <source_location>
#include <fstream>


namespace rs {
namespace log {
namespace _private {

    // static constexpr level GLOBAL_LEVEL = level::RS_LOG_LEVEL_GLOBAL;

    // static constexpr const char *const get_level_string (level log_level) {
    //     return LEVEL_STRINGS[static_cast<std::underlying_type_t<level>>(log_level) - 1];
    //     static_assert(sizeof(LEVEL_STRINGS)/sizeof(LEVEL_STRINGS[0]) == static_cast<std::underlying_type_t<level>>(level::N_LEVELS) - 1);
    // }


    /*
    // Helper to wrap a C-string whether owned or unowned.
    class unique_cstring {
    public:
        // Disable implicit ctors
        unique_cstring (void)                   = delete;
        unique_cstring (const unique_cstring&)  = delete;
        unique_cstring (unique_cstring&&)       = delete;

        unique_cstring (const char* promise_it_persists_str)
            : m_data_unowned (promise_it_persists_str)
            , m_is_owned     (false)
        {}

        // Do not free this string when we're done!
        template<size_t N>
        consteval unique_cstring (const char (&static_str)[N])
            : m_data_unowned (static_str)
            , m_is_owned     (false)
        {}

        // Free this string when we're done!
        unique_cstring (std::unique_ptr<char[]> str)
            : m_data_owned  (std::move(str))
            , m_is_owned    (true)
        {}

        // Make sure to free if owned!
        virtual ~unique_cstring (void) {
            if (m_is_owned) {
                m_data_owned.reset(nullptr);
            }
        }


        constexpr const char* get (void) const noexcept {
            return (m_is_owned)
                ? m_data_owned.get()
                : m_data_unowned;
        }

        constexpr operator bool (void) const {
            return this->get();
        }

    private:
        union {
            const char* const       m_data_unowned;
            std::unique_ptr<char[]> m_data_owned;
        };
        const bool m_is_owned;
    };

    template<typename... ArgTypes>
    static const unique_cstring format_string (const char *const RS_RESTRICT format, ArgTypes&&... args) {
        if constexpr (sizeof...(ArgTypes) > 0) {
            // Check format validity.
            // https://en.cppreference.com/w/cpp/io/c/snprintf
            const int size_req = 1 + std::snprintf(nullptr, 0, format, args...);    // +1 for null-terminator

            if (size_req > 0) {
                auto s = std::unique_ptr<char[]>(new char[size_req]);
                (void) std::snprintf(s.get(), size_req, format, args...);
                return unique_cstring(std::move(s));
            }
            else {
                return unique_cstring(nullptr);
            }
        }
        else {
            // No formatting required.
            return unique_cstring(format);
        }
    }



    template<typename... ArgTypes>
    static constexpr void log (level&& log_level, rs_source_context&& ctx, ArgTypes&&... args) {
        if constexpr (GLOBAL_LEVEL > level::DISABLED) {
            // Don't log anything above the global log level.
            if (log_level <= GLOBAL_LEVEL) {
                const unique_cstring formatted = format_string(ctx.c_str(), args...);
                const char* message = formatted.get();

                if (!message) {
                    message = "" RS_TEXT_COLOR_DARK_RED "(failed to format string)" RS_TEXT_COLOR_DEFAULT;
                    log_level = level::ERROR;
                }

                RS_LOG_FUNC("" RS_TEXT_COLOR_DEFAULT "%s %s\t" RS_TEXT_COLOR_GRAY "%s: %u" RS_TEXT_COLOR_DEFAULT "\n",
                    get_level_string(log_level),
                    message,
                    ctx.file_name(),
                    ctx.location().line());
            }
        }
    }
    */
} // rs::log::_private
} // rs::log
} // rs

#endif // RS_LOG_PRIVATE_H
