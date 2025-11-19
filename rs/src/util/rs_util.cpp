#include <cstdio>
#include <cerrno>
#include <sys/stat.h>

#ifdef _WIN32
    #include <io.h>
    #define RS_FILENO_FUNC  _fileno
    #define RS_ISATTY_FUNC  _isatty
    #define RS_FSTAT_FUNC   _fstat
    #define RS_FSTAT_TYPE   _stat
#else
    #include <unistd.h>
    #define RS_FILENO_FUNC  fileno
    #define RS_ISATTY_FUNC  isatty
    #define RS_FSTAT_FUNC   fstat
    #define RS_FSTAT_TYPE   stat
#endif

#define STR_DEFERED(x)  #x
#define STR(x)          STR_DEFERED(x)

namespace rs {
namespace util {

    static bool fileno (FILE* fp, int& __fd) {
        const int fd = RS_FILENO_FUNC(fp);
        if (-1 == fd) {
            perror(STR(RS_FILENO_FUNC));
            return false;
        }
        else {
            __fd = fd;
            return true;
        }
    }

    static bool isatty (FILE* fp) {
        int fd;
        if (fileno(fp, fd)) {
            if (0 != RS_ISATTY_FUNC(fd)) {
                return true;
            }

            if (errno != ENOTTY) {
                perror(STR(RS_ISATTY_FUNC));
            }
        }
        return false;
    }

    static bool fstat (FILE* fp, struct RS_FSTAT_TYPE* stat) {
        int fd;
        if (fileno(fp, fd)) {
            const int status = RS_FSTAT_FUNC(fd, stat);
            if (0 == status) {
                return true;
            }
            else {
                perror(STR(RS_FSTAT_FUNC));
            }
        }
        return false;
    }



    // bool is_writable (FILE* fp) {
    //     struct RS_FSTAT_TYPE stat;
    //     if (fstat(fp, &stat)) {
    //         // if (0200 & stat.s) {
    //         //     return true;
    //         // }
    //         access()
    //     }
    //     return false;
    // }

    bool is_terminal (FILE* fp) {
        return isatty(fp);
    }

} // rs::util
} // rs