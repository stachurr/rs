#ifndef RS_ANSI_H
#define RS_ANSI_H

#define RS_DEFAULT          "\x1b[39m"
#define RS_BLACK            "\x1b[30m"
#define RS_DARK_RED         "\x1b[31m"
#define RS_DARK_GREEN       "\x1b[32m"
#define RS_DARK_YELLOW      "\x1b[33m"
#define RS_DARK_BLUE        "\x1b[34m"
#define RS_DARK_MAGENTA     "\x1b[35m"
#define RS_DARK_CYAN        "\x1b[36m"
#define RS_GRAY             "\x1b[90m"
#define RS_RED              "\x1b[91m"
#define RS_GREEN            "\x1b[92m"
#define RS_YELLOW           "\x1b[93m"
#define RS_BLUE             "\x1b[94m"
#define RS_MAGENTA          "\x1b[95m"
#define RS_CYAN             "\x1b[96m"
#define RS_RGB(r,g,b)       "\x1b[38;2;" #r ";" #g ";" #b "m"

#define RS_DEFAULT_BG       "\x1b[49m"
#define RS_BLACK_BG         "\x1b[40m"
#define RS_DARK_RED_BG      "\x1b[41m"
#define RS_DARK_GREEN_BG    "\x1b[42m"
#define RS_DARK_YELLOW_BG   "\x1b[43m"
#define RS_DARK_BLUE_BG     "\x1b[44m"
#define RS_DARK_MAGENTA_BG  "\x1b[45m"
#define RS_DARK_CYAN_BG     "\x1b[46m"
#define RS_GRAY_BG          "\x1b[100m"
#define RS_RED_BG           "\x1b[101m"
#define RS_GREEN_BG         "\x1b[102m"
#define RS_YELLOW_BG        "\x1b[103m"
#define RS_BLUE_BG          "\x1b[104m"
#define RS_MAGENTA_BG       "\x1b[105m"
#define RS_CYAN_BG          "\x1b[106m"
#define RS_RGB_BG(r,g,b)    "\x1b[48;2;" #r ";" #g ";" #b "m"

#define RS_ORANGE           RS_RGB(255,140,0)
#define RS_ORANGE_BG        RS_RGB_BG(255,140,0)

#endif // RS_ANSI_H
