#ifndef MIU_WIN_H
#define MIU_WIN_H

#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600

#include <windows.h>
#include <commctrl.h>

#include "../miu.h"

namespace ns_miu {

#include "miu.win.widget.h"
  #include "miu.win.window.h"
  #include "miu.win.menucontrol.h"
    #include "miu.win.menugroup.h"
    #include "miu.win.menuitem.h"
    #include "miu.win.menucheckitem.h"
    #include "miu.win.menuradioitem.h"
    #include "miu.win.menuseparator.h"
  #include "miu.win.formcontrol.h"
    #include "miu.win.frame.h"
    #include "miu.win.canvas.h"
    #include "miu.win.label.h"
    #include "miu.win.button.h"
    #include "miu.win.checkbox.h"
    #include "miu.win.radiobox.h"
    #include "miu.win.editbox.h"
    #include "miu.win.listbox.h"
    #include "miu.win.combobox.h"
    #include "miu.win.progressbar.h"
    #include "miu.win.slider.h"

class pMiu {
public:
  Miu &self;
  void init();
  void term();
  bool run();
  bool pending();

  bool file_load(Window *focus, char *filename, const char *filter, const char *path);
  bool file_save(Window *focus, char *filename, const char *filter, const char *path);

  uint screen_width();
  uint screen_height();

  static pMiu& handle();
  pMiu(Miu&);

/* internal */
  HWND default_hwnd; //default parent window for all windowless controls
  HFONT default_font; //default font for all controls
  HBRUSH black_brush; //used for Canvas background
  HFONT create_font(const char *name, uint size);

  array<Widget*> widget_list;
  Widget* get_widget(uint instance);
  long wndproc(HWND, UINT, WPARAM, LPARAM);
  uint16 translate_key(uint key);
};

pMiu& pmiu();

} //namsepace ns_miu

#endif //ifndef MIU_WIN_H
