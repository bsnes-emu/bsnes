/*
  libui_gtk ~byuu (2007-01-29)
*/

#ifndef __LIBUI
#define __LIBUI

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

namespace libui {

void init();
void term();
bool run();
bool events_pending();

uint get_screen_width();
uint get_screen_height();

class Window;
typedef unsigned long WindowHandle;
#include "libui_gtk_control.h"

class Window { public:
struct {
  GtkWidget *window;
  GtkWidget *vcontainer, *container;
  array<Control*> control;
  uint control_index;

  bool has_menu;
  array<MenuGroup*> menu_group;
  uint menu_group_index;
} info;

  WindowHandle handle();
  void create(const char *style, uint width, uint height, const char *caption = "");
  void focus();
  void move(uint x, uint y);
  void resize(uint width, uint height);
  virtual void show();
  virtual void hide();
  virtual bool close() { return true; }
  virtual void keydown(uint key) {}
  virtual void keyup(uint key) {}

  void set_background_color(uint8 r, uint8 g, uint8 b);

  bool file_load(char *filename, const char *filter, const char *path = "");
  bool file_save(char *filename, const char *filter, const char *path = "");

  void menu_begin();
  void menu_end();
  void menu_group_begin(MenuGroup &group);
  void menu_group_end();
  void menu_add_item(Control &item);
  void menu_add_separator();

  void attach(Control &control, uint x, uint y, bool attach_to_window = true);
  virtual void clicked(Control*) {}

  Window() {
    info.control_index    = 1;
    info.has_menu         = false;
    info.menu_group_index = 1;
  }
};

};

#endif
