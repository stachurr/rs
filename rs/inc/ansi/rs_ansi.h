#ifndef RS_ANSI_H
#define RS_ANSI_H

/* This is a non-comprehensive list of ANSI Escape Codes.
 * [Ref](https://en.wikipedia.org/wiki/ANSI_escape_code#Select_Graphic_Rendition_parameters)
 */


///////////////////////
//  PRIVATE HELPERS  //
///////////////////////

#define _RS_ESC                 "\x1b"
#define _RS_CSI                 _RS_ESC "["
#define _RS_MAKE_CSI_CMD(cmd)   _RS_CSI cmd
#define _RS_MAKE_SGR(params)    _RS_MAKE_CSI_CMD(params "m")






///////////////////////
//  CURSOR POSITION  //
///////////////////////

#define RS_CURSOR_UP(n)         _RS_MAKE_CSI_CMD(#n "A")
#define RS_CURSOR_DOWN(n)       _RS_MAKE_CSI_CMD(#n "B")
#define RS_CURSOR_FWD(n)        _RS_MAKE_CSI_CMD(#n "C")
#define RS_CURSOR_BKWD(n)       _RS_MAKE_CSI_CMD(#n "D")
#define RS_CURSOR_NEXT_LINE(n)  _RS_MAKE_CSI_CMD(#n "E")
#define RS_CURSOR_PREV_LINE(n)  _RS_MAKE_CSI_CMD(#n "F")
#define RS_CURSOR_COL(c)        _RS_MAKE_CSI_CMD(#c "G")
#define RS_CURSOR_POS(r,c)      _RS_MAKE_CSI_CMD(#r ";" #c "H")

#define RS_CURSOR_SAVE_POS      _RS_MAKE_CSI_CMD("s")
#define RS_CURSOR_RESTORE_POS   _RS_MAKE_CSI_CMD("u")
#define RS_CURSOR_SHOW          _RS_MAKE_CSI_CMD("?25h")
#define RS_CURSOR_HIDE          _RS_MAKE_CSI_CMD("?25l")



///////////////////////
//   TERMINAL CTRL   //
///////////////////////

#define RS_TERM_ENABLE_FOCUS_REPORT     _RS_MAKE_CSI_CMD("?1004h")
#define RS_TERM_DISABLE_FOCUS_REPORT    _RS_MAKE_CSI_CMD("?1004l")

#define RS_TERM_ENABLE_ALT_BUFFER       _RS_MAKE_CSI_CMD("?1049h")
#define RS_TERM_DISABLE_ALT_BUFFER      _RS_MAKE_CSI_CMD("?1049l")

// Synonyms
#define RS_TERM_ALT_BUFFER              RS_TERM_ENABLE_ALT_BUFFER
#define RS_TERM_MAIN_BUFFER             RS_TERM_DISABLE_ALT_BUFFER



///////////////////////
//    TEXT FORMAT    //
///////////////////////

#define RS_TEXT_RESET               _RS_MAKE_SGR("0")
#define RS_TEXT_INC_INTENSITY       _RS_MAKE_SGR("1")
#define RS_TEXT_DEC_INTENSITY       _RS_MAKE_SGR("2")
#define RS_TEXT_ITALIC              _RS_MAKE_SGR("3")
#define RS_TEXT_UNDERLINE           _RS_MAKE_SGR("4")

#define RS_TEXT_STRIKE              _RS_MAKE_SGR("9")

#define RS_TEXT_NORMAL_INTENSITY    _RS_MAKE_SGR("22")
#define RS_TEXT_NITALIC             _RS_MAKE_SGR("23")
#define RS_TEXT_NUNDERLINE          _RS_MAKE_SGR("24")

#define RS_TEXT_NSTRIKE             _RS_MAKE_SGR("29")

// Synonyms
#define RS_TEXT_BOLD                RS_TEXT_INC_INTENSITY
#define RS_TEXT_FAINT               RS_TEXT_DEC_INTENSITY
#define RS_TEXT_NBOLD               RS_TEXT_NORMAL_INTENSITY
#define RS_TEXT_NFAINT              RS_TEXT_NORMAL_INTENSITY



