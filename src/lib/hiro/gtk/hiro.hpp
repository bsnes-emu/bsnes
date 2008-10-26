#ifndef HIRO_GTK_H
#define HIRO_GTK_H

#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <X11/extensions/dpms.h>
#include <X11/extensions/XTest.h>

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
