#ifndef HIRO_GTK_H
#define HIRO_GTK_H

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <X11/extensions/dpms.h>
#include <X11/extensions/XTest.h>

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

  unsigned screen_width();
  unsigned screen_height();

  void enable_screensaver();
  void disable_screensaver();

  static pHiro& handle();
  pHiro(Hiro&);

  /* internal */
  GdkScreen *screen;
  GdkColormap *colormap;
  PangoFontDescription *font;
  bool is_composited;
  char default_path[PATH_MAX];

  void set_default_path(const char*);
  bool is_screensaver_enabled;
  void screensaver_tick();
  uint16_t translate_key(unsigned key);
};

pHiro& phiro();

} //namespace libhiro

#endif //ifndef HIRO_GTK_H