///////////////////////
//    TEXT COLORS    //
///////////////////////

// RGBs
#define _RS_RGB_ORANGE  255,140,0

// FG Colors
#define RS_TEXT_COLOR_DEFAULT           _RS_MAKE_SGR("39")
#define RS_TEXT_COLOR_BLACK             _RS_MAKE_SGR("30")
#define RS_TEXT_COLOR_DARK_RED          _RS_MAKE_SGR("31")
#define RS_TEXT_COLOR_DARK_GREEN        _RS_MAKE_SGR("32")
#define RS_TEXT_COLOR_DARK_YELLOW       _RS_MAKE_SGR("33")
#define RS_TEXT_COLOR_DARK_BLUE         _RS_MAKE_SGR("34")
#define RS_TEXT_COLOR_DARK_MAGENTA      _RS_MAKE_SGR("35")
#define RS_TEXT_COLOR_DARK_CYAN         _RS_MAKE_SGR("36")
#define RS_TEXT_COLOR_GRAY              _RS_MAKE_SGR("90")
#define RS_TEXT_COLOR_RED               _RS_MAKE_SGR("91")
#define RS_TEXT_COLOR_GREEN             _RS_MAKE_SGR("92")
#define RS_TEXT_COLOR_YELLOW            _RS_MAKE_SGR("93")
#define RS_TEXT_COLOR_BLUE              _RS_MAKE_SGR("94")
#define RS_TEXT_COLOR_MAGENTA           _RS_MAKE_SGR("95")
#define RS_TEXT_COLOR_CYAN              _RS_MAKE_SGR("96")
#define RS_TEXT_COLOR_RGB(r,g,b)        _RS_MAKE_SGR("38;2;" #r ";" #g ";" #b)
#define RS_TEXT_COLOR_ORANGE            RS_TEXT_COLOR_RGB(_RS_RGB_ORANGE)

// BG Colors
#define RS_TEXT_COLOR_DEFAULT_BG        _RS_MAKE_SGR("49")
#define RS_TEXT_COLOR_BLACK_BG          _RS_MAKE_SGR("40")
#define RS_TEXT_COLOR_DARK_RED_BG       _RS_MAKE_SGR("41")
#define RS_TEXT_COLOR_DARK_GREEN_BG     _RS_MAKE_SGR("42")
#define RS_TEXT_COLOR_DARK_YELLOW_BG    _RS_MAKE_SGR("43")
#define RS_TEXT_COLOR_DARK_BLUE_BG      _RS_MAKE_SGR("44")
#define RS_TEXT_COLOR_DARK_MAGENTA_BG   _RS_MAKE_SGR("45")
#define RS_TEXT_COLOR_DARK_CYAN_BG      _RS_MAKE_SGR("46")
#define RS_TEXT_COLOR_GRAY_BG           _RS_MAKE_SGR("100")
#define RS_TEXT_COLOR_RED_BG            _RS_MAKE_SGR("101")
#define RS_TEXT_COLOR_GREEN_BG          _RS_MAKE_SGR("102")
#define RS_TEXT_COLOR_YELLOW_BG         _RS_MAKE_SGR("103")
#define RS_TEXT_COLOR_BLUE_BG           _RS_MAKE_SGR("104")
#define RS_TEXT_COLOR_MAGENTA_BG        _RS_MAKE_SGR("105")
#define RS_TEXT_COLOR_CYAN_BG           _RS_MAKE_SGR("106")
#define RS_TEXT_COLOR_RGB_BG(r,g,b)     _RS_MAKE_SGR("48;2;" #r ";" #g ";" #b)
#define RS_TEXT_COLOR_ORANGE_BG         RS_TEXT_COLOR_RGB_BG(_RS_RGB_ORANGE)

#endif // RS_ANSI_H
