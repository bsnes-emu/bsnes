/*
  libui_win ~byuu (2006-01-29)
*/

#ifndef __LIBUI
#define __LIBUI

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>

namespace libui {

void init();
void term();
bool run();
bool events_pending();

uint get_screen_width();
uint get_screen_height();

class Window;
typedef HWND WindowHandle;
#include "libui_win_control.h"

class Window { public:
struct {
  uint width, height;

  HWND hwnd;
  HWND hwnd_resize;
  HBRUSH background;
  bool center;
  array<Control*> control;
  uint control_index;

  bool has_menu;
  MenuGroup menu_owner;
  array<MenuGroup*> menu_group;
  uint menu_group_index;
  array<MenuRadioItem*> menu_check_list;
} info;
  long wndproc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam);

  WindowHandle handle();
  void create(const char *style, uint width, uint height, const char *caption);
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

  void move(Control &control, uint x, uint y);
  void attach(Control &control, uint x, uint y, bool attach_to_window = true);
  virtual void clicked(Control*) {}

  Window();
};

//platform dependent

static uint window_count = 0;

HFONT create_font(const char *name, uint size);
HWND message_window;

struct {
  HFONT variable;
  HFONT fixed;
} font;

};

#endif
