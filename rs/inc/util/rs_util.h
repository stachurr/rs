#ifndef RS_UTIL_H
#define RS_UTIL_H

#include "util/rs_util_private.h"

#include <type_traits>
#include <cstdio>
#include <memory>

namespace rs {
namespace util {

    // String literal or constant static char array.
    struct string_literal {
        template<size_t N>
        consteval string_literal (const char (&str)[N])
            : m_data (str)
            // , m_length  (N - 1)
        {}
        
        template<size_t N>
        string_literal (char (&str)[N]) = delete; // Must be const.
        string_literal (void)           = delete; // Literal required.
        string_literal (std::nullptr_t) = delete; // Literal required.


        constexpr operator const char* (void) const noexcept {
            return this->c_str();
        }

        constexpr const char* c_str (void) const noexcept {
            return m_data;
        }

        // constexpr size_t length (void) const noexcept {
        //     return m_length;
        // }


    private:
        const char* const   m_data;
        // const size_t        m_length;
    };


    // Helper to capture std::source_location with a string literal.
    struct string_literal_source_context {
        consteval string_literal_source_context (void)                                  = delete;
        consteval string_literal_source_context (const string_literal_source_context&)  = delete;
        consteval string_literal_source_context (string_literal_source_context&&)       = delete;

        template<size_t N>
        consteval string_literal_source_context (const char (&str)[N], const std::source_location& location = std::source_location::current())
            : m_str      (str)
            , m_location (_private::consteval_source_location_truncate_file_name(location))
        {}


        // The string literal.
        constexpr const string_literal& litstr (void) const noexcept {
            return m_str;
        }

        // The source location.
        constexpr const std::source_location& location (void) const noexcept {
            return m_location;
        }


    private:
        string_literal              m_str;
        const std::source_location  m_location;
    };


    // Container for a C-string whether heap or stack.
    class managed_cstring {
    public:
        managed_cstring (void)                   = delete;
        managed_cstring (const managed_cstring&) = delete;


        // (Unowned) You promise this char array persists.
        constexpr managed_cstring (const char* str)
            : m_data    (str)
            , m_owned   (false)
        {}

        // (Owned) This string will be deleted upon destruction.
        managed_cstring (std::unique_ptr<char[]> str)
            : m_data    (str.release())
            , m_owned   (true)
        {}

        // (Unowned) String literal.
        consteval managed_cstring (string_literal str)
            : m_data    (str)
            , m_owned   (false)
        {}

        // Make sure to free if owned.
        virtual ~managed_cstring (void) {
            if (m_owned && m_data) {
                delete[] const_cast<char*>(m_data);
            }
        }

        
        constexpr operator const char* (void) const noexcept {
            return this->get();
        }

        constexpr operator bool (void) const {
            return static_cast<bool>(this->get());
        }

        constexpr const char* get (void) const noexcept {
            return m_data;
        }

    private:
        const char* m_data;
        bool        m_owned;
    };

    template<typename... FormatArgTypes>
    static constexpr managed_cstring format_string (string_literal format, FormatArgTypes&&... args) {
        if constexpr (sizeof...(FormatArgTypes) > 0) {
            // (Check format validity)[https://en.cppreference.com/w/cpp/io/c/snprintf]
            int size_req = std::snprintf(nullptr, 0, format, args...);

            if (size_req > 0) {
                auto str = std::unique_ptr<char[]>(new char[size_req]);
                (void) std::snprintf(str.get(), size_req, format, args...);
                return managed_cstring(std::move(str)); // owned
            }
            else {
                return managed_cstring(nullptr); // unowned
            }
        }
        else {
            // No formatting required.
            return managed_cstring(format.c_str()); // unowned
        }
    }



    // // Check if a file pointer is writable.
    // bool is_writable (FILE* fp);

    // Check if a file pointer is a TTY/console.
    bool is_terminal (FILE* fp);

} // rs::util
} // rs

using rs_string_literal = rs::util::string_literal;
using rs_source_context_cstring = rs::util::string_literal_source_context;
using rs_managed_cstring = rs::util::managed_cstring;

#endif // RS_UTIL_H
