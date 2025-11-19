#ifndef RS_UTIL_PRIVATE_H
#define RS_UTIL_PRIVATE_H

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
    static consteval size_t consteval_strlen (const char (&litstr)[N]) {
        return N - 1;
    }


    template<typename T>
    static consteval bool consteval_memcmp (const T* b1, const T* b2, size_t n) {
        for (size_t i = 0; i < n; i++) {
            if (b1[i] != b2[i]) {
                return false;
            }
        }
        return true;
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


    // static consteval const char* consteval_source_location_get_shortname (const std::source_location& loc) {
    //     const size_t length = consteval_strlen(loc.file_name());
    //     const char* shortname = loc.file_name() + length;
    // 
    //     while (*shortname != '\\' && *shortname != '/') {
    //         shortname--;
    //     }
    //     shortname++;
    // 
    //     return shortname;
    // }

    static consteval std::source_location consteval_source_location_truncate_file_name (const std::source_location& location) {
#ifdef _WIN32
    #define RS_UTIL_PRIVATE_PATH_SEPARATOR  '\\'
#else
    #define RS_UTIL_PRIVATE_PATH_SEPARATOR  '/'
#endif

        // Point to the end of the full path and iterate
        // backwards until we encounter a path separator
        // or reach the beginning.
        //
        // The result is a pointer either offset forward
        // from the original or the same as the original.
        //
        const char* const BEGIN = location.file_name();
        const char* const END   = location.file_name() + consteval_strlen(location.file_name());
        const char* ptr = END;

        while (ptr != BEGIN) {
            if (*ptr == RS_UTIL_PRIVATE_PATH_SEPARATOR) {
                ptr++;
                break;
            }
            ptr--;
        }

        // Use the (potentially) different pointer.
        return std::source_location::current(location.line(),
                                             location.column(),
                                             ptr,
                                             location.function_name());
#undef RS_UTIL_PRIVATE_PATH_SEPARATOR
    }

} // rs::util::_private
} // rs::util
} // rs

#endif // RS_UTIL_PRIVATE_H
