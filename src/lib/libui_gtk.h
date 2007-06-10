/*
  libui_gtk ~byuu (2007-06-06)
  license: public domain
*/

#ifndef LIBUI_H
#define LIBUI_H

#include "libbase.h"
#include "libarray.h"
#include "libvector.h"
#include "libstring.h"
#include "libkeymap.h"

//TODO: hide this if possible
#include <gtk/gtk.h>

namespace libui {

class Window;
#include "libui_gtk_control.h"

void init();
void term();
bool run();
bool events_pending();

uint get_screen_width();
uint get_screen_height();

bool file_load(Window *owner, char *filename, const char *filter, const char *path = "");
bool file_save(Window *owner, char *filename, const char *filter, const char *path = "");

uint16 translate_key(uint key);

namespace Message {
  enum {
    Invalid = 0,
    Close,
    Block,
    KeyUp,
    KeyDown,
    Clicked,
    DoubleClicked,
    Changed,
  };
};

class Window { public:
enum Style {
  Center = 1,
};

MenuBar menu;
  void create(uint style, uint width, uint height, const char *caption = "");
  void set_text(const char *str);
  void set_background_color(uint8 r, uint8 g, uint8 b);
  void focus();
  bool focused();
  void move(uint x, uint y);
  void resize(uint width, uint height);
  virtual void show();
  virtual void hide();
  void show(bool state);
  bool visible();
  void fullscreen();
  void unfullscreen();
  void fullscreen(bool state);
  bool is_fullscreen();

  virtual bool message(uint id, uintptr_t param = 0) { return true; }

//private:
struct {
  GtkWidget *window, *menubar;
  GtkWidget *vcontainer, *container;
} info;

public:
  Window() {
    info.menubar = 0;
  }
};

};

#endif
