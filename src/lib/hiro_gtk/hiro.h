#ifndef HIRO_GTK_H
#define HIRO_GTK_H

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

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

  bool file_load(Window *focus, char *filename, const char *filter, const char *path);
  bool file_save(Window *focus, char *filename, const char *filter, const char *path);

  uint screen_width();
  uint screen_height();

  static pHiro& handle();
  pHiro(Hiro&);

  /* internal */
  uint16_t translate_key(uint key);
};

pHiro& phiro();

} //namespace libhiro

#endif //ifndef HIRO_GTK_H
