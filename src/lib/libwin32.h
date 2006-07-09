/*
  libwin32 : version 0.03 ~byuu (04/14/06)
*/

#ifndef __LIBWIN32
#define __LIBWIN32

#include "libstring.h"

#define WINVER       0x0400
#define _WIN32_WINNT 0x0400
#define _WIN32_IE    0x0400
#include <windows.h>
#include <commctrl.h>
#include <direct.h>

#define WINDOW_LIMIT  4095
#define CONTROL_LIMIT 4095

#define WINDOWID_INDEX  10000
#define CONTROLID_INDEX 20000

typedef HFONT Font;

namespace global {
bool cursor_visible = true;
Font font_default;
};

void alert(char *s, ...);
uint GetScreenWidth();
uint GetScreenHeight();
void ShowCursor();
void HideCursor();
void ParseStyleParam(const char *style, string &output);

class Window;
class Control;

#include "libwin32_window.h"
#include "libwin32_control.h"

long __stdcall CoreWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class WindowManager {
public:
uint     window_count;
Window **list;

  long WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  bool Add(Window *window);

  WindowManager();
} WindowList;

Font CreateFont(const char *name, uint height, const char *style = "");

class libwin32_init {
public:
  libwin32_init();
} __libwin32_init;

#endif
