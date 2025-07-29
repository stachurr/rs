#ifndef RS_SOURCE_LOCATION_CAPTURE_PRIVATE_H
#define RS_SOURCE_LOCATION_CAPTURE_PRIVATE_H

#include <source_location>
#ifdef __cpp_lib_unreachable
    #include <utility>
#endif


#ifdef _WIN32
    #define RS_RESTRICT __restrict
#elif defined(__GNUC__)
    #define RS_RESTRICT __restrict__
#else
    #define RS_RESTRICT
#endif



namespace rs {
namespace util {
namespace _private {

    static consteval size_t consteval_strlen (const char* string) {
        size_t len = 0;
        while (*string++) {
            len++;
        }
        return len;
    }

    template <size_t N>
    static consteval size_t consteval_strlen (const char (&string_literal)[N]) {
        return N - 1;
    }


    [[noreturn]]
    inline void unreachable (void) {
    #ifdef __cpp_lib_unreachable
        std::unreachable();
    #else
        // https://en.cppreference.com/w/cpp/utility/unreachable.html

        // Uses compiler specific extensions if possible.
        // Even if no extension is used, undefined behavior is still raised by
        // an empty function body and the noreturn attribute.
        #if defined(_MSC_VER) && !defined(__clang__) // MSVC
            __assume(false);
        #else // GCC, Clang
            __builtin_unreachable();
        #endif
    #endif
    }


    static consteval const char* consteval_source_location_get_shortname (const std::source_location& loc) {
        const size_t length = consteval_strlen(loc.file_name());
        const char* shortname = loc.file_name() + length;

        while (*shortname != '\\' && *shortname != '/') {
            shortname--;
        }
        shortname++;

        return shortname;
    }

} // rs::util::_private
} // rs::util
} // rs

#endif // RS_SOURCE_LOCATION_CAPTURE_PRIVATE_H
