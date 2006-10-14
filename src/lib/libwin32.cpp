#include "libbase.h"
#include "libwin32.h"

#include "libwin32_misc.cpp"
#include "libwin32_file.cpp"
#include "libwin32_window.cpp"
#include "libwin32_control.cpp"
#include "libwin32_label.cpp"
#include "libwin32_groupbox.cpp"
#include "libwin32_editbox.cpp"
#include "libwin32_button.cpp"
#include "libwin32_checkbox.cpp"
#include "libwin32_radiobox.cpp"
#include "libwin32_slider.cpp"
#include "libwin32_combobox.cpp"
#include "libwin32_listbox.cpp"
#include "libwin32_listview.cpp"

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  MessageBox(0, str, "bsnes", MB_OK);
}

uint GetScreenWidth()  { return GetSystemMetrics(SM_CXSCREEN); }
uint GetScreenHeight() { return GetSystemMetrics(SM_CYSCREEN); }

void ShowCursor() {
  if(global::cursor_visible == false) {
    global::cursor_visible = true;
    ShowCursor(TRUE);
  }
}

void HideCursor() {
  if(global::cursor_visible == true) {
    global::cursor_visible = false;
    ShowCursor(FALSE);
  }
}

void ParseStyleParam(const char *style, stringarray &output) {
string temp;
  strcpy(temp, style);
  strlower(temp);
  replace(temp, " ", "");
  strltrim(temp, "|");
  strrtrim(temp, "|");
  replace(temp, "||", "|");
  split(output, "|", temp);
}

long __stdcall CoreWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return WindowList.WndProc(hwnd, msg, wparam, lparam);
}

bool WindowManager::Add(Window *window) {
  if(window_count >= WINDOW_LIMIT)return false;
  list[window_count++] = window;
  return true;
}

long WindowManager::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  for(int i = 0; i < window_count; i++) {
    if(list[i]->hwnd == hwnd) {
      return list[i]->WndProc(msg, wparam, lparam);
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowManager::WindowManager() {
  window_count = 0;
}

Font CreateFont(const char *name, uint height, const char *style) {
HDC hdc = GetDC(0);
Font font = CreateFont(-MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), 72),
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);
  ReleaseDC(0, hdc);
  return font;
}

libwin32_init::libwin32_init() {
  WindowList.list = (Window**)malloc((WINDOW_LIMIT + 1) * sizeof(Window*));
  memset(WindowList.list, 0, (WINDOW_LIMIT + 1) * sizeof(Window*));

  InitCommonControls();
  global::font_default = CreateFont("Tahoma", 8);

WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = CoreWindowProc;
  wc.lpszClassName = "resize_class";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
}
