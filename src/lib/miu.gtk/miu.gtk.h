#ifndef MIU_GTK_H
#define MIU_GTK_H

#include "../miu.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

namespace ns_miu {

#include "miu.gtk.widget.h"
  #include "miu.gtk.window.h"
  #include "miu.gtk.menucontrol.h"
    #include "miu.gtk.menugroup.h"
    #include "miu.gtk.menuitem.h"
    #include "miu.gtk.menucheckitem.h"
    #include "miu.gtk.menuradioitem.h"
    #include "miu.gtk.menuseparator.h"
  #include "miu.gtk.formcontrol.h"
    #include "miu.gtk.frame.h"
    #include "miu.gtk.canvas.h"
    #include "miu.gtk.label.h"
    #include "miu.gtk.button.h"
    #include "miu.gtk.checkbox.h"
    #include "miu.gtk.radiobox.h"
    #include "miu.gtk.editbox.h"
    #include "miu.gtk.listbox.h"
    #include "miu.gtk.combobox.h"
    #include "miu.gtk.progressbar.h"
    #include "miu.gtk.slider.h"

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
  uint16 translate_key(uint key);
};

pMiu& pmiu();

} //namespace ns_miu

#endif //ifndef MIU_GTK_H
