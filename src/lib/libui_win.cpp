#include "libui_win.h"
#include "libui_win_window.cpp"
#include "libui_win_control.cpp"

namespace libui {

HFONT create_font(const char *name, uint size) {
HDC hdc = GetDC(0);
HFONT font = CreateFont(-MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72),
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);
  ReleaseDC(0, hdc);
  return font;
}

void init() {
  InitCommonControls();

WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = libui_wndproc;
  wc.lpszClassName = "libui_class";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  message_window = CreateWindow("libui_class", "", WS_POPUP,
    0, 0, 64, 64, 0, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(message_window, GWLP_USERDATA, 0);

  font.variable = create_font("Tahoma", 8);
  font.fixed    = create_font("Courier New", 8);
}

void term() {
}

bool run() {
MSG msg;
  if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return events_pending();
}

bool events_pending() {
MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

uint get_screen_width()  { return GetSystemMetrics(SM_CXSCREEN); }
uint get_screen_height() { return GetSystemMetrics(SM_CYSCREEN); }

};
