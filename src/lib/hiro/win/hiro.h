#ifndef HIRO_WIN_H
#define HIRO_WIN_H

#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE
#undef NOMINMAX

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600
#define NOMINMAX

#define UNICODE
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

namespace libhiro {

#include "widget.h"
  #include "window.h"
  #include "menucontrol.h"
    #include "menugroup.h"
    #include "menuitem.h"
    #include "menucheckitem.h"
    #include "menuradioitem.h"
    #include "menuseparator.h"
  #include "formcontrol.h"
    #include "frame.h"
    #include "canvas.h"
    #include "label.h"
    #include "button.h"
    #include "checkbox.h"
    #include "radiobox.h"
    #include "editbox.h"
    #include "listbox.h"
    #include "combobox.h"
    #include "progressbar.h"
    #include "slider.h"

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

  uint screen_width();
  uint screen_height();

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
  HFONT create_font(const char *name, uint size);

  array<Widget*> widget_list;
  Widget* get_widget(uint instance);
  LRESULT wndproc(HWND, UINT, WPARAM, LPARAM);
  uint16_t translate_key(uint key);
};

pHiro& phiro();

} //namsepace libhiro

#endif //ifndef HIRO_WIN_H
