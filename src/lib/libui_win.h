/*
  libui_win ~byuu (2007-06-05)
  license: public domain
*/

#ifndef LIBUI_H
#define LIBUI_H

#include "libbase.h"
#include "libarray.h"
#include "libvector.h"
#include "libstring.h"
#include "libkeymap.h"

#undef WINVER
#undef _WIN32_WINNT
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>

namespace libui {

class Window;
#include "libui_win_control.h"

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
  void set_text(const char *str, ...);
  void set_background_color(uint8 r, uint8 g, uint8 b);
  void focus();
  void move(uint x, uint y);
  void resize(uint width, uint height);
  virtual void show();
  virtual void hide();

  virtual bool message(uint id, uintptr_t param = 0) { return true; }

  void move(Control &control, uint x, uint y);
  void attach(Control &control);

  Window();

//platform-dependent:
  HWND handle();

//private:
struct {
  uint width, height;

  HWND hwnd;
  HWND hwnd_resize;
  HBRUSH background;
  HMENU menubar;
  bool center;
  array<Control*> control;
  uint control_index;

  array<MenuRadioItem*> menu_check_list;
} info;
  long wndproc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam);
};

//platform-dependent:

static uint window_count = 0;

static struct {
  HFONT variable;
  HFONT fixed;
} font;

HFONT create_font(const char *name, uint size);

};

#endif
