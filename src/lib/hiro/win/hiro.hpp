#ifndef HIRO_WIN_H
#define HIRO_WIN_H

#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE
#undef NOMINMAX
#undef _NO_OLDNAMES

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600
#define NOMINMAX
#define _NO_OLDNAMES

#define mkdir _mkdir
#define UNICODE
#include <windows.h>
#include <commctrl.h>
#include <io.h>
#include <direct.h>
#include <shlobj.h>
#undef mkdir

#include <nall/algorithm.hpp>
using nall::min;
using nall::max;

#include <nall/utf8.hpp>
#define utf8  nall::utf8_t
#define utf16 nall::utf16_t

extern int hiromain(int argc, const char *const argv[]);

namespace libhiro {

#include "widget.hpp"
  #include "window.hpp"
  #include "menucontrol.hpp"
    #include "menugroup.hpp"
    #include "menuitem.hpp"
    #include "menucheckitem.hpp"
    #include "menuradioitem.hpp"
    #include "menuseparator.hpp"
  #include "formcontrol.hpp"
    #include "frame.hpp"
    #include "canvas.hpp"
    #include "label.hpp"
    #include "button.hpp"
    #include "checkbox.hpp"
    #include "radiobox.hpp"
    #include "editbox.hpp"
    #include "listbox.hpp"
    #include "combobox.hpp"
    #include "progressbar.hpp"
    #include "slider.hpp"

class pHiro {
public:
  Hiro &self;
  void init();
  void term();
  bool run();
  bool pending();

  bool folder_select(Window *focus, char *filename, const char *path = "");
  bool file_open(Window *focus, char *filename, const char *path = "", const char *filter = "");
  bool file_save(Window *focus, char *filename, const char *path = "", const char *filter = "");

  unsigned screen_width();
  unsigned screen_height();

  void enable_screensaver();
  void disable_screensaver();

  static pHiro& handle();
  pHiro(Hiro&);

  /* internal */
  OSVERSIONINFO osversioninfo;
  bool is_screensaver_enabled;
  HWND default_hwnd; //default parent window for all windowless controls
  HFONT default_font; //default font for all controls
  HBRUSH black_brush; //used for Canvas background
  HFONT create_font(const char *name, unsigned size);

  array<Widget*> widget_list;
  Widget* get_widget(unsigned instance);
  LRESULT wndproc(HWND, UINT, WPARAM, LPARAM);
  uint16_t translate_key(unsigned key);
};

pHiro& phiro();

} //namsepace libhiro

#endif //ifndef HIRO_WIN_H
